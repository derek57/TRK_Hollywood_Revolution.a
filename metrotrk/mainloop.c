/****************************************************************************/
/*

FILE

    mainloop.c

    $Source: /u/debugger/embedded/CodeWarriorTRK/Portable/mainloop.c,v $

DESCRIPTION

    Main loop for TRK.

    Repeatedly handle host- and target-generated events.

HISTORY

    97 MAR 01 LLY    Removed #include __config.h

    $History: mainloop.c $
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 6/25/99    Time: 4:47p
//    Updated in $/Embedded/CodeWarriorTRK/Portable
//       - Moved all reset-handling code into a separate section
//         so that the main body of TRK code can be copied to RAM.
//       - Added a new self-debugging mode which periodically dumps
//         debug memory buffer contents to the debugger console window.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/CodeWarriorTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.

COPYRIGHT

    (c) 1997-2002 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "trktypes.h"
#include "dispatch.h"
#include "targimpl.h"
#include "serpoll.h"
#include "mainloop.h"


#define DEBUG_MAINLOOP  0


#if TRK_TRANSPORT_INT_DRIVEN
    extern volatile u8 *gTRKInputPendingPtr;
#endif


/******************************************************************************/
/*
 *    TRK_HandleRequestEvent
 *
 *    100 % DONE
 */
/******************************************************************************/
void TRK_HandleRequestEvent(NubEvent *event)
{
    MessageBuffer *buffer = TRKGetBuffer(event->fMessageBufferId);

#ifdef DEBUG_ASSERT
    ASSERT(buffer);
#endif

    TRK_DispatchMessage(buffer);

#if TRK_MSG_SEQUENCE_IDS
    /*
    ** If this was a cached request, it has been handled now, so clear the flag.
    */
    gRequestCached = FALSE;

    /*
    ** If the buffer was reused to form the reply, it will be saved in
    ** a global previous-ACK location (gPreviousAckId).  Disassociate it from
    ** the event so it won't be discarded with the event, but keep a
    ** local backup of the ID (discardPendingId).  Once it is no longer
    ** referenced from the gPreviousAck variable it can be discarded.
    */
    if (discardPendingId != kInvalidMessageBufferId)
    {
        discardPending = TRKGetBuffer(discardPendingId);

        if (gPreviousAck != discardPending)
        {
            TRK_ReleaseBuffer(discardPendingId);
            discardPendingId = kInvalidMessageBufferId;
        }
    }

    if (gPreviousAck == buffer)
    {
        discardPendingId = event->fMessageBufferId;
        event->fMessageBufferId = kInvalidMessageBufferId;
    }
#endif
}

/******************************************************************************/
/*
 *    TRKHandleSupportEvent
 *
 *    100 % DONE
 */
/******************************************************************************/
void TRKHandleSupportEvent(NubEvent *event)
{
#if defined(__MWERKS__)
    #pragma unused(event)
#elif defined(__GNUC__)
    UNUSED(event);
#endif

    TRKTargetSupportRequest();
}

/****************************************************************************/
/*
 *    TRK_Idle
 *
 *    Find a productive way to kill time while waiting for something
 *    interesting to happen.
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRK_Idle(void)
{
    /*
    **
    ** If the target process is stopped, then wait
    ** for something interesting to occur.  In a
    ** multi-process environment, this would block
    ** until an input event occurred.  In a bare-board
    ** system, nothing useful can be done here, so just
    ** assume that something happened and return.
    **
    ** If the target process is not stopped, then don't
    ** waste any more time in the TRK.  Let the target
    ** process continue.
    */

    if (TRKTargetStopped())
    {
        /* block until input event occurs */
    }
    else
    {
#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP) && defined(DEBUG_MAINLOOP)
        __do_console_dump();
#endif

        TRKTargetContinue();

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP) && defined(DEBUG_MAINLOOP)
        __do_console_dump();
#endif
    }
}

/****************************************************************************/
/*
 *    TRK_NubMainLoop
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRK_NubMainLoop(void)
{
    NubEvent    event;
    BOOL        stopRequested = FALSE;
    BOOL        isIdle = FALSE;

    while (!stopRequested)
    {
        if (TRKGetNextEvent(&event))
        {
            /*
            ** An event arrived, so get out of the idle state.
            */

            isIdle = FALSE;

            switch (event.fType)
            {
                case kNullEvent:
                    break;

                case kRequestEvent:
                    TRK_DispatchMessage(TRKGetBuffer(event.fMessageBufferId));
                    break;

                case kShutdownEvent:
                    stopRequested = TRUE;
                    break;

                case kBreakpointEvent:
                case kExceptionEvent:
                    TRKTargetInterrupt(&event);    /* let target-specific code handle it */
                    break;

                case kSupportEvent:
                    TRKTargetSupportRequest();
                    break;

                default:
                    break;
            }

            TRKDestructEvent(&event);        /* release resources used by the event */
        }
        else
        {
            /*
            ** Arrival here means there were no pending events.
            */

#if TRK_TRANSPORT_INT_DRIVEN
            if (!isIdle || *gTRKInputPendingPtr)
#else /* not TRK_TRANSPORT_INT_DRIVEN */
            if (!isIdle)
#endif /* TRK_TRANSPORT_INT_DRIVEN */
            {
                /*
                ** The TRK is not idle yet.  Switch to the idle state,
                ** but get input.  The input may spawn new events,
                ** which would kick us back out of the idle state.
                */

                isIdle = TRUE;
                TRKGetInput();
            }
            else
            {
                /*
                ** The TRK is idle.  We only get to this point when
                ** no events are pending and all input has been
                ** processed.  Wait until something interesting
                ** occurs.  Once it does, get out of the idle
                ** state and continue with the loop.
                */

                if (TRKTargetStopped() == FALSE)
                    TRKTargetContinue();

                isIdle = FALSE;
            }
        }
    }

    return;
}
