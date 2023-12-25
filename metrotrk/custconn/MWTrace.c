/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases.



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#ifdef __MWERKS__


#if 1 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV

#include "trk_prefix.h"
#include "MWTrace.h"


// MOVED TO MWTRACE_ASM.S FOR GCC
/****************************************************************************/
/*
 *    MWTRACE_FUNC
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
ASM_PREFIX void MWTRACE_FUNC()
{
    stwu      r1, -0x70(r1)
    bne       cr1, loc_28
    stfd      f1, 0x28(r1)
    stfd      f2, 0x30(r1)
    stfd      f3, 0x38(r1)
    stfd      f4, 0x40(r1)
    stfd      f5, 0x48(r1)
    stfd      f6, 0x50(r1)
    stfd      f7, 0x58(r1)
    stfd      f8, 0x60(r1)

loc_28:

    stw       r3, 8(r1)
    stw       r4, 0xC(r1)
    stw       r5, 0x10(r1)
    stw       r6, 0x14(r1)
    stw       r7, 0x18(r1)
    stw       r8, 0x1C(r1)
    stw       r9, 0x20(r1)
    stw       r10, 0x24(r1)
    addi      r1, r1, 0x70
    blr
}

#endif

#endif
