/****************************************************************************/
/*

FILE

    msg.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/msg.c,v 1.2 2000/04/27 02:32:14 smoore Exp $

DESCRIPTION

    Serial-framing based implementation of messages.


HISTORY

    97 FEB 07 LLY    Created.
    97 FEB 18 LLY    Escape checksum characters also, e.g.
                    0x7E --> 0x7D 0x5E, (not 0x7D 0x7E)

    $History: msg.c $
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
//    User: Smoore       Date: 6/25/98    Time: 2:02p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:25p
//    Updated in $/Embedded/MetroTRK/Portable
//    Enhanced debug messages.

AUTHORS

    Lawrence You <you@metrowerks.com>

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "dolphin_trk_glue.h"
#include "msg.h"
#include "msgxtrct.h"

#ifndef TRK_PACKET_IO
    #include "usr_put.h"
    #include "serframe.h"


    #define DEBUG_PACKET_OUT    0


    #ifndef FCSBITSIZE
        #error "Framing FCS not defined"
    #endif
#endif


static u16 gPacketSeq = 0;


#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKMessageAdd
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKMessageAdd(MessageBuffer *buffer, u32 data)
{
    DSError err;

    err = AppendBuffer(buffer, &data, sizeof(data));

    return err;
}

/****************************************************************************/
/*
 *    TRKMessageGet
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKMessageGet(MessageBuffer *buffer, u32 *data)
{
#if defined(__MWERKS__)
    #pragma unused(buffer, data)
#elif defined(__GNUC__)
    UNUSED(buffer);
    UNUSED(data);
#endif

    return kNoError;
}
#endif

/****************************************************************************/
/*
 *    TRK_MessageSend
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_MessageSend(MessageBuffer *buffer)
{
#ifdef TRK_PACKET_IO

    DSError err;

#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    u16 currentSequence = gPacketSeq;

    if (!gPacketSeq)
    {
        currentSequence = 1;
    }

    DSFetch_u16(&buffer->fData[6]) = (u16)currentSequence;

    gPacketSeq = (u16)((currentSequence & 0xff00ffff) + 1);
#endif

    //
    // For Dolphin & EXI2, write complete packets without framing.
    //
    err = (DSError)TRK_WriteUARTN((s8 *)&buffer->fData, buffer->fLength);

#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    if (err)
        OSReport("MetroTRK - TRK_WriteUARTN returned %ld\n", err);
#endif

    return kNoError;

#else /* #ifdef TRK_PACKET_IO */

    DSError            err;
    FCSType            fcs;
    FCSType            acc;
    s32                i;

    /*
     *    Walk through buffer, computing frame check sequence
     *    (checksum).
     */
    fcs = PPPINITFCS;

    for (i = 0; i < buffer->fLength; i++)
    {
        fcs = (FCSType)PPPFCS(fcs, buffer->fData[i]);

#if DEBUG_PACKET_OUT
        __puts("[");
        __puthex2(buffer->fData[i]);
        __puts("]");
    }
    __puts("\r\n");
#else
    }
#endif

    fcs = (FCSType)(fcs ^ PPPCOMPFCS);                            /* finish by complementing */

    __puts("MessageSend. FCS = ");
    __puthex8(fcs);
    __puts("\r\n");

    /*
     *    Send bytes out the UART:
     *    [0x7E (frame flag)] [...data...] [FCS:8 or FCS:16 or FCS:32] [0x7E (frame flag)]
     */

    /* FLAG */

    err = (DSError)WriteUART1(PPP_FLAG);

    /* DATA */

    if (err == kNoError)
    {
        for (i = 0; i < buffer->fLength; i++)
        {
            u8 c = buffer->fData[i];

            if ((c == PPP_FLAG) || (c == PPP_ESCAPE))
            {
                /* escape if necessary */
                err = (DSError)WriteUART1(PPP_ESCAPE);
                c ^= PPP_TRANS;

                if (err != kNoError)
                    break;
            }

            err = (DSError)WriteUART1(c);

            if (err != kNoError)
                break;
        } /* for */
    }

    /* FCS */

    /*
     *    FCS always goes out in little endian (LSB, ... , MSB) order
     */

    if (err == kNoError)
    {
        acc = fcs;                                    /* accumulator */

        for (i = 0; i < sizeof(FCSType); i++)
        {
            /* 2- or 4-bytes */
            u8 c = (FCSType)(acc & 0xFF);

#if FCSBITSIZE != FCS8
            acc >>= 8;
#endif

            if ((c == PPP_FLAG) || (c == PPP_ESCAPE))
            {
                /* escape if necessary */
                err = (DSError)WriteUART1(PPP_ESCAPE);
                c ^= PPP_TRANS;

                if (err != kNoError)
                    break;
            }

            err = (DSError)WriteUART1(c);

            if (err != kNoError)
                break;
        } /* for */
    }

    /* FLAG */

    if (err == kNoError)
    {
        err = (DSError)WriteUART1(PPP_FLAG);
    }

    return err;

#endif /* #ifdef TRK_PACKET_IO */
}
