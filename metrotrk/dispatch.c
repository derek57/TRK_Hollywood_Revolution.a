/****************************************************************************/
/*

FILE

    dispatch.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/dispatch.c,v 1.3 2000/06/16 21:52:53 smoore Exp $

DESCRIPTION

    Command dispatcher

HISTORY

    97 FEB 07 LLY    Created.
    97 FEB 12 LLY    Removed const from prototype.  Added calls to handler
                     functions.

    $History: dispatch.c $
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 2/08/99    Time: 12:50a
//    Updated in $/Embedded/MetroTRK/Portable
//    Added dispatch call for the CPUType command.
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
//    User: Smoore       Date: 6/25/98    Time: 2:35p
//    Updated in $/Embedded/MetroTRK/Portable\t
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 12/08/97   Time: 7:37p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Now calling DoUnsupported() instead of DoError() when for unsupported
//    commands.  Unsupported commands now return an ACK with a
//    kDSReplyUnsupportedCommandError rather than a kDSReplyNoError
//    code.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:21p
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


// Must stay here...
#define DEBUG_DISPATCH  0


#include "msghndlr.h"
#include "dispatch.h"

#if DEBUG_DISPATCH
    #include "osreport.h"
#endif

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
#include "nubassrt.h"
#include "usr_put.h"
#include "msg.h"
#include "mem_TRK.h"


//
// NOTE: MetroTRK for the Wii / NDEV did NOT make use of "VIRTUAL_DISPATCH"
//

#ifdef VIRTUAL_DISPATCH
/****************************************************************************/
/*
 *    The dispatch table is a jumptable for the handlers for each command.
 *    This table contains the default, minimum set.  The table can be
 *    overridden at runtime for customization by a custom nub.
 */
/****************************************************************************/

#if __MWERKS__
    #pragma options align=power
#endif /* __MWERKS__ */

u32 gDispatchTableSize = 0;

DispatchEntry gDispatchTable[] =
{
    {    &TRKDoUnsupported        },                /* 0x00 reserved (Error) */
    {    &TRK_DoConnect           },                /* 0x01 Connect */
    {    &TRKDoDisconnect         },                /* 0x02 Disconnect */
    {    &TRKDoReset              },                /* 0x03 Reset */
    {    &TRKDoVersions           },                /* 0x04 Versions */
    {    &TRKDoSupportMask        },                /* 0x05 SupportMask */
    {    &TRKDoCPUType            },                /* 0x06 CPUType */
    {    &TRKDoUnsupported        },                /* 0x07 reserved */

    {    &TRKDoUnsupported        },                /* 0x08 reserved */
    {    &TRKDoUnsupported        },                /* 0x09 reserved */
    {    &TRKDoUnsupported        },                /* 0x0A reserved */
    {    &TRKDoUnsupported        },                /* 0x0B reserved */
    {    &TRKDoUnsupported        },                /* 0x0C reserved */
    {    &TRKDoUnsupported        },                /* 0x0D reserved */
    {    &TRKDoUnsupported        },                /* 0x0E reserved */
    {    &TRKDoUnsupported        },                /* 0x0F reserved */

    {    &TRKDoReadMemory         },                /* 0x10 ReadMemory */
    {    &TRKDoWriteMemory        },                /* 0x11 WriteMemory */
    {    &TRKDoReadRegisters      },                /* 0x12 ReadRegisters */
    {    &TRKDoWriteRegisters     },                /* 0x13 WriteRegisters */
    {    &TRKDoUnsupported        },                /* 0x14 expansion: state command */
    {    &TRKDoUnsupported        },                /* 0x15 expansion: state command */
    {    &TRKDoFlushCache         },                /* 0x16 FlushCache */
    {    &TRKDoUnsupported        },                /* 0x17 expansion: state command */

    {    &TRKDoContinue           },                /* 0x18 Continue */
    {    &TRKDoStep               },                /* 0x19 Step */
    {    &TRKDoStop               },                /* 0x1A Stop */
    {    &TRKDoUnsupported        },                /* 0x1B expansion: control command */
    {    &TRKDoUnsupported        },                /* 0x1C expansion: control command */
    {    &TRKDoUnsupported        },                /* 0x1D expansion: control command */
    {    &TRKDoUnsupported        },                /* 0x1E expansion: control command */
    {    &TRKDoUnsupported        },                /* 0x1F expansion: control command */

#if DS_PROTOCOL == DS_PROTOCOL_RTOS

    {    &TRKDoUnsupported        },                /* 0x20 reserved */
    {    &TRKDoUnsupported        },                /* 0x21 reserved */
    {    &TRKDoUnsupported        },                /* 0x22 reserved */
    {    &TRKDoUnsupported        },                /* 0x23 reserved */
    {    &TRKDoUnsupported        },                /* 0x24 reserved */
    {    &TRKDoUnsupported        },                /* 0x25 reserved */
    {    &TRKDoUnsupported        },                /* 0x26 reserved */
    {    &TRKDoUnsupported        },                /* 0x27 reserved */

    {    &TRKDoUnsupported        },                /* 0x28 reserved */
    {    &TRKDoUnsupported        },                /* 0x29 reserved */
    {    &TRKDoUnsupported        },                /* 0x2A reserved */
    {    &TRKDoUnsupported        },                /* 0x2B reserved */
    {    &TRKDoUnsupported        },                /* 0x2C reserved */
    {    &TRKDoUnsupported        },                /* 0x2D reserved */
    {    &TRKDoUnsupported        },                /* 0x2E reserved */
    {    &TRKDoUnsupported        },                /* 0x2F reserved */

    {    &TRKDoUnsupported        },                /* 0x30 reserved */
    {    &TRKDoUnsupported        },                /* 0x31 reserved */
    {    &TRKDoUnsupported        },                /* 0x32 reserved */
    {    &TRKDoUnsupported        },                /* 0x33 reserved */
    {    &TRKDoUnsupported        },                /* 0x34 reserved */
    {    &TRKDoUnsupported        },                /* 0x35 reserved */
    {    &TRKDoUnsupported        },                /* 0x36 reserved */
    {    &TRKDoUnsupported        },                /* 0x37 reserved */

    {    &TRKDoUnsupported        },                /* 0x38 reserved */
    {    &TRKDoUnsupported        },                /* 0x39 reserved */
    {    &TRKDoUnsupported        },                /* 0x3A reserved */
    {    &TRKDoUnsupported        },                /* 0x3B reserved */
    {    &TRKDoUnsupported        },                /* 0x3C reserved */
    {    &TRKDoUnsupported        },                /* 0x3D reserved */
    {    &TRKDoUnsupported        },                /* 0x3E reserved */
    {    &TRKDoUnsupported        },                /* 0x3F reserved */

    /*
    ** The following commands may or may not be supported by a
    ** particular OS implementation.  If they are supported, use
    ** OverrideDispatch() to set the appropriate handler functions.
    */

    {    &TRKDoUnsupported        },                /* 0x40 OSCreateItem */
    {    &TRKDoUnsupported        },                /* 0x41 OSDeleteItem */
    {    &TRKDoUnsupported        },                /* 0x42 OSReadInfo */
    {    &TRKDoUnsupported        },                /* 0x43 OSWriteInfo */
    {    &TRKDoUnsupported        },                /* 0x44 reserved */
    {    &TRKDoUnsupported        },                /* 0x45 reserved */
    {    &TRKDoUnsupported        },                /* 0x46 reserved */
    {    &TRKDoUnsupported        },                /* 0x47 reserved */

    {    &TRKDoUnsupported        },                /* 0x48 OSWriteFile */
    {    &TRKDoUnsupported        },                /* 0x49 OSReadFile */
    {    &TRKDoUnsupported        },                /* 0x4A OSOpenFile */
    {    &TRKDoUnsupported        },                /* 0x4B OSCloseFile */
    {    &TRKDoUnsupported        },                /* 0x4C OSPositionFile */
    {    &TRKDoUnsupported        },                /* 0x4D reserved */
    {    &TRKDoUnsupported        },                /* 0x4E reserved */
    {    &TRKDoUnsupported        },                /* 0x4F reserved */

#endif /* #if DS_PROTOCOL == DS_PROTOCOL_RTOS */

    {    (DispatchFunctionPtr) 0  }                 /* sentinel */
};
#endif /* VIRTUAL_DISPATCH */
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKInitializeDispatcher
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKInitializeDispatcher(void)
{
#ifdef VIRTUAL_DISPATCH
    gDispatchTableSize = ((sizeof(gDispatchTable) / sizeof(DispatchEntry)) - 1);
#endif

    return kNoError;
}
#endif

#ifdef VIRTUAL_DISPATCH // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    OverrideDispatch
 */
/****************************************************************************/
DSError OverrideDispatch(MessageCommandID cmdID, const DispatchEntry *newEntry,
                                 DispatchEntry *returnedOldEntry)
{
    u32 result = kNoError;                /* assume OK */

#if DEBUG_ASSERT
    ASSERT(!"implemented, but untested");
    ASSERT(gDispatchTableSize);
#endif

    if (cmdID >= gDispatchTableSize)
    {
        result = kDispatchErr;
    }
    else
    {
        if (returnedOldEntry)
        {
            /* store the return value if the caller asked to get the old one */

            TRK_memcpy(returnedOldEntry, (gDispatchTable + cmdID),
                    sizeof(*returnedOldEntry));
        }

        /* stuff in the new dispatch vector */

        TRK_memcpy((gDispatchTable + cmdID), newEntry, sizeof(*newEntry));
    }

    return result;
}
#endif /* VIRTUAL_DISPATCH */

/****************************************************************************/
/*
 *    TRK_DispatchMessage
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRK_DispatchMessage(MessageBuffer *buffer)
{
    s32 err = kDispatchErr;

#ifdef DEBUG_ASSERT
    ASSERT(buffer);
#endif

    TRK_SetBufferPosition(buffer, kZero);

#if DEBUG_DISPATCH
    OSReport("cmd=%02x\n", (MessageCommandID)buffer->fData[4]);
#endif

    switch ((MessageCommandID)buffer->fData[4])
    {
        case kDSConnect:
            err = TRK_DoConnect(buffer);
            break;

        case kDSDisconnect:
            err = TRKDoDisconnect(buffer);
            break;

        case kDSReset:
            err = TRKDoReset(buffer);
            break;

        case kDSConfigTransport:
            err = TRKDoOverride(buffer);
            break;

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        case kDSVersions:
            err = TRKDoVersions(buffer);
            break;

        case kDSSupportMask:
            err = TRKDoSupportMask(buffer);
            break;
#endif

        case kDSReadMemory:
            err = TRKDoReadMemory(buffer);
            break;

        case kDSWriteMemory:
            err = TRKDoWriteMemory(buffer);
            break;

        case kDSReadRegisters:
            err = TRKDoReadRegisters(buffer);
            break;

        case kDSWriteRegisters:
            err = TRKDoWriteRegisters(buffer);
            break;

        case kDSContinue:
            err = TRKDoContinue(buffer);
            break;

        case kDSStep:
            err = TRKDoStep(buffer);
            break;

        case kDSStop:
            err = TRKDoStop(buffer);
            break;

        // This one seems to be a special case for the
        // Wii / NDEV as it wasn't included in the official
        // source code of CodeWarriorTRK / MetroTRK / AppTRK.

        case kDSCheckTargetState:
            err = TRKDoSetOption(buffer);

        default:
#if DEBUG_DISPATCH
            OSReport("invalid command: %02x\n", (MessageCommandID)buffer->fData[4]);
#endif
            break;
    }

    return err;
}
