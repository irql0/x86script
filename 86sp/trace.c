


#include "sp.h"

VOID
SpTraceLog(
    __in PWSTR Format,
    __in ... )
{
    STATIC HANDLE ConsoleOutput = 0;

    WCHAR   Buffer[ 256 ];
    DWORD   CharsWritten;
    VA_LIST ArgList;

    if (ConsoleOutput == 0) {

        ConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    }

    __crt_va_start( ArgList, Format );

    wvsprintfW( Buffer, Format, ArgList );

    __crt_va_end( ArgList );

    WriteConsoleW( ConsoleOutput, Buffer, lstrlenW( Buffer ), &CharsWritten, NULL );
}

VOID
SpTraceError(
    __in PLX_TOKEN  ExceptionToken,
    __in PWSTR      Format,
    __in ... )
{
    STATIC HANDLE ConsoleOutput = 0;

    WCHAR   Buffer[ 256 ];
    WCHAR   Buffer2[ 256 ];
    DWORD   CharsWritten;
    VA_LIST ArgList;

    if (ConsoleOutput == 0) {

        ConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    }

    __crt_va_start( ArgList, Format );

    wvsprintfW( Buffer, Format, ArgList );

    __crt_va_end( ArgList );

    if (ExceptionToken) {

        switch (ExceptionToken->TokenType) {
        case TokenString:;
        case TokenIdentifier:;
            wsprintfW( Buffer2, L"ERROR: %s on line %d: \"%s\".\n", Buffer, ExceptionToken->LineNumber, ExceptionToken->TypeIdentifier.Name );
            break;
        case TokenNumber:;
            wsprintfW( Buffer2, L"ERROR: %s on line %d: \"%s\".\n", Buffer, ExceptionToken->LineNumber, ExceptionToken->TypeIdentifier.Name );
            break;
        default:
            wsprintfW( Buffer2, L"ERROR: %s on line %d.\n", Buffer, ExceptionToken->LineNumber );
            break;
        }
    }
    else {

        wsprintfW( Buffer2, L"ERROR: %s.\n", Buffer );
    }

    WriteConsoleW( ConsoleOutput, Buffer2, lstrlenW( Buffer2 ), &CharsWritten, NULL );
    //__debugbreak( );
}

VOID
SpTraceWarning(
    __in PLX_TOKEN  ExceptionToken,
    __in PWSTR      Format,
    __in ... )
{
    STATIC HANDLE ConsoleOutput = 0;

    WCHAR   Buffer[ 256 ];
    WCHAR   Buffer2[ 256 ];
    DWORD   CharsWritten;
    VA_LIST ArgList;

    if (ConsoleOutput == 0) {

        ConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    }

    __crt_va_start( ArgList, Format );

    wvsprintfW( Buffer, Format, ArgList );

    __crt_va_end( ArgList );

    if (ExceptionToken) {

        switch (ExceptionToken->TokenType) {
        case TokenString:;
        case TokenIdentifier:;
            wsprintfW( Buffer2, L"WARNING: %s on line %d: \"%s\".\n", Buffer, ExceptionToken->LineNumber, ExceptionToken->TypeIdentifier.Name );
            break;
        case TokenNumber:;
            wsprintfW( Buffer2, L"WARNING: %s on line %d: \"%s\".\n", Buffer, ExceptionToken->LineNumber, ExceptionToken->TypeIdentifier.Name );
            break;
        default:
            wsprintfW( Buffer2, L"WARNING: %s on line %d.\n", Buffer, ExceptionToken->LineNumber );
            break;
        }
    }
    else {

        wsprintfW( Buffer2, L"WARNING: %s.\n", Buffer );
    }

    WriteConsoleW( ConsoleOutput, Buffer2, lstrlenW( Buffer2 ), &CharsWritten, NULL );
}

