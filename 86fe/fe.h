


#pragma once

#include <windows.h>
#include <intrin.h>

#undef  C_ASSERT
#define C_ASSERT( e )                       static_assert( e, "ASSERTION FAILURE" )

#define EXTERN                              extern
#define STATIC                              static

#define EXPORT_API                          __declspec(dllexport)
#define IMPORT_API                          __declspec(dllimport)

typedef wchar_t*                            PWSTR;
typedef char*                               VA_LIST;

VOID
FeConsoleOut(
    __in PWSTR Format,
    __in ... );

#define EXECUTION_MODE_GUI  0x01
#define EXECUTION_MODE_FILE 0x02

BOOLEAN
WINAPI
FeInitializeWindow(

);