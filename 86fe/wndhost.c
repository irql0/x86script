


#include "fe.h"

#define ID_EDITBOX_1    1001
#define ID_BUTTON_1     1002

HWND g_MainWindow;

LRESULT
CALLBACK
WndProc(
    __in HWND   Window,
    __in UINT   Message,
    __in LPARAM Param1,
    __in WPARAM Param2 )
{

    switch (Message) {
    case WM_COMMAND:;
        switch (Param1) {
        case ID_BUTTON_1:;

            PVOID AssemblyBuffer = VirtualAlloc( NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

            WCHAR EditBuffer[ 1024 ];
            WCHAR SourceBuffer[ 1024 ];

            __stosb( ( char* )&EditBuffer, 0, 2048 );
            __stosb( ( char* )&SourceBuffer, 0, 2048 );

            SendMessageW( GetDlgItem( g_MainWindow, ID_EDITBOX_1 ), WM_GETTEXT, ( WPARAM )1024, ( LPARAM )&EditBuffer );

            HMODULE sp = LoadLibraryW( L"86sp.dll" );

            if (sp == NULL) {

                FeConsoleOut( L"missing 86sp.dll\n" );
                break;
            }

            FARPROC proc = GetProcAddress( sp, "SpAssembleSourceToBuffer" );

            if (proc == NULL) {

                FeConsoleOut( L"86sp.dll invalid\n" );
                break;
            }

            int spStatus = proc( EditBuffer, AssemblyBuffer, 0x1000, AssemblyBuffer );

            if (spStatus == 0) {

                spStatus = ( ( int( __cdecl* )( ) )AssemblyBuffer )( );

                FeConsoleOut( L"[86fe] executed, returned: %p (%d)\n", spStatus, spStatus );
            }

            VirtualFree( AssemblyBuffer, 0x1000, MEM_RELEASE );

            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        ExitProcess( 0 );
    default:
        return DefWindowProcW( Window, Message, Param1, Param2 );
    }

    return TRUE;
}

BOOLEAN
WINAPI
FeInitializeWindow(

)
{
    MSG Message;
    WNDCLASSEXW Class;
    HINSTANCE BaseAddress;
    HDC DeviceContext;

    NONCLIENTMETRICSW Client;
    HFONT DefaultFont;
    HFONT CodeFont;

    HMODULE BaseShcore;
    BOOL( WINAPI*SetProcessDpiAwarenessContextProc )( LONG );
    BOOL( WINAPI*SetProcessDpiAwareProc )( );

    BaseShcore = LoadLibraryW( L"Shcore.dll" );
    SetProcessDpiAwarenessContextProc = NULL;

    if (BaseShcore != NULL) {

        //
        //  if shcore is there, this export is present.
        //

        SetProcessDpiAwarenessContextProc = ( BOOL( WINAPI* )( LONG ) )GetProcAddress( BaseShcore, "SetProcessDpiAwarenessContext" );

        if (SetProcessDpiAwarenessContextProc != NULL) {

            SetProcessDpiAwarenessContextProc( -3 );
        }
    }

    if (SetProcessDpiAwarenessContextProc == NULL) {

        //
        //  make sure winxp can run it.
        //

        SetProcessDpiAwareProc = ( BOOL( WINAPI* )( ) )GetProcAddress( LoadLibraryW( L"user32.dll" ), "SetProcessDPIAware" );

        if (SetProcessDpiAwareProc != NULL) {

            SetProcessDpiAwareProc( );
        }
    }

    BaseAddress = GetModuleHandleW( NULL );

    __stosb( ( unsigned char* )&Class, 0, sizeof( WNDCLASSEXW ) );
    Class.cbSize = sizeof( WNDCLASSEXW );
    Class.style = CS_HREDRAW | CS_VREDRAW;
    Class.lpfnWndProc = ( WNDPROC )WndProc;
    Class.cbClsExtra = 0;
    Class.cbWndExtra = 0;
    Class.hInstance = BaseAddress;
    Class.hIcon = NULL;
    Class.hCursor = LoadCursorW( NULL, IDC_ARROW );
    Class.hbrBackground = ( HBRUSH )COLOR_WINDOW;
    Class.lpszClassName = L"86fe";
    Class.hIconSm = NULL;

    if (RegisterClassExW( &Class ) == 0) {

        return FALSE;
    }

    g_MainWindow = CreateWindowExW( 0,
        L"86fe", L"dogCamp",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 525, 300,
        NULL, NULL, BaseAddress, NULL );

    if (g_MainWindow == NULL) {

        UnregisterClassW( L"86fe", BaseAddress );

        return FALSE;
    }

    ShowWindow( g_MainWindow, SW_SHOW );
    UpdateWindow( g_MainWindow );

    __stosb( ( unsigned char* )&Client, 0, sizeof( NONCLIENTMETRICSW ) );
    Client.cbSize = sizeof( NONCLIENTMETRICSW );

    SystemParametersInfoW( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICSW ), &Client, 0 );

    DefaultFont = CreateFontIndirectW( &Client.lfMessageFont );

    DeviceContext = GetDC( NULL );
    CodeFont = CreateFontW(
        -MulDiv( 10, GetDeviceCaps( DeviceContext, LOGPIXELSY ), 72 ), 0, 0, 0,
        FW_DONTCARE,
        FALSE, FALSE,
        FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
        L"consolas" );
    ReleaseDC( NULL, DeviceContext );

    HWND MainEdit = CreateWindowExW(
        0,
        L"EDIT", L"",
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        0, 0,
        410, 271,
        g_MainWindow, ( HMENU )ID_EDITBOX_1, BaseAddress, NULL );
    SendMessageW( MainEdit, WM_SETFONT, ( WPARAM )CodeFont, FALSE );

    HWND MainButton = CreateWindowExW(
        0,
        L"BUTTON", L"run",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        425, 10,
        80, 25,
        g_MainWindow, ( HMENU )ID_BUTTON_1, BaseAddress, NULL );
    SendMessageW( MainButton, WM_SETFONT, ( WPARAM )DefaultFont, FALSE );

    while (GetMessageW( &Message, NULL, 0, 0 )) {

        TranslateMessage( &Message );
        DispatchMessageW( &Message );
    }
}

/*

%externdef user32:MessageBoxW

push ebp
mov ebp, esp

push 0
push msg1
push msg1
push 0
call MessageBoxW

push 0
push msg2
push msg2
push 0
call MessageBoxW

pop ebp
ret

msg1: dw "pog", 0
msg2: dw "pogs", 0

*/