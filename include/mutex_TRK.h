/****************************************************************************/
/*

FILE
    mutex_TRK.h

    $Header: /u/debugger/embedded/MetroTRK/Portable/mutex_TRK.h,v 1.1 1999/08/11 21:53:27 harper Exp $

    Synchronization primitives for default implementation.

    Note: this implementation does not provide any functionality.

DESCRIPTION

HISTORY

    97 FEB 12 LLY    Added header block

    $History: mutex_TRK.h $
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 10/06/97   Time: 3:06p
//    Updated in $/Embedded/MetroTRK/Portable
//    Replaced "#pragma once" with #ifdef for compatibility with 3rd-party
//    tools.  Also, renamed "mutex.h" to "mutex_TRK.h" to avoid MSL
//    conflicts.

COPYRIGHT

    (c) 1996-7 Metrowerks Corporation
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MUTEX_TRK_H__
#define __MUTEX_TRK_H__


/*
 *    includes
 */
#include "trktypes.h"
#include "trk_prefix.h"

/*
 *    Error codes
 */
enum
{                                /* DSError; see dserror.h */
    kMutexAcquireErr = 0x0200
};

/*
 *    Types
 */
typedef u32 DSMutex;             /* implementation specific */

/****************************************************************************/

__extern_c

    DSError TRKInitializeMutex(DSMutex *mutex);
    DSError TRKAcquireMutex(DSMutex *mutex);
    DSError TRKReleaseMutex(DSMutex *mutex);

__end_extern_c


#endif /* _MUTEX_TRK_H_ */
