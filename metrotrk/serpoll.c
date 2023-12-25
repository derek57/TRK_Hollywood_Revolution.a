/****************************************************************************/
/*

FILE

    serpoll.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/Attic/serpoll.c,v 1.4 2000/05/16 23:59:48 smoore Exp $

DESCRIPTION

    TRK serial polling

MODIFICATIONS

    96 JUN 01 TS    Original implementation for PowerTV/XPPC Nub Protocol
    96 SEP 22 LLY    Reformatted source code and comments
    96 OCT 19 LLY    Converted for MIPS
    96 NOV 02 LLY    Converted for Motorola PowerPC MPC821
    97 FEB 12 LLY    Removed msgproto.h from include list.  Removed const
                     pointer passed to Dispatch
    97 FEB 12 LLY    Initialized dispatcher
    97 FEB 15 LLY    m821.c split into portable portion, now serpoll.c
    96 MAR 01 LLY    Removed #include __config.h

    $History: serpoll.c $
//
//    *****************  Version 9  *****************
//    User: Smoore       Date: 7/30/99    Time: 3:43p
//    Updated in $/Embedded/MetroTRK/Portable
//       - Optimized the message input loop to allow higher-speed
//         communications on low-speed CPU's.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 6/25/98    Time: 2:05p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 11/07/97   Time: 5:19p
//    Updated in $/Embedded/MetroTRK/Portable
//    Added EOL at end of file.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 10/15/97   Time: 11:52a
//    Updated in $/Embedded/MetroTRK/Portable
//    Some non-static functions were being declared as static, causing
//    compile
//    errors when C++ enabled.  Fixed.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:41p
//    Updated in $/Embedded/MetroTRK/Portable
//    Enhanced debug messages.

COPYRIGHT

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "nubevent.h"
#include "msg.h"
#include "serpoll.h"
#include "dolphin_trk_glue.h"


#define DEBUG_SERPOLL   0

/*
** DEBUG_PACKET causes the entire contents of each received packet
** to be written out as debug information.  It is OK to use this
** even when debug info goes out via the serial port.
*/

#define DEBUG_PACKET    (0)


#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    Framing Types
 */
/****************************************************************************/

typedef enum ReceiverState
{
    kWaitFlag,
    kFoundFlag,
    kInFrame,
    kFrameOverflow
} ReceiverState;

typedef struct FramingState
{
    MessageBufferId  fBufferID;
    MessageBuffer    *fBuffer;

    ReceiverState    fReceiveState;
    BOOL             fEscape;
    FCSType          fFCS;
} FramingState;

static FramingState gFramingState;

/****************************************************************************/
/*
 *   static function prototypes
 */
/****************************************************************************/

#if __MWERKS__
    #pragma mark Static Function Prototypes
#endif /* __MWERKS__ */

__extern_c

    void TRK_Idle(void);
    BOOL TRKGetNextEvent(NubEvent *event);
    MessageBufferId TRKTestForPacket(void);

__end_extern_c

static void DiscardFrame(FramingState *frameState);
static void RejectFrame(FramingState *framingState, DSReplyError replyError);
static BOOL ProcessFrame(FramingState *frameState);

/****************************************************************************/
/*
 *   global variables
 */
/****************************************************************************/
#endif

#if __MWERKS__
    #pragma mark -
    #pragma mark Global Variables
#endif /* __MWERKS__ */

#if TRK_TRANSPORT_INT_DRIVEN
    volatile u8 *gTRKInputPendingPtr;
#endif // TRK_TRANSPORT_INT_DRIVEN

#if __MWERKS__
    #pragma mark -
#endif /* __MWERKS__ */

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    DiscardFrame
 *
 *    Release resources used for the current frame.
 */
/****************************************************************************/
static void DiscardFrame(FramingState *state)
{
    if (state->fBufferID != kInvalidMessageBufferId)
    {
        ReleaseBuffer(state->fBufferID);
        state->fBufferID = kInvalidMessageBufferId;
    }

    state->fBuffer = NULL;
    state->fReceiveState = kWaitFlag;
}

/****************************************************************************/
/*
 *    RejectFrame
 *
 *    A message with initial frame byte and data arrived, but it is invalid.
 */
/****************************************************************************/
static void RejectFrame(FramingState *framingState, DSReplyError replyError)
{
    MessageBuffer *msgBuffer;

    __puts("RejectFrame\r\n");

    msgBuffer = framingState->fBuffer;
    ASSERT(msgBuffer);

    /*
    ** Send a NAK with the given error.
    */

    TRKStandardACK(msgBuffer, kDSReplyNAK, replyError);

    /*
    ** Throw away the data, release the frame.
    */

    DiscardFrame(framingState);
}
#endif

/****************************************************************************/
/*
 *    TRKTestForPacket
 *
 *    Poll the serial port and post a message if the message is complete.
 *
 *    The following is true when this routine exits:
 *
 *        - All of the input was read.
 *            --OR--
 *        - A complete message was read.
 *
 *    *** This should move into another file.
 *    *** Initialization of gFramingState.fBufferID should be part of nub init.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
MessageBufferId TRKTestForPacket(void)
{
    MessageBufferId bufferID;
    MessageBufferId replyID;
    MessageBuffer *msgbuf;
    s32 data[16];

    //
    // For Dolphin & EXI2 - read incoming messages as complete packets.
    // Bypass the framing code.
    //
    // Check for incoming bytes.  If we have some, read them.
    //
    if (TRKPollUART() <= kNoError)
        return kInvalidMessageBufferId;

    //
    // Allocate a buffer to store the incoming message.
    //

    replyID = TRK_GetFreeBuffer(&bufferID, &msgbuf);
    TRK_SetBufferPosition(msgbuf, kZero);

    if (TRKReadUARTN(data, TRK_MSG_HEADER_LENGTH) == kUARTNoError)
    {
        //
        // Read the data into the buffer & return the buffer ID.
        //
        MessageAddBlock_ui8(msgbuf, (const u8 *)data, TRK_MSG_HEADER_LENGTH);
        replyID = bufferID;

        //
        // If the message is too big, send out a NAK.  Flush out the
        // incoming data first.
        //
        if ((data[0] - TRK_MSG_HEADER_LENGTH) > kUARTNoError)
        {
            s8 buffer[DS_MAXMESSAGESIZE];

            if (TRKReadUARTN(buffer, (u32)(data[0] - TRK_MSG_HEADER_LENGTH)) == kUARTNoError)
                MessageAddBlock_ui8(msgbuf, (const u8 *)buffer, data[0]);
            else
            {
                TRK_ReleaseBuffer(replyID);
                replyID = kInvalidMessageBufferId;
            }
        }
    }
    else
    {
        //
        // If we get to this point, we didn't receive a valid message.
        //
        TRK_ReleaseBuffer(replyID);
        replyID = kInvalidMessageBufferId;
    }

    return replyID;
}

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    ProcessFrame
 *
 *    A message with initial frame byte and data arrived.
 */
/****************************************************************************/
static BOOL ProcessFrame(FramingState *framingState)
{
    MessageBuffer *msgBuffer;

    __puts("ProcessFrame\r\n");

    msgBuffer = framingState->fBuffer;
    ASSERT(msgBuffer);

#if DEBUG_PACKET
    {
        s32 i;

        __puts("Received packet: ");

        for (i = 0; i < msgBuffer->fLength; i++)
        {
            __putchar('[');
            __puthex2(msgBuffer->fData[i]);
            __putchar(']');
        }

        __puts("\r\n");
    }
#endif /* DEBUG_PACKET */

    /*
     *    The buffer contains data in the following form:
     *        [data] [FCS8]
     *    or:
     *        [data] [FCS16]
     *    or:
     *        [data] [FCS32]
     *
     *    where data is arbitrary length and FCSxx is 1, 2 or 4 bytes
     */
    if (msgBuffer->fLength < (DS_MIN_MSG_LENGTH + sizeof(FCSType)))
    {
        /*
         *    data received is too short
         */
        __puts("frame is too short\r\n");
        __puthex8(msgBuffer->fLength);
        __puts("\r\n");

        RejectFrame(framingState, kDSReplyPacketSizeError);

        return FALSE;
    }

    /*
     *    check for FCS
     */

    if (framingState->fFCS != 0xFF)
    {
        /*
         *    back checksum (FCS)
         */
        RejectFrame(framingState, kDSReplyBadFCS);

        __puts("Bad FCS = ");
        __puthex8(framingState->fFCS);
        __puts("\r\n");

        return FALSE;
    }

    /*
     *    trim back packet, eliminating FCS
     */
    msgBuffer->fPosition = 0;        /* rewind position to beginning*/
    msgBuffer->fLength -= sizeof(FCSType);

    __puts("fLength = ");
    __puthex8(msgBuffer->fLength);
    __puts("\r\n");

    return TRUE;
}
#endif

/****************************************************************************/
/*
 *    TRKGetInput
 *
 *    Note:  This should be abstracted to a transport layer that handles
 *    input devices other than serial.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
void TRKGetInput(void)
{
    MessageBufferId bufferID = TRKTestForPacket();

    if (bufferID != kInvalidMessageBufferId)
    {
        /*
         *    message arrived
         */

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
        MessageCommandID command;
        u8 data;

        MessageBuffer *msgBuffer = TRKGetBuffer(bufferID);

        TRK_SetBufferPosition(msgBuffer, kZero);
        MessageGet_ui8(msgBuffer, &data);
        command = (MessageCommandID)data;

        if (command < kDSReplyACK)
        {
            /* only ack commands */

            TRKProcessInput(bufferID);
        }
        else
        {
#if DEBUG_ASSERT
            ASSERT(!"spurious ACK/NAK received in event loop");
#endif
            TRK_ReleaseBuffer(bufferID);
        }
#endif
        TRKProcessInput(bufferID);
    }

    return;
}

/****************************************************************************/
/*
 *    TRKProcessInput
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
void TRKProcessInput(MessageBufferId bufferID)
{
    NubEvent event;

    TRKConstructEvent(&event, kRequestEvent);
    event.fMessageBufferId = bufferID; /* event owns buffer here */

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    //
    // Note: This assumes that the buffer came from the framing
    // state machine.  This will have to change when the I/O code
    // is revamped.
    //
    gFramingState.fBufferID = kInvalidMessageBufferId;
#endif

#if DEBUG_SERPOLL
    __puts("posting event\r\n");
#endif

    TRKPostEvent(&event);

    return;
}

/****************************************************************************/
/*
 *    TRKInitializeSerialHandler
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKInitializeSerialHandler(void)
{
#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    gFramingState.fBufferID = kInvalidMessageBufferId;
    gFramingState.fReceiveState = kWaitFlag;
    gFramingState.fEscape = FALSE;
#endif

#if (DS_SERIAL == DS_SERIAL_POLLING)

    // nothing to initialize; we're going to poll instead
    return kNoError;

#elif (DS_SERIAL == DS_SERIAL_INTERRUPT)

    // initialize the interrupt-handler for serial
    return kNoError;

#else

    #pragma error

#endif
}

/****************************************************************************/
/*
 *    TRKTerminateSerialHandler
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTerminateSerialHandler(void)
{
#if (DS_SERIAL == DS_SERIAL_POLLING)

    // nothing to terminate

#elif (DS_SERIAL == DS_SERIAL_INTERRUPT)

    // initialize the interrupt-handler for serial

#else

    #pragma error

#endif

    return kNoError;
}
