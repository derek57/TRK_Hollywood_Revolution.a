/****************************************************************************/
/*

FILE

    msgbuf.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/msgbuf.c,v 1.3 2000/06/23 23:16:47 smoore Exp $

DESCRIPTION

    Message buffers

HISTORY
    97 FEB 02 LLY    Added Append
    97 FEB 12 LLY    Added SetBufferPosition.

    $History: msgbuf.c $
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 7/30/99    Time: 3:39p
//    Updated in $/Embedded/MetroTRK/Portable
//       - Optimized the message input loop to allow higher-speed
//         communications on low-speed CPU's.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 1/21/98    Time: 5:46p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in changes from VR TRK.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 12/08/97   Time: 7:38p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added 64-bit versions of the ReadBuffer and AppendBuffer functions.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 10/06/97   Time: 3:03p
//    Updated in $/Embedded/MetroTRK/Portable
//    Renamed "mutex.h" to "mutex_TRK.h" to avoid MSL conflicts.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:28p
//    Updated in $/Embedded/MetroTRK/Portable
//    Replaced compile-time switches for endianness with dynamic endian
//    detection and handling.

COPYRIGHT
    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "nubassrt.h"
#include "msgbuf.h"
#include "msgxtrct.h"
#include "nubinit.h"
#include "mem_TRK.h"

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #include "mutex_TRK.h"
#endif


/****************************************************************************/
/*
 *     Local Types and Variables
 */
/****************************************************************************/

typedef struct MessageBuffers
{
    MessageBuffer    fBuffers[kMessageBufferCount];
} MessageBuffers;

MessageBuffers gTRKMsgBufs;

/****************************************************************************/
/*
 *     static function declarations
 */
/****************************************************************************/

static void TRK_SetBufferUsed(MessageBuffer *buffer, BOOL inUse);

/****************************************************************************/
/*
 *    TRK_SetBufferUsed
 *
 *    100 % DONE
 */
/****************************************************************************/
static void TRK_SetBufferUsed(MessageBuffer *buffer, BOOL inUse)
{
#if DEBUG_ASSERT
    ASSERT(buffer);
#endif

    buffer->fInUse = inUse;
}

/****************************************************************************/
/*
 *    TRK_InitializeMessageBuffers
 *
 *    100 % DONE (Contents were changed between releases to the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_InitializeMessageBuffers(void)
{
    MessageBufferId i;

    for (i = 0; i < kMessageBufferCount; i++)
    {
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKInitializeMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
        TRKAcquireMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
#endif

        gTRKMsgBufs.fBuffers[i].fInUse = FALSE;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKReleaseMutex(&gTRKMsgBufs.fBuffers[i].fMutex);
#endif
    }

    return kNoError;
}

/****************************************************************************/
/*
 *    TRK_GetFreeBuffer
 *
 *    100 % DONE (Contents were changed between releases to the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_GetFreeBuffer(MessageBufferId *resultBufferID,
                            MessageBuffer **resultBuffer)
{
    DSError err = kMessageBuffersFull;          /* assume failure */
    MessageBufferId i;
    MessageBuffer *b;

    *resultBuffer = NULL;

    /*
        linear search through the buffers to find one that is not in use
    */

    for (i = 0; i < kMessageBufferCount; i++)
    {
        b = TRKGetBuffer(i);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKAcquireMutex(&b->fMutex);
#endif

        if (b->fInUse == FALSE)
        {
            /* initialize buffer */

            TRKResetBuffer(b, kDontZero);
            TRK_SetBufferUsed(b, TRUE);

            *resultBuffer = b;
            *resultBufferID = i;
            err = kNoError;

            i = kMessageBufferCount;        /* terminate iteration */
        }

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKReleaseMutex(&b->fMutex);
#endif
    }

    if (err == kMessageBuffersFull)
        OSReport("MetroTRK - ERROR : No buffer available\n");

    return err;
}

/****************************************************************************/
/*
 *    TRKGetBuffer
 *
 *    Get a pointer to a single buffer structure
 *
 *    100 % DONE
 */
/****************************************************************************/
MessageBuffer *TRKGetBuffer(MessageBufferId bufferID)
{
    MessageBuffer *buffer = NULL;

    if ((bufferID >= 0) && (bufferID < kMessageBufferCount))
    {
        buffer = &gTRKMsgBufs.fBuffers[bufferID];
    }

    return buffer;
}

/****************************************************************************/
/*
 *    TRK_ReleaseBuffer
 *
 *    Mark the buffer as unused
 *
 *    100 % DONE (Contents were changed between releases to the Wii / NDEV)
 */
/****************************************************************************/
void TRK_ReleaseBuffer(MessageBufferId bufferID)
{
    if (bufferID == kInvalidMessageBufferId)
        return;

    if ((bufferID >= 0) && (bufferID < kMessageBufferCount))
    {
        MessageBuffer *b = &gTRKMsgBufs.fBuffers[bufferID];

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKAcquireMutex(&b->fMutex);
#endif

        TRK_SetBufferUsed(b, FALSE);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRKReleaseMutex(&b->fMutex);
#endif
    }
}

/****************************************************************************/
/*
 *    TRKResetBuffer
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKResetBuffer(MessageBuffer *buffer, ResetBufferOption option)
{
#if DEBUG_ASSERT
    ASSERT(buffer);
#endif

    buffer->fLength = 0;                    /* reset logical length */
    buffer->fPosition = 0;                  /* reset current stream position */

    if (option == kZero)
    {
        TRK_memset(buffer->fData, kZero, kMessageBufferSize);
    }
}

/****************************************************************************/
/*
 *    TRK_SetBufferPosition
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRK_SetBufferPosition(MessageBuffer *buffer, u32 position)
{
    DSError err = kNoError;

#if DEBUG_ASSERT
    ASSERT(buffer);
#endif

    if (position > kMessageBufferSize)
    {
        err = kMessageBufferOverflow;
    }
    else
    {
        buffer->fPosition = position;

        if (position > buffer->fLength)
        {
            buffer->fLength = position;
        }
    }

    return err;
}

/****************************************************************************/
/*
 *    TRK_AppendBuffer
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRK_AppendBuffer(MessageBuffer *buffer, const void *data, size_t length)
{
    DSError  err = kNoError;
    u32      remaining;

#if DEBUG_ASSERT
    ASSERT(buffer);                            /* buffer must be valid */
    ASSERT(data);                              /* data pointer must be valid */
    ASSERT(buffer->fInUse);                    /* buffer must be allocated */
#endif

    if (length == 0)
        return err;

    /*
     *    Check the remaining space in buffer
     */

    remaining = (kMessageBufferSize - buffer->fPosition);

    if (remaining < length)
    {
        /* limit the copy amount */

        err = kMessageBufferOverflow;
        length = remaining;
    }

    /*
    ** Optimized for the 1-byte case.  This helps the input loop
    ** deal with high-speed communications.
    */

    if (length == 1)
        buffer->fData[buffer->fPosition] = DSFetch_u8(data);
    else
        TRK_memcpy((buffer->fData + buffer->fPosition), data, length);

    buffer->fPosition += length;
    buffer->fLength = buffer->fPosition;

#if DEBUG_ASSERT
    ASSERT((buffer->fPosition >= 0) && (buffer->fPosition <= kMessageBufferSize));
#endif

    return err;
}

/****************************************************************************/
/*
 *    AppendBuffer
 *
 *    There's something odd about inlining so put this separately here...
 *
 *    100 % DONE (Basically a copy of TRK_AppendBuffer but used for inlining)
 */
/****************************************************************************/
static inline DSError AppendBuffer(MessageBuffer *buffer, const void *data, size_t length)
{
    DSError  err = kNoError;
    u32      remaining;

#if DEBUG_ASSERT
    ASSERT(buffer);                            /* buffer must be valid */
    ASSERT(data);                              /* data pointer must be valid */
    ASSERT(buffer->fInUse);                    /* buffer must be allocated */
#endif

    if (length == 0)
        return err;

    /*
     *    Check the remaining space in buffer
     */

    remaining = (kMessageBufferSize - buffer->fPosition);

    if (remaining < length)
    {
        /* limit the copy amount */

        err = kMessageBufferOverflow;
        length = remaining;
    }

    /*
    ** Optimized for the 1-byte case.  This helps the input loop
    ** deal with high-speed communications.
    */

    if (length == 1)
        buffer->fData[buffer->fPosition] = DSFetch_u8(data);
    else
        TRK_memcpy((buffer->fData + buffer->fPosition), data, length);

    buffer->fPosition += length;
    buffer->fLength = buffer->fPosition;

#if DEBUG_ASSERT
    ASSERT((buffer->fPosition >= 0) && (buffer->fPosition <= kMessageBufferSize));
#endif

    return err;
}

/****************************************************************************/
/*
 *    TRK_ReadBuffer
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRK_ReadBuffer(MessageBuffer *buffer, void *data, size_t length)
{
    DSError  err = kNoError;
    u32      remaining;

#if DEBUG_ASSERT
    ASSERT(buffer);                            /* buffer must be valid */
    ASSERT(data);                              /* data pointer must be valid */
    ASSERT(buffer->fInUse);                    /* buffer must be allocated */
#endif

    if (length == 0)
        return err;

    /*
     *    Check the remaining space in buffer
     */

    remaining = (buffer->fLength - buffer->fPosition);

    if (length > remaining)
    {
        /* limit the copy amount */

        err = kMessageBufferUnderflow;
        length = remaining;
    }

    TRK_memcpy(data, (buffer->fData + buffer->fPosition), length);

    buffer->fPosition += length;

#if DEBUG_ASSERT
    ASSERT((buffer->fPosition >= 0) && (buffer->fPosition <= kMessageBufferSize));
#endif

    return err;
}

/****************************************************************************/
/*
 *    ReadBuffer
 *
 *    There's something odd about inlining so put this separately here...
 *
 *    100 % DONE (A modified copy of TRK_ReadBuffer but used for inlining)
 */
/****************************************************************************/
static inline DSError ReadBuffer(MessageBuffer *buffer, void *data, size_t length)
{
    DSError  err = kNoError;
    u32      remaining;

#if DEBUG_ASSERT
    ASSERT(buffer);                            /* buffer must be valid */
    ASSERT(data);                              /* data pointer must be valid */
    ASSERT(buffer->fInUse);                    /* buffer must be allocated */
#endif

    if (length == 0)
        return err;

    /*
     *    Check the remaining space in buffer
     */

    remaining = (buffer->fLength - buffer->fPosition);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    if (length < remaining)
#else
    if (length > remaining)
#endif
    {
        /* limit the copy amount */

        err = kMessageBufferUnderflow;
        length = remaining;
    }

    TRK_memcpy(data, (buffer->fData + buffer->fPosition), length);

    buffer->fPosition += length;

#if DEBUG_ASSERT
    ASSERT((buffer->fPosition >= 0) && (buffer->fPosition <= kMessageBufferSize));
#endif

    return err;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer1_ui8
 *
 *    100 % DONE
 */
/****************************************************************************/

/* SCM - replaced with inline version down below
DSError TRKAppendBuffer1_ui8(MessageBuffer *buffer, const u8 data)
{
    return AppendBuffer(buffer, (const void *)&data, sizeof(data));
}
*/

/*
 * This u8 one is a special case and can be s16-circuited.
 * This optimization helps the input loop deal with high-
 * speed communications.
 */

#ifdef _ENTERPRISE_C_
DSError TRKAppendBuffer1_ui8(MessageBuffer *buffer, const u8 data)
#else
inline DSError TRKAppendBuffer1_ui8(MessageBuffer *buffer, const u8 data)
#endif
{
#ifdef _ENTERPRISE_C_
    #pragma inline
#endif
    if (buffer->fPosition >= kMessageBufferSize)
        return kMessageBufferOverflow;

    buffer->fData[buffer->fPosition++] = data;
    buffer->fLength++;

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer1_ui16
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer1_ui16(MessageBuffer *buffer, const u16 data)
{
    /*
    ** Translate data here based on target-endianness
    */

    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
    {
        bigEndianData = (u8 *)&data;
    }
    else
    {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[1];
        bigEndianData[1] = byteData[0];
    }

    return AppendBuffer(buffer, (const void *)bigEndianData,
                            sizeof(data));
}

/****************************************************************************/
/*
 *    TRKAppendBuffer1_ui32
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer1_ui32(MessageBuffer *buffer, const u32 data)
{
    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
    {
        bigEndianData = (u8 *)&data;
    }
    else
    {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[3];
        bigEndianData[1] = byteData[2];
        bigEndianData[2] = byteData[1];
        bigEndianData[3] = byteData[0];
    }

    return AppendBuffer(buffer, (const void *)bigEndianData,
                            sizeof(data));
}

/****************************************************************************/
/*
 *    TRKAppendBuffer1_ui64
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer1_ui64(MessageBuffer *buffer, const u64 data)
{
    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
    {
        bigEndianData = (u8 *)&data;
    }
    else
    {
        byteData = (u8 *)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[7];
        bigEndianData[1] = byteData[6];
        bigEndianData[2] = byteData[5];
        bigEndianData[3] = byteData[4];
        bigEndianData[4] = byteData[3];
        bigEndianData[5] = byteData[2];
        bigEndianData[6] = byteData[1];
        bigEndianData[7] = byteData[0];
    }

    return AppendBuffer(buffer, (const void *)bigEndianData,
                            sizeof(data));
}

/****************************************************************************/
/*
 *    TRKAppendBuffer1_ui128
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer1_ui128(MessageBuffer *buffer, const ui128 data)
{
    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 *byteData;
    u8 swapBuffer[sizeof(u128)];

    if (gTRKBigEndian)
    {
        bigEndianData = (u8 *)data;
    }
    else
    {
        byteData = (u8 *)data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[15];
        bigEndianData[1] = byteData[14];
        bigEndianData[2] = byteData[13];
        bigEndianData[3] = byteData[12];
        bigEndianData[4] = byteData[11];
        bigEndianData[5] = byteData[10];
        bigEndianData[6] = byteData[9];
        bigEndianData[7] = byteData[8];
        bigEndianData[8] = byteData[7];
        bigEndianData[9] = byteData[6];
        bigEndianData[10] = byteData[5];
        bigEndianData[11] = byteData[4];
        bigEndianData[12] = byteData[3];
        bigEndianData[13] = byteData[2];
        bigEndianData[14] = byteData[1];
        bigEndianData[15] = byteData[0];
    }

    return AppendBuffer(buffer, (const void *)bigEndianData,
                            sizeof(u128));
}

/****************************************************************************/
/*
 *    TRKAppendBuffer_ui8
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer_ui8(MessageBuffer *buffer, const u8 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKAppendBuffer1_ui8(buffer, data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer_ui16
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer_ui16(MessageBuffer *buffer, const u16 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKAppendBuffer1_ui16(buffer, data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer_ui32
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer_ui32(MessageBuffer *buffer, const u32 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKAppendBuffer1_ui32(buffer, data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer_ui64
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer_ui64(MessageBuffer *buffer, const u64 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKAppendBuffer1_ui64(buffer, data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKAppendBuffer_ui128
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKAppendBuffer_ui128(MessageBuffer *buffer, const ui128 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKAppendBuffer1_ui128(buffer, data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer1_ui8
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer1_ui8(MessageBuffer *buffer, u8 *data)
{
    return ReadBuffer(buffer, (void *)data, 1);
}

/****************************************************************************/
/*
 *    TRKReadBuffer1_ui16
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer1_ui16(MessageBuffer *buffer, u16 *data)
{
    DSError err;

    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;

    err = ReadBuffer(buffer, (void *)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && (err == kNoError))
    {
        u8 *byteData = (u8 *)data;

        byteData[0] = bigEndianData[1];
        byteData[1] = bigEndianData[0];
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer1_ui32
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer1_ui32(MessageBuffer *buffer, u32 *data)
{
    DSError err;

    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;

    err = ReadBuffer(buffer, (void *)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && (err == kNoError))
    {
        u8 *byteData = (u8 *)data;

        byteData[0] = bigEndianData[3];
        byteData[1] = bigEndianData[2];
        byteData[2] = bigEndianData[1];
        byteData[3] = bigEndianData[0];
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer1_ui64
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer1_ui64(MessageBuffer *buffer, u64 *data)
{
    DSError err;

    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;

    err = ReadBuffer(buffer, (void *)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && (err == kNoError))
    {
        u8 *byteData = (u8 *)data;

        byteData[0] = bigEndianData[7];
        byteData[1] = bigEndianData[6];
        byteData[2] = bigEndianData[5];
        byteData[3] = bigEndianData[4];
        byteData[4] = bigEndianData[3];
        byteData[5] = bigEndianData[2];
        byteData[6] = bigEndianData[1];
        byteData[7] = bigEndianData[0];
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer1_ui128
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer1_ui128(MessageBuffer *buffer, ui128 *data)
{
    DSError err;

    /*
    ** Translate data here based on target-endianess.
    */

    u8 *bigEndianData;
    u8 swapBuffer[sizeof(u128)];

    if (gTRKBigEndian)
        bigEndianData = (u8 *)data;
    else
        bigEndianData = swapBuffer;

    err = ReadBuffer(buffer, (void *)bigEndianData, sizeof(u128));

    if (!gTRKBigEndian && (err == kNoError))
    {
        u8 *byteData = (u8 *)data;

        byteData[0] = bigEndianData[15];
        byteData[1] = bigEndianData[14];
        byteData[2] = bigEndianData[13];
        byteData[3] = bigEndianData[12];
        byteData[4] = bigEndianData[11];
        byteData[5] = bigEndianData[10];
        byteData[6] = bigEndianData[9];
        byteData[7] = bigEndianData[8];
        byteData[8] = bigEndianData[7];
        byteData[9] = bigEndianData[6];
        byteData[10] = bigEndianData[5];
        byteData[11] = bigEndianData[4];
        byteData[12] = bigEndianData[3];
        byteData[13] = bigEndianData[2];
        byteData[14] = bigEndianData[1];
        byteData[15] = bigEndianData[0];
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer_ui8
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer_ui8(MessageBuffer *buffer, u8 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKReadBuffer1_ui8(buffer, &data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer_ui16
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer_ui16(MessageBuffer *buffer, u16 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKReadBuffer1_ui16(buffer, &data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer_ui32
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer_ui32(MessageBuffer *buffer, u32 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKReadBuffer1_ui32(buffer, &data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer_ui64
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer_ui64(MessageBuffer *buffer, u64 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKReadBuffer1_ui64(buffer, &data[i]);

    return err;
}

/****************************************************************************/
/*
 *    TRKReadBuffer_ui128
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKReadBuffer_ui128(MessageBuffer *buffer, ui128 *data, s32 count)
{
    DSError err;
    s32 i;

    for (i = 0, err = kNoError; (err == kNoError) && (i < count); i++)
        err = TRKReadBuffer1_ui128(buffer, ((ui128 *)&data[i]));

    return err;
}
