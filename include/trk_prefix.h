/****************************************************************************/
/*

DESCRIPTION

    CodeWarriorTRK prefix header file.  Contains definitions required for
    building CodeWarriorTRK.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __TRK_PREFIX_H__
#define __TRK_PREFIX_H__


#ifndef __cplusplus

#include "trktypes.h"


#if defined(__MWERKS__)
    #define ASM_PREFIX asm
#elif defined(__GNUC__)
    #define ASM_PREFIX
#endif

/*
** CodeWarriorTRK is never built as C++ code, so 'extern C' declarations are
** not needed in the headers.
*/

#define __extern_c
#define __end_extern_c

#else

#undef __extern_c
#undef __end_extern_c

#define __extern_c        extern "C" {
#define __end_extern_c    }

#endif /* #ifndef __CPLUSPLUS__ */

__extern_c

#ifdef FIX_FUNCTION_PROTOTYPES
    extern void OSReport(const s8 *msg, ...);
#endif

    extern void TRK_board_display(s8 *);

__end_extern_c


#endif // __TRK_PREFIX_H__
