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


#include "MWEvent_gc.h"

#ifdef __GNUC__
    #include <ogc/cond.h>
#endif


#if 1 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV

#if defined(__MWERKS__)
    OSCond condition;
#elif defined(__GNUC__)
    cond_t condition;
#endif


/****************************************************************************/
/*
 *    MWInitializeEvent
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWInitializeEvent(void)
{
#if defined(__MWERKS__)
    OSInitCond(&condition);
#elif defined(__GNUC__)
    LWP_CondInit(&condition);
#endif

    return;
}

/****************************************************************************/
/*
 *    MWWaitForEvent
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
#if (defined(HAVE_RVL_SDK) || defined(__MWERKS__)) && !defined(__GNUC__)
void MWWaitForEvent(OSMutex *mutex)
#else
void MWWaitForEvent(mutex_t mutex)
#endif
{
#if defined(__MWERKS__)
    OSWaitCond(&condition, mutex);
#elif defined(__GNUC__)
    LWP_CondWait(condition, mutex);
#endif

    return;
}

/****************************************************************************/
/*
 *    MWSetEvent
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWSetEvent(void)
{
#if defined(__MWERKS__)
    OSSignalCond(&condition);
#elif defined(__GNUC__)
    LWP_CondSignal(condition);
#endif

    return;
}

/****************************************************************************/
/*
 *    MWResetEvent
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWResetEvent(void)
{
    return;
}

/****************************************************************************/
/*
 *    MWTerminateEvent
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void MWTerminateEvent(void)
{
#if 0
#if defined(__GNUC__)
    LWP_CondDestroy(condition);
#endif
#endif

    return;
}

#endif
