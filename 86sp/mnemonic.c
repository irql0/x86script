


#include "sp.h"
// mnemonic, op1, op2, {optypes}, {opsizes}, modrm(or), {prefixes}
SP_INSTRUCTION_DEF g_x86[ ] = {
    //
    //  instructions which are just swapped d bit's, 
    //  do not need to be on this list.
    //  s bit's however, should be.
    //

    { L"mov", 0xC6, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x00, {0} },
    { L"mov", 0xC7, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x00, {0} },
    { L"mov", 0x88, 0x00, {OperandModRm, OperandRegister}, {8, 8}, 0x00, {0} },
    { L"mov", 0x89, 0x00, {OperandModRm, OperandRegister}, {32, 32}, 0x00, {0} },
    { L"inc", 0xFF, 0x00, {OperandModRm}, {32}, 0x00, {0} },
    { L"dec", 0xFF, 0x00, {OperandModRm}, {32}, 0x08, {0} },
    { L"call", 0xFF, 0x00, {OperandModRm}, {32}, 0x10, {0} },
    { L"jmp", 0xFF, 0x00, {OperandModRm}, {32}, 0x20, {0} },
    { L"push", 0xFF, 0x00, {OperandModRm}, {32}, 0x30, {0} },
    { L"call", 0xE8, 0x00, {OperandRelative}, {32}, 0x00, {0} },
    { L"jmp", 0xE9, 0x00, {OperandRelative}, {32}, 0x00, {0} },

    { L"jo", 0x80, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jno", 0x81, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jb", 0x82, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jnae", 0x82, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jc", 0x82, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jnb", 0x83, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jae", 0x83, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jnc", 0x83, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jz", 0x84, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"je", 0x84, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jnz", 0x85, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jne", 0x85, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jbe", 0x86, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jna", 0x86, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jnbe", 0x87, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"ja", 0x87, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"js", 0x88, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jns", 0x89, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jp", 0x8A, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jpe", 0x8A, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jnp", 0x8B, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jpo", 0x8B, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jl", 0x8C, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jnge", 0x8C, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jge", 0x8D, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jle", 0x8E, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jng", 0x8E, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jnl", 0x8D, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"jnle", 0x8F, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },
    { L"jg", 0x8F, 0x00, {OperandRelative}, {32}, 0x00, {0x0F} },

    { L"ret", 0xC3, 0x00, {OperandInvalid}, {0}, 0x00, {0} },
    { L"push", 0x68, 0x00, {OperandImmediate}, {32}, 0x00, {0} },
    { L"pop", 0x8F, 0x00, {OperandModRm}, {32}, 0x00, {0} },

    { L"int", 0xCD, 0x00, {OperandImmediate}, {8}, 0x00, {0} },
    { L"int3", 0xCC, 0x00, {OperandImmediate}, {8}, 0x00, {0} },

    { L"add", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x00, {0} },
    { L"or", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x08, {0} },
    { L"adc", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x10, {0} },
    { L"sbb", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x18, {0} },
    { L"and", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x20, {0} },
    { L"sub", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x28, {0} },
    { L"xor", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x30, {0} },
    { L"cmp", 0x80, 0x00, {OperandModRm, OperandImmediate}, {8, 8}, 0x38, {0} },

    { L"add", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x00, {0} },
    { L"or", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x08, {0} },
    { L"adc", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x10, {0} },
    { L"sbb", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x18, {0} },
    { L"and", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x20, {0} },
    { L"sub", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x28, {0} },
    { L"xor", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x30, {0} },
    { L"cmp", 0x81, 0x00, {OperandModRm, OperandImmediate}, {32, 32}, 0x38, {0} },

    { L"lea", 0x8D, 0x00, {OperandRegister, OperandModRm}, {32}, 0x00, {0}},

    { L"cmp", 0x38, 0x00, {OperandModRm, OperandRegister}, {8, 8}, 0x00, {0}},
    { L"cmp", 0x39, 0x00, {OperandModRm, OperandRegister}, {32, 32}, 0x00, {0}},



};

SPSTATUS
SpParseMnemonic(
    __in PSP_SOURCE_CONTEXT     SourceContext,
    __in PLX_TOKEN              Token,
    __in PSP_INSTRUCTION_INFO   Instruction )
{
    //
    //  if an operand is an immediate, it can be sign extended to
    //  any size.
    //

    //
    //  16 bit is just, 32 bit with a prefix, almost any 32 bit instruction
    //  can become 16 bit with this prefix, so we extend
    //

    for (ULONG32 Op = 0; Op < 4; Op++) {

        if (Instruction->OperandSize[ Op ] == 16) {

            Op = 0;
            while (Op < 4) {

                if (Instruction->OperandSize[ Op ] == 16) {

                    Instruction->OperandSize[ Op ] = 32;
                }
                Op++;
            }
            Instruction->Has.OperandSizePrefix = TRUE;
            break;
        }
    }

    for (ULONG CurrentInstruction = 0;
        CurrentInstruction < sizeof( g_x86 ) / sizeof( SP_INSTRUCTION_DEF );
        CurrentInstruction++) {

        if (( g_x86[ CurrentInstruction ].OperandSize[ 0 ] != Instruction->OperandSize[ 0 ] && Instruction->OperandType[ 0 ] != OperandImmediate ) ||
            ( g_x86[ CurrentInstruction ].OperandSize[ 1 ] != Instruction->OperandSize[ 1 ] && Instruction->OperandType[ 1 ] != OperandImmediate ) ||
            ( g_x86[ CurrentInstruction ].OperandSize[ 2 ] != Instruction->OperandSize[ 2 ] && Instruction->OperandType[ 2 ] != OperandImmediate ) ||
            ( g_x86[ CurrentInstruction ].OperandSize[ 3 ] != Instruction->OperandSize[ 3 ] && Instruction->OperandType[ 3 ] != OperandImmediate )) {

            continue;
        }

        if (lstrcmpiW( g_x86[ CurrentInstruction ].Mnemonic, Token->TypeIdentifier.Name ) != 0) {

            continue;
        }

        if (( g_x86[ CurrentInstruction ].OperandType[ 0 ] == Instruction->OperandType[ 0 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 1 ] == Instruction->OperandType[ 1 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 2 ] == Instruction->OperandType[ 2 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 3 ] == Instruction->OperandType[ 3 ] ) || (
            g_x86[ CurrentInstruction ].OperandType[ 0 ] == OperandModRm &&
            g_x86[ CurrentInstruction ].OperandType[ 0 ] == Instruction->OperandType[ 1 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 1 ] == Instruction->OperandType[ 0 ] ) || (
            g_x86[ CurrentInstruction ].OperandType[ 0 ] == OperandRelative &&
            Instruction->OperandType[ 0 ] == OperandImmediate &&
            g_x86[ CurrentInstruction ].OperandType[ 1 ] == Instruction->OperandType[ 1 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 2 ] == Instruction->OperandType[ 2 ] &&
            g_x86[ CurrentInstruction ].OperandType[ 3 ] == Instruction->OperandType[ 3 ] )) {

            if (g_x86[ CurrentInstruction ].OperandType[ 0 ] == OperandRelative &&
                Instruction->OperandType[ 0 ] == OperandImmediate) {

                if (g_x86[ CurrentInstruction ].Prefix[ 0 ] != 0) {

                    Instruction->Immediate--;

                    if (g_x86[ CurrentInstruction ].Prefix[ 1 ] != 0) {

                        Instruction->Immediate--;

                        if (g_x86[ CurrentInstruction ].Prefix[ 2 ] != 0) {

                            Instruction->Immediate--;

                            if (g_x86[ CurrentInstruction ].Prefix[ 3 ] != 0) {

                                Instruction->Immediate--;
                            }
                        }
                    }
                }


                if (g_x86[ CurrentInstruction ].Opcode1 != 0) {

                    Instruction->Immediate--;
                }

                if (g_x86[ CurrentInstruction ].Opcode2 != 0) {

                    Instruction->Immediate--;
                }

                if (Instruction->Has.ModRM) {

                    Instruction->Immediate--;
                }

                if (Instruction->Has.Sib) {

                    Instruction->Immediate--;
                }

                if (Instruction->Has.Displacement32) {

                    Instruction->Immediate -= 4;
                }

                if (Instruction->Has.Displacement8) {

                    Instruction->Immediate--;
                }

                if (Instruction->Has.Immediate) {

                    Instruction->Immediate -= ( g_x86[ CurrentInstruction ].OperandSize[ 0 ] / 8 );
                }

                Instruction->Immediate = Instruction->Immediate - SourceContext->Base - SourceContext->BufferIndex;
            }

            if (Instruction->SegmentOverride != 0) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->SegmentOverride;
            }

            if (Instruction->Has.OperandSizePrefix) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = 0x66;
            }

            if (g_x86[ CurrentInstruction ].Prefix[ 0 ] != 0) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Prefix[ 0 ];

                if (g_x86[ CurrentInstruction ].Prefix[ 1 ] != 0) {

                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Prefix[ 1 ];

                    if (g_x86[ CurrentInstruction ].Prefix[ 2 ] != 0) {

                        SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Prefix[ 2 ];

                        if (g_x86[ CurrentInstruction ].Prefix[ 3 ] != 0) {

                            SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Prefix[ 3 ];
                        }
                    }
                }
            }

            if (g_x86[ CurrentInstruction ].OperandType[ 0 ] == OperandModRm &&
                g_x86[ CurrentInstruction ].OperandType[ 0 ] == Instruction->OperandType[ 1 ]) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Opcode1 | 2;
            }
            else {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Opcode1;
            }

            if (g_x86[ CurrentInstruction ].Opcode2) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = g_x86[ CurrentInstruction ].Opcode2;
            }

            if (Instruction->Has.ModRM) {

                Instruction->ModRM.Byte |= g_x86[ CurrentInstruction ].ModRM;
                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->ModRM.Byte;
            }

            if (Instruction->Has.Sib) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Sib.Byte;
            }

            if (Instruction->Has.Displacement32) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Displacement & 0xFF;
                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Displacement >> 8 & 0xFF;
                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Displacement >> 16 & 0xFF;
                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Displacement >> 24 & 0xFF;
            }

            if (Instruction->Has.Displacement8) {

                SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Displacement & 0xFF;
            }

            if (Instruction->Has.Immediate) {
                ULONG Op = 0;

                while (Op < 4) {

                    if (Instruction->OperandType[ Op ] == OperandImmediate) {

                        break;
                    }

                    Op++;
                }

                if (g_x86[ CurrentInstruction ].OperandSize[ Op ] == 32) {

                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate & 0xFF;
                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate >> 8 & 0xFF;
                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate >> 16 & 0xFF;
                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate >> 24 & 0xFF;
                }
                else if (g_x86[ CurrentInstruction ].OperandSize[ Op ] == 16) {

                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate & 0xFF;
                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate >> 8 & 0xFF;
                }
                else {

                    SourceContext->Buffer[ SourceContext->BufferIndex++ ] = Instruction->Immediate & 0xFF;
                }
            }

            return SPSTATUS_SUCCESS;
        }

    }

    return SPSTATUS_UNSUCCESSFUL;
}

