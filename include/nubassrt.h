/****************************************************************************/
/*

DESCRIPTION

    Standalone assertions for the CodeWarrior Debugger Server for
    embedded products (TRK).

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __NUBASSRT_H__
#define __NUBASSRT_H__


#include "trktypes.h"
#include "usr_put.h"
#include "trk_prefix.h"


/*
** Undefine DEBUG_ASSERT to 1 in order to get ASSERTS.  Normally this
** would be defined to 0 in order to improve code size and speed.
** At the moment it is automatically set to 1 when debug output is
** enabled (see usr_put.h).
*/


#undef ASSERT
#undef FAIL


#ifndef DEBUG_ASSERT

    #define ASSERT(condition) ((void) 0)
    #define FAIL(condition)   ((void) 0)

#else

    #define ASSERT(condition) ((condition) ? ((void) 0) : TRKNubAssertionFailed(#condition, __FILE__, __LINE__, FALSE))
    #define FAIL(condition)   ((condition) ? ((void) 0) : TRKNubAssertionFailed(#condition, __FILE__, __LINE__, TRUE))

#endif /* #if !DEBUG_ASSERT */

__extern_c

    void TRKNubAssertionFailed(s8 *condition, s8 *filename, s32 lineno, BOOL die);

__end_extern_c


#endif /* __NUBASSRT_H__ */
