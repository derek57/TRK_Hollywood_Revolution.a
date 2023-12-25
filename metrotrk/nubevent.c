/****************************************************************************/
/*

FILE

    nubevent.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/nubevent.c,v 1.2 2000/04/27 02:33:27 smoore Exp $

DESCRIPTION

    Event queue.

HISTORY

    $History: nubevent.c $
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 11/07/97   Time: 5:17p
//    Updated in $/Embedded/MetroTRK/Portable
//    Added EOL at end of file.

AUTHORS

    Lawrence You <you@metrowerks.com>

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


/*
    CWDS includes
*/
#include "nubevent.h"
#include "mem_TRK.h"

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #include "mutex_TRK.h"
#endif


/****************************************************************************/
/*
 *    Local Types and Variables
 */
/****************************************************************************/

/*
** The minimum number of events in the system is 2.  However, if
** multiple threads can generate events simultaneously, then the
** number should be (1 + <max simultaneous thread events>).
**
** If a particular target requires more than 2, it should define
** TRK_EXTRA_EVENT_COUNT in target.h.
*/

#ifndef TRK_EXTRA_EVENT_COUNT
    #define TRK_EXTRA_EVENT_COUNT 0
#endif

#define NUBQUEUELENGTH    (2 + TRK_EXTRA_EVENT_COUNT)


const NubEventId kTRKFirstEventID = 0x100;

typedef struct EventQueue
{
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSMutex    fMutex;
#endif

    s32        fCount;          /* "active" entries in the queue */
    s32        fFirst;          /* current item in list */
    NubEvent   fEventList[NUBQUEUELENGTH];
    NubEventId fEventID;        /* next ID to be used */
} EventQueue;

static EventQueue gTRKEventQueue;


#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    #if !defined(DS_MALLOC_MAPPING) || (DS_MALLOC_MAPPING == DS_MALLOC_NOT_AVAILABLE)
        #define DS_DYNAMIC_EVENT_QUEUE 0
    #else
        #define DS_DYNAMIC_EVENT_QUEUE 1
    #endif
#endif


/****************************************************************************/
/*
 *    TRKInitializeEventQueue
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKInitializeEventQueue(void)
{
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKInitializeMutex(&gTRKEventQueue.fMutex);
    TRKAcquireMutex(&gTRKEventQueue.fMutex);
#endif

    gTRKEventQueue.fCount = 0;
    gTRKEventQueue.fFirst = 0;
    gTRKEventQueue.fEventID = kTRKFirstEventID;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKReleaseMutex(&gTRKEventQueue.fMutex);
#endif

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKCopyEvent
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKCopyEvent(NubEvent *dstEvent, const NubEvent *srcEvent)
{
#ifdef DEBUG_ASSERT
    ASSERT((dstEvent != NULL) && (srcEvent != NULL));
#endif

    TRK_memcpy(dstEvent, srcEvent, sizeof(*dstEvent));
}


/****************************************************************************/
/*
 *    TRKGetNextEvent
 *
 *    100 % DONE
 */
/****************************************************************************/
BOOL TRKGetNextEvent(NubEvent *resultEvent)
{
    BOOL err = FALSE;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKAcquireMutex(&gTRKEventQueue.fMutex);
#endif

#ifdef DEBUG_ASSERT
    ASSERT((gTRKEventQueue.fCount >= 0) && (gTRKEventQueue.fCount <= NUBQUEUELENGTH));
    ASSERT((gTRKEventQueue.fFirst >= 0) && (gTRKEventQueue.fFirst < NUBQUEUELENGTH));
#endif

    if (gTRKEventQueue.fCount > 0)
    {
        TRKCopyEvent(resultEvent, &gTRKEventQueue.fEventList[gTRKEventQueue.fFirst]);

        gTRKEventQueue.fCount--;
        gTRKEventQueue.fFirst++;

        if (gTRKEventQueue.fFirst == NUBQUEUELENGTH)
            gTRKEventQueue.fFirst = 0;

        err = TRUE;
    }

#ifdef DEBUG_ASSERT
    ASSERT((gTRKEventQueue.fCount >= 0) && (gTRKEventQueue.fCount < NUBQUEUELENGTH));
    ASSERT((gTRKEventQueue.fFirst >= 0) && (gTRKEventQueue.fFirst < NUBQUEUELENGTH));
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKReleaseMutex(&gTRKEventQueue.fMutex);
#endif

    return err;
}


/****************************************************************************/
/*
 *    TRKPostEvent
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKPostEvent(const NubEvent *copiedEvent)
{
    DSError err = kNoError;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKAcquireMutex(&gTRKEventQueue.fMutex);
#endif

#ifdef DEBUG_ASSERT
    ASSERT((gTRKEventQueue.fCount >= 0) && (gTRKEventQueue.fCount <= NUBQUEUELENGTH));
    ASSERT((gTRKEventQueue.fFirst >= 0) && (gTRKEventQueue.fFirst < NUBQUEUELENGTH));
#endif

    if (gTRKEventQueue.fCount == NUBQUEUELENGTH)
    {
        err = kEventQueueFull;
        OSReport("MetroTRK - Event Queue full\n");
    }
    else
    {
        /* copy event to next slot in event queue */

        s32 nextEntry = ((gTRKEventQueue.fFirst + gTRKEventQueue.fCount) % NUBQUEUELENGTH);

        /* set the ID for the event in the queue */

        TRKCopyEvent(&gTRKEventQueue.fEventList[nextEntry], copiedEvent);
        gTRKEventQueue.fEventList[nextEntry].fId = gTRKEventQueue.fEventID;

        gTRKEventQueue.fEventID++;

        if (gTRKEventQueue.fEventID < kTRKFirstEventID)
            gTRKEventQueue.fEventID = kTRKFirstEventID;

        /* increment count of entries in the queue */

        gTRKEventQueue.fCount++;
    }

#ifdef DEBUG_ASSERT
    ASSERT((gTRKEventQueue.fCount) > 0 && (gTRKEventQueue.fCount <= NUBQUEUELENGTH));
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    TRKReleaseMutex(&gTRKEventQueue.fMutex);
#endif

    return err;
}


/****************************************************************************/
/*
 *    TRKConstructEvent
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKConstructEvent(NubEvent *newEvent, NubEventType type)
{
#ifdef DEBUG_ASSERT
    ASSERT(newEvent != NULL);
#endif

    newEvent->fType = type;
    newEvent->fId = kInvalidEventId;
    newEvent->fMessageBufferId = kInvalidMessageBufferId;

    return;
}

/****************************************************************************/
/*
 *    TRKDestructEvent
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKDestructEvent(NubEvent *oldEvent)
{
    TRK_ReleaseBuffer(oldEvent->fMessageBufferId);

    return;
}
