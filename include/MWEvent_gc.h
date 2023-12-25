/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __MWEVENT_GC_H__
#define __MWEVENT_GC_H__


/*
 *    includes
 */
#include "trktypes.h"

#ifdef __GNUC__
    #include <ogc/mutex.h>
#endif

#include "trk_prefix.h"


#if defined(HAVE_RVL_SDK)
    #include <revolution/os/OSMutex.h>
#else
    #if defined(__MWERKS__)
        #define OSCond          u64
        #define OSMutex         u32
        #define mutex_t         u32
    #endif
#endif


__extern_c

#if (defined(HAVE_RVL_SDK) || defined(__MWERKS__)) && !defined(__GNUC__)
    extern void OSInitCond(OSCond *cond);
    extern void OSWaitCond(OSCond *cond, OSMutex *mutex);
    extern void OSSignalCond(OSCond *cond);
#endif

    void MWInitializeEvent(void);

#if (defined(HAVE_RVL_SDK) || defined(__MWERKS__)) && !defined(__GNUC__)
    void MWWaitForEvent(OSMutex *mutex);
#else
    void MWWaitForEvent(mutex_t mutex);
#endif

    void MWSetEvent(void);
    void MWResetEvent(void);
    void MWTerminateEvent(void);

__end_extern_c


#endif /* __MWEVENT_GC_H__ */
