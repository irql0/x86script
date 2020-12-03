


#include "sp.h"

SP_REGISTER_DEF g_Registers[ ] = {
    { L"eax", 32, 0, RegisterGeneralPurpose },
    { L"ecx", 32, 1, RegisterGeneralPurpose },
    { L"edx", 32, 2, RegisterGeneralPurpose },
    { L"ebx", 32, 3, RegisterGeneralPurpose },
    { L"esp", 32, 4, RegisterGeneralPurpose },
    { L"ebp", 32, 5, RegisterGeneralPurpose },
    { L"esi", 32, 6, RegisterGeneralPurpose },
    { L"edi", 32, 7, RegisterGeneralPurpose },
    { L"ax", 16, 0, RegisterGeneralPurpose },
    { L"cx", 16, 1, RegisterGeneralPurpose },
    { L"dx", 16, 2, RegisterGeneralPurpose },
    { L"bx", 16, 3, RegisterGeneralPurpose },
    { L"sp", 16, 4, RegisterGeneralPurpose },
    { L"bp", 16, 5, RegisterGeneralPurpose },
    { L"si", 16, 6, RegisterGeneralPurpose },
    { L"di", 16, 7, RegisterGeneralPurpose },
    { L"al", 8, 0, RegisterGeneralPurpose },
    { L"cl", 8, 1, RegisterGeneralPurpose },
    { L"dl", 8, 2, RegisterGeneralPurpose },
    { L"bl", 8, 3, RegisterGeneralPurpose },
    { L"ah", 8, 4, RegisterGeneralPurpose },
    { L"ch", 8, 5, RegisterGeneralPurpose },
    { L"dh", 8, 6, RegisterGeneralPurpose },
    { L"bh", 8, 7, RegisterGeneralPurpose },
    { L"es", 16, 0, RegisterSegment },
    { L"cs", 16, 1, RegisterSegment },
    { L"ss", 16, 2, RegisterSegment },
    { L"ds", 16, 3, RegisterSegment },
    { L"fs", 16, 4, RegisterSegment },
    { L"gs", 16, 5, RegisterSegment },
    { L"cr0", 32, 0, RegisterControl },
    { L"cr2", 32, 1, RegisterControl },
    { L"cr3", 32, 2, RegisterControl },
    { L"cr4", 32, 3, RegisterControl },
};



SP_SIZE_OVERRIDE_DEF g_SizeOverrides[ ] = {
    { L"byte", 8 },
    { L"word", 16 },
    { L"dword", 32 },
    { L"lime", 32 },
    { L"qword", 64 },
    { L"tword", 80 },
    { L"mmword", 64 },
    { L"xmmword", 128 },
    { L"ymmword", 256 },
    { L"zmmword", 512 },
};

SPSTATUS
SpIdentifyRegister(
    __in  PLX_TOKEN         Token,
    __out PSP_REGISTER_DEF* Register )
{

    //
    //  mov ?, 4
    //

    for (ULONG32 RegId = 0; RegId < sizeof( g_Registers ) / sizeof( SP_REGISTER_DEF ); RegId++) {

        if (lstrcmpiW( Token->TypeIdentifier.Name, g_Registers[ RegId ].Name ) == 0) {

            *Register = &g_Registers[ RegId ];
            return SPSTATUS_SUCCESS;
        }
    }

    *Register = NULL;
    return SPSTATUS_SUCCESS;
}

SPSTATUS
SpIdentifySizeOverride(
    __in  PLX_TOKEN*                Token,
    __out PSP_SIZE_OVERRIDE_DEF*    SizeOverride )
{

    if (( *Token )->TokenType == TokenIdentifier &&
        ( *Token )->Flink->TokenType == TokenIdentifier &&
        lstrcmpiW( ( *Token )->Flink->TypeIdentifier.Name, L"ptr" ) == 0)
    {
        //
        //  mov ? ptr [eax+ecx*8], 4
        //

        for (ULONG OverrideId = 0; OverrideId < sizeof( g_SizeOverrides ) / sizeof( SP_SIZE_OVERRIDE_DEF ); OverrideId++) {

            if (lstrcmpiW( ( *Token )->TypeIdentifier.Name, g_SizeOverrides[ OverrideId ].Name ) == 0) {

                //
                //  skip the "? ptr" tokens
                //

                *Token = ( *Token )->Flink->Flink;
                *SizeOverride = &g_SizeOverrides[ OverrideId ];
                return SPSTATUS_SUCCESS;
            }
        }

        SpTraceError( *Token, L"invalid size override" );
        *SizeOverride = NULL;
        return SPSTATUS_UNSUCCESSFUL;
    }

    *SizeOverride = NULL;
    return SPSTATUS_SUCCESS;
}

SPSTATUS
SpParseAddressing(
    __in  PLX_TOKEN*             Token,
    __out PSP_ADDRESSING_INFO    Addressing )
{
    SPSTATUS spStatus;
    PSP_SIZE_OVERRIDE_DEF SizeOverride;

    spStatus = SpIdentifySizeOverride( Token, &SizeOverride );
    if (!SP_SUCCESS( spStatus )) {

        return spStatus;
    }

    if (SizeOverride != NULL) {

        Addressing->OverrideSize = SizeOverride->Size;
    }

    if (( *Token )->TokenType == TokenIdentifier &&
        ( *Token )->Flink->TokenType == TokenColon) {

        //
        //  segment override
        //  mov dword ptr cs:[ecx+edx*4+0x24], ecx
        //

        Addressing->OverrideSegment = ( *Token );

        ( *Token ) = ( *Token )->Flink->Flink;

        if (( *Token )->TokenType != TokenLBracket) {

            //
            //  fuck off invalids like 
            //  mov dword ptr cs:ecx
            //

            SpTraceError( *Token, L"invalid use of segment register" );
            return SPSTATUS_UNSUCCESSFUL;
        }
    }

    if (( *Token )->TokenType == TokenIdentifier) {

        //
        //  single register or symbol
        //  mov dword ptr eax, ???
        //

        Addressing->TokenBase = ( *Token );

        *Token = ( *Token )->Flink;
        return SPSTATUS_SUCCESS;
    }

    if (( *Token )->TokenType == TokenNumber) {

        //
        //  immediate
        //  mov dword ptr eax, 0x43
        //

        Addressing->TokenDisp = ( *Token );

        *Token = ( *Token )->Flink;
        return SPSTATUS_SUCCESS;
    }

    if (( *Token )->TokenType != TokenLBracket) {

        //
        //  unknown?
        //  mov dword ptr ???
        //

        SpTraceError( *Token, L"unrecognised operand addressing" );
        return SPSTATUS_UNSUCCESSFUL;
    }

    Addressing->TokenLBracket = *Token;

    *Token = ( *Token )->Flink;

    while (( *Token )->TokenType != TokenRBracket) {

        switch (( *Token )->TokenType) {
        case TokenNumber:

            switch (( *Token )->Flink->TokenType) {
            case TokenMul:
                Addressing->TokenScale = *Token;
                Addressing->TokenIndex = ( *Token )->Flink->Flink;

                if (( *Token )->Flink->Flink->Flink->TokenType != TokenRBracket) {

                    *Token = ( *Token )->Flink->Flink->Flink->Flink;
                }
                else {

                    *Token = ( *Token )->Flink->Flink->Flink;
                }

                break;
            case TokenAdd:
                Addressing->TokenDisp = *Token;
                *Token = ( *Token )->Flink->Flink;
                break;
            case TokenRBracket:
                Addressing->TokenDisp = *Token;
                *Token = ( *Token )->Flink;
                break;
            default:
                SpTraceError( ( *Token )->Flink, L"unexpected token" );
                return SPSTATUS_UNSUCCESSFUL;
            }

            break;
        case TokenIdentifier:

            switch (( *Token )->Flink->TokenType) {
            case TokenMul:
                Addressing->TokenIndex = *Token;
                Addressing->TokenScale = ( *Token )->Flink->Flink;

                if (( *Token )->Flink->Flink->Flink->TokenType != TokenRBracket) {

                    *Token = ( *Token )->Flink->Flink->Flink->Flink;
                }
                else {

                    *Token = ( *Token )->Flink->Flink->Flink;
                }

                break;
            case TokenAdd:
                Addressing->TokenBase = ( *Token );
                *Token = ( *Token )->Flink->Flink;
                break;
            case TokenRBracket:
                Addressing->TokenBase = ( *Token );
                *Token = ( *Token )->Flink;
                break;
            default:
                SpTraceError( ( *Token )->Flink, L"unexpected token" );
                return SPSTATUS_UNSUCCESSFUL;
            }

            break;
        default:

            SpTraceError( *Token, L"unexpected token" );
            return SPSTATUS_UNSUCCESSFUL;
        }
    }

    Addressing->TokenRBracket = *Token;
    *Token = ( *Token )->Flink;

    return SPSTATUS_SUCCESS;
}

SPSTATUS
SpParseOperand(
    __in  PSP_SOURCE_CONTEXT    SourceContext,
    __in  PLX_TOKEN*            Token,
    __out PSP_INSTRUCTION_INFO  Instruction )
{
    SPSTATUS spStatus;
    PSP_SYMBOL Symbol;
    PSP_REGISTER_DEF Register;
    SP_ADDRESSING_INFO Addressing;

    RtlZeroMemory( &Addressing, sizeof( SP_ADDRESSING_INFO ) );

    spStatus = SpParseAddressing( Token, &Addressing );
    if (!SP_SUCCESS( spStatus )) {

        return spStatus;
    }

    if (Addressing.OverrideSize) {

        Instruction->OperandSize[ Instruction->OperandCount ] = Addressing.OverrideSize;
    }

    if (Addressing.OverrideSegment) {

        switch (*( ULONG* )Addressing.OverrideSegment->TypeIdentifier.Name) {
        case 's\0f':
            Instruction->SegmentOverride = 0x64;
            break;
        case 's\0g':
            Instruction->SegmentOverride = 0x65;
            break;
        case 's\0e':
            Instruction->SegmentOverride = 0x26;
            break;
        case 's\0s':
            Instruction->SegmentOverride = 0x36;
            break;
        case 's\0d':

            break;
        case 's\0c':
            Instruction->SegmentOverride = 0x2E;
            break;
        default:
            SpTraceError( Addressing.OverrideSegment, L"invalid segment override %.4x", *( ULONG* )Addressing.OverrideSegment->TypeIdentifier.Name );

            return SPSTATUS_UNSUCCESSFUL;
        }
    }

    if (Addressing.TokenLBracket != NULL) {

        Instruction->OperandType[ Instruction->OperandCount ] = OperandModRm;
        Instruction->Has.ModRM = TRUE;
        Instruction->ModRM.Reg = Instruction->ModRM.Rm;
        Instruction->ModRM.Rm = 0;
        Instruction->ModRM.Mod = 0;

        if (Instruction->OperandCount != 0) {

            Instruction->OperandType[ 0 ] = OperandRegister;
        }

        if (Addressing.TokenScale != NULL) {

            //
            //  encodes any scaled index bytes
            //  mov dword ptr [ecx+edx*8], ???
            //

            Instruction->Has.Sib = TRUE;
            Instruction->ModRM.Rm = 0x4;

            switch (Addressing.TokenScale->TypeNumber.Number) {
            case 1:
                Instruction->Sib.Scale = 0;
                break;
            case 2:
                Instruction->Sib.Scale = 1;
                break;
            case 4:
                Instruction->Sib.Scale = 2;
                break;
            case 8:
                Instruction->Sib.Scale = 3;
                break;
            default:
                SpTraceError( Addressing.TokenScale, L"invalid scale size for sib" );
                break;
            }

            if (Addressing.TokenDisp == NULL) {

                Instruction->ModRM.Mod = MOD_SIB_MOD0;
            }
            else {

                Instruction->ModRM.Mod = MOD_SIB_MOD2;
                Instruction->Displacement = Addressing.TokenDisp->TypeNumber.Number;
                Instruction->Has.Displacement32 = TRUE;
            }

            if (Addressing.TokenBase) {

                spStatus = SpIdentifyRegister( Addressing.TokenBase, &Register );

                if (!SP_SUCCESS( spStatus ) || Register == NULL || Register->Size != 32) {

                    SpTraceError( Addressing.TokenBase, L"invalid base register for scaled index byte" );
                    return spStatus;
                }

                Instruction->Sib.Base = ( UCHAR )Register->RegId;
            }
            else {

                Instruction->Sib.Base = 0x5;
            }

            spStatus = SpIdentifyRegister( Addressing.TokenIndex, &Register );

            if (!SP_SUCCESS( spStatus ) || Register == NULL || Register->Size != 32) {

                SpTraceError( Addressing.TokenBase, L"invalid index register for scaled index byte" );
                return spStatus;
            }

            Instruction->Sib.Index = ( UCHAR )Register->RegId;
        }
        else {

            //
            //  mod-rm of the mod-reg-rm
            //

            if (Addressing.TokenBase == NULL) {

                Instruction->ModRM.Rm = 0x5;
                Instruction->ModRM.Mod = 0;
                Instruction->Displacement = Addressing.TokenDisp->TypeNumber.Number;
                Instruction->Has.Displacement32 = TRUE;
            }
            else {

                spStatus = SpIdentifyRegister( Addressing.TokenBase, &Register );

                if (!SP_SUCCESS( spStatus )) {

                    SpTraceError( Addressing.TokenBase, L"invalid base register for mod-rm" );
                    return spStatus;
                }

                if (Register == NULL) {
                    //symbol

                }
                else {

                    if (Register->Size != 32) {

                        SpTraceError( Addressing.TokenBase, L"invalid base register for mod-rm" );
                    }

                    Instruction->ModRM.Rm = ( UCHAR )Register->RegId;
                    Instruction->ModRM.Mod = MOD_REGISTER;

                    if (Addressing.TokenDisp != NULL) {

                        Instruction->ModRM.Mod = (
                            Addressing.TokenDisp->TypeNumber.Number <= 128 &&
                            Addressing.TokenDisp->TypeNumber.Number >= -127 ) ? MOD_1BYTE_DISP : MOD_4BYTE_DISP;
                        Instruction->Displacement = Addressing.TokenDisp->TypeNumber.Number;
                        Instruction->Has.Displacement32 = Instruction->ModRM.Mod == MOD_4BYTE_DISP;
                        Instruction->Has.Displacement8 = !Instruction->Has.Displacement32;
                    }
                }
            }

        }
    }
    else {

        if (Addressing.TokenBase != NULL) {

            spStatus = SpIdentifyRegister( Addressing.TokenBase, &Register );

            if (!SP_SUCCESS( spStatus )) {

                SpTraceError( Addressing.TokenBase, L"invalid register" );
                return spStatus;
            }

            if (Register == NULL) {

                //
                //  symbol
                //

                Symbol = SpFindSymbol( SourceContext, Addressing.TokenBase->TypeIdentifier.Name );

                Instruction->OperandType[ Instruction->OperandCount ] = OperandImmediate;
                Instruction->OperandSize[ Instruction->OperandCount ] = Instruction->OperandSize[ 0 ];
                Instruction->Immediate = Symbol->Value;
                Instruction->Has.Immediate = TRUE;

            }
            else {

                Instruction->OperandType[ Instruction->OperandCount ] = OperandRegister;
                Instruction->Has.ModRM = TRUE;

                if (Instruction->OperandSize[ Instruction->OperandCount ] != 0 &&
                    Instruction->OperandSize[ Instruction->OperandCount ] != Register->Size) {

                    SpTraceWarning( Addressing.TokenBase, L"ignored size override applied to register" );
                }

                Instruction->OperandSize[ Instruction->OperandCount ] = Register->Size;

                if (Instruction->OperandCount == 0) {

                    Instruction->ModRM.Mod = MOD_REGISTER_ADDR;
                    Instruction->ModRM.Rm = ( UCHAR )Register->RegId;
                    Instruction->OperandType[ Instruction->OperandCount ] = OperandModRm;
                }
                else {

                    Instruction->ModRM.Reg = ( UCHAR )Register->RegId;
                }
            }
        }
        else {

            //
            //  immediate
            //  mov dword ptr [ecx+edx*4+4], 0x4
            //

            Instruction->OperandType[ Instruction->OperandCount ] = OperandImmediate;
            Instruction->OperandSize[ Instruction->OperandCount ] = Instruction->OperandSize[ 0 ];
            Instruction->Immediate = Addressing.TokenDisp->TypeNumber.Number;
            Instruction->Has.Immediate = TRUE;
        }
    }

    Instruction->OperandCount++;
    return SPSTATUS_SUCCESS;
}
