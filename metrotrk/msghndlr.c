/****************************************************************************/
/*

FILE

    msghndlr.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/msghndlr.c,v 1.3 2000/06/16 21:52:53 smoore Exp $

DESCRIPTION

    TRK command handlers.  Default implementation.

    To override implementation, create the overriding functions and add it to
    the link list (project window) before this file.

HISTORY

    97 FEB 12 LLY    Created.
    97 FEB 14 LLY    Added return type for errors.
    97 JUN 10 SCM    Enhanced error detection and reporting.  Added support
                    for exception detection.

    $History: msghndlr.c $
//
//    *****************  Version 15  *****************
//    User: Smoore       Date: 6/25/99    Time: 4:47p
//    Updated in $/Embedded/MetroTRK/Portable
//       - Moved all reset-handling code into a separate section
//         so that the main body of TRK code can be copied to RAM.
//       - Added a new self-debugging mode which periodically dumps debug
//         memory buffer contents to the debugger console window.
//
//    *****************  Version 14  *****************
//    User: Smoore       Date: 2/08/99    Time: 12:56a
//    Updated in $/Embedded/MetroTRK/Portable
//    Implemented CPUType command.
//
//    Removed all calls to TargetRead*()/TargetWrite*() functions.  Replaced
//    with direct calls to TargetAccess*() functions.
//
//    *****************  Version 13  *****************
//    User: Smoore       Date: 11/10/98   Time: 1:07p
//    Updated in $/Embedded/MetroTRK/Portable
//    Fixed message size check bug in DoFlushCache().
//
//    *****************  Version 12  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 11  *****************
//    User: Smoore       Date: 6/25/98    Time: 2:35p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//    *****************  Version 13  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 12  *****************
//    User: Smoore       Date: 1/16/98    Time: 11:52a
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Enhanced debug output.
//
//    *****************  Version 11  *****************
//    User: Smoore       Date: 12/08/97   Time: 7:40p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Now uses PCtype for PC values, for which the specific definition is
//    target-specific.
//
//    *****************  Version 9  *****************
//    User: Smoore       Date: 11/07/97   Time: 5:17p
//    Updated in $/Embedded/MetroTRK/Portable
//    Removed non-printable characters from file.
//
//    *****************  Version 8  *****************
//    User: Smoore       Date: 10/15/97   Time: 11:51a
//    Updated in $/Embedded/MetroTRK/Portable
//    Added debug messages and re-ordered one 'if' statement to workaround
//    a codegen bug in the MIPS compiler.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 10/06/97   Time: 3:04p
//    Updated in $/Embedded/MetroTRK/Portable
//    Added debug messages and a local prototype of __reset().
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 9/02/97    Time: 3:13p
//    Updated in $/Embedded/MetroTRK/Portable
//    Added step-over options to DoStep.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:32p
//    Updated in $/Embedded/MetroTRK/Portable
//    Enhanced debug messages, enhanced error handling for memory read
//    and write commands, enhanced error checking for step.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 7/15/97    Time: 10:01a
//    Updated in $/Embedded/MetroTRK/Portable
//    Fixed msg size check in DoStep().
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 7/15/97    Time: 9:17a
//    Updated in $/Embedded/MetroTRK/Portable
//    Enhanced step command to support stepping until the PC is out of a
//    specified memory range.
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
//    *****************  Version 8  *****************
//    User: Smoore       QDate: 6/30/97    Time: 5:16 PM
//    Updated in $/Embedded/CWDS/Portable
//    Added single step support.
//
//    *****************  Version 7  *****************
//    User: Smoore       QDate: 6/25/97    Time: 4:26 PM
//    Updated in $/Embedded/CWDS/Portable
//    Added reset support.
//
//    *****************  Version 6  *****************
//    User: Smoore       QDate: 6/10/97    Time: 11:37 AM
//    Updated in $/CWDS/Portable
//    Enhanced error detection and reporting.  Added support for exception
//    detection.

AUTHORS

    (c) 1997-2000 Metrowerks Corporation
    All rights reserved.

COPYRIGHT

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "osreport.h"
#include "targimpl.h"
#include "msg.h"
#include "msghndlr.h"
#include "msgxtrct.h"
#include "dolphin_trk_glue.h"
#include "copy_vectors.h"
#include "target_options.h"
#include "reset.h"
#include "asm_regs.h"

#ifdef VIRTUAL_DISPATCH // NOT IMPLEMENTED ON THE WII / NDEV
    #include "targimpl_ppc.h"
    #include "usr_put.h"
#endif

// This is only required to match up with the hex binary data of the original object file
#include "dummy_a.h"


#define DEBUG_MSG_HANDLER_ERROR         0
#define DEBUG_MSG_HANDLER_UNSUPPORTED   0
#define DEBUG_MSG_HANDLER_CONNECT       0
#define DEBUG_MSG_HANDLER_DISCONNECT    0
#define DEBUG_MSG_HANDLER_RESET         0
#define DEBUG_MSG_HANDLER_OVERRIDE      0
#define DEBUG_MSG_HANDLER_VERSIONS      0
#define DEBUG_MSG_HANDLER_SUPPORTMASK   0
#define DEBUG_MSG_HANDLER_CPUTYPE       0
#define DEBUG_MSG_HANDLER_READ_MEM      0
#define DEBUG_MSG_HANDLER_WRITE_MEM     0
#define DEBUG_MSG_HANDLER_READ_REG      0
#define DEBUG_MSG_HANDLER_WRITE_REG     0
#define DEBUG_MSG_HANDLER_FLUSH         0
#define DEBUG_MSG_HANDLER_CONTINUE      0
#define DEBUG_MSG_HANDLER_STEP          0
#define DEBUG_MSG_HANDLER_STOP          0
#define DEBUG_MSG_HANDLER_OPTION        0

#define PPC_CACHE_ALIGNMENT             32


#if __MWERKS__
    #pragma mark  Standard ACK/NAK
#endif /* __MWERKS__ */


static u32 g_CurrentSequence = 0;
static BOOL IsTRKConnected = FALSE;


// For the Wii / NDEV, it was done this way...
//
// Custom structure used as temporary buffer for message data transmission
typedef struct
{
    u32 msg_length;
    u8 reply[4];
    u8 status[4];
    u16 sequence[2];
    u8 data[4];

    // most likely used for padding to 64 bytes
    // (as this is the length of the whole message)
    u8 unknown[0x2C];
} msgbuf_t;


/****************************************************************************/
/*
 *    GetTRKConnected
 *
 *    100 % DONE (This function is NOT part of the official TRK source code)
 */
/****************************************************************************/
BOOL GetTRKConnected(void)
{
    return IsTRKConnected;
}

/****************************************************************************/
/*
 *    SetTRKConnected
 *
 *    100 % DONE (This function is NOT part of the official TRK source code)
 */
/****************************************************************************/
BOOL SetTRKConnected(BOOL result)
{
    IsTRKConnected = result;

    return result;
}

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKMessageIntoReply
 *
 *    Modify the receive buffer to become the reply, using the ack sequence ID
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKMessageIntoReply(MessageBuffer *b,
                        u8 ackCmd,
                        DSReplyError errSentInAck)
{
    DSError err = kNoError;

#ifdef DEBUG_ASSERT
    ASSERT(b);
    ASSERT((ackCmd == kDSReplyACK) || (ackCmd == kDSReplyNAK));
#endif

    TRKResetBuffer(b, kDontZero);     /* rewind buffer, don't clear */

    MessageAdd_ui8(b, ackCmd);
    MessageAdd_ui8(b, (u8)errSentInAck);

    FAIL(err == kNoError);            /* shouldn't be possible */

    return;
}
#endif

/****************************************************************************/
/*
 *    TRKSendACK
 *
 *    Send an ACK to the sender with retries.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKSendACK(MessageBuffer *b)
{
    /*
     *    Send the acknowledgement
     */

    return TRK_MessageSend(b);
}

/****************************************************************************/
/*
 *    TRKStandardACK
 *
 *    Send an ACK to the sender.  Reuse the receive buffer for sending.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKStandardACK(MessageBuffer *b, u8 ackCmd, DSReplyError errSentInAck)
{
#ifndef DEBUG_ASSERT
    #if defined(__MWERKS__)
        #pragma unused(b)
    #elif defined(__GNUC__)
        UNUSED(b);
    #endif
#endif

    msgbuf_t buffer;
    u32 nextSequence;

#ifdef DEBUG_ASSERT
    ASSERT(b);
    ASSERT((ackCmd == kDSReplyACK) || (ackCmd == kDSReplyNAK));
#endif

    /*
     *    convert the received message into a reply
     */

    // Reset buffer
    TRK_memset(&buffer, kZero, sizeof(msgbuf_t));

    nextSequence = (g_CurrentSequence + 1);

    buffer.reply[0] = ackCmd;
    buffer.msg_length = sizeof(msgbuf_t);
    DSFetch_s8(&buffer.status) = (s8)errSentInAck;
    DSFetch_u32(&buffer.sequence) = nextSequence;
    g_CurrentSequence = (nextSequence + 1);

    TRK_WriteUARTN((s8 *)&buffer, sizeof(msgbuf_t));

    return kNoError;
}

#if __MWERKS__
    #pragma mark -
    #pragma mark  Non-Commands
#endif /* __MWERKS__ */

/****************************************************************************/
/*
 *    TRKDoError
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoError(MessageBuffer *b)
{
#if defined(__MWERKS__)
    #pragma unused(b)
#elif defined(__GNUC__)
    UNUSED(b);
#endif

#if DEBUG_MSG_HANDLER_ERROR
    OSReport("DoError\n");

    /* incorrect command */
#endif

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKDoUnsupported
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoUnsupported(MessageBuffer *b)
{
#if DEBUG_MSG_HANDLER_UNSUPPORTED
    OSReport("DoUnsupported\n");
#endif

    TRKStandardACK(b, kDSReplyACK, kDSReplyUnsupportedCommandError);

    return kNoError;
}

#if __MWERKS__
    #pragma mark -
    #pragma mark  Meta Commands
#endif /* __MWERKS__ */

/****************************************************************************/
/*
 *    meta command set
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    TRK_DoConnect
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_DoConnect(MessageBuffer *b)
{
#if DEBUG_MSG_HANDLER_CONNECT
    OSReport("DoConnect\n");
#endif

    IsTRKConnected = TRUE;

    TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKDoDisconnect
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoDisconnect(MessageBuffer *b)
{
    NubEvent event;

#if DEBUG_MSG_HANDLER_DISCONNECT
    OSReport("DoDisconnect\n");
#endif

    //
    // Remove target state associated with the current session.
    //

    IsTRKConnected = FALSE;

    TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

    TRKConstructEvent(&event, kShutdownEvent); /* let the main loop terminate */
    TRKPostEvent(&event);

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKDoReset
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoReset(MessageBuffer *b)
{
#if DEBUG_MSG_HANDLER_RESET
    OSReport("DoReset\n");
#endif

    /*
    ** Send out an ACK before the reset, since the reset call will not return.
    */

    TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

    /*
    ** This call will not return.  It will cause the DS to restart from
    ** scratch.
    */

    __TRKreset();

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKDoOverride - For copying vectors
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoOverride(MessageBuffer *b)
{
#if DEBUG_MSG_HANDLER_OVERRIDE
    OSReport("DoOverride\n");
#endif

    TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

    __TRK_copy_vectors();

    return kNoError;
}

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKDoVersions - Return kernel and protocol versions
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoVersions(MessageBuffer *b)
{
    DSError    err = kNoError;

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    DSVersions versions;

#if DEBUG_MSG_HANDLER_VERSIONS
    __puts("DoVersions\r\n");
#endif

    /* check received message size */

    if (b->fLength != DS_MIN_MSG_VERSIONS_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }

    /*
    ** Reuse buffer to write the reply.
    ** Append command (ui8), error (ui8) - standard reply header
    */

    TRKMessageIntoReply(b, kDSReplyACK, kDSReplyNoError);

    /*
    ** Append the four version bytes.
    */

    if (err == kNoError)
        err = TRKTargetVersions(&versions);

    if (err == kNoError)
        err = MessageAdd_ui8(b, versions.kernelMajor);

    if (err == kNoError)
        err = MessageAdd_ui8(b, versions.kernelMinor);

    if (err == kNoError)
        err = MessageAdd_ui8(b, versions.protocolMajor);

    if (err == kNoError)
        err = MessageAdd_ui8(b, versions.protocolMinor);

    if (err != kNoError)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyCWDSError);
    }

#if DEBUG_MSG_HANDLER_VERSIONS
    __puts("DoVersions: sending version data\r\n");
#endif

    err = TRKSendACK(b);

#if DEBUG_MSG_HANDLER_VERSIONS
    if (err != kNoError)
    {
        __puts("error occurred\r\n");
        __puthex8(err);
        __puts("\r\n");
    }
#endif
#endif

    return err;
}
#endif

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKDoSupportMask - Send bit mask indicating which commands are supported.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoSupportMask(MessageBuffer *b)
{
    DSError       err = kNoError;

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    DSSupportMask mask;

#if DEBUG_MSG_HANDLER_SUPPORTMASK
    __puts("DoSupportMask\r\n");
#endif

    /* check received message size */

    if (b->fLength != DS_MIN_MSG_SUPPORTMASK_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }

    /*
    ** Reuse buffer to write the reply.
    ** Append command (ui8), error (ui8) - standard reply header
    */

    TRKMessageIntoReply(b, kDSReplyACK, kDSReplyNoError);

    /*
    ** Append support mask.  The support mask is a byte stream, so it is
    ** OK to use MessageAddBlock.
    */

    if (err == kNoError)
        err = TRKTargetSupportMask(&mask);

    if (err == kNoError)
        err = MessageAddBlock(b, &mask, sizeof(mask));

    /*
    ** Append the protocol level.
    */

    if (err == kNoError)
        err = MessageAdd_ui8(b, DS_PROTOCOL);

    if (err != kNoError)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyCWDSError);
    }

#if DEBUG_MSG_HANDLER_SUPPORTMASK
    __puts("DoSupportMask: sending support mask\r\n");
#endif

    err = TRKSendACK(b);

#if DEBUG_MSG_HANDLER_SUPPORTMASK
    if (err != kNoError)
    {
        __puts("error occurred\r\n");
        __puthex8(err);
        __puts("\r\n");
    }
#endif
#endif

    return err;
}
#endif

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKDoCPUType - Return CPU type and attributes.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoCPUType(MessageBuffer *b)
{
    DSError   err = kNoError;

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    DSCPUType cpuType;

#if DEBUG_MSG_HANDLER_CPUTYPE
    __puts("DoCPUType\r\n");
#endif

    /* check received message size */

    if (b->fLength != DS_MIN_MSG_CPUTYPE_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }

    /*
    ** Reuse buffer to write the reply.
    ** Append command (ui8), error (ui8) - standard reply header
    */

    TRKMessageIntoReply(b, kDSReplyACK, kDSReplyNoError);

    /*
    ** Append the four version bytes.
    */

    if (err == kNoError)
        err = TRKTargetCPUType(&cpuType);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.cpuMajor);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.cpuMinor);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.bigEndian);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.defaultTypeSize);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.fpTypeSize);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.extended1TypeSize);

    if (err == kNoError)
        err = MessageAdd_ui8(b, cpuType.extended2TypeSize);

    if (err != kNoError)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyCWDSError);
    }

#if DEBUG_MSG_HANDLER_CPUTYPE
    __puts("DoCPUType: sending version data\r\n");
#endif

    err = TRKSendACK(b);

#if DEBUG_MSG_HANDLER_CPUTYPE
    if (err != kNoError)
    {
        __puts("error occurred\r\n");
        __puthex8(err);
        __puts("\r\n");
    }
#endif
#endif

    return err;
}
#endif

#if __MWERKS__
    #pragma mark -
    #pragma mark  State Commands
#endif /* __MWERKS__ */

/****************************************************************************/
/*
 *    target "state" command set
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    TRKDoReadMemory
 *
 *    For 32-bit address spaces, this command implementation should be
 *    sufficent for reading memory.  To customize this function, make a copy
 *    and use the OverrideDispatch function to dispatch commands to that
 *    function instead.
 *
 *    uses > DS_MAXREADWRITELENGTH of stack space
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoReadMemory(MessageBuffer *b)
{
    DSError      err;
    DSReplyError msg_reply;
    u8           msg_options;
    u32          msg_start;
    u16          msg_length;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u32          msg_process;
    u32          msg_thread;
#endif

    err = kNoError;

#if DEBUG_MSG_HANDLER_READ_MEM
    /* check received message size */

    if (b->fLength != DS_MIN_MSG_READMEMORY_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }
#endif

    //
    // Get ReadMemory parameters from the message.
    //

    msg_reply = kDSReplyNoError;
    msg_options = DSFetch_u8(&b->fData[8]);
    msg_start = DSFetch_u32(&b->fData[16]);
    msg_length = DSFetch_u16(&b->fData[12]);

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_process);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_thread);
#endif

#if DEBUG_MSG_HANDLER_READ_MEM
    __puts("DoReadMemory\r\n");
    __puts(" options: ");
    __puthex2(msg_options);
    __puts("\r\n length: ");
    __puthex4(msg_length);
    __puts("\r\n addr: ");
    __puthex8(msg_start);
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
#endif
    __puts("\r\n");
#endif

    /*
     *    do some preliminary checks
     */

    /* standard read only works for 32-bit memory */

    if (msg_options & DS_MSG_MEMORY_EXTENDED)
    {
#if DEBUG_MSG_HANDLER_READ_MEM
        __puts("Extended option unsupported\r\n");
#endif
        return TRKStandardACK(b, kDSReplyACK, kDSReplyUnsupportedOptionError);
    }

#if DEBUG_MSG_HANDLER_READ_MEM
    if (msg_length > DS_MAXREADWRITELENGTH)
    {
        __puts("Length too large: ");
        __puthex4(msg_length);
        __puts(" > ");
        __puthex4(DS_MAXREADWRITELENGTH);
        __puts("\r\n");

        return TRKStandardACK(b, kDSReplyACK, kDSReplyParameterError);
    }
#endif

    if (err == kNoError)
    {
        // 32-bit buffer alignment was added for the Wii / NDEV
        u8 buf[DS_MAXREADWRITELENGTH + 0x20] __attribute__((aligned(PPC_CACHE_ALIGNMENT)));

        size_t length = msg_length;

        /*
        ** Do the memory read.
        */

        err = XTargetAccessMemory((void *)&buf, (void *)msg_start, &length,
            (msg_options & DS_MSG_MEMORY_USERVIEW) ? kUserMemory : kDebuggerMemory, TRUE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            , msg_process, msg_thread
#endif
            );

        TRKResetBuffer(b, kZero);

        if (err == kNoError)
        {
            msgbuf_t buffer;

            // Reset buffer
            TRK_memset(&buffer, kZero, sizeof(msgbuf_t));

            DSFetch_s8(&buffer.status) = (s8)err;
            buffer.msg_length = (length + TRK_MSG_HEADER_LENGTH);
            buffer.reply[0] = kDSReplyACK;
            DSFetch_u32(&buffer.sequence) = g_CurrentSequence;
            g_CurrentSequence++;

            /*
            ** Append the length and data to the message.
            */

            MessageAddBlock(b, (u8 *)&buffer, sizeof(msgbuf_t));

            if ((msg_options & sizeof(msgbuf_t)) != 0)
            {
                err = MessageAddBlock(b, (const void *)&buf[msg_start & 0x1F], length);
            }
            else
            {
                err = MessageAddBlock(b, (const void *)&buf, length);
            }
        }
    }

    if (err != kNoError)
    {
        switch (err)
        {
            case kCWDSException:
                msg_reply = kDSReplyCWDSException;
                break;

            case kInvalidMemory:
                msg_reply = kDSReplyInvalidMemoryRange;
                break;

            case kInvalidProcessId:
                msg_reply = kDSReplyInvalidProcessId;
                break;

            case kInvalidThreadId:
                msg_reply = kDSReplyInvalidThreadId;
                break;

            case kOsError:
                msg_reply = kDSReplyOsError;
                break;

            default:
                msg_reply = kDSReplyCWDSError;
                break;
        }

#if DEBUG_MSG_HANDLER_READ_MEM
        __puts("DoReadMemory error: ");
        __puthex8(err);
        __puts(" (");
        __puthex2(msg_reply);
        __puts(")\r\n");
#endif

        return TRKStandardACK(b, kDSReplyACK, msg_reply);
    }

#if DEBUG_MSG_HANDLER_READ_MEM
    __puts("DoReadMemory: sending memory read data\r\n");
#endif

    return TRKSendACK(b);
}


// This is only required to match up with the hex binary data of the original object file
#include "dummy_b.h"


/****************************************************************************/
/*
 *    TRKDoWriteMemory
 *
 *    uses > DS_MAXREADWRITELENGTH of stack space
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoWriteMemory(MessageBuffer *b)
{
    DSError      err;
    DSReplyError msg_reply;
    u8           msg_options;
    u32          msg_start;
    u16          msg_length;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u32          msg_process;
    u32          msg_thread;
#endif

    err = kNoError;

#if DEBUG_MSG_HANDLER_WRITE_MEM
    /* check received message size */

    if (b->fLength <= DS_MIN_MSG_WRITEMEMORY_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }
#endif

    /*
    ** Get WriteMemory parameters from the message.
    */

    msg_reply = kDSReplyNoError;
    msg_options = DSFetch_u8(&b->fData[8]);
    msg_start = DSFetch_u32(&b->fData[16]);
    msg_length = DSFetch_u16(&b->fData[12]);

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_process);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_thread);
#endif

#if DEBUG_MSG_HANDLER_WRITE_MEM
    __puts("DoWriteMemory\r\n");
    __puts(" options: ");
    __puthex2(msg_options);
    __puts("\r\n length: ");
    __puthex4(msg_length);
    __puts("\r\n addr: ");
    __puthex8(msg_start);
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
#endif
    __puts("\r\n");
#endif

    /*
     *    do some preliminary checks
     */

    /* standard write only works for 32-bit memory */

    if (msg_options & DS_MSG_MEMORY_EXTENDED)
    {
#if DEBUG_MSG_HANDLER_WRITE_MEM
        __puts("Extended option unsupported\r\n");
#endif
        return TRKStandardACK(b, kDSReplyACK, kDSReplyUnsupportedOptionError);
    }

#if DEBUG_MSG_HANDLER_WRITE_MEM
    if ((b->fLength != (DS_MIN_MSG_WRITEMEMORY_LENGTH + msg_length)) ||
        (msg_length > DS_MAXREADWRITELENGTH))
    {
        __puts("Size error. fLength = ");
        __puthex8(b->fLength);
        __puts("\r\n");

        return TRKStandardACK(b, kDSReplyACK, kDSReplyParameterError);
    }
#endif

    if (err == kNoError)
    {
        // 32-bit buffer alignment was added for the Wii / NDEV
        u8 buf[DS_MAXREADWRITELENGTH + 0x20] __attribute__((aligned(PPC_CACHE_ALIGNMENT)));

        size_t length = msg_length;

        TRK_SetBufferPosition(b, sizeof(msgbuf_t));

        /*
         *    write memory; write into the message buffer
         */

        MessageGetBlock(b, (void *)&buf, length);

        err = XTargetAccessMemory((void *)&buf, (void *)msg_start, &length,
            (msg_options & DS_MSG_MEMORY_USERVIEW) ? kUserMemory : kDebuggerMemory, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            , msg_process, msg_thread
#endif
            );

        /*
        ** Reuse buffer to write the reply.
        ** Append command (u8), error (u8) - standard reply header
        */

        TRKResetBuffer(b, kZero);

        if (err == kNoError)
        {
            msgbuf_t buffer;

            // Reset buffer
            TRK_memset(&buffer, kZero, sizeof(msgbuf_t));

            buffer.msg_length = sizeof(msgbuf_t);
            buffer.reply[0] = kDSReplyACK;
            DSFetch_s8(&buffer.status) = (s8)err;
            DSFetch_u32(&buffer.sequence) = g_CurrentSequence;
            g_CurrentSequence++;

            /*
            ** Append the data length.
            */

            err = MessageAddBlock(b, (u8 *)&buffer, sizeof(msgbuf_t));
        }
    }

    if (err != kNoError)
    {
        switch (err)
        {
            case kCWDSException:
                msg_reply = kDSReplyCWDSException;
                break;

            case kInvalidMemory:
                msg_reply = kDSReplyInvalidMemoryRange;
                break;

            case kInvalidProcessId:
                msg_reply = kDSReplyInvalidProcessId;
                break;

            case kInvalidThreadId:
                msg_reply = kDSReplyInvalidThreadId;
                break;

            case kOsError:
                msg_reply = kDSReplyOsError;
                break;

            default:
                msg_reply = kDSReplyCWDSError;
                break;
        }

#if DEBUG_MSG_HANDLER_WRITE_MEM
        __puts("DoWriteMemory error: ");
        __puthex8(err);
        __puts(" (");
        __puthex2(msg_reply);
        __puts(")\r\n");
#endif

        return TRKStandardACK(b, kDSReplyACK, msg_reply);
    }

#if DEBUG_MSG_HANDLER_WRITE_MEM
    __puts("DoWriteMemory: sending ack\r\n");
#endif

    return TRKSendACK(b);
}

/****************************************************************************/
/*
 *    TRKDoReadRegisters
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoReadRegisters(MessageBuffer *b)
{
    DSError      err;
    size_t       registerDataLength;
    DSReplyError msg_reply;
    u16          msg_lastRegister;
    u16          msg_firstRegister;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u32          msg_process;
    u32          msg_thread;
#endif

#if DEBUG_MSG_HANDLER_READ_REG
    /* check received message size */

    if (b->fLength != DS_MIN_MSG_READREGISTERS_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }
#endif

    /*
    ** Get command parameters from the message.
    */

    msg_reply = kDSReplyNoError;
    msg_lastRegister = DSFetch_u16(&b->fData[12]);
    msg_firstRegister = DSFetch_u16(&b->fData[16]);

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_process);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_thread);
#endif

#if DEBUG_MSG_HANDLER_READ_REG
    __puts("\r\nDoReadRegisters\r\n");
    __puts("\r\n firstRegister: ");
    __puthex4(msg_firstRegister);
    __puts("\r\n lastRegister: ");
    __puthex4(msg_lastRegister);
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
#endif
    __puts("\r\n");

    if (!XTargetStopped(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        msg_process, msg_thread
#endif
        ))
    {
        __puts("Target is not stopped\r\n");

        return TRKStandardACK(b, kDSReplyACK, kDSReplyNotStopped);
    }
#endif

    /*
    ** Check parameters.
    */

    if (msg_lastRegister > msg_firstRegister)
    {
        TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidRegisterRange);

        err = kNoError;
    }
    else
    {
        /*
        ** Reuse buffer to write the reply.
        ** Append command (ui8), error (ui8) - standard reply header
        */

        msgbuf_t buffer;

        buffer.reply[0] = kDSReplyACK;
        buffer.msg_length = 0x468;    // ???
        DSFetch_u32(&buffer.sequence) = g_CurrentSequence;
        g_CurrentSequence++;

        TRKResetBuffer(b, kZero);

        MessageAddBlock_ui8(b, (const void *)&buffer, sizeof(msgbuf_t));

        /*
         *    read registers; write into the message buffer
         */

        err = XTargetAccessDefault(0, 0x24, b, &registerDataLength, TRUE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            , msg_process, msg_thread
#endif
            );

        if (err == kNoError)
            err = XTargetAccessFP(0, 0x21, b, &registerDataLength, TRUE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );

        if (err == kNoError)
            err = XTargetAccessExtended1(0, 0x60, b, &registerDataLength, TRUE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );

        if (err == kNoError)
            err = XTargetAccessExtended2(0, 0x1f, b, &registerDataLength, TRUE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );

        if (err != kNoError)
        {
            switch (err)
            {
                case kUnsupportedError:
                    msg_reply = kDSReplyUnsupportedOptionError;
                    break;

                case kInvalidRegister:
                    msg_reply = kDSReplyInvalidRegisterRange;
                    break;

                case kCWDSException:
                    msg_reply = kDSReplyCWDSException;
                    break;

                case kInvalidProcessId:
                    msg_reply = kDSReplyInvalidProcessId;
                    break;

                case kInvalidThreadId:
                    msg_reply = kDSReplyInvalidThreadId;
                    break;

                case kOsError:
                    msg_reply = kDSReplyOsError;
                    break;

                default:
                    msg_reply = kDSReplyCWDSError;
                    break;
            }

#if DEBUG_MSG_HANDLER_READ_REG
            __puts("\r\nDoReadRegister error: ");
            __puthex8(err);
            __puts(" (");
            __puthex2(msg_reply);
            __puts(")\r\n");
#endif

            TRKStandardACK(b, kDSReplyACK, msg_reply);

            err = kNoError;
        }
        else
        {
#if DEBUG_MSG_HANDLER_READ_REG
            __puts("\r\nDoReadRegisters: sending memory read data\r\n");
#endif
            err = TRKSendACK(b);
        }
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKDoWriteRegisters
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoWriteRegisters(MessageBuffer *b)
{
    size_t       registerDataLength;
    DSError      err;
    DSReplyError msg_reply;
    u8           msg_options;
    u16          msg_lastRegister;
    u16          msg_firstRegister;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u32          msg_process;
    u32          msg_thread;
#endif

#if DEBUG_MSG_HANDLER_WRITE_REG
    /* check received message size */

    if (b->fLength <= DS_MIN_MSG_WRITEREGISTERS_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }
#endif

    /*
    ** Get command parameters from the message.
    */

    msg_reply = kDSReplyNoError;
    msg_options = DSFetch_u8(&b->fData[8]);
    msg_lastRegister = DSFetch_u16(&b->fData[12]);
    msg_firstRegister = DSFetch_u16(&b->fData[16]);

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_process);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_thread);
#endif

    TRK_SetBufferPosition(b, kZero);

#if DEBUG_MSG_HANDLER_WRITE_REG
    __puts("\r\nDoWriteRegisters");
    __puts("\r\n options: ");
    __puthex2(msg_options);
    __puts("\r\n firstRegister: ");
    __puthex4(msg_firstRegister);
    __puts("\r\n lastRegister: ");
    __puthex4(msg_lastRegister);
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
#endif
    __puts("\r\n");

    if (!XTargetStopped(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        msg_process, msg_thread
#endif
        ))
    {
        __puts("Target is not stopped\r\n");

        return TRKStandardACK(b, kDSReplyACK, kDSReplyNotStopped);
    }
#endif

    /*
    ** Check parameters.
    */

    if (msg_lastRegister > msg_firstRegister)
    {
        TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidRegisterRange);

        return kNoError;
    }

    TRK_SetBufferPosition(b, sizeof(msgbuf_t));

    // BUG / FEATURE OR ENDIANNESS RELATION ???
    //
    // For the Wii / NDEV, they are writing registers in the
    // opposite direction than reading them as it seems...

    switch (msg_options)
    {
        case kDSRegistersDefault:
            err = XTargetAccessDefault(msg_lastRegister, msg_firstRegister, b, &registerDataLength, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );
            break;

        case kDSRegistersFP:
            err = XTargetAccessFP(msg_lastRegister, msg_firstRegister, b, &registerDataLength, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );
            break;

        case kDSRegistersExtended1:
            err = XTargetAccessExtended1(msg_lastRegister, msg_firstRegister, b, &registerDataLength, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );
            break;

        case kDSRegistersExtended2:
            err = XTargetAccessExtended2(msg_lastRegister, msg_firstRegister, b, &registerDataLength, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                , msg_process, msg_thread
#endif
                );
            break;

        default:
            err = kUnsupportedError;
            break;
    }

    /*
    ** Reuse buffer to write the reply.
    ** Append command (ui8), error (ui8) - standard reply header
    */

    TRKResetBuffer(b, kZero);

    if (err == kNoError)
    {
        msgbuf_t buffer;

        // Reset buffer
        TRK_memset(&buffer, kZero, sizeof(msgbuf_t));

        buffer.msg_length = sizeof(msgbuf_t);
        buffer.reply[0] = kDSReplyACK;
        DSFetch_s8(&buffer.status) = (s8)err;
        DSFetch_u32(&buffer.sequence) = g_CurrentSequence;
        g_CurrentSequence++;

        err = MessageAddBlock(b, &buffer, sizeof(msgbuf_t));
    }

    if (err != kNoError)
    {
        switch (err)
        {
            case kUnsupportedError:
                msg_reply = kDSReplyUnsupportedOptionError;
                break;

            case kInvalidRegister:
                msg_reply = kDSReplyInvalidRegisterRange;
                break;

            case kMessageBufferUnderflow:
                /*
                ** Underflow happens because data in packet was insufficient.
                */
                msg_reply = kDSReplyPacketSizeError;
                break;

            case kCWDSException:
                msg_reply = kDSReplyCWDSException;
                break;

            case kInvalidProcessId:
                msg_reply = kDSReplyInvalidProcessId;
                break;

            case kInvalidThreadId:
                msg_reply = kDSReplyInvalidThreadId;
                break;

            case kOsError:
                msg_reply = kDSReplyOsError;
                break;

            default:
                msg_reply = kDSReplyCWDSError;
                break;
        }

#if DEBUG_MSG_HANDLER_WRITE_REG
        __puts("DoWriteRegisters error: ");
        __puthex8(err);
        __puts(" (");
        __puthex2(msg_reply);
        __puts(")\r\n");
#endif

        TRKStandardACK(b, kDSReplyACK, msg_reply);

        err = kNoError;
    }
    else
    {
#if DEBUG_MSG_HANDLER_WRITE_REG
        __puts("DoWriteRegisters: sending ack\r\n");
#endif
        err = TRKSendACK(b);
    }

    return err;
}


#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKDoFlushCache
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoFlushCache(MessageBuffer *b)
{
    u8           msg_command;
    u8           msg_options;

    u32          msg_start = 0;
    u32          msg_end = 0;
    DSError      err = kNoError;
    DSReplyError replyErr = kDSReplyNoError;

    /* check received message size */

    if (b->fLength != DS_MIN_MSG_FLUSHCACHE_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }

    /*
    ** Get command parameters from the message.
    */

    TRK_SetBufferPosition(b, kZero);

    if (err == kNoError)
        err = MessageGet_ui8(b, &msg_command);

    if (err == kNoError)
        err = MessageGet_ui8(b, &msg_options);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_start);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_end);

#if DEBUG_MSG_HANDLER_FLUSH
    __puts("\r\nDoFlushCache");
    __puts("\r\n options: ");
    __puthex2(msg_options);
    __puts("\r\n start: ");
    __puthex4(msg_start);
    __puts("\r\n end: ");
    __puthex4(msg_end);
    __puts("\r\n");
#endif

    /*
    ** Check parameters.
    */

    if (msg_start > msg_end)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidMemoryRange);
    }

    if (err == kNoError)
        err = TRKTargetFlushCache(msg_options, (void *)msg_start,
                                (void *)msg_end);

    /*
    ** Reuse buffer to write the reply.
    ** Append command (ui8), error (ui8) - standard reply header
    */

    if (err == kNoError)
        TRKMessageIntoReply(b, kDSReplyACK, kDSReplyNoError);

    if (err != kNoError)
    {
        switch (err)
        {
            case kUnsupportedError:
                replyErr = kDSReplyUnsupportedOptionError;
                break;

            default:
                replyErr = kDSReplyCWDSError;
                break;
        }

#if DEBUG_MSG_HANDLER_FLUSH
        __puts("DoFlushCache error: ");
        __puthex8(err);
        __puts(" (");
        __puthex2(replyErr);
        __puts(")\r\n");
#endif

        return TRKStandardACK(b, kDSReplyACK, replyErr);
    }

#if DEBUG_MSG_HANDLER_FLUSH
    __puts("DoFlushCache: sending ack\r\n");
#endif

    err = TRKSendACK(b);

    return err;
}
#endif

#if __MWERKS__
    #pragma mark -
    #pragma mark  Execution Commands
#endif /* __MWERKS__ */

/****************************************************************************/
/*
 *    target "execution" command set
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    TRKDoContinue
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoContinue(MessageBuffer *b)
{
    DSError err;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u8      msg_command;
    u32     msg_process;
    u32     msg_thread;

    err = kNoError;

    TRK_SetBufferPosition(b, kZero);

    if (err == kNoError)
        err = MessageGet_ui8(b, &msg_command);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_process);

    if (err == kNoError)
        err = MessageGet_ui32(b, &msg_thread);
#endif

#if DEBUG_MSG_HANDLER_CONTINUE
    OSReport("DoContinue\n");
#endif

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
    __puts("\r\n");
#endif

    if (!XTargetStopped(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        msg_process, msg_thread
#endif
        ))
    {
#if DEBUG_MSG_HANDLER_CONTINUE
        OSReport("Target is not stopped\n");
#endif
        TRKStandardACK(b, kDSReplyACK, kDSReplyNotStopped);

        err = kNoError;
    }
    else
    {
        TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP) && defined(DEBUG_MSG_HANDLER_CONTINUE)
        __do_console_dump();
#endif

        err = XTargetContinue(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            msg_process, msg_thread
#endif
            );

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP) && defined(DEBUG_MSG_HANDLER_CONTINUE)
        __do_console_dump();
#endif
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKDoStep
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoStep(MessageBuffer *b)
{
    s32     msg_options;
    u8      msg_count;
    u32     msg_rangeStart;
    u32     msg_rangeEnd;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u32     msg_process;
    u32     msg_thread;
#endif
    PCType  target_pc;

    DSError err = kNoError;

#if DEBUG_MSG_HANDLER_STEP
    /* check received message size */

    if (b->fLength < DS_MIN_MSG_STEP_LENGTH)
    {
        return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
    }
#endif

    /*
    ** Get Step parameters from the message.
    */

    TRK_SetBufferPosition(b, kZero);

    msg_options = DSFetch_u8(&b->fData[8]);
    msg_rangeStart = DSFetch_u32(&b->fData[16]);
    msg_rangeEnd = DSFetch_u32(&b->fData[20]);

#if DEBUG_MSG_HANDLER_STEP
    OSReport("DoStep\n");
    OSReport(" options: 0x%02x\n", msg_options);
#endif

    /*
    ** Read parameters based on options.
    */

    switch (msg_options)
    {
        case kDSStepIntoCount:
        case kDSStepOverCount:
            msg_count = DSFetch_u8(&b->fData[12]);

#if DEBUG_MSG_HANDLER_STEP
            __puts("\r\n count: ");
            __puthex2(msg_count);
#endif

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            if (err == kNoError)
                err = MessageGet_ui32(b, &msg_process);

            if (err == kNoError)
                err = MessageGet_ui32(b, &msg_thread);

            __puts("\r\n process: ");
            __puthex8(msg_process);
            __puts("\r\n thread: ");
            __puthex8(msg_thread);

            if (err == kNoError)
            {
                err = OsTargetValidStep(msg_process, msg_thread);

                switch (err)
                {
                    case kInvalidThreadId:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidThreadId);
                        break;

                    case kInvalidProcessId:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidProcessId);
                        break;

                    case kOsError:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyOsError);
                        break;
                }
            }
#endif

            /*
            ** Must step over at least one instruction.
            */

            if ((u32)msg_count < 1)
                return TRKStandardACK(b, kDSReplyACK, kDSReplyParameterError);

            break;

        case kDSStepIntoRange:
        case kDSStepOverRange:

#if DEBUG_MSG_HANDLER_STEP
            /* check received message size */

            if (b->fLength != DS_MIN_MSG_STEP_RANGE_LENGTH)
            {
                return TRKStandardACK(b, kDSReplyACK, kDSReplyPacketSizeError);
            }

            __puts("\r\n rangeStart: ");
            __puthex8(msg_rangeStart);
            __puts("\r\n rangeEnd: ");
            __puthex8(msg_rangeEnd);
#endif

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
            if (err == kNoError)
                err = MessageGet_ui32(b, &msg_process);

            if (err == kNoError)
                err = MessageGet_ui32(b, &msg_thread);

            __puts("\r\n process: ");
            __puthex8(msg_process);
            __puts("\r\n thread: ");
            __puthex8(msg_thread);

            if (err == kNoError)
            {
                err = OsTargetValidStep(msg_process, msg_thread);

                switch (err)
                {
                    case kInvalidThreadId:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidThreadId);
                        break;

                    case kInvalidProcessId:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyInvalidProcessId);
                        break;

                    case kOsError:
                        return TRKStandardACK(b, kDSReplyACK, kDSReplyOsError);
                        break;
                }
            }
#endif

            /*
            ** If the PC is not within the specified range already,
            ** then the step does not make sense.
            **
            ** Note: The conversion to (long) before converting to (ui32)
            ** is a workaround for a MIPS compiler bug.  Only the
            ** (ui32) conversion should be needed.
            */

            target_pc = XTargetGetPC(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                msg_process, msg_thread
#endif
                );

            if ((target_pc < (PCType)msg_rangeStart) ||
                (target_pc > (PCType)msg_rangeEnd))
                return TRKStandardACK(b, kDSReplyACK, kDSReplyParameterError);

            break;

        default:
#if DEBUG_MSG_HANDLER_STEP
            __puts("\r\n  Option Invalid\r\n");
#endif
            return TRKStandardACK(b, kDSReplyACK, kDSReplyUnsupportedOptionError);
    }

    /*
     *    do some preliminary checks
     */

    if (!XTargetStopped(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        msg_process, msg_thread
#endif
        ))
    {
#if DEBUG_MSG_HANDLER_STEP
        OSReport("Target is not stopped\n");
#endif
        return TRKStandardACK(b, kDSReplyACK, kDSReplyNotStopped);
    }
    else
    {
        /*
        ** Send the ACK before doing the single step, since the single step will
        ** return control to the target program.
        */

        err = TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

        switch (msg_options)
        {
            case kDSStepIntoCount:
            case kDSStepOverCount:
                err = XTargetSingleStep(msg_count, (msg_options == kDSStepOverCount)
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                    , msg_process, msg_thread
#endif
                    );
                break;

            case kDSStepIntoRange:
            case kDSStepOverRange:
#if DEBUG_MSG_HANDLER_STEP
                OSReport(" rangeStart: 0x%08x\n", msg_rangeStart);
                OSReport(" rangeEnd: 0x%08x\n", msg_rangeEnd);
#endif
                err = XTargetStepOutOfRange(msg_rangeStart, msg_rangeEnd, (msg_options == kDSStepOverRange)
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
                    , msg_process, msg_thread
#endif
                    );
                break;
        }
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKDoStop
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKDoStop(MessageBuffer *b)
{
    DSError      err;
    DSReplyError replyErr;
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    u8           msg_command;
    u8           msg_options;
    u32          msg_process;
    u32          msg_thread;

    err = kNoError;

    TRK_SetBufferPosition(b, kZero);

    if (err == kNoError)
        err = MessageGet_ui8(b, &msg_command);

    if (err == kNoError)
        err = MessageGet_ui8(b, &msg_options);
#endif

#if DEBUG_MSG_HANDLER_STOP
    __puts("DoStop");
#endif

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    if ((err == kNoError) &&
        ((msg_options == kDSStopProcess) || (msg_options == kDSStopThread)))
        err = MessageGet_ui32(b, &msg_process);

    if ((err == kNoError) &&
        (msg_options == kDSStopThread))
        err = MessageGet_ui32(b, &msg_thread);

#if DEBUG_MSG_HANDLER_STOP
    __puts("\r\n options: ");
    __puthex2(msg_options);
    __puts("\r\n process: ");
    __puthex8(msg_process);
    __puts("\r\n thread: ");
    __puthex8(msg_thread);
#endif
#endif

#if DEBUG_MSG_HANDLER_STOP
    __puts("\r\n");
#endif

    err = XTargetStop(
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        msg_options, msg_process, msg_thread
#endif
        );

    switch (err)
    {
        case kNoError:
            replyErr = kDSReplyNoError;
            break;

        case kInvalidProcessId:
            replyErr = kDSReplyInvalidProcessId;
            break;

        case kInvalidThreadId:
            replyErr = kDSReplyInvalidThreadId;
            break;

        case kOsError:
            replyErr = kDSReplyOsError;
            break;

        default:
            replyErr = kDSReplyError;
            break;
    }

    err = TRKStandardACK(b, kDSReplyACK, replyErr);

    return err;
}

/****************************************************************************/
/*
 *    TRKDoSetOption
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKDoSetOption(MessageBuffer *b)
{
    u8 msg_reply = DSFetch_u8(&b->fData[12]);

#if DEBUG_MSG_HANDLER_OPTION
    OSReport("DoSetOption\n");
#endif

    if (b->fData[8] == kDSReplyError)
    {
        OSReport("\nMetroTRK Option : SerialIO - ");

        if (msg_reply != kDSReplyNoError)
            OSReport("Enable\n");
        else
            OSReport("Disable\n");

        SetUseSerialIO(msg_reply);
    }

    TRKStandardACK(b, kDSReplyACK, kDSReplyNoError);

    return kNoError;
}
