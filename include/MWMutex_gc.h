/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __MWMUTEX_GC_H__
#define __MWMUTEX_GC_H__


/*
 *    includes
 */
#include "trktypes.h"

#ifdef __GNUC__
    #include <ogc/mutex.h>
#endif

#include "MWEvent_gc.h"
#include "trk_prefix.h"


__extern_c

    DSError MWInitializeMutex(mutex_t *mutex);
    DSError MWLockMutex(mutex_t mutex);
    void MWUnlockMutex(mutex_t mutex);
    void MWTerminateMutex(mutex_t mutex);

__end_extern_c


#endif /* __MWMUTEX_GC_H__ */
