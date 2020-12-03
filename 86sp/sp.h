


#pragma once

//
//  sp = source/script parser.
//

#pragma warning(disable:4201)
#pragma warning(disable:4214)

#include <windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <intrin.h>

#undef  RtlZeroMemory
#undef  RtlSetMemory8
#undef  RtlCopyMemory
#define RtlZeroMemory( dst, size )          __stosb( ( unsigned char* )( dst ), 0, ( size_t )( size ) )
#define RtlSetMemory8( dst, value, size )   __stosb( ( unsigned char* )( dst ), ( unsigned char )( value ), ( size_t )( size ) )
#define RtlCopyMemory( dst, src, size )     __movsb( ( unsigned char* )( dst ), ( unsigned char* )( src ), ( size_t )( size ) )

#undef  C_ASSERT
#define C_ASSERT( e )                       static_assert( e, "ASSERTION FAILURE" )

#define EXTERN                              extern
#define STATIC                              static

#define EXPORT_API                          __declspec(dllexport)
#define IMPORT_API                          __declspec(dllimport)

typedef wchar_t*                            PWSTR;
typedef char*                               VA_LIST;

typedef ULONG32                             SPSTATUS;

#define SPSTATUS_SUCCESS                    ( ( SPSTATUS )( 0x00000000L ) )
#define SPSTATUS_UNSUCCESSFUL               ( ( SPSTATUS )( 0x00000001L ) )
#define SPSTATUS_SKIP_LINE                  ( ( SPSTATUS )( 0x00000002L ) )

#define SP_SUCCESS( status )                ( ( ( SPSTATUS )( status ) ) == ( SPSTATUS_SUCCESS ) )

BOOL
WINAPI
SpLibraryEvent(
    __in HINSTANCE  LibraryHandle,
    __in DWORD      Event,
    __in PVOID      Reserved );

typedef struct _SP_SOURCE_CONTEXT   *PSP_SOURCE_CONTEXT;
typedef struct _LX_TOKEN            *PLX_TOKEN;
typedef struct _SP_SYMBOL           *PSP_SYMBOL;

FORCEINLINE
PVOID
SpAllocateMemory(
    __in SIZE_T Length )
{


    return HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY, Length );
}

FORCEINLINE
VOID
SpFreeMemory(
    __in PVOID Memory )
{

    HeapFree( GetProcessHeap( ), 0, Memory );
}

EXPORT_API
VOID
SpTraceLog(
    __in PWSTR Format,
    __in ... );

#include "lx.h"

EXPORT_API
VOID
SpTraceError(
    __in PLX_TOKEN  ExceptionToken,
    __in PWSTR      Format,
    __in ... );

EXPORT_API
VOID
SpTraceWarning(
    __in PLX_TOKEN  ExceptionToken,
    __in PWSTR      Format,
    __in ... );

typedef struct _SP_SOURCE_CONTEXT {

    PWSTR       ScriptFile;
    PWSTR       SourceBuffer;

    //
    //  lists of lexer tokens and symbol's
    //  the last field is an optimization for allocating.
    //

    PLX_TOKEN   TokenList;
    PLX_TOKEN   TokenLast;

    PSP_SYMBOL  SymbolList;
    PSP_SYMBOL  SymbolLast;

    PCHAR       Buffer;
    ULONG       BufferIndex;

    ULONG       Pass;
    ULONG       Base;

} SP_SOURCE_CONTEXT, *PSP_SOURCE_CONTEXT;

typedef enum _REGTYPE86 {
    RegisterInvalid,
    RegisterGeneralPurpose,
    RegisterControl,
    RegisterDebug,
    RegisterSegment,
    RegisterMaximum

} REGTYPE86;

typedef struct _SP_REGISTER_DEF {
    PWSTR       Name;
    ULONG       Size;
    ULONG       RegId;
    REGTYPE86   Type;

} SP_REGISTER_DEF, *PSP_REGISTER_DEF;

EXTERN SP_REGISTER_DEF g_Registers[ ];

typedef struct _SP_SIZE_OVERRIDE_DEF {
    PWSTR       Name;
    ULONG       Size;

} SP_SIZE_OVERRIDE_DEF, *PSP_SIZE_OVERRIDE_DEF;

EXTERN SP_SIZE_OVERRIDE_DEF g_SizeOverrides[ ];

typedef struct _SP_ADDRESSING_INFO {

    PLX_TOKEN   OverrideSegment;
    ULONG       OverrideSize;

    //
    //  is this an rm?
    //

    PLX_TOKEN   TokenLBracket;
    PLX_TOKEN   TokenRBracket;

    //
    //  scaled index info, TokenBase & TokenScale are used at regular reg
    //  when it's a modrm/reg
    //

    PLX_TOKEN   TokenScale;
    PLX_TOKEN   TokenIndex;
    PLX_TOKEN   TokenBase;

    //
    //  modrm/sib displacement and immediate values (indicated by brackets)
    //

    PLX_TOKEN   TokenDisp;

} SP_ADDRESSING_INFO, *PSP_ADDRESSING_INFO;

#define MOD_REGISTER        0 // [ eax ]
#define MOD_1BYTE_DISP      1 // [ eax + disp8 ]
#define MOD_4BYTE_DISP      2 // [ eax + disp32 ]
#define MOD_REGISTER_ADDR   3 //   eax

#define MOD_SIB_MOD0        0 // [ reg32 + reg32 * scale ]
#define MOD_SIB_MOD1        1 // [ disp8  + reg8  + reg32 * scale ]
#define MOD_SIB_MOD2        2 // [ disp32 + reg32 + reg32 * scale ]
#define MOD_SIB_MOD3        0 // [ disp32 + reg32 * scale ] (mod = 0, base = 101)

typedef union _RM86 {
    struct {
        UCHAR   Rm : 3;
        UCHAR   Reg : 3;
        UCHAR   Mod : 2;
    };

    UCHAR       Byte;
} RM86, *PRM86;

typedef union _SIB86 {
    struct {
        UCHAR   Base : 3;
        UCHAR   Index : 3;
        UCHAR   Scale : 2;
    };

    UCHAR       Byte;
} SIB86, *PSIB86;

typedef enum _OPTYPE86 {
    OperandInvalid,
    OperandImmediate,
    OperandModRm,
    OperandRelative,
    OperandRegister,
    OperandControlRegister,
    OperandSegmentRegister,
    OperandMaximum

} OPTYPE86;

typedef struct _SP_INSTRUCTION_INFO {

    RM86            ModRM;
    SIB86           Sib;
    LONG            Displacement;
    LONG            Immediate;

    OPTYPE86        OperandType[ 4 ];
    ULONG           OperandSize[ 4 ];
    UCHAR           OperandCount;
    UCHAR           SegmentOverride;

    struct {

        UCHAR       ModRM : 1;
        UCHAR       Sib : 1;
        UCHAR       Displacement32 : 1;
        UCHAR       Displacement8 : 1;
        UCHAR       Immediate : 1;
        UCHAR       OperandSizePrefix : 1;
    } Has;

} SP_INSTRUCTION_INFO, *PSP_INSTRUCTION_INFO;

EXPORT_API
SPSTATUS
SpAssembleSourceToBuffer(
    __in PWSTR Source,
    __in PVOID Buffer,
    __in ULONG Length,
    __in ULONG BaseAddress );

typedef struct _SP_SYMBOL {
    PWSTR   Name;
    ULONG   Value;
    BOOLEAN Declared;

    struct _SP_SYMBOL* Flink;
} SP_SYMBOL, *PSP_SYMBOL;

PSP_SYMBOL
SpFindSymbol(
    __in PSP_SOURCE_CONTEXT SourceContext,
    __in PWCHAR             Name );

SPSTATUS
SpPreprocessContext(
    __in PSP_SOURCE_CONTEXT SourceContext );

typedef struct _SP_INSTRUCTION_DEF {
    PWSTR       Mnemonic;
    UCHAR       Opcode1;
    UCHAR       Opcode2;
    OPTYPE86    OperandType[ 4 ];
    ULONG       OperandSize[ 4 ];
    UCHAR       ModRM;

    UCHAR       Prefix[ 4 ];


} SP_INSTRUCTION_DEF, *PSP_INSTRUCTION_DEF;

EXTERN SP_INSTRUCTION_DEF g_x86[ ];

SPSTATUS
SpParseOperand(
    __in  PSP_SOURCE_CONTEXT    SourceContext,
    __in  PLX_TOKEN*            Token,
    __out PSP_INSTRUCTION_INFO  Instruction );

SPSTATUS
SpParseMnemonic(
    __in PSP_SOURCE_CONTEXT     SourceContext,
    __in PLX_TOKEN              Token,
    __in PSP_INSTRUCTION_INFO   Instruction );

SPSTATUS
SpParsePseudoMnemonic(
    __in PSP_SOURCE_CONTEXT SourceContext,
    __in PLX_TOKEN*         Token );