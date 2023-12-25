/****************************************************************************/
/*

FILE

    support.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/support.c,v 1.4 2000/06/16 21:52:54 smoore Exp $

DESCRIPTION

    Support commands from CWDS to host debugger.

    Send messages (support) from CWDS to the host debugger.


HISTORY

    97 JUN 06 SCM    Created.

    $History: support.c $
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
//    User: Smoore       Date: 4/01/98    Time: 10:42a
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging from $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Por
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 3/31/98    Time: 5:44p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Modified console I/O to accept input without error even if it is less
//    that
//    what was requested.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 3/19/98    Time: 12:35a
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Merging in console I/O support from the main repository.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 2/02/98    Time: 6:06p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging changes from NEC V8xx TRK.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 2/02/98    Time: 11:05a
//    Updated in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Portable
//    Removed Fputs command support and replaced it with WriteFile/ReadFile
//    support.

COPYRIGHT

    (c) 1997, 1998 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "osreport.h"
#include "nubassrt.h"
#include "notify.h"
#include "msg.h"
#include "msgxtrct.h"
#include "support.h"
#include "serpoll.h"
#include "mem_TRK.h"
#include "string_TRK.h"

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #include "cc_gdev.h"
#endif


/*
** Set this to 1 to see detailed information about normal processing
** in these support functions (error msgs output via OSReport() when
** one of the DEBUGIO_ macros is defined will always be emitted
** regardless of whether DEBUG_SUPPORT_CALLS is (1) or (0) .)
*/

#define DEBUG_SUPP_REPLY_ENABLED    0

#define SUPPORT_TRY_COUNT           3

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #define ResetTimeout(timer_ptr)     (*timer_ptr = 0);
    #define Timeout(timer_ptr)          ((++(*timer_ptr)) >= 0x4C4B3EC)
#endif


// Custom structure used as temporary buffer for message data transmission
typedef struct
{
    u32 msg_length;
    u8 command[4];
    u8 handle[4];
    u16 length[2];
    u8 data[4];

    // most likely used for padding to 64 bytes
    // (as this is the length of the whole message)
    u8 unknown[0x2C];
} msgbuf_t;


/******************************************************************************/
/*
 *    TRK_SuppAccessFile
 *
 *    Handles support requests from the target program to read/write
 *    data to/from a file on the host.  Converts the request into one
 *    or more messages and sends them to the host.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_SuppAccessFile(DSFileHandle file_handle, u8 *data, s32 *count,
                        DSIOResult *io_result, BOOL need_reply,
                        BOOL read)
{
    DSError         err;
    MessageBufferId replyID;
    MessageBuffer   *replyBuffer;
    MessageBufferId bufferID;
    MessageBuffer   *buffer;
    s32             length;
    size_t          done;
    u8              msg_io_result;
    u16             msg_length;
    BOOL            abort;
    msgbuf_t        tmpbuf;

#if DEBUG_ASSERT
    ASSERT((count != NULL) && (io_result != NULL));
#endif

    if ((data == NULL) || (*count == 0))
        return kUserError;

    /*
    ** The message size is limited, so the access may need to
    ** be broken up into multiple read/write messages.  Loop until
    ** everything has been read/written or an error occurs.
    **
    ** 'length' is the amount read/written in a particular iteration,
    ** and 'done' tracks the total amount read/written so far.
    */

    for (abort = FALSE, done = 0, err = kNoError, *io_result = kDSIONoError;
         !abort && (done < (u32)*count) && (err == kNoError) && (*io_result == kDSIONoError);
         done += length)
    {
        // Reset Buffer
        TRK_memset(&tmpbuf, kZero, TRK_MSG_HEADER_LENGTH);

        /*
        ** Set the length to max if there is more than that
        ** left to read/write.  Otherwise, set it to the number
        ** of remaining bytes.
        */

        if ((*count - done) > DS_MAXREADWRITELENGTH)
            length = DS_MAXREADWRITELENGTH;
        else
            length = (s32)(*count - done);

        tmpbuf.command[0] = (read ? kDSReadFile : kDSWriteFile);
        tmpbuf.msg_length = (read ? TRK_MSG_HEADER_LENGTH : ((u32)length + TRK_MSG_HEADER_LENGTH));
        *(DSFileHandle *)tmpbuf.handle = file_handle;
        tmpbuf.length[0] = (u16)length;

        /*
        ** Get a buffer and build the message.
        */

        TRK_GetFreeBuffer(&bufferID, &buffer);

        err = MessageAddBlock_ui8(buffer, (const u8 *)&tmpbuf, TRK_MSG_HEADER_LENGTH);

        /*
        ** If writing, copy the write data to the message.
        */

        if (!read)
        {
            if (err == kNoError)
                err = MessageAddBlock_ui8(buffer, &data[done], (s32)length);
        }

        __puts("SuppAccessFile: sending request\r\n");

        if (err == kNoError)
        {
            /*
            ** If a reply is desired, send it as a request,
            ** which will look for a reply and retry the
            ** message up to the specified count if no reply
            ** or an improper reply is given.  The reply
            ** is returned.
            ** If no reply is desired, just use MessageSend()
            ** to send it out once.
            */

            if (need_reply)
            {
                msg_length = 0;

                /*
                ** Cannot really timeout on a console read message.
                ** For console input, must continue to wait until
                ** someone gives us some input.
                */

                err = TRK_RequestSend(buffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                                      , (read ? (u32)DS_MIN_REPLY_READFILE_LENGTH :
                                      (u32)DS_MIN_REPLY_WRITEFILE_LENGTH),
                                      SUPPORT_TRY_COUNT,
                                      !(read && (file_handle == kDSStdin))
#endif
                                      );

                if (err == kNoError)
                    replyBuffer = TRKGetBuffer(replyID);

                /*
                ** Get the io_result code and the read/written
                ** length from the reply.
                */

                msg_length = DSFetch_u16(&replyBuffer->fData[20]);
                msg_io_result = (u8)DSFetch_u32(&replyBuffer->fData[16]);

                /*
                ** If reading, get the returned data from the
                ** reply. If the returned length is bigger than
                ** what MetroTRK asked for, don't copy the
                ** data to the reply buffer since it will cause
                ** buffer overflow.
                */

                if (read && (err == kNoError) && (!(msg_length > (u32)length)))
                {
                    /*
                    ** Skip the ACK & error-code in the reply.
                    ** The RequestSend() routine did not return
                    ** an error, so it is safe to assume they
                    ** are OK.
                    */

                    TRK_SetBufferPosition(replyBuffer, TRK_MSG_HEADER_LENGTH);

                    err = MessageGetBlock_ui8(replyBuffer, &data[done], msg_length);

                    if (err == kMessageBufferUnderflow)
                        err = kNoError;
                }

                /*
                ** If the returned length is not what
                ** MetroTRK asked for, then MetroTRK will
                ** stop the current file access and set
                ** msg_io_result to kDSIOError depending
                ** on the file mode.
                **
                ** In reading mode
                ** ===============
                **
                ** The returned length will be smaller than
                ** what MetroTRK asked for when the target
                ** program request an entire buffer of input,
                ** but the console return only a single
                ** line.
                **
                ** For example: The target program requests
                ** an array of characters, but the console
                ** returns empty string.
                **
                ** The returned length will be larger than
                ** what MetroTRK asked for when the target
                ** program request single line of input,
                ** but the console returns the entire buffer
                ** of input.
                **
                ** For example: The target program requests
                ** a single character input, but the console
                ** returns more than one character input.
                **
                ** In writting mode
                ** ================
                **
                ** When the returned length is smaller or
                ** larger than what MetroTRK asked for, MetroTRK
                ** will set msg_io_result to kDSIOError in writting
                ** mode.
                */

                if (msg_length != (u32)length)
                {
                    length = msg_length;
                    abort = TRUE;
                }

                *io_result = (DSIOResult)msg_io_result;

                TRK_ReleaseBuffer(replyID);
            }
            else
            {
                err = TRK_MessageSend(buffer);
            }
        }

        TRK_ReleaseBuffer(bufferID);
    }

    /*
    ** Set the count to the actual amount read/written and return.
    */

    *count = (s32)done;

    return err;
}

/****************************************************************************/
/*
 *    TRK_RequestSend
 *
 *    Send a request message to the host and wait for a reply.  If
 *    no reply is returned within a certain timeout period (target-
 *    defined), then re-send the request.  If an improper reply is
 *    received (command is not an ACK, or an error code is returned,
 *    or the received reply is too small), then that reply is
 *    thrown away and the request is re-sent.  The retries parameter
 *    indicates the maximum number of times to re-send the message,
 *    not including the original send.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_RequestSend(MessageBuffer *buffer, MessageBufferId *replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                        , size_t min_size, s32 max_retries, BOOL check_timeout
#endif
                        )
{
    DSError       err;
    MessageBuffer *replyBuffer;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    u32           timer;
    s32           tries;
#endif

    u8            msg_command;
    u8            msg_error;
    BOOL          bad_reply;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #if defined(__MWERKS__)
        #pragma unused(min_size)
    #elif defined(__GNUC__)
        UNUSED(min_size);
    #endif

    for (tries = (max_retries + 1), *replyID = kInvalidMessageBufferId;
         tries && (*replyID == kInvalidMessageBufferId) && (err == kNoError);
         tries--)
    {
#endif

        /*
        ** Loop, sending the message and waiting for a reply.
        ** If the reply is bad or there is a timeout, the loop
        ** repeats until the number of retries is exhausted.
        ** On loop exit, if a good reply was received, the
        ** *replyID will be a valid buffer ID, otherwise it
        ** will be kInvalidMessageBufferID.
        ** An internal error will abort the loop early.
        */
        err = TRK_MessageSend(buffer);

        if (err == kNoError)
        {
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
            if (check_timeout)
                ResetTimeout(&timer);
#endif
            /*
            ** When non-acknowledgement messages arrive, continue
            ** waiting.
            */

            do
            {
                /*
                ** Wait for a packet to arrive.
                */

                do
                {
                    *replyID = TRKTestForPacket();
                } while ((*replyID == kInvalidMessageBufferId)
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                         && !(check_timeout && Timeout(&timer))
#endif
                         );
                /*
                ** If a reply was received, verify that it is valid.
                */

                if (*replyID != kInvalidMessageBufferId)
                {
                    __puts("\r\nRequestSend: packet received\r\n");

                    bad_reply = FALSE;

                    replyBuffer = TRKGetBuffer(*replyID);
                    TRK_SetBufferPosition(replyBuffer, kZero);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                    OutputData(replyBuffer->fData, replyBuffer->fLength);
#endif

                    /*
                    ** If the reply is not an acknowledgement, assume
                    ** it is an asynchronous request from the debugger
                    ** and process it as such.  Then continue waiting
                    ** for an acknowledgement.
                    */

                    msg_command = replyBuffer->fData[4];

                    if ((u32)msg_command < kDSReplyACK)
                    {
#if DEBUG_SUPP_REPLY_ENABLED
                        __puts("Caching incoming request\r\n");
#endif
                        TRKProcessInput(*replyID);
                        *replyID = kInvalidMessageBufferId;
                        continue;
                    }
                }

                break;
            } while (1);

            if ((s32)*replyID != kInvalidMessageBufferId)
            {
                /*
                ** The reply must be at least the minimum size.
                */

                if (replyBuffer->fLength < TRK_MSG_HEADER_LENGTH)
                {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
                    OSReport("MetroTRK - bad reply size %ld\n", replyBuffer->fLength);
#endif
                    bad_reply = TRUE;
                }

                /*
                ** Get the message command and error values and
                ** verify that this is an ACK message with no
                ** errors.
                */

                // this "if" condition is used twice but the contents
                // cannot be mixed as this messes up the disassembly
                // compared to the original object file...
                //
                // most likely there once was additional code between
                // these which wasn't compiled into the final object
                // file due to the use of an "#if..." condition check
                if ((err == kNoError) && !bad_reply)
                    msg_error = replyBuffer->fData[8];
                
#if DEBUG_SUPP_REPLY_ENABLED
                if (msg_error)
                    OSReport("RequestSend: Non-ACK or ACK-error received.\n");
#endif

                if ((err == kNoError) && !bad_reply)
                {
                    if (((MessageCommandID)msg_command != kDSReplyACK)
                        || ((DSReplyError)msg_error != kDSReplyNoError))
                    {
                        bad_reply = TRUE;
                        __puts("\r\nRequestSend: Non-ACK or ACK-error received.\r\n");
                    }
                }

                /*
                ** If the reply wasn't good, get rid of it and
                ** reset the *replyID.
                */

                if ((err != kNoError) || bad_reply)
                {
                    __puts("\r\nRequestSend: bad reply\r\n");

                    TRK_ReleaseBuffer(*replyID);
                    *replyID = kInvalidMessageBufferId;
                }
                else
                {
                    __puts("\r\nRequestSend: reply OK\r\n");
                }
            }
            else
            {
                __puts("RequestSend: timed out waiting for reply\r\n");
            }
        }
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    }
#endif

    if (*replyID == kInvalidMessageBufferId)
    {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        OSReport("MetroTRK - failed in RequestSend\n");
#endif
        err = kWaitACKError;
    }

    return err;
}

/****************************************************************************/
/*
 *    HandleOpenFileSupportRequest
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
DSError HandleOpenFileSupportRequest(DSFileHandle handle, u8 length, u32 *options, DSIOResult *io_result)
{
    DSError         err;
    MessageBufferId replyID;
    MessageBufferId bufferID;
    MessageBuffer   *replyBuffer;
    MessageBuffer   *buffer;
    msgbuf_t        tmpbuf;

    // Reset Buffer
    TRK_memset(&tmpbuf, kZero, TRK_MSG_HEADER_LENGTH);

    DSFetch_u32(options) = 0;

    tmpbuf.command[0] = kDSOpenFile;
    tmpbuf.msg_length = (TRK_strlen((const s8 *)handle) + TRK_MSG_REPLY_HEADER_LENGTH);
    DSFetch_u8(tmpbuf.handle) = length;
    tmpbuf.length[0] = (u16)(TRK_strlen((const s8 *)handle) + 1);

    TRK_GetFreeBuffer(&bufferID, &replyBuffer);
    err = MessageAddBlock_ui8(replyBuffer, (const u8 *)&tmpbuf, TRK_MSG_HEADER_LENGTH);

    if (err == kNoError)
    {
        u32 msg_length = TRK_strlen((const s8 *)handle);

        err = MessageAddBlock_ui8(replyBuffer, (const u8 *)handle, (s32)(msg_length + 1));
    }

    if (err == kNoError)
    {
        *io_result = kDSIONoError;
        err = TRK_RequestSend(replyBuffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                              , DS_MIN_REPLY_OPENFILE_LENGTH, SUPPORT_TRY_COUNT, FALSE
#endif
                              );

        if (err == kNoError)
            buffer = TRKGetBuffer(replyID);

        *io_result = *(DSIOResult *)&buffer->fData[16];
        *options = DSFetch_u32(&buffer->fData[8]);

        TRK_ReleaseBuffer(replyID);
    }

    TRK_ReleaseBuffer(bufferID);

    return err;
}

/****************************************************************************/
/*
 *    HandleCloseFileSupportRequest
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
DSError HandleCloseFileSupportRequest(DSFileHandle handle, DSIOResult *io_result)
{
    DSError         err;
    MessageBufferId replyID;
    MessageBufferId bufferID;
    MessageBuffer   *replyBuffer;
    MessageBuffer   *buffer;
    msgbuf_t        tmpbuf;
    
    // Reset Buffer
    TRK_memset(&tmpbuf, kZero, TRK_MSG_HEADER_LENGTH);

    tmpbuf.command[0] = kDSCloseFile;
    tmpbuf.msg_length = TRK_MSG_HEADER_LENGTH;
    *(DSFileHandle *)tmpbuf.handle = handle;

    err = TRK_GetFreeBuffer(&bufferID, &replyBuffer);

    if (err == kNoError)
        err = MessageAddBlock_ui8(replyBuffer, (const u8 *)&tmpbuf, TRK_MSG_HEADER_LENGTH);

    if (err == kNoError)
    {
        *io_result = kDSIONoError;
        err = TRK_RequestSend(replyBuffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                              , DS_MIN_REPLY_CLOSEFILE_LENGTH, SUPPORT_TRY_COUNT, FALSE
#endif
                              );

        // this "if" condition is used twice but the contents
        // cannot be mixed as this messes up the disassembly
        // compared to the original object file...
        //
        // most likely there once was additional code between
        // these which wasn't compiled into the final object
        // file due to the use of an "#if..." condition check
        if (err == kNoError)
            buffer = TRKGetBuffer(replyID);

#if DEBUG_SUPP_REPLY_ENABLED
        if (buffer == NULL)
            OSReport("No buffer available\n");
#endif

        if (err == kNoError)
            *io_result = *(DSIOResult *)&buffer->fData[16];

        TRK_ReleaseBuffer(replyID);
    }

    TRK_ReleaseBuffer(bufferID);

    return err;
}

/****************************************************************************/
/*
 *    HandlePositionFileSupportRequest
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
DSError HandlePositionFileSupportRequest(DSFileHandle handle, s32 *length, u8 data, DSIOResult *io_result)
{
    DSError         err;
    MessageBufferId replyID;
    MessageBufferId bufferID;
    MessageBuffer   *replyBuffer;
    MessageBuffer   *buffer;
    msgbuf_t        tmpbuf;

    // Reset Buffer
    TRK_memset(&tmpbuf, kZero, TRK_MSG_HEADER_LENGTH);

    tmpbuf.command[0] = kDSPositionFile;
    tmpbuf.msg_length = TRK_MSG_HEADER_LENGTH;
    *(DSFileHandle *)tmpbuf.handle = handle;
    DSFetch_s32(tmpbuf.length) = *length;
    tmpbuf.data[0] = data;

    err = TRK_GetFreeBuffer(&bufferID, &replyBuffer);

    if (err == kNoError)
        err = MessageAddBlock_ui8(replyBuffer, (const u8 *)&tmpbuf, TRK_MSG_HEADER_LENGTH);

    if (err == kNoError)
    {
        *io_result = kDSIONoError;
        *length = -1;

        err = TRK_RequestSend(replyBuffer, &replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                              , DS_MIN_REPLY_POSITIONFILE_LENGTH, SUPPORT_TRY_COUNT, FALSE
#endif
                              );

        if (err == kNoError)
        {
            buffer = TRKGetBuffer(replyID);

            if (buffer)
            {
                *io_result = *(DSIOResult *)&buffer->fData[16];
                *length = DSFetch_s32(&buffer->fData[24]);
            }
        }

        TRK_ReleaseBuffer(replyID);
    }

    TRK_ReleaseBuffer(bufferID);

    return err;
}
