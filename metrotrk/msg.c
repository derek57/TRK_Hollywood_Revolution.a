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
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
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
    s32 err;

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
    err = TRK_WriteUARTN((s8 *)&buffer->fData, buffer->fLength);

#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    if (err)
        OSReport("MetroTRK - TRK_WriteUARTN returned %ld\n", err);
#endif

    return kNoError;
}
