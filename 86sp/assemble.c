


#include "sp.h"

SPSTATUS
SpAssembleContext(
    __in PSP_SOURCE_CONTEXT SourceContext )
{
    SPSTATUS            spStatus;
    PLX_TOKEN           CurrentToken;
    SP_INSTRUCTION_INFO Instruction;
    PSP_SYMBOL          Symbol;
    PLX_TOKEN           MnemonicToken;

    //
    //  the initial base is always the buffer base.
    //

    spStatus = SpPreprocessContext( SourceContext );
    if (!SP_SUCCESS( spStatus )) {

        SpTraceError( NULL, L"preprocessor failure" );
        return spStatus;
    }

    CurrentToken = SourceContext->TokenList;

PassCode:;
    while (CurrentToken != NULL &&
        CurrentToken->TokenType != TokenEndOfFile) {

        RtlZeroMemory( &Instruction, sizeof( SP_INSTRUCTION_INFO ) );

        if (LxIsInvalidLine( CurrentToken )) {

            LxNextLine( &CurrentToken );
            continue;
        }

        if (LxIsPreprocessorLine( CurrentToken )) {

            if (lstrcmpiW( CurrentToken->Flink->TypeIdentifier.Name, L"base" ) == 0) {

                SourceContext->Base = CurrentToken->Flink->Flink->TypeNumber.Number;
            }

            LxNextLine( &CurrentToken );
            continue;
        }

        if (LxIsSymbolLine( CurrentToken )) {

            Symbol = SpFindSymbol( SourceContext, CurrentToken->TypeIdentifier.Name );
            Symbol->Declared = TRUE;
            Symbol->Value = SourceContext->Base + SourceContext->BufferIndex;

            CurrentToken = CurrentToken->Flink->Flink;

            if (CurrentToken->TokenType == TokenEndOfLine) {

                CurrentToken = CurrentToken->Flink;
                continue;
            }
        }

        //
        //  regular instruction
        //

        MnemonicToken = CurrentToken;

        if (MnemonicToken->TokenType != TokenIdentifier) {

            SpTraceError( MnemonicToken, L"unrecognised token, expected identifer" );
        }

        spStatus = SpParsePseudoMnemonic( SourceContext, &CurrentToken );

        if (!SP_SUCCESS( spStatus )) {

            if (spStatus == SPSTATUS_SKIP_LINE) {

                continue;
            }

            SpTraceError( CurrentToken, L"failed to parse pseudo mnemonic" );
            return spStatus;
        }

        if (CurrentToken->Flink->TokenType != TokenEndOfLine) {

            do {
                CurrentToken = CurrentToken->Flink;
                spStatus = SpParseOperand( SourceContext, &CurrentToken, &Instruction );

                if (!SP_SUCCESS( spStatus )) {

                    SpTraceError( CurrentToken, L"failed to parse operand" );
                    return spStatus;
                }

            } while (CurrentToken->TokenType == TokenComma);
        }
        else {

            CurrentToken = CurrentToken->Flink;
        }

        spStatus = SpParseMnemonic( SourceContext, MnemonicToken, &Instruction );

        if (!SP_SUCCESS( spStatus )) {

            SpTraceError( MnemonicToken, L"failed to parse mnemonic" );
            return spStatus;
        }
    }

    if (SourceContext->Pass == 0) {

        SourceContext->BufferIndex = 0;
        CurrentToken = SourceContext->TokenList;

        SourceContext->Pass++;
        goto PassCode;
    }

    return SPSTATUS_SUCCESS;
}

SPSTATUS
SpAssembleSourceToBuffer(
    __in PWSTR Source,
    __in PVOID Buffer,
    __in ULONG Length,
    __in ULONG BaseAddress )
{
    Source;
    Buffer;
    Length;

    SPSTATUS spStatus;
    SP_SOURCE_CONTEXT SourceContext;

    RtlZeroMemory( &SourceContext, sizeof( SP_SOURCE_CONTEXT ) );
    SourceContext.SourceBuffer = Source;
    SourceContext.Buffer = Buffer;
    SourceContext.BufferIndex = 0;
    SourceContext.Base = BaseAddress;

    spStatus = LxAnalyseSource( &SourceContext );
    if (!SP_SUCCESS( spStatus )) {

        return spStatus;
    }

    spStatus = SpAssembleContext( &SourceContext );
    if (!SP_SUCCESS( spStatus )) {

        return spStatus;
    }

    SpTraceLog( L"[86sp] assembler result: \n" );

    for (ULONG i = 0; i < SourceContext.BufferIndex; i++) {

        SpTraceLog( L"%.2x ", SourceContext.Buffer[ i ] & 0xFF );
    }

    SpTraceLog( L"\n" );

    return SPSTATUS_SUCCESS;
}