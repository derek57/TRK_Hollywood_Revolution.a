/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases by MetroWerks / Freescale Semiconductor / NXP.

    DDH ("Dolphin Development Hardware") module over EXI: WAS NEVER USED

COPYRIGHT

    2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "trktypes.h"
#include "cc_ddh.h"
#include "CircleBuffer.h"


#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV

// (0x800) - same size as the gTRKInputPendingPtr <-> Context
#define CIRCLEBUFFER_SIZE   2048

#define NUM_CIRCLEBUFFERS   7


enum
{
    GDEV_ERROR_NOT_INITIALIZED = -0x2711,
    GDEV_ERROR_ALREADY_INITIALIZED = -0x2715,
    GDEV_ERROR_NO_BUFFER_DATA = -0x2719
};


// NEVER USED
#ifndef __GNUC__ // stop complaining about unused variables
static u32 gLastError = kDDHNoError;
#endif

static BOOL gIsInitialized = FALSE;

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
 *    ddh_cc_initialize
 *
 *    100 % DONE
 */
/****************************************************************************/
#if defined(__MWERKS__)
DDHError ddh_cc_initialize(volatile u8 **inputPendingPtrRef, EXICB cb)
#elif defined(__GNUC__)
DDHError ddh_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb)
#endif
{
    EXI2_Init(inputPendingPtrRef, cb);

    CircleBufferInitialize(gRecvCB, (u32)gRecvBuf, CIRCLEBUFFER_SIZE);

    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_shutdown
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_shutdown(void)
{
    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_open
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_open(void)
{
    if (gIsInitialized != FALSE)
        return GDEV_ERROR_ALREADY_INITIALIZED;

    gIsInitialized = TRUE;

    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_close
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_close(void)
{
    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_read
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_read(u8 *buffer, u32 length)
{
    DDHError err;
    u8 circlebuffer[CIRCLEBUFFER_SIZE];

    err = kDDHNoError;

    if (!gIsInitialized)
        return GDEV_ERROR_NOT_INITIALIZED;

    while (CBGetBytesAvailableForRead(gRecvCB) < length)
    {
        s32 query_length;

        err = kDDHNoError;
        query_length = EXI2_Poll();

        if (query_length)
        {
            err = (s32)EXI2_ReadN(circlebuffer, (u32)query_length);

            if (!err)
                CircleBufferWriteBytes(gRecvCB, circlebuffer, (size_t)query_length);
        }
    }

    if (!err)
        CircleBufferReadBytes(gRecvCB, buffer, length);

    return err;
}

/****************************************************************************/
/*
 *    ddh_cc_write
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_write(u8 *buffer, s32 length)
{
    if (!gIsInitialized)
        return GDEV_ERROR_NOT_INITIALIZED;

    while (length > 0)
    {
        s32 bytes_written = (s32)EXI2_WriteN(buffer, (u32)length);

        if (!bytes_written)
            break;

        buffer += bytes_written;
        length -= bytes_written;
    }

    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_pre_continue
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_pre_continue(void)
{
    EXI2_Unreserve();

    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_post_stop
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_post_stop(void)
{
    EXI2_Reserve();

    return kDDHNoError;
}

/****************************************************************************/
/*
 *    ddh_cc_peek
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_peek(void)
{
    u8 buffer[CIRCLEBUFFER_SIZE];
    s32 query_length = EXI2_Poll();

    if (query_length <= 0)
        return kDDHNoError;

    if (EXI2_ReadN(buffer, (u32)query_length) == kDDHNoError)
        CircleBufferWriteBytes(gRecvCB, buffer, (size_t)query_length);
    else
        return GDEV_ERROR_NO_BUFFER_DATA;

    return query_length;
}

/****************************************************************************/
/*
 *    ddh_cc_initinterrupts
 *
 *    100 % DONE
 */
/****************************************************************************/
DDHError ddh_cc_initinterrupts(void)
{
    EXI2_EnableInterrupts();

    return kDDHNoError;
}

#endif
