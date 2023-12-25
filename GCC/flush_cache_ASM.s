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

.globl TRK_flush_cache
TRK_flush_cache:

    lis       r5, 0xffff
    ori       r5, r5, 0xfff1
    and       r5, r5, r3                # make address 8-byte aligned
    subf      r3, r5, r3                # get unaligned-aligned difference
    add       r4, r4, r3                # increase size accordingly

rept:

    dcbst     0,r5
    dcbf      0,r5

    sync

    icbi      0,r5

    addic     r5,r5,0x8
    subic.    r4,r4,0x8
    bge       rept
    isync

    blr

#endif
