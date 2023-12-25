/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/flush_cache.h,v 1.2 2006/07/11 10:01:29 r04474 Exp $

DESCRIPTION

     For the given memory range, forces all modified data cache contents to be
     written to memory and invalidates the associated instruction cache
     entries.

HISTORY

    $History: flush_cache.h $

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2006
*/
/****************************************************************************/


#ifndef __FLUSH_CACHE_H__
#define __FLUSH_CACHE_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

    ASM_PREFIX void TRK_flush_cache(u32 address, u32 size);

__end_extern_c


#endif // __FLUSH_CACHE_H__
