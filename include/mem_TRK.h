/****************************************************************************/
/*

DESCRIPTION

    Standalone Standard C Library Implementation for memory operations.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MEM_TRK_H__
#define __MEM_TRK_H__


#include "trk_prefix.h"


__extern_c

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #ifdef __POWERPC__
        #pragma push
        #pragma section code_type ".init" data_mode=far_abs code_mode=far_abs
    #endif
#endif

    void *TRK_memset(void *dst, s32 val, size_t len);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #ifdef __POWERPC__
        #pragma pop
    #endif
#endif

    void *TRK_memcpy(void *dst, const void *src, size_t len);
    void *TRK_memmove(void *dst, const void *src, size_t len);

__end_extern_c


/*
 * Macros which map the TRK_ allocation routines to actual functions:
 * ANSI ones, custom ones, or none for platforms where dyanmic memory
 * is not available. Projects that are for platforms that do support
 * dynamic memory need to define DS_MALLOC_MAPPING accordingly (preferably
 * in their prefix file, but at a minimum, before the include of this
 * header file).
 */

#define DS_MALLOC_NOT_AVAILABLE        1
#define DS_MALLOC_ANSI                 2
#define DS_MALLOC_CUSTOM               3

#if !defined(DS_MALLOC_MAPPING) || (DS_MALLOC_MAPPING == DS_MALLOC_NOT_AVAILABLE)
    #define TRK_malloc(s)              (NULL)
    #define TRK_calloc(c, s)           (NULL)
    #define TRK_realloc(p, s)          (NULL)
    #define TRK_free(p)
#elif DS_MALLOC_MAPPING == DS_MALLOC_ANSI
    #define TRK_malloc(s)              malloc((s))
    #define TRK_calloc(c, s)           calloc((c), (s))
    #define TRK_realloc(p, s)          realloc((p), (s))
    #define TRK_free(p)                free(p)
#elif DS_MALLOC_MAPPING == DS_MALLOC_CUSTOM
    /* project is providing it's own mapping of these macros in, probably, a prefix file */
#else
    #error unexpected DS_MALLOC_MAPPING value
#endif


#endif /* __MEM_TRK_H__ */
