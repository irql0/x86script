


#include "fe.h"

VOID
FeConsoleOut(
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