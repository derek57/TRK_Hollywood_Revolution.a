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


#ifdef __GNUC__
    #include <ogc/mutex.h>
#endif

#include "dserror.h"
#include "msg.h"
#include "msgxtrct.h"
#include "MWMutex_gc.h"


/****************************************************************************/
/*
 *    MWInitializeMutex
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
DSError MWInitializeMutex(mutex_t *mutex)
{
#if 0
#if defined(__MWERKS__)
    DSFetch_u32(mutex) = 0;

    return kInternalError;
#elif defined(__GNUC__)
    return LWP_MutexInit(mutex, FALSE);
#endif
#else
    DSFetch_u32(mutex) = 0;

    return kInternalError;
#endif
}

/****************************************************************************/
/*
 *    MWLockMutex
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
DSError MWLockMutex(mutex_t mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
#if 0
    return LWP_MutexLock(mutex);
#else
    UNUSED(mutex);
#endif
#endif

    return kInternalError;
}

/****************************************************************************/
/*
 *    MWUnlockMutex
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWUnlockMutex(mutex_t mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
#if 0
    LWP_MutexUnlock(mutex);
#else
    UNUSED(mutex);
#endif
#endif

    return;
}

/****************************************************************************/
/*
 *    MWTerminateMutex
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWTerminateMutex(mutex_t mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
#if 0
    LWP_MutexDestroy(mutex);
#else
    UNUSED(mutex);
#endif
#endif

    return;
}
