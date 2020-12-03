


#include "fe.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

VOID
FeProcessStartup(

)
{

    LPWSTR  CommandLine;
    LPWSTR  ArgVector;
    ULONG   ArgCount;
    ULONG   ExecMode;
    LPWSTR  RequestFile;

    CommandLine = GetCommandLineW( );
    ArgVector = CommandLineToArgvW( CommandLine, &ArgCount );

    if (ArgCount <= 1) {

        //
        //  argv[ 0 ] = L"86fe.exe"
        //

        //FeConsoleOut( L"invalid arguments.\n\n\t/gui\t\tdragonoid.\n\t[FILE]\t\tjust a file path to parse and run.\n" );

        ExecMode = EXECUTION_MODE_GUI;
    }
    else {

        ArgCount--;
        while (ArgCount--) {

            if (lstrcmpiW( ArgVector[ ArgCount ], L"gui" ) == 0) {

                ExecMode |= EXECUTION_MODE_GUI;
                continue;
            }
            else {

                RequestFile = ArgVector[ ArgCount ];
                ExecMode |= EXECUTION_MODE_FILE;
                continue;
            }
        }
    }

    LocalFree( ( HLOCAL )ArgVector );

    if (ExecMode & EXECUTION_MODE_GUI) {

        FeInitializeWindow( );
    }

#if 0
    GetProcAddress( LoadLibraryW( L"86sp.dll" ), "SpAssembleSourceToBuffer" )(
        L"%externdef user32:MessageBoxA\n"
        L"mov dword ptr [0x40+ecx+edx*4], ecx\n"
        L"doggers:mov dword ptr [ecx], doggers\n"
        L"mov ecx, dword ptr cs:[edx]\n"
        L"jz doggers\n"
        L"lea cx, [0x40+ecx+edx*4]\n"
        L"cmp ecx, 4\n"
        L"jne doggers;dogshit\n"
        L"dw \"Hi im pog dog.\", 0",
        VirtualAlloc( NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ),
        0x1000,
        0x10000 );
#endif

    Sleep( ( unsigned )-1 );
    ExitProcess( 0 );
}