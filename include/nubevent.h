/****************************************************************************/
/*

DESCRIPTION

    Event queue.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __NUBEVENT_H__
#define __NUBEVENT_H__


/****************************************************************************/
/*
 *    Includes
 */
/****************************************************************************/
#include "trktypes.h"
#include "msgbuf.h"
#include "trk_prefix.h"


enum
{                                  /* DSError; see dserror.h */
    kEventQueueFull = 0x0100
};

/****************************************************************************/
/*
 *    Types
 */
/****************************************************************************/

/****************************************************************************/
typedef u32 NubEventId;            /* unique ID for nub event */

enum
{
    kInvalidEventId = 0            /* NubEventId */
};

/****************************************************************************/
typedef enum NubEventType
{
    kNullEvent,                    /* an event with no data */
    kShutdownEvent,                /* shutdown event */
    kRequestEvent,                 /* packet from host */
    kBreakpointEvent,              /* breakpoint was hit */
    kExceptionEvent,               /* non-breakpoint exception */
    kSupportEvent,                 /* target support services */
    kContinueEvent                 /* continue executing a target process event */
} NubEventType;

/****************************************************************************/
typedef struct NubEvent
{
    NubEventType        fType;
    NubEventId          fId;
    MessageBufferId     fMessageBufferId;
} NubEvent;

/****************************************************************************/
/*
 *    Function Prototypes
 */
/****************************************************************************/
__extern_c

    DSError TRKInitializeEventQueue(void);

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    DSError TerminateEventQueue();
#endif

    BOOL TRKGetNextEvent(NubEvent *copiedEvent);
    DSError TRKPostEvent(const NubEvent *copiedEvent);
    void TRKCopyEvent(NubEvent *dstEvent, const NubEvent *srcEvent);

    void TRKConstructEvent(NubEvent *newEvent, NubEventType type);
    void TRKDestructEvent(NubEvent *oldEvent);

__end_extern_c


#endif /* __NUBEVENT_H__ */
