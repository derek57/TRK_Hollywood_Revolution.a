/****************************************************************************/
/*

FILE

    msgcmd.h

    $Header: /u/debugger/embedded/MetroTRK/Export/msgcmd.h,v 1.6.4.1 2001/05/03 15:53:11 wpaul Exp $

DESCRIPTION

    Monitor command constants and structures

HISTORY

    97 FEB 07 LLY    Created.
    97 FEB 15 LLY    Eliminated some ambiguities.
    97 MAY 23 SCM    Changed register r/w commands to make communication
                     with host debugger more efficient.  Added SupportMask
                     command. Added additional return info to the
                     NotifyStopped command.  Added fields to the
                     VersionReply message.

    $History: msgcmd.h $
//
//    *****************  Version 21  *****************
//    User: Smoore       Date: 7/30/99    Time: 3:37p
//    Updated in $/Embedded/MetroTRK/Export
//    - Added M68k & ColdFire processor minor version #'s.
//
//    *****************  Version 19  *****************
//    User: Smoore       Date: 6/08/99    Time: 6:34p
//    Updated in $/Embedded/MetroTRK/Export
//    Expanded PPC processor type defines for 555 and 5xx in general.
//
//    *****************  Version 17  *****************
//    User: Smoore       Date: 4/07/99    Time: 11:13p
//    Updated in $/Embedded/MetroTRK/Export
//    Added processor type definition for MPC8260.
//
//    *****************  Version 15  *****************
//    User: Smoore       Date: 3/15/99    Time: 1:33p
//    Updated in $/Embedded/MetroTRK/Export
//    Added PPC processor types.
//
//    *****************  Version 14  *****************
//    User: Smoore       Date: 2/08/99    Time: 10:44p
//    Updated in $/Embedded/MetroTRK/Export
//    Added additional processor minor types for NEC V8xx.
//
//    *****************  Version 13  *****************
//    User: Smoore       Date: 2/08/99    Time: 12:49a
//    Updated in $/Embedded/MetroTRK/Export
//    Updated API version to 1.8.  Added fields to the reply format for the
//    kDSCPUType message.
//
//    *****************  Version 12  *****************
//    User: Smoore       Date: 7/15/98    Time: 4:35p
//    Updated in $/Embedded/MetroTRK/Export
//    Updated to accomodate latest changes in main source base.
//
//    *****************  Version 10  *****************
//    User: Smoore       Date: 6/25/98    Time: 2:51p
//    Updated in $/Embedded/MetroTRK/Export
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//    *****************  Version 9  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Export
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 2/02/98    Time: 6:04p
//    Updated in $/Embedded/MetroTRK/Export
//    Merging changes from NEC V8xx TRK.
//
//    *****************  Version 8  *****************
//    User: Smoore       Date: 1/30/98    Time: 2:29p
//    Updated in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Export
//    Removed DSFPuts command and added DSWriteFile and DSReadFile
//    commands.  Supports only console I/O for now, but intended to support
//    general file I/O in the future.  Incremented API version to 1.3.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 11/07/97   Time: 5:15p
//    Updated in $/Embedded/MetroTRK/Export
//    Replace '#pragma once' with #ifdef's.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 9/02/97    Time: 3:12p
//    Updated in $/Embedded/MetroTRK/Export
//    Added step-over options to the step command.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 8/07/97    Time: 1:34p
//    Updated in $/Embedded/MetroTRK/Export
//    Added kDSStop command (unsupported) and enhanced command
//    documentation.
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 7/15/97    Time: 9:17a
//    Updated in $/Embedded/MetroTRK/Export
//    Enhanced step command to support stepping until the PC is out of a
//    specified memory range.

AUTHORS

    Steve Moore  <smoore@metrowerks.com>
    Lawrence You <you@metrowerks.com>

    (c) 1996-9 Metrowerks Corporation
    All rights reserved.

*/
/****************************************************************************/
#ifndef __MSGCMD_H__
#define __MSGCMD_H__


#include "trktypes.h"


/****************************************************************************/
/*
 *    Commands
 *
 *    constant definitions of commands sent from host to target
 *
 *    L1 = Level 1 CWDS (required)
 *    L2 = Level 2 CWDS (extended board-level debugger)
 *    L3 = Level 3 CWDS (Hypervisor-level debugger)
 *    L4 = Level 3 CWDS (OS-level debugger)
 */
/****************************************************************************/

#define DS_PROTOCOL_MIN         1    /* L1 */
#define DS_PROTOCOL_BOARD       2    /* L2 */
#define DS_PROTOCOL_HYPER       3    /* L3 */
#define DS_PROTOCOL_RTOS        4    /* L4 */


#if (!defined(DS_PROTOCOL)) || (DS_PROTOCOL == 0)
    #pragma error "Protocol level must be defined"
#endif


#define DS_PROTOCOL_MAJOR_VERSION_1    1
#define DS_PROTOCOL_MINOR_VERSION_10   10

#define DS_PROTOCOL_MAJOR_VERSION      DS_PROTOCOL_MAJOR_VERSION_1
#define DS_PROTOCOL_MINOR_VERSION      DS_PROTOCOL_MINOR_VERSION_10

#define DS_PROTOCOL_VERSION_STRING     version_str(DS_PROTOCOL_MAJOR_VERSION, \
                                                 DS_PROTOCOL_MINOR_VERSION)

/*
 *    MessageCommandID
 */

typedef enum MessageCommandID
{
    /* meta commands, minimum feature set */

    kDSPing                       = 0x00,     /*                            */
    kDSConnect                    = 0x01,     /*    L1    L2    L3    L4    */
    kDSDisconnect                 = 0x02,     /*                L3    L4    */
    kDSReset                      = 0x03,     /*          L2    L3    L4    */
    kDSVersions                   = 0x04,     /*    L1    L2          L4    */
    kDSSupportMask                = 0x05,     /*    L1    L2    L3    L4    */
    kDSCPUType                    = 0x06,     /*          L2    L3    L4    */

    // Wii / NDEV makes use of this... (it's NOT part of TRK protocol v1.10)
    kDSConfigTransport            = 0x07,     /*          L2          L4    */

    kDSConfigAutoDownload         = 0x08,     /*                L3          */

    /* state commands */

    kDSReadMemory                 = 0x10,     /*    L1    L2    L3    L4    */
    kDSWriteMemory                = 0x11,     /*    L1    L2    L3    L4    */
    kDSReadRegisters              = 0x12,     /*    L1    L2    L3    L4    */
    kDSWriteRegisters             = 0x13,     /*    L1    L2    L3    L4    */
    kDSFillMemory                 = 0x14,     /*                            */
    kDSCopyMemory                 = 0x15,     /*                            */
    kDSFlushCache                 = 0x16,     /*                            */

    // Wii / NDEV makes use of this... (it's NOT part of TRK protocol v1.10)
    kDSCheckTargetState           = 0x17,     /*                      L4    */

    /* execution commands */

    kDSContinue                   = 0x18,     /*    L1    L2    L3    L4    */
    kDSStep                       = 0x19,     /*          L2    L3    L4    */
    kDSStop                       = 0x1a,     /*          L2    L3    L4    */
    kDSSetBreak                   = 0x1b,     /*          L2          L4    */
    kDSClearBreak                 = 0x1c,     /*          L2          L4    */
    kDSDownload                   = 0x1d,     /*                            */
    kDSModifyBreakThread          = 0x1e,     /*                      L4    */

    /* Note: kDSSetWatch & kDSClearWatch are obsolete and have been removed */

    /* host->target IO management */
    kDSNotifyFileInput            = 0x20,     /*          L2          L4    */
    kDSBlockFileIo                = 0x21,     /*          L2          L4    */

    /* host->target OS commands */

    kDSOSCreateItem               = 0x40,     /*                      L4    */
    kDSOSDeleteItem               = 0x41,     /*                      L4    */
    kDSOSReadInfo                 = 0x42,     /*                      L4    */
    kDSOSWriteInfo                = 0x43,     /*                      L4    */

    kDSOSWriteFile                = 0x48,     /*                      L4    */
    kDSOSReadFile                 = 0x49,     /*                      L4    */
    kDSOSOpenFile                 = 0x4a,     /*                      L4    */
    kDSOSCloseFile                = 0x4b,     /*                      L4    */
    kDSOSPositionFile             = 0x4c,     /*                      L4    */
    kDSOSInstallFile              = 0x4d,     /*                      L4    */
    kDSOSInstallFile2             = 0x4e,     /*                      L4    */

    kDSOSSendSignal               = 0x4f,     /*                      L4    */

    /* Dummy message to get the watchpoints support */
    kDSWatchpoints                = 0x50,     /*                            */

    /* Dummy message to get the 64 bits support */
    kDSExtendedSpace              = 0x60,     /*                            */

    /*
     * 0x70 - 0x7F reserved for host->target target-specific commands
     */

    /* replies */

    kDSReplyACK                   = 0x80,     /*    L1    L2    L3    L4    */
    kDSReplyNAK                   = 0xFF,     /*    L1    L2    L3    L4    */

    /*
     * target->host notification
     */

    kDSNotifyStopped              = 0x90,     /*    L1    L2    L3    L4    */

    /*          OBSOLETE!         */
    kDSNotifyException            = 0x91,     /*                            */

    /* kDSNotifyException (0x91) is included for historical */
    /* reasons only; DON'T IMPLEMENT IN NEW DEBUG NUB CODE! */
    kDSNotifyInternalError        = 0x92,     /*                            */

    //not used, remove?
    //kDSNotifyBreak                = 0x93,     /*          L2          L4    */

    kDSNotifyCoreReleased         = 0x94,     /*                L3          */

    /* Not actually sent from target but used internally on host-side after
    attaching to a process */
    kDSNotifyAttachedToAllThreads = 0x98,     /*                            */

    /*
     * target->host OS notification
     */

    kDSOSNotifyCreated            = 0xa0,     /*                      L4    */
    kDSOSNotifyDeleted            = 0xa1,     /*                      L4    */
    kDSOSNotifyProcessorStarted   = 0xa2,     /*                      L4    */
    kDSOSNotifyProcessorStandby   = 0xa6,     /*                      L4    */
    kDSOSNotifyProcessorReset     = 0xa7,     /*                      L4    */

    /*
     * target->host support commands
     */

    kDSWriteFile                  = 0xD0,     /*          L2          L4    */
    kDSReadFile                   = 0xD1,     /*          L2          L4    */
    kDSOpenFile                   = 0xD2,     /*          L2          L4    */
    kDSCloseFile                  = 0xD3,     /*          L2          L4    */
    kDSPositionFile               = 0xD4      /*          L2          L4    */

    /*
     * 0xE0 - 0xEF reserved for target->host target-specific commands
     */
} MessageCommandID;

/****************************************************************************/
/*
 *
 *    Formatted data for messages sent between host and target.
 *
 *    Primitive Scalar Types:
 *
 *        Only use hard data types with fixed sizes:
 *        u8        8-bit unsigned integer
 *        u16       16-bit unsigned integer
 *        u32       32-bit unsigned integer
 *        u8 []     array of bytes
 *        char[]    array of characters (C string)
 *        etc.
 *
 *    Endian:
 *
 *        Messages are sent in big-endian format.
 *
 *        Example layouts:
 *
 * Type  Value                  Big Endian                Little Endian
 * ----- ------------- --------------------------- ---------------------------
 *    u8 0x12          [0x12]                      [0x12]
 *   u16 0x1234        [0x12] [0x34]               [0x34] [0x12]
 *   u32 0x12345678    [0x12] [0x34] [0x56] [0x78] [0x78] [0x56] [0x34] [0x12]
 *  u8[] 0x12 0x34     [0x12] [0x34]               [0x12] [0x34]
 * u16[] 0x1234 0x5678 [0x12] [0x34] [0x56] [0x78] [0x34] [0x12] [0x78] [0x56]
 * u32[] 0x12345678    [0x12] [0x34] [0x56] [0x78] [0x78] [0x56] [0x34] [0x12]
 *       0x9ABCDEF0    [0x9A] [0xBC] [0xDE] [0xF0] [0xF0] [0xDE] [0xBC] [0x9A]
 * char[] "Abcd 234"   [0x41] [0x62] [0x63] [0x64] [0x41] [0x62] [0x63] [0x64]
 *                     [0x20] [0x32] [0x33] [0x34] [0x20] [0x32] [0x33] [0x34]
 *
 *    Alignment:
 *
 *    Messages have no alignment restrictions.  For transmission efficiency,
 *    alignment padding is not recommended in messages.
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    M E S S A G E  F O R M A T S
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    Basic Send Message
 *
 *        u8        command                - (1) MessageCommandID
 *                                         <1> total
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    Basic Reply Message
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                                         <2> total
 *
 *    A NAK reply indicates that an invalid message or portion of a message
 *    was received.  The error value will specify the problem.
 *
 *    An ACK with an error value other than kDSReplyNoError indicates that
 *    the message was valid but could not be processed for some reason (e.g.,
 *    invalid memory ranges, bad parameters, etc.).  The error value will
 *    specify the problem.
 *
 *    Some command replies include additional information that is specific to
 *    the command.  This additional information is included ONLY if the
 *    reply is an ACK with kDSReplyNoError.
 */
/****************************************************************************/

#define DS_MIN_MSG_LENGTH                    1
#define DS_MIN_REPLY_LENGTH                  2

/****************************************************************************/
/*
 *    Required Send and Reply Messsages
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    kDSPing - basic formats
 *
 *    Can be sent by the host at any time.  Simply replies with an
 *    ACK.  Currently not supported.
 */
/****************************************************************************/

#define DS_MIN_MSG_PING_LENGTH               DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_PING_LENGTH             DS_MIN_REPLY_LENGTH


/****************************************************************************/
/*
 *    kDSConnect - basic formats
 *
 *    The host should send a connect message once at the beginning of
 *    a debug session.
 */
/****************************************************************************/

#define DS_MIN_MSG_CONNECT_LENGTH            DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_CONNECT_LENGTH          DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSDisconnect - basic formats
 *
 *    The host should send a disconnect message once at the end of
 *    a debug session.
 */
/****************************************************************************/

#define DS_MIN_MSG_DISCONNECT_LENGTH         DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_DISCONNECT_LENGTH       DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSReset - basic formats
 *
 *    Sent by the host.  Causes a soft reset on the target board.  The
 *    ACK reply is sent before reset.  The host should also expect a
 *    welcome message after reset.
 */
/****************************************************************************/

#define DS_MIN_MSG_RESET_LENGTH              DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_RESET_LENGTH            DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSVersions
 *
 *    Send - basic format
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        kernelMajor            - (1)
 *        u8        kernelMinor            - (1)
 *        u8        protocolMajor          - (1)
 *        u8        protocolMinor          - (1)
 *                                         <6> total
 *
 *    Sent by the host, returns TRK version numbers.
 */
/****************************************************************************/


#define DS_MIN_MSG_VERSIONS_LENGTH           DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_VERSIONS_LENGTH         (DS_MIN_REPLY_LENGTH + 4)

/****************************************************************************/
/*
 *    kDSCPUType
 *
 *    Send - basic format
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        CPU major type         - (1) DS_CPU_MAJOR_* (see below)
 *        u8        CPU minor type         - (1) DS_CPU_* (see below)
 *        u8        big_endian             - (1) 1 ==> big endian, 0 ==> little endian
 *        u8        default type size      - (1) size of DefaultType
 *        u8        FP type size           - (1) size of FPType (0 if unsupported)
 *        u8        extended1 type size    - (1) size of Extended1Type (0 if unsupported)
 *        u8        extended2 type size    - (1) size of Extended2Type (0 if unsupported)
 *                                         <9>
 *
 *    Sent by the host, returns information indicating the CPU type and
 *    CPU implementation details for the target system.
 */
/****************************************************************************/


#define DS_MIN_MSG_CPUTYPE_LENGTH            DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_CPUTYPE_LENGTH          (DS_MIN_REPLY_LENGTH + 7)

/*
** CPU Major types.
*/

#define DS_CPU_MAJOR_PPC                     0x00
#define DS_CPU_MAJOR_MIPS                    0x01
#define DS_CPU_MAJOR_NEC_V8XX                0x02
#define DS_CPU_MAJOR_MOT_DSP                 0x03
#define DS_CPU_MAJOR_ARM                     0x04
#define DS_CPU_MAJOR_X86                     0x05
#define DS_CPU_MAJOR_MCORE                   0x06
#define DS_CPU_MAJOR_M68K                    0x07    /* includes ColdFire */
#define DS_CPU_MAJOR_SUPERH                  0x08

/*
** MIPS CPU minor types.
*/

#define DS_CPU_MIPS_R3000                    0x00
#define DS_CPU_MIPS_R3081E                   0x01

#define DS_CPU_MIPS_VR4100                   0x20
#define DS_CPU_MIPS_VR4300                   0x21
#define DS_CPU_MIPS_VR4500                   0x22
#define DS_CPU_MIPS_VR4111                   0x23
#define DS_CPU_MIPS_TR4101                   0x24

#define DS_CPU_MIPS_VR5000                   0x40

/*
** PowerPC CPU minor types.
*/

#define DS_CPU_PPC_403                       0x00

#define DS_CPU_PPC_5XX                       0x20    /* used when not precisely known */
#define DS_CPU_PPC_505                       0x21
#define DS_CPU_PPC_509                       0x22
#define DS_CPU_PPC_50X                       0x27    /* used when not precisely known */
#define DS_CPU_PPC_555                       0x28

#define DS_CPU_PPC_603E                      0x40

#define DS_CPU_PPC_7XX                       0x50    /* used when not precisely known */
#define DS_CPU_PPC_740                       0x51
#define DS_CPU_PPC_750                       0x52
#define DS_CPU_PPC_7400                      0x53
#define DS_CPU_PPC_7410                      0x54

#define DS_CPU_PPC_8260                      0x61

#define DS_CPU_PPC_8XX                       0x80    /* used when not precisely known */
#define DS_CPU_PPC_821                       0x81
#define DS_CPU_PPC_823                       0x82
#define DS_CPU_PPC_860                       0x83


/*
** NEC V8xx CPU minor types.
*/

#define DS_CPU_NEC_V8XX_V810                 0x00
#define DS_CPU_NEC_V8XX_V821                 0x01

#define DS_CPU_NEC_V8XX_V830                 0x10
#define DS_CPU_NEC_V8XX_V831                 0x11
#define DS_CPU_NEC_V8XX_V832                 0x12

#define DS_CPU_NEC_V8XX_V850                 0x20
#define DS_CPU_NEC_V8XX_V853                 0x21
#define DS_CPU_NEC_V8XX_V850E                0x22
#define DS_CPU_NEC_V8XX_V850_SA1             0x23

/*
** M68K/Coldfire CPU minor types.
*/

#define DS_CPU_M68K_68000                    0x00
#define DS_CPU_M68K_68020                    0x02
#define DS_CPU_M68K_68030                    0x04
#define DS_CPU_M68K_68040                    0x06
#define DS_CPU_M68K_68060                    0x08

#define DS_CPU_M68K_68302                    0x20
#define DS_CPU_M68K_68306                    0x22
#define DS_CPU_M68K_68307                    0x24
#define DS_CPU_M68K_68328                    0x26
#define DS_CPU_M68K_68EZ328                  0x28
#define DS_CPU_M68K_68VZ328                  0x29
#define DS_CPU_M68K_68340                    0x2a
#define DS_CPU_M68K_68360                    0x2c

#define DS_CPU_M68K_CF5102                   0x80
#define DS_CPU_M68K_CF5202                   0x82
#define DS_CPU_M68K_CF5204                   0x84
#define DS_CPU_M68K_CF5206                   0x86
#define DS_CPU_M68K_CF5206E                  0x88
#define DS_CPU_M68K_CF5307                   0x8a

/*
** MCore CPU minor types.
*/

#define DS_CPU_MCORE_200                     0x00
#define DS_CPU_MCORE_340                     0x20

/****************************************************************************/
/*
 *    kDSSupportMask
 *
 *    Send - basic format
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        mask[32]               - (32) DSSupportMask
 *        u8        protocolLevel          - (1) value of DS_PROTOCOL (1-3)
 *                                         <34>
 *
 *    Sent by the host, returns support mask indicating which commands are
 *    supported by that particular TRK.
 */
/****************************************************************************/

/*
 * Meaning: (bit x == 1) <==> (command x is supported)
 * where bit 0 is the least significant bit of mask[0] and
 * bit 255 is the most significant bit of mask[31].
 */
typedef u8    DSSupportMask[32];             /* 256 bits total */

#define DS_MIN_MSG_SUPPORTMASK_LENGTH        DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_SUPPORTMASK_LENGTH      (DS_MIN_REPLY_LENGTH + sizeof(DSSupportMask) + 1)

/****************************************************************************/
/*
 *    kDSReadMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length
 *        u32       start                  - (4) memory start address
 *
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length
 *        u32       startHigh              - (4) memory start high-order address
 *        u32       startLow               - (4) memory start low-order address
 *                                         <12> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length
 *        u32       start                  - (4) memory start address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <16> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length
 *        u32       startHigh              - (4) memory start high-order address
 *        u32       startLow               - (4) memory start low-order address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <20> total
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u16       length                 - (2) amount that was read
 *        u8        data[]                 - (0 to 2048) data
 *                                         <4 + length> total
 *
 *    Sent by the host, returns the requested memory contents.  The TRK
 *    will attempt to catch and handle any memory access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/


#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_READMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 15)
#else
    #define DS_MIN_MSG_READMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 7)
#endif
#define DS_MIN_REPLY_READMEMORY_LENGTH       (DS_MIN_REPLY_LENGTH + 2)

/****************************************************************************/
/*
 *    kDSWriteMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length in bytes
 *        u32       start                  - (4) memory start address
 *        u8        data[]                 - (0 to 2048) data
 *                                         <8 + length> total
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length in bytes
 *        u32       startHigh              - (4) memory start high-order address
 *        u32       startLow               - (4) memory start low-order address
 *        u8        data[]                 - (0 to 2048) data
 *                                         <12 + length> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length in bytes
 *        u32       start                  - (4) memory start address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *        u8        data[]                 - (0 to 2048) data
 *                                         <16 + length> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u16       length                 - (2) memory data length in bytes
 *        u32       startHigh              - (4) memory start high-order address
 *        u32       startLow               - (4) memory start low-order address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *        u8        data[]                 - (0 to 2048) data
 *                                         <20 + length> total
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u16       length                 - (2) amount that was written in bytes
 *                                         <4> total
 *
 *    Sent by the host, writes the specified values to target memory  The TRK
 *    will attempt to catch and handle any memory access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_WRITEMEMORY_LENGTH    (DS_MIN_MSG_LENGTH + 15)
#else
    #define DS_MIN_MSG_WRITEMEMORY_LENGTH    (DS_MIN_MSG_LENGTH + 7)
#endif
#define DS_MIN_REPLY_WRITEMEMORY_LENGTH      (DS_MIN_REPLY_LENGTH + 2)

/****************************************************************************/
/*
 *    kDSFillMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u8        fillValue              - (1) value with which to fill memory
 *        u32       length                 - (4) data length in bytes
 *        u32       start                  - (4) memory start address
 *                                         <11> total
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u8        fillValue              - (1) value with which to fill memory
 *        u32       length                 - (4) data length in bytes
 *        u32       start                  - (4) memory start address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <19> total
 *
 *    Reply - Basic Reply
 *
 *    Sent by the host, fills the specified memory range with the given
 *    fillValue.  The TRK    will attempt to catch and handle any memory
 *    access exceptions that occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_FILLMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 18)
#else
    #define DS_MIN_MSG_FILLMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 10)
#endif
#define DS_MIN_REPLY_FILLMEMORY_LENGTH       (DS_MIN_REPLY_LENGTH)

/****************************************************************************/
/*
 *    kDSCopyMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u32       length                 - (4) data length in bytes
 *        u32       source                 - (4) source memory start address
 *        u32       destination            - (4) destination memory start address
 *                                         <14> total
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) One of DS_MSG_MEMORY_* below
 *        u32       length                 - (4) data length in bytes
 *        u32       source                 - (4) source memory start address
 *        u32       destination            - (4) destination memory start address
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <22> total
 *
 *    Reply - Basic Reply
 *
 *    Sent by the host, copies a block of memory with the specified
 *    size from the source address to the destination address.  This
 *    operation will function correctly even if the source and
 *    destination memory ranges overlap.  The TRK    will attempt to
 *    catch and handle any memory access exceptions that occur during
 *    this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_COPYMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 21)
#else
    #define DS_MIN_MSG_COPYMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 13)
#endif
#define DS_MIN_REPLY_COPYMEMORY_LENGTH       (DS_MIN_REPLY_LENGTH)

                                             /* option flags in DSMessage.options */
#define DS_MSG_MEMORY_SEGMENTED              0x01  /* non-flat addr space */
#define DS_MSG_MEMORY_EXTENDED               0x02  /* > 32-bit data addr */
#define DS_MSG_MEMORY_PROTECTED              0x04  /* non-user memory */
#define DS_MSG_MEMORY_USERVIEW               0x08  /* breakpoints are invisible */

/*
** The desired memory space can be specified using one of
** the following options.  It can be logically OR'ed with
** the flags defined above.
*/

#define DS_MSG_MEMORY_SPACE_PROGRAM          0x00
#define DS_MSG_MEMORY_SPACE_DATA             0x40
#define DS_MSG_MEMORY_SPACE_IO               0x80

// Added these 2 for the Wii / NDEV
#define TRK_MSG_HEADER_LENGTH                DS_MSG_MEMORY_SPACE_DATA
#define TRK_MSG_REPLY_HEADER_LENGTH          (TRK_MSG_HEADER_LENGTH + DS_MIN_MSG_LENGTH)

/*
** Use this mask to extract the memory space specifier
** from the options field.  The specifier occupies the
** 2 most significant bits.
*/

#define DS_MSG_MEMORY_SPACE_MASK             0xC0

#define DS_MAXREADWRITELENGTH                0x0800    /* 2K data portion */

#define DS_MAXMESSAGESIZE                    (DS_MAXREADWRITELENGTH + 0x80)
                                             /* max size of all message including cmd hdr */


/****************************************************************************/
/*
 *    kDSFlushCache
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DS_MSG_CACHE_* (see below)
 *        u32       start                  - (4) start of mem region for which to clear cache
 *        u32       end                    - (4) end of mem region for which to clear cache
 *                                         <10> total
 *
 *    Reply - Basic Reply
 *
 *    Sent by the host, flushes all cache entries corresponding to the
 *    given memory range, at least.  Additional cache entries may
 *    be flushed as well, depending on the target implementation.
 *
 *    This command can only be used on a stopped system.
 */
/****************************************************************************/

#define DS_MIN_MSG_FLUSHCACHE_LENGTH         (DS_MIN_MSG_LENGTH + 9)
#define DS_MIN_REPLY_FLUSHCACHE_LENGTH       (DS_MIN_REPLY_LENGTH)

/*
** The following values may be logically OR'ed together in the
** options field of the kDSFlushCache command to specify
** the cache types to be flushed.
*/

#define DS_MSG_CACHE_TYPE_INSTRUCTION        0x01
#define DS_MSG_CACHE_TYPE_DATA               0x02
#define DS_MSG_CACHE_TYPE_SECONDARY          0x04


/****************************************************************************/
/*
 *    kDSReadRegisters
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageRegisterOptions
 *        u16       firstRegister          - (2) first register ID
 *        u16       lastRegister           - (2) last register ID
 *                                         <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageRegisterOptions
 *        u16       firstRegister          - (2) first register ID
 *        u16       lastRegister           - (2) last register ID
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <14>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                  register data          - (0-2048)
 *                                         <2 + register data>
 *
 *    Sent by the host, reads the requested register contents.  The TRK
 *    will attempt to catch and handle any access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_READREGISTERS_LENGTH  (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_READREGISTERS_LENGTH  (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_READREGISTERS_LENGTH    DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSWriteRegisters
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageRegisterOptions
 *        u16       firstRegister          - (2) first register ID
 *        u16       lastRegister           - (2) last register ID
 *                  register data          - (0-2048)
 *                                         <6 + register data>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageRegisterOptions
 *        u16       firstRegister          - (2) first register ID
 *        u16       lastRegister           - (2) last register ID
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                  register data          - (0-2048)
 *                                         <14 + register data>
 *
 *    Reply - basic format
 *
 *    Sent by the host, writes the specified values to target registers.
 *    The TRK will attempt to catch and handle any access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_WRITEREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_WRITEREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_WRITEREGISTERS_LENGTH   DS_MIN_REPLY_LENGTH

                                             /* option flags for reading registers */

#define DS_MSG_REGISTERS_TYPE_MASK           0x07    /* "register type" mask */
#define DS_MSG_REGISTERS_TYPE_SHIFT          0       /* # bits to shift mask from lsb. */

/*
 *    kDSReadRegisters & kDSWriteRegister options
 */

typedef enum DSMessageRegisterOptions
{
    kDSRegistersDefault   = 0x0,            /* Default register block */
    kDSRegistersFP        = 0x1,            /* floating point registers */
    kDSRegistersExtended1 = 0x2,            /* Extended register set 1 */
    kDSRegistersExtended2 = 0x3             /* Extended register set 2 */
} DSMessageRegisterOptions;


/****************************************************************************/
/*
 *    kDSContinue
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        basic format
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <9>
 *
 *    Reply - basic format
 *
 *
 *    Sent by the host.  Begins execution on the target at the current PC.
 *
 *    Replies with an ACK before execution starts.  Once execution begins,
 *    most commands which access that process or thread will not be
 *    accepted.  For DS_PROTOCOL values of 1 or 2, this means that it
 *    will not respond to any of the state or execution commands
 *    except kDSStop.  It will respond to meta-commands.  For
 *    a DS_PROTOCOL value of 3, it will not accept any commands
 *    specifying that process or thread except kDSStop or
 *    kDSOSDeleteItem.
 *
 *    The host should wait for a NotifyStopped or NotifyException command
 *    and reply with a corresponding ACK.  Once that process and thread
 *    have stopped again, MetroTRK will once again accept other commands
 *    which access them.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_CONTINUE_LENGTH       (DS_MIN_MSG_LENGTH + 8)
#else
    #define DS_MIN_MSG_CONTINUE_LENGTH       DS_MIN_MSG_LENGTH
#endif
#define DS_MIN_REPLY_CONTINUE_LENGTH         DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSStep
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoCount || options == kDSStepOverCount)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStepOptions
 *        u8        count                  - (1) # of instructions
 *                                         <3>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoCount || options == kDSStepOverCount)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStepOptions
 *        u8        count                  - (1) # of instructions
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <11>
 *
 *    Sent by the host.  Steps over the specified number of instructions.
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoRange || options == kDSStepOverRange)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStepOptions
 *        u32       rangeStart             - (4) Start address of memory range
 *        u32       rangeEnd               - (4) End address of memory range
 *                                         <10>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoRange || options == kDSStepOverRange)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStepOptions
 *        u32       rangeStart             - (4) Start address of memory range
 *        u32       rangeEnd               - (4) End address of memory range
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <18>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Steps until the PC is outside of the specified
 *    range.
 *
 *    Replies with an ACK before execution starts.  Once execution begins,
 *    most commands which access that process or thread will not be
 *    accepted.  For DS_PROTOCOL values of 1 or 2, this means that it
 *    will not respond to any of the state or execution commands
 *    except kDSStop.  It will respond to meta-commands.  For
 *    a DS_PROTOCOL value of 3, it will not accept any commands
 *    specifying that process or thread except kDSStop or
 *    kDSOSDeleteItem.
 *
 *    The host should wait for a NotifyStopped or NotifyException command
 *    and reply with a corresponding ACK.  Once that process and thread
 *    have stopped again, MetroTRK will once again accept other commands
 *    which access them.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_STEP_COUNT_LENGTH     (DS_MIN_MSG_LENGTH + 10)
    #define DS_MIN_MSG_STEP_RANGE_LENGTH     (DS_MIN_MSG_LENGTH + 17)
#else
    #define DS_MIN_MSG_STEP_COUNT_LENGTH     (DS_MIN_MSG_LENGTH + 2)
    #define DS_MIN_MSG_STEP_RANGE_LENGTH     (DS_MIN_MSG_LENGTH + 9)
#endif
#define DS_MIN_MSG_STEP_LENGTH               (DS_MIN_MSG_STEP_COUNT_LENGTH)
#define DS_MIN_REPLY_STEP_LENGTH             DS_MIN_REPLY_LENGTH

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageStepOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSStepIntoCount = 0x00,            /* Exec count instructions & stop */
    kDSStepIntoRange = 0x01,            /* Exec until PC is out of specified range */
    kDSStepOverCount = 0x10,            /* Step over 1*count instructions & stop */
    kDSStepOverRange = 0x11             /* Step over until PC is out of specified range */
} DSMessageStepOptions;


/****************************************************************************/
/*
 *    kDSStop
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        basic format
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopSystem)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStopOptions
 *                                         <2>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopProcess)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStopOptions
 *        u32       processID              - (4) unique process ID
 *                                         <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopThread)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageStopOptions
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <10>
 *
 *    Sent by the host.  Forces the target system/process/thread to stop.
 *    Replies with an ACK.
 *
 *    Reply - basic format
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_STOP_LENGTH           (DS_MIN_MSG_LENGTH + 3)
#else
    #define DS_MIN_MSG_STOP_LENGTH           (DS_MIN_MSG_LENGTH + 2)
#endif
#define DS_MIN_REPLY_STOP_LENGTH             DS_MIN_REPLY_LENGTH

#if DS_PROTOCOL == DS_PROTOCOL_RTOS

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageStopOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSStopSystem    = 0x0,        /* Stop the entire system */
    kDSStopProcess   = 0x1,        /* Stop all threads in a specific process */
    kDSStopThread    = 0x2         /* Stop a specific thread in a specific process */
} DSMessageStopOptions;

#endif

/****************************************************************************/
/*
 *    kDSSetBreak
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) Location of breakpoint
 *                                         <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) Location of breakpoint
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <14>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Sets a breakpoint at the specified location.
 *    Replies with an ACK.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_SETBREAK_LENGTH       (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_SETBREAK_LENGTH       (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_SETBREAK_LENGTH         DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSClearBreak
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) Location of breakpoint
 *                                         <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) Location of breakpoint
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <14>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Clears the breakpoint at the specified location.
 *    Replies with an ACK.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_CLEARBREAK_LENGTH     (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_CLEARBREAK_LENGTH     (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_CLEARBREAK_LENGTH       DS_MIN_REPLY_LENGTH

/*
** Options to specify hardware/software breakpoints are pre-defined.
** Desired target-specific options can be added here.
*/

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
typedef enum
{
    kDSBreakSoftware    = 0x00,       /* Set/clear a software breakpoint */
    kDSBreakHardware    = 0x01        /* Set/clear a hardware breakpoint */
} DSMessageBreakOptions;
#endif

/****************************************************************************/
/*
 *    kDSSetWatch
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) start of watch range
 *        u32       length                 - (4) length of watch range
 *                                         <10>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageBreakOptions
 *        u32       address                - (4) start of watch range
 *        u32       length                 - (4) length of watch range
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <18>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Sets a watchpoint on the specified memory
 *    range.
 *    Replies with an ACK.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_SETWATCH_LENGTH       (DS_MIN_MSG_LENGTH + 17)
#else
    #define DS_MIN_MSG_SETWATCH_LENGTH       (DS_MIN_MSG_LENGTH + 9)
#endif
#define DS_MIN_REPLY_SETWATCH_LENGTH         DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSClearWatch
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageWatchOptions
 *        u32       length                 - (4) length of watch range
 *        u32       address                - (4) start of watch range
 *                                         <10>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageWatchOptions
 *        u32       length                 - (4) length of watch range
 *        u32       address                - (4) start of watch range
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <18>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Clears the watchpoint on the specified memory
 *    range.
 *    Replies with an ACK.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_CLEARWATCH_LENGTH     (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_CLEARWATCH_LENGTH     (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_CLEARWATCH_LENGTH       DS_MIN_REPLY_LENGTH

/*
** The following values should be logically OR'ed with one
** of the DSMessageWatchOptions values to specify the
** type of access which will cause the target program to
** stop.  More that one stop type can be specified.
*/

#define DS_MSG_WATCH_CONDITION_READ          0x10    /* stop on read access */
#define DS_MSG_WATCH_CONDITION_WRITE         0x20    /* stop on write access */

/*
** Use the following mask to extract the watch condition flags
** from the options.
*/

#define DS_MSG_WATCH_CONDITION_MASK          0x30

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageWatchOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSWatchSoftware    = 0x00,       /* Set/clear a software breakpoint */
    kDSWatchHardware    = 0x01        /* Set/clear a hardware breakpoint */
} DSMessageWatchOptions;

/****************************************************************************/
/*
 *    kDSDownload
 *
 *    Send: if (DSMessageDownloadOptions == kDSMessageDownloadTFTP)
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        options                - (1) DSMessageDownloadOptions (see below)
 *        u32       IP address             - (4) IP address of the target system
 *        u32       TFTP address           - (4) IP address of the TFTP server
 *        u32       length                 - (4) file size in bytes
 *        u32       start address          - (4) load address of the file
 *        u16       name_length            - (2) length of file name in bytes
 *        u8[]      file name              (0 - 2048) name of the file to download
 *                                         <20 + name_length>
 *
 *    Reply - basic format
 *
 *    Sent by the host.  Downloads a file to the target system according
 *    to the specified parameters.  This command is intended to allow
 *    downloading via communication channels other than the TRK's command
 *    channel.
 *    Replies with an ACK.
 */
/****************************************************************************/

#define DS_MIN_MSG_DOWNLOAD_LENGTH           (DS_MIN_MSG_LENGTH + 19)
#define DS_MIN_REPLY_CLEARBREAK_LENGTH       DS_MIN_REPLY_LENGTH

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageDownloadOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSMessageDownloadTFTP        = 0x0
} DSMessageDownloadOptions;

/****************************************************************************/
/*
 *    kDSNotifyStopped
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *                  target-defined info (PC, instr @PC, exception word)
 *                                         <1 + target info> total
 *
 *    Reply - basic format
 *
 *    Sent by the target.  This message will only be sent when the target
 *    is executing (after a kDSContinue or kDSStep command).  It indicates
 *    that either the target hit a breakpoint or the current step command
 *    is complete.  Expects an ACK in response.
 */
/****************************************************************************/

#define DS_MIN_MSG_NOTIFYSTOPPED_LENGTH      DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_NOTIFYSTOPPED_LENGTH    DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSNotifyException
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *                  target-defined info (PC, instr @PC, exception word)
 *                                         <1 + target info> total
 *
 *    Reply - basic format
 *
 *    Sent by the target.  This message will only be sent when the target
 *    is executing (after a kDSContinue or kDSStep command).  It indicates
 *    that an exception occurred in the target program.
 *    Expects an ACK in response.
 */
/****************************************************************************/

#define DS_MIN_MSG_NOTIFYEXCEPTION_LENGTH    DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_NOTIFYEXCEPTION_LENGTH  DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSNotifyInternalError
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        error code             - (1) DSInternalError
 *                                         <2> total
 *
 *    Reply - basic format
 *
 *    Sent by the target.  It indicates that an internal error occurred
 *    in MetroTRK.
 *    Expects an ACK in response.
 */
/****************************************************************************/

#define DS_MIN_MSG_NOTIFYINTERNALERROR_LENGTH   (DS_MIN_MSG_LENGTH + 1)
#define DS_MIN_REPLY_NOTIFYINTERNALERROR_LENGTH DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSOSNotifyCreated
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       itemType               - (2) DSOSItemTypes
 *                  target-defined info (e.g., process ID for kDSOSProcessItem)
 *                                         <3 + target info> total
 *
 *    Reply - basic format
 *
 *    Sent by the target.  This message indicates that an item of the
 *    given type has been created.
 *    Expects an ACK in response.
 */
/****************************************************************************/

#define DS_MIN_MSG_OSNOTIFYCREATED_LENGTH    DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_OSNOTIFYCREATED_LENGTH  DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSOSNotifyDeleted
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       objectType             - (2) DSOSItemTypes
 *                  target-defined info (e.g., process ID for kDSOSProcessItem)
 *                                         <3 + target info> total
 *
 *    Reply - basic format
 *
 *    Sent by the target.  This message indicates that an item of the
 *    given type has been deleted.
 *    Expects an ACK in response.
 */
/****************************************************************************/

#define DS_MIN_MSG_OSNOTIFYDELETED_LENGTH    (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_OSNOTIFYDELETED_LENGTH  DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSOSCreateItem
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       itemType               - (2) DSOSItemTypes
 *                  target-defined info (e.g., DLL name for kDSOSDLLItem)
 *                                         <3 + target info> total
 *
 *    Reply
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                  target-defined info (e.g., code & data offset for
 *                                         kDSOSProcessItem)
 *                                         <2 + target info> total
 *
 *    Sent by the host.  Creates an item of the specified type in the
 *    system.  It allows the host to create processes (load), threads,
 *    DLL's, and any other system resources.  This command may either
 *    operate synchronously or asynchronously.  The behavior may be
 *    differ on an item-by-item basis.  The two scenarios:
 *
 *        synchronous: The host sends a OSCreateItem command.  The
 *            target system creates the item and then replies with an
 *            ACK containing specific information about the
 *            created item.
 *
 *        asynchronous: The host sends a OSCreateItem command.  The
 *            target system queues the request and replies with
 *            an ACK.  At some later time, when the item is
 *            actually created, the target system sends an
 *            OSNotifyCreated message to the host containing
 *            specific information about the created item.
 *
 *    Expects an ACK in response.  The reply may contain specific
 *    information about the created item, such as code and data
 *    offsets for relocatable processes (if the operation is
 *    synchronous).
 */
/****************************************************************************/

#define DS_MIN_MSG_OSCREATEITEM_LENGTH       (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_OSCREATEITEM_LENGTH     DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSOSDeleteItem
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       itemType               - (2) DSOSItemTypes
 *                  target-defined info (e.g., DLL name for kDSOSDLLItem)
 *                                         <3 + target info> total
 *
 *    Reply
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                  target-defined info
 *                                         <2 + target info> total
 *
 *    Sent by the host.  Deletes the item of the specified type from the
 *    system.  It allows the host to delete processes (kill), threads,
 *    DLL's, and any other system resources.  This command may either
 *    operate synchronously or asynchronously.  The behavior may be
 *    differ on an item-by-item basis.  The two scenarios:
 *
 *        synchronous: The host sends a OSDeleteItem command.  The
 *            target system deletes the item and then replies with an
 *            ACK containing specific information about the
 *            deleted item.
 *
 *        asynchronous: The host sends a OSDeleteItem command.  The
 *            target system queues the request and replies with
 *            an ACK.  At some later time, when the item is
 *            actually deleted, the target system sends an
 *            OSNotifyDeleted message to the host containing
 *            specific information about the deleted item.
 *
 *    Expects an ACK in response.  The reply may contain specific
 *    information about the deleted item (if the operation is
 *    synchronous).
 */
/****************************************************************************/

#define DS_MIN_MSG_OSDELETEITEM_LENGTH       (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_OSDELETEITEM_LENGTH     DS_MIN_REPLY_LENGTH

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargOSItemTypes enum (see msgtarg.h).
*/

typedef enum
{
    kDSOSProcessItem    = 0x0000,
    kDSOSThreadItem     = 0x0001,
    kDSOSDLLItem        = 0x0002,
    kDSOSAppItem        = 0x0003,
    kDSOSMemBlockItem   = 0x0004
    /* 0x0005 - 0x00ff reserved for expansion */
} DSOSItemTypes;

/****************************************************************************/
/*
 *    kDSOSReadInfo
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       infoType               - (2) DSOSInfoTypes
 *                  target-defined info (e.g., process ID for
 *                                         kDSOSProcessStateInfo)
 *                                         <3 + target info> total
 *
 *    Reply
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                  target-defined info (e.g., process state for
 *                                         kDSOSProcessStateInfo)
 *                                         <2 + target info> total
 *
 *    Sent by the host.  Reads the requested info from the target
 *    system.  It allows the host to get information about the
 *    internal system state.
 */
/****************************************************************************/

#define DS_MIN_MSG_OSREADINFO_LENGTH         (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_OSREADINFO_LENGTH       DS_MIN_REPLY_LENGTH

/****************************************************************************/
/*
 *    kDSOSWriteInfo
 *
 *    Send:
 *
 *        u8        command                - (1) MessageCommandID
 *        u16       infoType               - (2) DSOSInfoTypes
 *                  target-defined info (e.g., process ID and state for
 *                                         kDSOSProcessStateInfo)
 *                                         <3 + target info> total
 *
 *    Reply
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *                  target-defined info
 *                                         <2 + target info> total
 *
 *    Sent by the host.  Writes the given info in the target system.
 *    It allows the host to modify the internal system state.
 */
/****************************************************************************/

#define DS_MIN_MSG_OSWRITEINFO_LENGTH        (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_OSWRITEINFO_LENGTH      DS_MIN_REPLY_LENGTH

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargOSInfoTypes enum (see msgtarg.h).
*/

typedef enum
{
    kDSOSProcessList    = 0x0000,
    kDSOSProcessState   = 0x0001,
    kDSOSThreadList     = 0x0002,
    kDSOSThreadState    = 0x0003,
    kDSOSDLLList        = 0x0004,
    kDSOSDLLState       = 0x0005
    /* 0x0006 - 0x00ff reserved for expansion */
} DSOSInfoTypes;

/****************************************************************************/
/*
 *    kDSWriteFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) DSFileHandles (see below)
 *        u16       length                 - (2) file data length in bytes
 *                  file data              - (0 to 2048)
 *                                         <7 + length>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) DSFileHandles (see below)
 *        u16       length                 - (2) file data length in bytes
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                  file data              - (0 to 2048)
 *                                         <15 + length>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        io_result              - (1) DSIOResult (see below)
 *        u16       length                 - (2) amount that was written in bytes
 *                                         <5>
 *
 *    Sent by the target.  Instructs the host to write the specified
 *    data to the file with the given handle.  Assumes that stdout
 *    and stderr are always open for writing.
 *    Expects a reply from the host in the above format.
 *
 *    Note: The startup welcome message is sent as a kDSWriteFile
 *    message, but it is a special case and does not require a reply.
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_WRITEFILE_LENGTH      (DS_MIN_MSG_LENGTH + 14)
#else
    #define DS_MIN_MSG_WRITEFILE_LENGTH      (DS_MIN_MSG_LENGTH + 6)
#endif
#define DS_MIN_REPLY_WRITEFILE_LENGTH        (DS_MIN_REPLY_LENGTH + 3)

/****************************************************************************/
/*
 *    kDSReadFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) DSFileHandles (see below)
 *        u16       length                 - (2) file data length in bytes
 *                                         <7>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) DSFileHandles (see below)
 *        u16       length                 - (2) file data length in bytes
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <15>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        io_result              - (1) DSIOResult (see below)
 *        u16       length                 - (2) amount that was read in bytes
 *                  file data              - (0 to 2048)
 *                                         <5 + length>
 *
 *    Sent by the target.  Instructs the host to read the specified
 *    amount of data from the file with the given handle.  Assumes
 *    that stdin is always open for reading.
 *    Expects a reply from the host in the above format.
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_READFILE_LENGTH       (DS_MIN_MSG_LENGTH + 14)
#else
    #define DS_MIN_MSG_READFILE_LENGTH       (DS_MIN_MSG_LENGTH + 6)
#endif
#define DS_MIN_REPLY_READFILE_LENGTH         (DS_MIN_REPLY_LENGTH + 3)

/****************************************************************************/
/*
 *    kDSOpenFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        modes                  - (1) DSFileOpenModes (see below)
 *        u16       name_length            - (2) file name length in bytes
 *        u8[]      file_name              - (0 to 2048) file name (no string
 *                                                  terminator)
 *                                         <4 + name_length>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        modes                  - (1) DSFileOpenModes (see below)
 *        u16       name_length            - (2) file name length in bytes
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *        u8[]      file_name              - (0 to 2048) file name (no string
 *                                                  terminator)
 *                                         <12 + name_length>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        io_result              - (1) DSIOResult (see below)
 *        u32       file_handle            - (4) handle of the newly opened file
 *                                         <7>
 *
 *    Sent by the target.  Instructs the host to open a file with the
 *    given name and modes.  The resulting file_handle can then be
 *    used by kDSWriteFile, kDSReadFile, kDSCloseFile, and kDSPositionFile.
 *    Expects a reply from the host in the above format.
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_OPENFILE_LENGTH       (DS_MIN_MSG_LENGTH + 11)
#else
    #define DS_MIN_MSG_OPENFILE_LENGTH       (DS_MIN_MSG_LENGTH + 3)
#endif
#define DS_MIN_REPLY_OPENFILE_LENGTH         (DS_MIN_REPLY_LENGTH + 5)

/****************************************************************************/
/*
 *    kDSCloseFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) handle of the file to be closed
 *                                         <5>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u32       file_handle            - (4) handle of the file to be closed
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <13>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        io_result              - (1) DSIOResult (see below)
 *                                         <3>
 *
 *    Sent by the target.  Instructs the host to close the file with the
 *    specified file_handle.
 *    Expects a reply from the host in the above format.
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_CLOSEFILE_LENGTH      (DS_MIN_MSG_LENGTH + 12)
#else
    #define DS_MIN_MSG_CLOSEFILE_LENGTH      (DS_MIN_MSG_LENGTH + 4)
#endif
#define DS_MIN_REPLY_CLOSEFILE_LENGTH        (DS_MIN_REPLY_LENGTH + 1)

/****************************************************************************/
/*
 *    kDSPositionFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        mode                   - (1) DSFilePositionModes (see below)
 *        u32       file_handle            - (4) handle of the file to be positioned
 *        u32       offset                 - (4) position offset
 *                                         <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        mode                   - (1) DSFilePositionModes (see below)
 *        u32       file_handle            - (4) handle of the file to be positioned
 *        u32       offset                 - (4) position offset
 *        u32       processID              - (4) unique process ID
 *        u32       threadID               - (4) unique thread ID
 *                                         <14>
 *
 *    Reply:
 *
 *        u8        command                - (1) ack/nak MessageCommandID
 *        u8        error                  - (1) CWDS error value
 *        u8        io_result              - (1) DSIOResult (see below)
 *                                         <3>
 *
 *    Sent by the target.  Instructs the host to set the file position
 *    of the specified file.  The new position will be at the specified
 *    offset from one of three reference points in the file, as specified
 *    by the mode.
 *    Expects a reply from the host in the above format.
 */
/****************************************************************************/

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_POSITIONFILE_LENGTH   (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_POSITIONFILE_LENGTH   (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_POSITIONFILE_LENGTH     (DS_MIN_REPLY_LENGTH + 1)

/*
** These are the predefined file handles.  Additional files may
** be opened via the kDSOpenFile command.  New file handles
** returned by that command may be used in addition to the
** ones defined here.
*/

typedef enum DSFileHandle
{
    kDSStdin  = 0x00,
    kDSStdout = 0x01,
    kDSStderr = 0x02
} DSFileHandle;

/*
** The result of an I/O command can be any one of the following.
*/

typedef enum DSIOResult
{
    kDSIONoError    = 0x00,
    kDSIOError      = 0x01,
    kDSIOEOF        = 0x02
} DSIOResult;

/*
** When opening a file, the following values can be OR'ed together
** to specify the desired open modes.  The following table shows
** the correlation between the various modes and the standard mode
** option for fopen:
**
**    read    write    append    binary          fopen mode
**    ----    -----    ------    ------        --------------
**    yes                                           "r"
**             yes                                  "w"
**             yes      yes                         "a"
**    yes      yes                              "r+" or "w+"
**    yes      yes      yes                         "a+"
**    yes                         yes               "rb"
**             yes                yes               "wb"
**             yes      yes       yes               "ab"
**    yes      yes                yes          "r+b" or "w+b"
**    yes      yes      yes       yes               "a+b"
**
** Note that when the append mode is specified, the write mode
** is always assumed.
*/

typedef enum DSFileOpenModes
{
    kDSFileOpenRead      = 0x01,
    kDSFileOpenWrite     = 0x02,
    kDSFileOpenAppend    = 0x04,
    kDSFileOpenBinary    = 0x08
} DSFileOpenModes;

/*
** Use one of the following values to specify a file positioning mode.
*/

typedef enum DSFilePositionModes
{
    kDSFileSeekSet        = 0x00,        /* Position is offset from file start */
    kDSFileSeekCur        = 0x01,        /* Position is offset from current pos. */
    kDSFileSeekEnd        = 0x02         /* Position is offset from file end */
} DSFilePositionModes;

/****************************************************************************/
/*
 *    kDSOSWriteFile
 *    kDSOSReadFile
 *    kDSOSOpenFile
 *    kDSOSCloseFile
 *    kDSOSPositionFile
 *
 *    The format and function of these commands is identical to
 *    that of the kDSWriteFile, kDSReadFile, kDSOpenFile,
 *    kDSCloseFile, and kDSPositionFile commands described above.
 *
 *    The difference is that these commands are sent from the host to
 *    the target and operate on files on the target system.
 */
/****************************************************************************/


/****************************************************************************/
/*
 *    Error values returned in ACK/NAK packets
 */
/****************************************************************************/
typedef enum
{
    kDSReplyNoError                 = 0x00, /* no error */
    kDSReplyError                   = 0x01, /* generic error in CWDS message */
    kDSReplyPacketSizeError         = 0x02, /* unexpected pkt size in send msg */
    kDSReplyCWDSError               = 0x03, /* internal error occurred in CWDS */
    kDSReplyEscapeError             = 0x04, /* escape followed by frame flag */
    kDSReplyBadFCS                  = 0x05, /* bad FCS in packet */
    kDSReplyOverflow                = 0x06, /* packet too long */

    kDSReplyUnsupportedCommandError = 0x10, /* command not supported */
    kDSReplyParameterError          = 0x11, /* command param out of range */
    kDSReplyUnsupportedOptionError  = 0x12, /* an option was not supported */
    kDSReplyInvalidMemoryRange      = 0x13, /* read/write to invalid memory */
    kDSReplyInvalidRegisterRange    = 0x14, /* read/write invalid registers */
    kDSReplyCWDSException           = 0x15, /* exception occurred in CWDS */
    kDSReplyNotStopped              = 0x16, /* targeted system or thread is running */

    kDSReplyOsError                 = 0x20, /* general OS-related error */
    kDSReplyInvalidProcessId        = 0x21, /* request specified invalid process */
    kDSReplyInvalidThreadId         = 0x22  /* request specified invalid thread */
} DSReplyError;


#endif /* __MSGCMD_H__ */
