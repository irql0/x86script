


#include "sp.h"

SPSTATUS
SpParsePseudoMnemonic(
    __in PSP_SOURCE_CONTEXT SourceContext,
    __in PLX_TOKEN*         Token )
{
    BOOLEAN FinalPass = FALSE;

    if (lstrcmpiW( ( *Token )->TypeIdentifier.Name, L"db" ) == 0) {
        *Token = ( *Token )->Flink;

        do {
        LoopStart1:;
            switch (( *Token )->TokenType) {
            case TokenString:

                for (int i = 0; ( *Token )->TypeString.Name[ i ]; i++) {

                    *( char* )( &SourceContext->Buffer[ SourceContext->BufferIndex + i ] ) = ( char )( *Token )->TypeString.Name[ i ];
                }

                SourceContext->BufferIndex += lstrlenW( ( *Token )->TypeIdentifier.Name );
                break;
            case TokenNumber:
                *( ( char* )&SourceContext->Buffer[ SourceContext->BufferIndex++ ] ) = ( char )( *Token )->TypeNumber.Number;
                break;
            default:
                SpTraceError( ( *Token ), L"unrecognised preprocessor define directive operand" );

                return SPSTATUS_UNSUCCESSFUL;
            }

            *Token = ( *Token )->Flink;

            if (( *Token )->TokenType == TokenEndOfLine) {

                break;
            }
            else if (( *Token )->TokenType == TokenComma) {

                *Token = ( *Token )->Flink;
            }
            else if (FinalPass) {

                break;
            }
            else {

                FinalPass = TRUE;
                goto LoopStart1;
            }

        } while (TRUE);

        *Token = ( *Token )->Flink;
        return SPSTATUS_SKIP_LINE;
    }
    else if (lstrcmpiW( ( *Token )->TypeIdentifier.Name, L"dw" ) == 0) {

        *Token = ( *Token )->Flink;

        do {
        LoopStart2:;
            switch (( *Token )->TokenType) {
            case TokenString:

                for (int i = 0; ( *Token )->TypeString.Name[ i ]; i++) {

                    *( USHORT* )( &SourceContext->Buffer[ SourceContext->BufferIndex + ( i * sizeof( USHORT ) ) ] ) = ( *Token )->TypeString.Name[ i ];
                }

                SourceContext->BufferIndex += ( lstrlenW( ( *Token )->TypeIdentifier.Name ) * sizeof( USHORT ) );
                break;
            case TokenNumber:
                *( ( USHORT* )&SourceContext->Buffer[ SourceContext->BufferIndex ] ) = ( USHORT )( *Token )->TypeNumber.Number;
                SourceContext->BufferIndex += 2;
                break;
            default:
                SpTraceError( ( *Token ), L"unrecognised preprocessor define directive operand" );

                return SPSTATUS_UNSUCCESSFUL;
            }

            *Token = ( *Token )->Flink;

            if (( *Token )->TokenType == TokenEndOfLine) {

                break;
            }
            else if (( *Token )->TokenType == TokenComma) {

                *Token = ( *Token )->Flink;
            }
            else if (FinalPass) {

                break;
            }
            else {

                FinalPass = TRUE;
                goto LoopStart2;
            }

        } while (TRUE);

        *Token = ( *Token )->Flink;
        return SPSTATUS_SKIP_LINE;
    }
    else if (lstrcmpiW( ( *Token )->TypeIdentifier.Name, L"dd" ) == 0) {
        *Token = ( *Token )->Flink;

        do {
        LoopStart3:;
            switch (( *Token )->TokenType) {
            case TokenString:

                for (int i = 0; ( *Token )->TypeString.Name[ i ]; i++) {

                    *( ULONG32* )( &SourceContext->Buffer[ SourceContext->BufferIndex + ( i * sizeof( ULONG32 ) ) ] ) = ( *Token )->TypeString.Name[ i ];
                }

                SourceContext->BufferIndex += ( lstrlenW( ( *Token )->TypeIdentifier.Name ) * sizeof( ULONG32 ) );
                break;
            case TokenNumber:
                *( ( ULONG32* )&SourceContext->Buffer[ SourceContext->BufferIndex ] ) = ( *Token )->TypeNumber.Number;
                SourceContext->BufferIndex += sizeof( ULONG32 );
                break;
            default:
                SpTraceError( ( *Token ), L"unrecognised preprocessor define directive operand" );

                return SPSTATUS_UNSUCCESSFUL;
            }

            *Token = ( *Token )->Flink;

            if (( *Token )->TokenType == TokenEndOfLine) {

                break;
            }
            else if (( *Token )->TokenType == TokenComma) {

                *Token = ( *Token )->Flink;
            }
            else if (FinalPass) {

                break;
            }
            else {

                FinalPass = TRUE;
                goto LoopStart3;
            }

        } while (TRUE);

        *Token = ( *Token )->Flink;
        return SPSTATUS_SKIP_LINE;
    }

    return SPSTATUS_SUCCESS;
}
