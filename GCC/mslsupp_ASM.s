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

.extern tmpnam
.extern GetTRKConnected
.extern TRKOpenFile
.extern _stringBase0
.extern convertFileMode


.globl __open_temp_file
__open_temp_file:
    stwu      r1, -0x20(r1)
    mflr      r0
    lis       r3, _stringBase0@ha
    stw       r0, 0x24(r1)
    addi      r3, r3, _stringBase0@l
    stw       r31, 0x1C(r1)
    bl        tmpnam
    cmpwi     r3, 0
    mr        r31, r3
    bne       loc_44C
    li        r3, 1
    b         loc_4CC

loc_44C:

    li        r0, 3
    lwz       r4, 0xC(r1)
    insrwi    r4, r0, 3,2
    li        r3, 1
    li        r0, 2
    insrwi    r4, r0, 2,0
    rlwinm    r0, r4, 0,7,4
    insrwi    r0, r3, 3,7
    oris      r0, r0, 8
    stw       r0, 0x8(r1)
    bl        GetTRKConnected
    cmpwi     r3, 0
    bne       loc_488
    li        r3, 1
    b         loc_4CC

loc_488:

    addi      r3, r1, 0x8
    bl        convertFileMode
    clrlwi    r5, r3, 24
    mr        r4, r31
    li        r3, 0xD2
    li        r6, 0
    bl        TRKOpenFile
    clrlwi.   r0, r3, 24
    beq       loc_4B8
    cmpwi     r0, 2
    beq       loc_4C0
    b         loc_4C8

loc_4B8:

    li        r3, 0
    b         loc_4CC

loc_4C0:

    li        r3, 2
    b         loc_4CC

loc_4C8:

    li        r3, 1

loc_4CC:

    lwz       r0, 0x24(r1)
    lwz       r31, 0x1C(r1)
    mtlr      r0
    addi      r1, r1, 0x20
    blr

#endif
