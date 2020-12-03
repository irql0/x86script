


#include "sp.h"

SPSTATUS
SpPreprocessContext(
    __in PSP_SOURCE_CONTEXT SourceContext )
{

    PLX_TOKEN CurrentToken = SourceContext->TokenList;

    while (CurrentToken != NULL &&
        CurrentToken->TokenType != TokenEndOfFile) {

        if (LxIsPreprocessorLine( CurrentToken )) {

            CurrentToken = CurrentToken->Flink;

            if (CurrentToken->TokenType != TokenIdentifier) {

                SpTraceError( CurrentToken, L"unexpected token for preprocessor" );
                return SPSTATUS_UNSUCCESSFUL;
            }

            if (lstrcmpiW( CurrentToken->TypeIdentifier.Name, L"include" ) == 0) {

                SpTraceWarning( CurrentToken, L"preprocessor doesnt support includes just yet" );
            }
            else if (lstrcmpiW( CurrentToken->TypeIdentifier.Name, L"externdef" ) == 0) {
                CurrentToken = CurrentToken->Flink;

                CHAR Buffer[ 256 ];

                PWSTR Library = CurrentToken->TypeIdentifier.Name;
                CurrentToken = CurrentToken->Flink->Flink;
                PWSTR Export = CurrentToken->TypeIdentifier.Name;

                PSP_SYMBOL Symbol = SpFindSymbol( SourceContext, Export );

                WideCharToMultiByte( CP_UTF8, 0, Export, -1, Buffer, 256, " ", NULL );

                Symbol->Declared = TRUE;
                Symbol->Value = ( ULONG )GetProcAddress( LoadLibraryW( Library ), Buffer );
            }
            else {

                SpTraceError( CurrentToken, L"invalid preprocessor directive" );
                return SPSTATUS_UNSUCCESSFUL;
            }
        }

        LxNextLine( &CurrentToken );
    }

    return SPSTATUS_SUCCESS;
}
