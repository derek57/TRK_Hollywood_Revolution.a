/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases by MetroWerks / Freescale Semiconductor / NXP.

    Filename ("cc_gdev.c") was "main.c" for v0.1 of the TRK to the Wii / NDEV

COPYRIGHT

    2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#ifdef __GNUC__
    #include <ogc/irq.h>
#endif

#include "trktypes.h"
#include "cc_gdev.h"
#include "CircleBuffer.h"
#include "dbgdrv.h"


// (0x500) - same size as the gTRKInputPendingPtr <-> Context
#define CIRCLEBUFFER_SIZE   1280

#define NUM_CIRCLEBUFFERS   7


enum
{
    GDEV_ERROR_NOT_INITIALIZED = -0x2711,
    GDEV_ERROR_ALREADY_INITIALIZED = -0x2715,
    GDEV_ERROR_NO_BUFFER_DATA = -0x2719
};


// NEVER USED
#ifndef __GNUC__ // stop complaining about unused variables
static u32 gLastError = kGDEVNoError;
#endif

static BOOL gIsInitialized = FALSE;


/****************************************************************************/
/*
 *    OutputData
 *
 *    100 % DONE with help of Chippy @ decomp.me (Discord)
 */
/****************************************************************************/
void OutputData(u8 *data, s32 length)
{
#if defined(__MWERKS__)

    #if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        #pragma unused(data, length)
    #else
        #pragma unused(data)
    #endif

#elif defined(__GNUC__)

    #if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        UNUSED(data);
    #endif
    UNUSED(length);

#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    s32 i;

    for (i = 0; i < length; i++)
    {
    }

#endif

    return;
}


static u32 gRecvCB[NUM_CIRCLEBUFFERS];
static u8 gRecvBuf[CIRCLEBUFFER_SIZE];


/****************************************************************************/
/*
 *    IsInitialized
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
static BOOL IsInitialized(void)
{
    return gIsInitialized;
}

/****************************************************************************/
/*
 *    gdev_cc_initialize
 *
 *    100 % DONE
 */
/****************************************************************************/
#if defined(__MWERKS__)
GDEVError gdev_cc_initialize(volatile u8 **inputPendingPtrRef, DBCallback cb)
#elif defined(__GNUC__)
GDEVError gdev_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb)
#endif
{
    DBInitComm(inputPendingPtrRef, cb);

    CircleBufferInitialize(gRecvCB, (u32)gRecvBuf, CIRCLEBUFFER_SIZE);

    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_shutdown
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_shutdown(void)
{
    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_open
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_open(void)
{
    if (gIsInitialized != FALSE)
        return GDEV_ERROR_ALREADY_INITIALIZED;

    gIsInitialized = TRUE;

    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_close
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_close(void)
{
    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_read
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_read(u8 *buffer, s32 length)
{
    GDEVError err;
    u8 circlebuffer[CIRCLEBUFFER_SIZE];

    err = kGDEVNoError;

    if (!gIsInitialized)
        return GDEV_ERROR_NOT_INITIALIZED;

    while (CBGetBytesAvailableForRead(gRecvCB) < (u32)length)
    {
        s32 query_length;

        err = kGDEVNoError;
        query_length = DBQueryData();

        if (query_length)
        {
            err = (s32)DBRead(circlebuffer, (u32)length);

            if (!err)
                CircleBufferWriteBytes(gRecvCB, circlebuffer, (size_t)query_length);
        }
    }

    if (!err)
        CircleBufferReadBytes(gRecvCB, buffer, (size_t)length);

    return err;
}

/****************************************************************************/
/*
 *    gdev_cc_write
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_write(u8 *buffer, s32 length)
{
    if (!gIsInitialized)
        return GDEV_ERROR_NOT_INITIALIZED;

    while (length > 0)
    {
        u32 bytes_written = DBWrite(buffer, (u32)length);

        if (!bytes_written)
            break;

        buffer += bytes_written;
        length -= bytes_written;
    }

    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_pre_continue
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_pre_continue(void)
{
    DBClose();

    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_post_stop
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_post_stop(void)
{
    DBOpen();

    return kGDEVNoError;
}

/****************************************************************************/
/*
 *    gdev_cc_peek
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_peek(void)
{
    u8 buffer[CIRCLEBUFFER_SIZE];
    s32 query_length = DBQueryData();

    if (query_length <= 0)
        return kGDEVNoError;

    if (DBRead(buffer, (u32)query_length) == kGDEVNoError)
        CircleBufferWriteBytes(gRecvCB, buffer, (size_t)query_length);
    else
        return GDEV_ERROR_NO_BUFFER_DATA;

    return query_length;
}

/****************************************************************************/
/*
 *    gdev_cc_initinterrupts
 *
 *    100 % DONE
 */
/****************************************************************************/
GDEVError gdev_cc_initinterrupts(void)
{
    DBInitInterrupts();

    return kGDEVNoError;
}
