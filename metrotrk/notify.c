/****************************************************************************/
/*

FILE

    notify.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/notify.c,v 1.2 2000/06/16 21:52:54 smoore Exp $

DESCRIPTION

    Event notification from TRK to host debugger.

    Send messages (notification) from TRK to the host debugger.

HISTORY

    97 MAR 03 LLY    Created.
    97 JUN 09 SCM    Added support for exception notifications.

    $History: notify.c $
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 2/02/98    Time: 6:05p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging changes from NEC V8xx TRK.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 2/02/98    Time: 10:51a
//    Updated in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Portable
//    Enhanced to timeout and retry NotifyStopped messages rather
//    than just sending them once.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 7/14/97    Time: 3:15p
//    Created in $/Embedded/MetroTRK/Portable
//    Reorganized CWDS and changed name back to TRK.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 7/14/97    Time: 3:07p
//    Created in $/Embedded/MetroTRK/MetroTRK/Portable
//    Reorganized CWDS and changed name back to TRK
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 7/14/97    Time: 2:59p
//    Created in $/Embedded/TEMP/MetroTRK/Portable
//    Reorganized CWDS files and changed name back to TRK
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 7/03/97    Time: 10:21a
//    Created in $/Embedded/MIPS2.0/new_TRK/Portable
//    Reorganized CWDS directories and changed name back to TRK
//
//    *****************  Version 4  *****************
//    User: Smoore       QDate: 6/10/97    Time: 11:36 AM
//    Updated in $/CWDS/Portable
//    Added support for exception notifications.

COPYRIGHT

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "notify.h"
#include "targimpl.h"
#include "support.h"


#define NOTIFY_TRY_COUNT    3


/*
** Set this to 1 to see detailed information about normal processing
** of Notify events.  (error msgs output via __puts() when one of the
** DEBUGIO_ macros is defined will always be emitted regardless of
** whether DEBUG_NOTIFY is (1) or (0) .)
*/

#define DEBUG_NOTIFY        0


#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKWaitForACK
 */
/****************************************************************************/
DSError TRKWaitForACK(void)
{
    DSError           err;
    MessageBufferId   bufferID;
    MessageBuffer     *buffer;
    u8                msg_command;

    err = kNoError;

    /*
    ** Wait for a packet to arrive.
    */

    do
    {
        bufferID = TRKTestForPacket();
    } while (bufferID == kInvalidMessageBufferId);

    __puts("WaitForACK: packet received\n");

    /*
    ** Check the response.  Make sure it is an ACK.
    */

    buffer = TRKGetBuffer(bufferID);
    TRK_SetBufferPosition(buffer, kZero);
    MessageGet_ui8(buffer, &msg_command);

    if ((MessageCommandID)msg_command != kDSReplyACK)
        err = kWaitACKError;

    ReleaseBuffer(bufferID);

    return err;
}
#endif

/****************************************************************************/
/*
 *    TRKDoNotifyStopped - used to send stop
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoNotifyStopped(MessageCommandID command)
{
    DSError         err;
    MessageBufferId replyID, bufferID;
    MessageBuffer   *buffer;

#if DEBUG_NOTIFY
    OSReport("TRKDoNotifyStopped: command 0x%04x\n", command);
#endif

    // Get a send buffer, create a message
    err = TRK_GetFreeBuffer(&bufferID, &buffer);

    if (err == kNoError)
    {
        /*
         * Add target-defined info.  Usually the PC, instruction at
         * PC, and exception word.
         */

        if (command == kDSNotifyStopped)
            XTargetAddStopInfo(buffer);
        else
            XTargetAddExceptionInfo(buffer);

        /*
         *    Send the notification
         */

        err = TRK_RequestSend(buffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                              , DS_MIN_REPLY_NOTIFYSTOPPED_LENGTH, NOTIFY_TRY_COUNT, TRUE
#endif
                              );

        if (err == kNoError)
            TRK_ReleaseBuffer(replyID);
        else
        {
#if DEBUG_NOTIFY
            if (err == kWaitACKError)
                OSReport("TRKDoNotifyStopped: Invalid ACK received or timeout\n");
            else
                OSReport("TRKDoNotifyStopped: Send failed\n");
#endif
        }

        TRK_ReleaseBuffer(bufferID);
    }

    return err;
}

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    TRKDoNotify - used to send generic notifications
 */
/****************************************************************************/
DSError TRKDoNotify(MessageBuffer *buffer)
{
    DSError         err;
    s32             notifyTries;
    MessageBufferId replyID;

    //
    // Send the notification
    //
    err = kNoError;

    notifyTries = NOTIFY_TRY_COUNT;        // try a few times

    err = TRK_RequestSend(buffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                          , DS_MIN_REPLY_LENGTH,
                          NOTIFY_TRY_COUNT, TRUE
#endif
                          );

    if (err == kNoError)
        TRK_ReleaseBuffer(replyID);
    else
    {
#if DEBUG_NOTIFY
        if (err == kWaitACKError)
            __puts("DoNotify: Invalid ACK received or timeout\r\n");
        else
            __puts("DoNotify: Send failed\r\n");
#endif
    }

    return err;
}
#endif
