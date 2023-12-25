/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED

*/
/****************************************************************************/


#include "asm.h"


.text


#ifdef __GNUC__

.extern  _db_stack_addr
.extern TRK_main
.extern TRKSaveExtended1Block
.extern gTRKCPUState
.extern InitMetroTRKCommTable


.globl InitMetroTRK
InitMetroTRK:
    subi      r1, r1, 4
    stw       r3, 0(r1)
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l
    stmw      r0, 0(r3)
    lwz       r4, 0(r1)
    addi      r1, r1, 4
    stw       r1, 4(r3)
    stw       r4, 0xC(r3)
    mfspr     r4, LR
    stw       r4, 0x84(r3)
    stw       r4, 0x80(r3)
    mfcr      r4
    stw       r4, 0x88(r3)
    mfmsr     r4
    ori       r3, r4, 0x8000
    xori      r3, r3, 0x8000
    mtmsr     r3
    mtspr     SRR1, r4
    bl        TRKSaveExtended1Block
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l

    # For some reason, this is problematic to GCC:
#    lmw       r0, 0(r3)

    li        r0, 0

    # Instruction address breakpoint register
    mtspr     1010, r0

    # Data address breakpoint register
    mtspr     1013, r0

    lis       r1, _db_stack_addr@h
    ori       r1, r1, _db_stack_addr@l
    mr        r3, r5
    bl        InitMetroTRKCommTable
    cmpwi     r3, 1
    bne       main
    lwz       r4, 0x84(r3)
    mtlr      r4

    # For some reason, this is problematic to GCC:
#    lmw       r0, 0(r3)

    blr

main:

    b         TRK_main
    blr



# NEVER USED
.globl InitMetroTRK_BBA
InitMetroTRK_BBA:
    addi      r1, r1, -4
    stw       r3, 0(r1)
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l
    stmw      r0, 0(r3)
    lwz       r4, 0(r1)
    addi      r1, r1, 4
    stw       r1, 4(r3)
    stw       r4, 0xC(r3)
    mflr      r4
    stw       r4, 0x84(r3)
    stw       r4, 0x80(r3)
    mfcr      r4
    stw       r4, 0x88(r3)
    mfmsr     r4
    ori       r3, r4, 0x8000
    mtmsr     r3
    mtsrr1    r4
    bl        TRKSaveExtended1Block
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l

    # For some reason, this is problematic to GCC:
#    lmw       r0, 0(r3)

    li        r0, 0

    # Instruction address breakpoint register
    mtspr     1010, r0

    # Data address breakpoint register
    mtspr     1013, r0

    lis       r1, _db_stack_addr@h
    ori       r1, r1, _db_stack_addr@l
    li        r3, 2
    bl        InitMetroTRKCommTable
    cmpwi     r3, 1
    bne       main_TRK
    lwz       r4, 0x84(r3)
    mtlr      r4

    # For some reason, this is problematic to GCC:
#    lmw       r0, 0(r3)

    blr

main_TRK:

    b         TRK_main
    blr

#endif
