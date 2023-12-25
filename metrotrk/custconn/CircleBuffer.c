/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases by MetroWerks / Freescale Semiconductor / NXP.

COPYRIGHT

    2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "MWCriticalSection_gc.h"
#include "CircleBuffer.h"
#include "trkstring.h"
#include "msgbuf.h"


/****************************************************************************/
/*
 *    CBGetBytesAvailableForRead
 *
 *    100 % DONE
 */
/****************************************************************************/
u32 CBGetBytesAvailableForRead(u32 *buffer)
{
    return buffer[4];
}

/****************************************************************************/
/*
 *    CBGetBytesAvailableForWrite
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
u32 CBGetBytesAvailableForWrite(u32 *buffer)
{
    return buffer[5];
}

/****************************************************************************/
/*
 *    CircleBufferInitialize
 *
 *    100 % DONE
 */
/****************************************************************************/
void CircleBufferInitialize(u32 *dest, u32 src, u32 buffersize)
{
    dest[2] = src;
    dest[3] = buffersize;
    dest[0] = dest[2];
    dest[1] = dest[2];
    dest[4] = 0;
    dest[5] = dest[3];

    MWInitializeCriticalSection(&dest[6]);
}

/****************************************************************************/
/*
 *    CircleBufferTerminate
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
void CircleBufferTerminate(u8 *buffer)
{
    MWTerminateCriticalSection(&buffer[24]);
    memset(buffer, kZero, 0x1C);
}

/****************************************************************************/
/*
 *    CircleBufferWriteBytes
 *
 *    100 % DONE
 */
/****************************************************************************/
CBError CircleBufferWriteBytes(u32 *buffer, u8 *data, size_t length)
{
    size_t size;

    if (length > buffer[5])
        return kCBDataError;

    MWEnterCriticalSection(&buffer[6]);
    size = buffer[3] - (buffer[1] - buffer[2]);

    if (size >= length)
    {
        memcpy((void *)buffer[1], data, length);
        buffer[1] += length;
    }
    else
    {
        memcpy((void *)buffer[1], data, size);
        memcpy((void *)buffer[2], &data[size], (length - size));
        buffer[1] = ((buffer[2] + length) - size);
    }

    if (buffer[3] == (buffer[1] - buffer[2]))
        buffer[1] = buffer[2];

    buffer[5] -= length;
    buffer[4] += length;
    MWExitCriticalSection(&buffer[6]);

    return kCBNoError;
}

/****************************************************************************/
/*
 *    CircleBufferReadBytes
 *
 *    100 % DONE
 */
/****************************************************************************/
CBError CircleBufferReadBytes(u32 *buffer, u8 *data, size_t length)
{
    size_t size;

    if (length > buffer[4])
        return kCBDataError;

    MWEnterCriticalSection(&buffer[6]);
    size = buffer[3] - (buffer[0] - buffer[2]);

    if (length < size)
    {
        memcpy(data, (void *)buffer[0], length);
        buffer[0] += length;
    }
    else
    {
        memcpy(data, (void *)buffer[0], size);
        memcpy(&data[size], (void *)buffer[2], (length - size));
        buffer[0] = ((buffer[2] + length) - size);
    }

    if (buffer[3] == (buffer[0] - buffer[2]))
        buffer[0] = buffer[2];

    buffer[5] += length;
    buffer[4] -= length;
    MWExitCriticalSection(&buffer[6]);

    return kCBNoError;
}
