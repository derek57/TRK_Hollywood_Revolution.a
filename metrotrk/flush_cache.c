/****************************************************************************/
/*

FILE

    flush_cache.c

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/flush_cache.c,v 1.2 2006/07/11 10:01:29 r04474 Exp $

DESCRIPTION

    For the given memory range, forces all modified data cache contents to be
    written to memory and invalidates the associated instruction cache
    entries.

HISTORY

    $History: flush_cache.c $

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2006

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "flush_cache.h"
#include "proc_mpc_7xx_603e.h"


/****************************************************************************/
/*
 *    TRK_flush_cache
 *
 *    For the given memory range, forces all modified data cache contents to be
 *    written to memory and invalidates the associated instruction cache
 *    entries.
 *
 *    100 % DONE
 */
/****************************************************************************/

// MOVED TO FLUSH_CACHE_ASM.S FOR GCC
#ifdef __MWERKS__

ASM_PREFIX void TRK_flush_cache(u32, u32)
{
    // r3 is Address, r4 is size
    nofralloc

#if __PPC_DCACHE__ || __PPC_ICACHE__

    lis       r5, 0xffff
    ori       r5, r5, 0xfff1
    and       r5, r5, r3                // make address 8-byte aligned
    subf      r3, r5, r3                // get unaligned-aligned difference
    add       r4, r4, r3                // increase size accordingly

rept:

#if __PPC_DCACHE__
    dcbst     0, r5
    dcbf      0, r5
#endif

    sync

#if __PPC_ICACHE__
    icbi      0, r5
#endif

    addic     r5, r5, 0x8
    subic.    r4, r4, 0x8
    bge       rept
    isync
    blr

#endif
}

#endif
