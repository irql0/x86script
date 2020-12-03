


#include "sp.h"

PSP_SYMBOL
SpAllocateSymbol(
    __in PSP_SOURCE_CONTEXT SourceContext )
{
    PSP_SYMBOL Symbol;

    Symbol = SpAllocateMemory( sizeof( SP_SYMBOL ) );

    if (SourceContext->SymbolList == NULL) {

        SourceContext->SymbolList = Symbol;
        SourceContext->SymbolLast = Symbol;
        return Symbol;
    }

    SourceContext->SymbolLast->Flink = Symbol;
    SourceContext->SymbolLast = Symbol;

    return Symbol;
}

VOID
SpFreeSymbol(
    __in PSP_SYMBOL Symbol )
{

    SpFreeMemory( Symbol );
}

VOID
SpFreeSymbolList(
    __in PSP_SOURCE_CONTEXT SourceContext )
{
    PSP_SYMBOL FreeingSymbol;
    PSP_SYMBOL CurrentSymbol = SourceContext->SymbolList;

    SourceContext->SymbolList = NULL;
    SourceContext->SymbolLast = NULL;

    while (CurrentSymbol != NULL) {
        FreeingSymbol = CurrentSymbol;

        CurrentSymbol = CurrentSymbol->Flink;

        SpFreeSymbol( FreeingSymbol );
    }

    return;
}

PSP_SYMBOL
SpFindSymbol(
    __in PSP_SOURCE_CONTEXT SourceContext,
    __in PWCHAR             Name )
{
    PSP_SYMBOL CurrentSymbol = SourceContext->SymbolList;

    while (CurrentSymbol != NULL) {

        if (lstrcmpiW( CurrentSymbol->Name, Name ) == 0) {

            if (SourceContext->Pass != 0 &&
                !CurrentSymbol->Declared) {

                SpTraceWarning( NULL, L"symbol definition not found for \"%s\" on second pass, defaulted to 0", Name );
                return CurrentSymbol;
            }

            return CurrentSymbol;
        }

        CurrentSymbol = CurrentSymbol->Flink;
    }

    CurrentSymbol = SpAllocateSymbol( SourceContext );
    CurrentSymbol->Declared = FALSE;
    CurrentSymbol->Name = Name;
    CurrentSymbol->Value = 0;

    return CurrentSymbol;
}