/****************************************************************************/
/*

DESCRIPTION

    Standalone Standard C Library Implementation for string operations.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __STRING_TRK_H__
#define __STRING_TRK_H__


#include "trk_prefix.h"


//#define SUPPRESS_WELCOME_MESSAGE    (1)


__extern_c

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #ifndef SUPPRESS_WELCOME_MESSAGE
        s8 *TRK_strcat(s8 *dst, const s8 *src);
    #endif
#endif

    size_t TRK_strlen(const s8 *str);

__end_extern_c


#endif /* __STRING_TRK_H__ */
