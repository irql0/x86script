


#include "sp.h"

PLX_TOKEN
LxAllocateToken(
    __in PSP_SOURCE_CONTEXT SourceContext )
{
    PLX_TOKEN Token;

    Token = SpAllocateMemory( sizeof( LX_TOKEN ) );

    if (SourceContext->TokenList == NULL) {

        SourceContext->TokenList = Token;
        SourceContext->TokenLast = Token;
        return Token;
    }

    SourceContext->TokenLast->Flink = Token;
    SourceContext->TokenLast = Token;

    return Token;
}

VOID
LxFreeToken(
    __in PLX_TOKEN Token )
{

    SpFreeMemory( Token );
}

VOID
LxFreeTokenList(
    __in PSP_SOURCE_CONTEXT SourceContext )
{
    PLX_TOKEN FreeingToken;
    PLX_TOKEN CurrentToken = SourceContext->TokenList;

    SourceContext->TokenList = NULL;
    SourceContext->TokenLast = NULL;

    while (CurrentToken != NULL) {
        FreeingToken = CurrentToken;

        CurrentToken = CurrentToken->Flink;

        LxFreeToken( FreeingToken );
    }

    return;
}

SPSTATUS
LxAnalyseSource(
    __in PSP_SOURCE_CONTEXT SourceContext )
{

    PLX_TOKEN   CurrentToken;
    PWSTR       SourceBuffer;
    ULONG       LineNumber;

    LineNumber = 0;
    SourceBuffer = SourceContext->SourceBuffer;

    while (*SourceBuffer) {

        if (*SourceBuffer == ';') {

            while (*SourceBuffer != 0 && *SourceBuffer != '\n') {

                SourceBuffer++;
            }

            if (*SourceBuffer == '\n') {

                SourceBuffer++;
            }
            continue;
        }

        if (*SourceBuffer == ' ' ||
            *SourceBuffer == '\t' ||
            *SourceBuffer == '\r') {

            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '\"') {

            SourceBuffer++;
            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenString;
            CurrentToken->LineNumber = LineNumber;
            CurrentToken->TypeIdentifier.Name = SpAllocateMemory( 512 );
            for (int i = 0; *SourceBuffer != '\"'; i++) {

                if (*SourceBuffer == '\r') {

                    continue;
                }

                if (*SourceBuffer == '\n') {

                    SpTraceError( NULL, L"matching end quote not found on line %d", LineNumber );
                    LxFreeTokenList( SourceContext );
                    return SPSTATUS_UNSUCCESSFUL;
                }

                CurrentToken->TypeIdentifier.Name[ i ] = *SourceBuffer++;
            }
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '\n') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenEndOfLine;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            LineNumber++;
            continue;
        }

        if (*SourceBuffer == '%') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenTag;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == ',') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenComma;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == ':') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenColon;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '[') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenLBracket;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == ']') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenRBracket;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '+') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenAdd;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '-') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenSub;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer == '*') {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenMul;
            CurrentToken->LineNumber = LineNumber;
            SourceBuffer++;
            continue;
        }

        if (*SourceBuffer >= '0' &&
            *SourceBuffer <= '9') {


            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenNumber;
            CurrentToken->LineNumber = LineNumber;

            int Base = 10;
            int i = 0;

            if (SourceBuffer[ i ] == '0' &&
                SourceBuffer[ i + 1 ] == 'x') {

                Base = 16;
                i += 2;
            }

            for (; SourceBuffer[ i ] != 0 &&
                ( ( SourceBuffer[ i ] >= '0' && SourceBuffer[ i ] <= '9' ) ||
                ( SourceBuffer[ i ] >= 'a' && SourceBuffer[ i ] <= 'f' ) ); i++) {

                if (SourceBuffer[ i ] >= '0' && SourceBuffer[ i ] <= '9') {

                    CurrentToken->TypeNumber.Number = CurrentToken->TypeNumber.Number * Base + SourceBuffer[ i ] - '0';
                }
                else {

                    CurrentToken->TypeNumber.Number = CurrentToken->TypeNumber.Number * Base + ( SourceBuffer[ i ] - 'a' ) + 10;
                }
            }

            SourceBuffer += i;
            continue;
        }

        if (( *SourceBuffer >= 'a' && *SourceBuffer <= 'z' ) ||
            ( *SourceBuffer >= 'A' && *SourceBuffer <= 'Z' )) {

            CurrentToken = LxAllocateToken( SourceContext );
            CurrentToken->TokenType = TokenIdentifier;
            CurrentToken->LineNumber = LineNumber;
            CurrentToken->TypeIdentifier.Name = SpAllocateMemory( 512 );
            for (int i = 0;
                *SourceBuffer &&
                ( *SourceBuffer >= 'a' && *SourceBuffer <= 'z' ) ||
                ( *SourceBuffer >= 'A' && *SourceBuffer <= 'Z' ) ||
                ( *SourceBuffer >= '0' && *SourceBuffer <= '9' ) ||
                ( *SourceBuffer == '_' );
                SourceBuffer++, i++) {

                CurrentToken->TypeIdentifier.Name[ i ] = *SourceBuffer;
                CurrentToken->TypeIdentifier.Name[ i + 1 ] = 0;
            }

            continue;
        }

        SpTraceError( NULL, L"unrecognised token on line %d, 0x%.4x '%c'", LineNumber, *SourceBuffer & 0xFFFF, *SourceBuffer );
        LxFreeTokenList( SourceContext );
        return SPSTATUS_UNSUCCESSFUL;
    }

    CurrentToken = LxAllocateToken( SourceContext );
    CurrentToken->TokenType = TokenEndOfLine;
    CurrentToken->LineNumber = LineNumber++;
    CurrentToken = LxAllocateToken( SourceContext );
    CurrentToken->TokenType = TokenEndOfFile;
    CurrentToken->LineNumber = LineNumber;

    return SPSTATUS_SUCCESS;
}
