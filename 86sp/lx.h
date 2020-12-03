


#pragma once

typedef enum _LX_TOKEN_TYPE {
    TokenUnknown,
    TokenEndOfFile,
    TokenEndOfLine,
    TokenComma,
    TokenColon,
    TokenAdd,
    TokenSub,
    TokenMul,
    TokenLBracket,
    TokenRBracket,
    TokenIdentifier,
    TokenNumber,
    TokenTag,
    TokenString,
    TokenMaximum

} LX_TOKEN_TYPE;

typedef struct _LX_TOKEN {
    LX_TOKEN_TYPE   TokenType;
    ULONG           LineNumber;

    union {
        struct {
            LONG    Number;
        } TypeNumber;

        struct {
            PWSTR   Name;
        } TypeIdentifier;

        struct {
            PWSTR   Name;
        } TypeString;
    };

    struct _LX_TOKEN* Flink;
} LX_TOKEN, *PLX_TOKEN;

SPSTATUS
LxAnalyseSource(
    __in PSP_SOURCE_CONTEXT SourceContext );

FORCEINLINE
BOOLEAN
LxIsInvalidLine(
    __in PLX_TOKEN Token )
{
    return ( Token->TokenType == TokenEndOfLine );
}

FORCEINLINE
BOOLEAN
LxIsPreprocessorLine(
    __in PLX_TOKEN Token )
{

    return ( Token->TokenType == TokenTag );
}

FORCEINLINE
BOOLEAN
LxIsSymbolLine(
    __in PLX_TOKEN Token )
{
    //
    //  this function only checks if there is a symbol at the start,
    //  there can be an instruction directly after it
    //
    //  pog: mov dword ptr [ecx+edx*4+0x20], ecx
    //

    return ( Token->TokenType == TokenIdentifier &&
        Token->Flink->TokenType == TokenColon );
}

FORCEINLINE
VOID
LxNextLine(
    __in PLX_TOKEN* Token )
{
    while (
        ( *Token )->TokenType != TokenEndOfLine &&
        ( *Token )->TokenType != TokenEndOfFile) {

        *Token = ( *Token )->Flink;
    }

    if (( *Token )->TokenType == TokenEndOfLine) {

        *Token = ( *Token )->Flink;
    }
}