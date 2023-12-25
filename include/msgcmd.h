/****************************************************************************/
/*

DESCRIPTION

    Monitor command constants and structures

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MSGCMD_H__
#define __MSGCMD_H__


#include "trktypes.h"


/*
** This is the maximum number of fields that will be encountered
** in a message, including the header field(s).
*/
#ifndef DS_MAXMESSAGEFIELDCOUNT
    #define DS_MAXMESSAGEFIELDCOUNT    15
#endif

/*
** Some items are declared here only for debug purposes (e.g.,
** string names for messages and fields).  The following macros
** allow these items to be removed when debugging is disabled,
** eliminating otherwise wasted data space.
*/

#ifndef DEBUG_MSGCMD
    #include "prefix_SUPP_TRK_EPPC.h"
    #error "DEBUG_MSGCMD must be defined to either 0 or 1"
#endif

#if DEBUG_MSGCMD
    #define __debug_only_parm(x)         x,
    #define __debug_only_field(x)        x;
#else
    #define __debug_only_parm(x)
    #define __debug_only_field(x)
#endif

/*
** Descriptor for a message field.
*/
typedef struct TrkMsgFieldDesc
{
    __debug_only_field(s8 *name)           /* Field name - for debugging */
    s32 length;                            /* Length of the field in bytes */
} TrkMsgFieldDesc;

/*
** Use this macro to initialize TrkMsgFieldDesc variables.
*/
#define __trk_msg_field(name, length)    { __debug_only_parm(name) length }

/*
** Descriptor for a message.  Each message consists of a header followed
** by additional fields.  Note that length, count, and farray should
** only account for the additional fields.  For example: if the header
** has 1 field and the message has 3 additional fields (4 total), count
** should be 3.
**
** Only fixed-length numeric fields should be included in the descriptor.
** Variable-length portions of a message, such as memory data, are
** typically handled by code that is specific to the message type.
** They are outside the scope of the general-purpose routines which use
** these descriptors.
*/
#define __TrkMsgFieldMax    DS_MAXMESSAGEFIELDCOUNT

typedef struct TrkMsgDesc
{
    __debug_only_field(s8 *name)                         /* Message name - for debugging */
    const struct TrkMsgDesc    *header;                  /* Descriptor for header/previous fragment */
    s32                        length;                   /* Length of the message in bytes */
    u32                        count;                    /* Number of fields in the message */
    TrkMsgFieldDesc            farray[__TrkMsgFieldMax]; /* Descriptors for the message fields */
} TrkMsgDesc;

//#define TRK_MSGCMD_INIT        1

#ifdef TRK_MSGCMD_INIT
    #define DESCDECLNC
    #define DESCDECL           const
#else
    #define DESCDECLNC         extern
    #define DESCDECL           extern const
#endif

/****************************************************************************/
/*
 *    Commands
 *
 *    constant definitions of commands sent from host to target
 *
 *    L1 = Level 1 CWDS (required)
 *    L2 = Level 2 CWDS (extended board-level debugger)
 *    L3 = Level 3 CWDS (OS-level debugger)
 *
 * Note that a compilation unit must define DS_PROTOCOL before including
 * this file. Because of delayed macro resolution, you can use these
 * DS_PROTOCOL_* macros before they are defined. In other words, the following
 * example is valid.
 *
 *         #define DS_PROTOCOL    DS_PROTOCOL_RTOS
 *         #include "msgcmd.h"
 */
/****************************************************************************/

#define DS_PROTOCOL_MIN            1    /* L1 */
#define DS_PROTOCOL_BOARD          2    /* L2 */
#define DS_PROTOCOL_RTOS           3    /* L3 */

#if (!defined(DS_PROTOCOL)) || (DS_PROTOCOL == 0)
    #error "Protocol level must be defined"
#endif

/*
** The current protocol version.
*/

#define DS_PROTOCOL_MAJOR_VERSION_1    1
#define DS_PROTOCOL_MINOR_VERSION_10   10

#define DS_PROTOCOL_MAJOR_VERSION      DS_PROTOCOL_MAJOR_VERSION_1
#define DS_PROTOCOL_MINOR_VERSION      DS_PROTOCOL_MINOR_VERSION_10

#define DS_PROTOCOL_VERSION_STRING     version_str(DS_PROTOCOL_MAJOR_VERSION, \
                                                 DS_PROTOCOL_MINOR_VERSION)

/*
** Named constants for significant protocol revisions can be added
** here (the existing definitions should not be changed).
*/

/* Sequence IDs were introduced in version 2.0 */
#define DS_PROTOCOL_MAJOR_VERSION_SEQUENCEIDS     2
#define DS_PROTOCOL_MINOR_VERSION_SEQUENCEIDS     0

/* Multi-channel communication, ethernet, & other comm enhancements in 3.0 */
#define DS_PROTOCOL_MAJOR_VERSION_MULTICHANNEL    3
#define DS_PROTOCOL_MINOR_VERSION_MULTICHANNEL    0


#if __MWERKS__
    #pragma mark  MessageCommandID command value list
#endif /* __MWERKS__ */

/****************************************************************************/
/*
 *    MessageCommandID command value list
 */
/****************************************************************************/

typedef enum MessageCommandID
{
    /* meta commands, minimum feature set */

    kDSPing                        = 0x00,        /*                          */
    kDSConnect                     = 0x01,        /*    L1    L2    L3        */
    kDSDisconnect                  = 0x02,        /*                L3        */
    kDSReset                       = 0x03,        /*          L2    L3        */
    kDSVersions                    = 0x04,        /*    L1    L2    L3        */
    kDSSupportMask                 = 0x05,        /*    L1    L2    L3        */
    kDSCPUType                     = 0x06,        /*          L2    L3        */
    kDSConfigTransport             = 0x07,        /*          L2    L3        */

    /* state commands */

    kDSReadMemory                  = 0x10,        /*    L1    L2    L3        */
    kDSWriteMemory                 = 0x11,        /*    L1    L2    L3        */
    kDSReadRegisters               = 0x12,        /*    L1    L2    L3        */
    kDSWriteRegisters              = 0x13,        /*    L1    L2    L3        */
    kDSFillMemory                  = 0x14,        /*                          */
    kDSCopyMemory                  = 0x15,        /*                          */
    kDSFlushCache                  = 0x16,        /*                          */

    // This was once part of the TRK to the Wii / NDEV and it seems as it
    // was renamed later on to "kDSCheckTargetState" for protocols > v1.10.
    kDSOption                      = 0x17,

    /* execution commands */

    kDSContinue                    = 0x18,        /*    L1    L2    L3        */
    kDSStep                        = 0x19,        /*          L2    L3        */
    kDSStop                        = 0x1a,        /*          L2    L3        */
    kDSSetBreak                    = 0x1b,        /*          L2    L3        */
    kDSClearBreak                  = 0x1c,        /*          L2    L3        */
    kDSDownload                    = 0x1d,        /*                          */
    kDSModifyBreakThread           = 0x1e,        /*                L3        */
    /* Note: kDSSetWatch & kDSClearWatch are obsolete and have been removed */

    /* host->target IO management */
    kDSNotifyFileInput             = 0x20,        /*          L2    L3        */
    kDSBlockFileIo                 = 0x21,        /*          L2    L3        */

    /* host->target OS commands */

    kDSOSCreateItem                = 0x40,        /*                L3        */
    kDSOSDeleteItem                = 0x41,        /*                L3        */
    kDSOSReadInfo                  = 0x42,        /*                L3        */
    kDSOSWriteInfo                 = 0x43,        /*                L3        */

    kDSOSWriteFile                 = 0x48,        /*                L3        */
    kDSOSReadFile                  = 0x49,        /*                L3        */
    kDSOSOpenFile                  = 0x4a,        /*                L3        */
    kDSOSCloseFile                 = 0x4b,        /*                L3        */
    kDSOSPositionFile              = 0x4c,        /*                L3        */
    kDSOSInstallFile               = 0x4d,        /*                L3        */
    kDSOSInstallFile2              = 0x4e,        /*                L3        */

    /*
     * 0x70 - 0x7F reserved for host->target target-specific commands
     */

    /* replies */

    kDSReplyACK                    = 0x80,        /*    L1    L2    L3        */
    kDSReplyNAK                    = 0xFF,        /*    L1    L2    L3        */


    /*
     * target->host notification
     */

    kDSNotifyStopped               = 0x90,        /*    L1    L2    L3        */
    kDSNotifyException             = 0x91,        /*  OBSOLETE!               */
        /* kDSNotifyException (0x91) is included for historical */
        /* reasons only; DON'T IMPLEMENT IN NEW DEBUG NUB CODE! */
    kDSNotifyInternalError         = 0x92,        /*                          */
    kDSNotifyBreak                 = 0x93,        /*          L2    L3        */

    /*
     * target->host OS notification
     */

    kDSOSNotifyCreated             = 0xa0,        /*                L3        */
    kDSOSNotifyDeleted             = 0xa1,        /*                L3        */
    kDSOSNotifyProcessorStarted    = 0xa2,        /*                L3        */
    kDSOSNotifyProcessorStandby    = 0xa6,        /*                L3        */
    kDSOSNotifyProcessorReset      = 0xa7,        /*                L3        */

    /*
     * target->host support commands
     */

    kDSWriteFile                   = 0xD0,        /*          L2    L3        */
    kDSReadFile                    = 0xD1,        /*          L2    L3        */
    kDSOpenFile                    = 0xD2,        /*          L2    L3        */
    kDSCloseFile                   = 0xD3,        /*          L2    L3        */
    kDSPositionFile                = 0xD4         /*          L2    L3        */

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
 *        u8      8-bit u32eger
 *        u16     16-bit u32eger
 *        u32     32-bit u32eger
 *        u8[]    array of bytes
 *        s8[]    array of characters (C string)
 *        etc.
 *
 *    Endian:
 *
 *        Messages are sent in big-endian format.
 *
 *        Example layouts:
 *
 *   Type  Value                 Big Endian                    Little Endian
 * -------------------- ---------------------------     ---------------------------
 *   u8    0x12          [0x12]                          [0x12]
 *   u16   0x1234        [0x12] [0x34]                   [0x34] [0x12]
 *   u32   0x12345678    [0x12] [0x34] [0x56] [0x78]     [0x78] [0x56] [0x34] [0x12]
 *   u8[]  0x12   0x34   [0x12] [0x34]                   [0x12] [0x34]
 *   u16[] 0x1234 0x5678 [0x12] [0x34] [0x56] [0x78]     [0x34] [0x12] [0x78] [0x56]
 *   u32[] 0x12345678    [0x12] [0x34] [0x56] [0x78]     [0x78] [0x56] [0x34] [0x12]
 *         0x9ABCDEF0    [0x9A] [0xBC] [0xDE] [0xF0]     [0xF0] [0xDE] [0xBC] [0x9A]
 *   s8[]  "Abcd  234"   [0x41] [0x62] [0x63] [0x64]     [0x41] [0x62] [0x63] [0x64]
 *                       [0x20] [0x32] [0x33] [0x34]     [0x20] [0x32] [0x33] [0x34]
 *
 *    Alignment:
 *
 *    Messages have no alignment restrictions.  For transmission efficiency,
 *    alignment padding is not recommended in messages.
 */
/****************************************************************************/

#if __MWERKS__
    #pragma mark  -
    #pragma mark Message Formats
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    M E S S A G E  F O R M A T S
 */
/****************************************************************************/

#if __MWERKS__
    #pragma mark  Message Header (common)
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    Message Header (new format):
 *
 *        u8        command                - (1) MessageCommandID
 *        u8        sequenceId             - (1) sequence ID of message
 *                                         <2> total
 *    - OR - (old format):
 *
 *        u8        command                - (1) MessageCommandID
 *                                         <1> total
 *
 *    All messages, including replies begin with this header.  Some may consist
 *    of nothing else (e.g., the Reset message).  Others contain additional
 *    information following the header.
 *
 *    Currently CodeWarriorTRK only supports sending 1 message at a time, which
 *    means that the sender must wait for a reply to each message before
 *    sending the next message.  For this case, the sequence IDs follow
 *    these rules:
 *        - For non-reply messages, the sequence ID should be 1 larger
 *          than the sequence ID of the previous message from the same
 *          sender, or 0x01 if the previous value was 0xFF.  The host
 *          and target sequence ID's are independent.
 *          A sequence ID of 0 is a special case.  It is always valid
 *          and resets the sequence so that the next expected ID is 1
 *          for both the host and target.
 *        - For ACK reply messages, the sequence ID should equal the
 *          sequence ID of the message to which it is replying.
 *        - For NAK reply messages, if the error code is
 *          kDSReplySequenceMissing then the sequence ID should be the
 *          next value that was expected by the receiver.  For
 *          any other error code, the sequence ID is indeterminate.
 *        - If a received, non-reply sequence ID is the same as the
 *          previous received sequence ID, the receiver should re-send
 *          the reply to the previous message *without* re-processing
 *          the message.  It is important for certain messages (e.g., file
 *          writes) that duplicates not be re-processed.
 *        - If a received, non-reply sequence ID is not equal to the
 *          previous received sequence ID, the next expected ID, or 0,
 *          the receiver should send a NAK reply with the
 *          kDSReplySequenceMissing error code and discard the
 *          received message.
 *
 *    The sequenceId field is new in version 2.0 of the protocol.  For
 *    backwards compatibility, descriptors are included for both the new
 *    and the old header formats.  The new format will be used by default,
 *    but it can be changed at runtime.  The old format can also be forced
 *    at build time by defining the following in target.h:
 *        #define TRK_MSG_SEQUENCE_IDS        0
 *
 */
/****************************************************************************/

#ifndef TRK_MSG_HEADER_LENGTH
    #define TRK_MSG_HEADER_LENGTH       0x40
#endif

#define TRK_MSG_HEADER_COUNT        2                /* 2 field in header */

/*
** For L1 and L2, sequence IDs are disabled by default.  For L3 they are
** enabled by default.  Individual targets can override this setting by
** defining TRK_MSG_SEQUENCE_IDS in target.h.
*/

#ifndef TRK_MSG_SEQUENCE_IDS
    #if DS_PROTOCOL < DS_PROTOCOL_RTOS
        #ifndef TRK_MSG_SEQUENCE_IDS
            #define TRK_MSG_SEQUENCE_IDS        0
        #endif
    #else
        #ifndef TRK_MSG_SEQUENCE_IDS
            #define TRK_MSG_SEQUENCE_IDS        1
        #endif
    #endif
#endif

/* The current header descriptor as of protocol version 2.0 */
#define TRK_MSG_HEADER_20_INIT \
{ \
    __debug_only_parm("Header") \
    NULL,                                        /* previous fragment/header */ \
    2,                                           /* # of bytes */ \
    TRK_MSG_HEADER_COUNT,                        /* # of fields */ \
    {                                            /* field descriptors: */ \
        __trk_msg_field("command", 1), \
        __trk_msg_field("sequence ID", 1) \
    } \
}

DESCDECL TrkMsgDesc gTrkMsgHeader20
#ifdef TRK_MSGCMD_INIT
    = TRK_MSG_HEADER_20_INIT
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* The old header descriptor prior to protocol version 2.0 */
#define TRK_MSG_HEADER_PRE_20_INIT \
{ \
    __debug_only_parm("Header") \
    NULL,                                        /* previous fragment/header */ \
    1,                                           /* # of bytes */ \
    TRK_MSG_HEADER_COUNT,                        /* # of fields */ \
    {                                            /* field descriptors: */ \
        __trk_msg_field("command", 1), \
        __trk_msg_field("sequence ID (N/A)", 0)  /* placeholder to make indices consistent */ \
    } \
}

DESCDECL TrkMsgDesc gTrkMsgHeaderPre20
#ifdef TRK_MSGCMD_INIT
    = TRK_MSG_HEADER_PRE_20_INIT
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* Use new header by default - can be changed at runtime. */
DESCDECLNC TrkMsgDesc gTrkMsgHeader
#ifdef TRK_MSGCMD_INIT
    #if TRK_MSG_SEQUENCE_IDS
        = TRK_MSG_HEADER_20_INIT;
    #else
        = TRK_MSG_HEADER_PRE_20_INIT
    #endif
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields. */
#define TRK_MSG_COMMAND_INDEX    0
#define TRK_MSG_SID_INDEX        1

#if __MWERKS__
    #pragma mark  Reply Header (common)
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    Reply Message Header
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     error                  - (1) CWDS error value
 *                                      <1 + header> total
 *
 *    The command for a reply message must be either kDSReplyACK or kDSReplyNAK.
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
#define TRK_MSG_REPLY_HEADER_LENGTH    (1 + TRK_MSG_HEADER_LENGTH)    /* 1 addl bytes  */
#define TRK_MSG_REPLY_HEADER_COUNT     (1 + TRK_MSG_HEADER_COUNT)     /* 1 addl fields */

DESCDECL TrkMsgDesc gTrkMsgReplyHeader
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Reply Header")
        &gTrkMsgHeader,                                /* previous fragment/header */
        1,                                             /* # of addl bytes */
        1,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("error", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_REPLY_ERROR_INDEX            (0 + TRK_MSG_HEADER_COUNT)

/* Retain these for compatibility with older code */
#define DS_MIN_MSG_LENGTH                    TRK_MSG_HEADER_LENGTH
#define DS_MIN_REPLY_LENGTH                  TRK_MSG_REPLY_HEADER_LENGTH

/****************************************************************************/
/*
 *    Required Send and Reply Messsages
 */
/****************************************************************************/

#if __MWERKS__
    #pragma mark  Ping
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSPing
 *
 *    Send:
 *
 *        ....    <Message Header>      ... (see gTrkMsgHeader)
 *                                      <header> total
 *    Reply:
 *
 *        ....    <Reply Header>        ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Can be sent by the host at any time.  Simply replies with an
 *    ACK.  Currently not supported.
 */
/****************************************************************************/

#define DS_MIN_MSG_PING_LENGTH                DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_PING_LENGTH              DS_MIN_REPLY_LENGTH


#if __MWERKS__
    #pragma mark  Connect
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSConnect
 *
 *    Send:
 *
 *        ....    <Message Header>      ... (see gTrkMsgHeader)
 *                                      <header> total
 *    Reply:
 *
 *        ....    <Reply Header>        ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    The host should send a connect message once at the beginning of
 *    a debug session.
 */
/****************************************************************************/

#define DS_MIN_MSG_CONNECT_LENGTH             DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_CONNECT_LENGTH           DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  Disconnect
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSDisconnect
 *
 *    Send:
 *
 *        ....    <Message Header>      ... (see gTrkMsgHeader)
 *                                      <header> total
 *    Reply:
 *
 *        ....    <Reply Header>        ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    The host should send a disconnect message once at the end of
 *    a debug session.
 */
/****************************************************************************/

#define DS_MIN_MSG_DISCONNECT_LENGTH          DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_DISCONNECT_LENGTH        DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  Reset
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSReset
 *
 *    Send:
 *
 *        ....    <Message Header>      ... (see gTrkMsgHeader)
 *                                      <header> total
 *    Reply:
 *
 *        ....    <Reply Header>        ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *  Sent by the host.  Causes a soft reset on the target board.  The
 *    ACK reply is sent before reset.  The host should also expect a
 *    welcome message after reset.
 */
/****************************************************************************/

#define DS_MIN_MSG_RESET_LENGTH               DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_RESET_LENGTH             DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  Versions
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSVersions
 *
 *    Send:
 *
 *        ....    <Message Header>      ... (see gTrkMsgHeader)
 *                                      <header> total
 *
 *    Reply:
 *
 *        ....    <Reply Header>        ... (see gTrkMsgReplyHeader)
 *        u8        kernelMajor         - (1)
 *        u8        kernelMinor         - (1)
 *        u8        protocolMajor       - (1)
 *        u8        protocolMinor       - (1)
 *                                      <4 + header> total
 *
 *    Sent by the host, returns TRK version numbers.
 */
/****************************************************************************/
#define DS_MIN_MSG_VERSIONS_LENGTH            DS_MIN_MSG_LENGTH

DESCDECL TrkMsgDesc gTrkMsgVersionsReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Versions Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        4,                                            /* # of addl bytes */
        4,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("kernel major", 1),
            __trk_msg_field("kernel minor", 1),
            __trk_msg_field("protocol major", 1),
            __trk_msg_field("protocol minor", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_VR_KMAJOR_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_VR_KMINOR_INDEX        (1 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_VR_PMAJOR_INDEX        (2 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_VR_PMINOR_INDEX        (3 + TRK_MSG_REPLY_HEADER_COUNT)

#if __MWERKS__
    #pragma mark  CPUType
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSCPUType
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *                                      <header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u8     CPU major type         - (1) DS_CPU_MAJOR_* (see below)
 *        u8     CPU minor type         - (1) DS_CPU_* (see below)
 *        u8     big_endian             - (1) 1 ==> big endian, 0 ==> little endian
 *        u8     default type size      - (1) size of DefaultType
 *        u8     FP type size           - (1) size of FPType (0 if unsupported)
 *        u8     extended1 type size    - (1) size of Extended1Type (0 if unsupported)
 *        u8     extended2 type size    - (1) size of Extended2Type (0 if unsupported)
 *                                      <7 + header> total
 *
 *    Sent by the host, returns information indicating the CPU type and
 *    CPU implementation details for the target system.
 */
/****************************************************************************/
#define DS_MIN_MSG_CPUTYPE_LENGTH            DS_MIN_MSG_LENGTH

DESCDECL TrkMsgDesc gTrkMsgCpuTypeReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("CPU Type Reply")
        &gTrkMsgReplyHeader,                        /* previous fragment/header */
        7,                                          /* # of addl bytes */
        7,                                          /* # of addl fields */
        {                                           /* addl field descriptors: */
            __trk_msg_field("CPU major", 1),
            __trk_msg_field("CPU minor", 1),
            __trk_msg_field("big endian", 1),
            __trk_msg_field("default type size", 1),
            __trk_msg_field("FP type size", 1),
            __trk_msg_field("extended1 type size", 1),
            __trk_msg_field("extended2 type size", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_CTR_CMAJOR_INDEX            (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_CMINOR_INDEX            (1 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_BIGENDIAN_INDEX         (2 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_DEFAULT_SIZE_INDEX      (3 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_FP_SIZE_INDEX           (4 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_EXTENDED1_SIZE_INDEX    (5 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_CTR_EXTENDED2_SIZE_INDEX    (6 + TRK_MSG_REPLY_HEADER_COUNT)

/*
** CPU Major types.
*/

#define DS_CPU_MAJOR_PPC                    0x00
#define DS_CPU_MAJOR_MIPS                   0x01
#define DS_CPU_MAJOR_NEC_V8XX               0x02
#define DS_CPU_MAJOR_MOT_DSP                0x03
#define DS_CPU_MAJOR_ARM                    0x04
#define DS_CPU_MAJOR_X86                    0x05
#define DS_CPU_MAJOR_MCORE                  0x06
#define DS_CPU_MAJOR_M68K                   0x07    /* includes ColdFire */
#define DS_CPU_MAJOR_SUPERH                 0x08
#define DS_CPU_MAJOR_STARCORE               0x09

/*
** ARM CPU minor types.
*/

#define DS_CPU_ARM_920T                     0x00

/*
** MIPS CPU minor types.
*/

#define DS_CPU_MIPS_R3000                   0x00
#define DS_CPU_MIPS_R3081E                  0x01

#define DS_CPU_MIPS_VR4100                  0x20
#define DS_CPU_MIPS_VR4300                  0x21
#define DS_CPU_MIPS_VR4500                  0x22
#define DS_CPU_MIPS_VR4111                  0x23
#define DS_CPU_MIPS_TR4101                  0x24
#define DS_CPU_MIPS_EZ4103                  0x25
#define DS_CPU_MIPS_RM7000                  0x26
#define DS_CPU_MIPS_MIPS32_4kc              0x27
#define DS_CPU_MIPS_RM5231                  0x28

#define DS_CPU_MIPS_VR5000                  0x40


/*
** PowerPC CPU minor types.
*/

#define    DS_CPU_PPC_403                   0x00

#define DS_CPU_PPC_5XX                      0x20    /* used when not precisely known */
#define DS_CPU_PPC_505                      0x21
#define DS_CPU_PPC_509                      0x22
#define DS_CPU_PPC_50X                      0x27    /* used when not precisely known */
#define DS_CPU_PPC_555                      0x28

#define DS_CPU_PPC_E600                     0x30

#define DS_CPU_PPC_603E                     0x40

#define DS_CPU_PPC_7XX                      0x50    /* used when not precisely known */
#define DS_CPU_PPC_740                      0x51
#define DS_CPU_PPC_750                      0x52
#define DS_CPU_PPC_7400                     0x53
#define DS_CPU_PPC_7410                     0x54

#define DS_CPU_PPC_8260                     0x61

#define DS_CPU_PPC_E300                     0x70
#define DS_CPU_PPC_E300C2                   0x71

#define DS_CPU_PPC_8XX                      0x80    /* used when not precisely known */
#define DS_CPU_PPC_821                      0x81
#define DS_CPU_PPC_823                      0x82
#define DS_CPU_PPC_860                      0x83

#define DS_CPU_PPC_E500                     0x90
#define DS_CPU_PPC_E500MC                   0x91

/*
** PowerPC PVR masks
*/

#define E300      0x80800000
#define E300C2    0x80840000
#define E500      0x80200000
#define E500MC    0x80230000
#define E600      0x80040000


/*
** NEC V8xx CPU minor types.
*/

#define DS_CPU_NEC_V8XX_V810             0x00
#define DS_CPU_NEC_V8XX_V821             0x01

#define DS_CPU_NEC_V8XX_V830             0x10
#define DS_CPU_NEC_V8XX_V831             0x11
#define DS_CPU_NEC_V8XX_V832             0x12

#define DS_CPU_NEC_V8XX_V850             0x20
#define DS_CPU_NEC_V8XX_V853             0x21
#define DS_CPU_NEC_V8XX_V850E            0x22
#define DS_CPU_NEC_V8XX_V850_SA1         0x23

/*
** M68K/Coldfire CPU minor types.
*/

#define DS_CPU_M68K_68000                0x00
#define DS_CPU_M68K_68020                0x02
#define DS_CPU_M68K_68030                0x04
#define DS_CPU_M68K_68040                0x06
#define DS_CPU_M68K_68060                0x08

#define DS_CPU_M68K_68302                0x20
#define DS_CPU_M68K_68306                0x22
#define DS_CPU_M68K_68307                0x24
#define DS_CPU_M68K_68328                0x26
#define DS_CPU_M68K_68EZ328              0x28
#define DS_CPU_M68K_68VZ328              0x29
#define DS_CPU_M68K_68340                0x2a
#define DS_CPU_M68K_68360                0x2c

#define DS_CPU_M68K_CF5102               0x80
#define DS_CPU_M68K_CF5202               0x82
#define DS_CPU_M68K_CF5204               0x84
#define DS_CPU_M68K_CF5206               0x86
#define DS_CPU_M68K_CF5206E              0x88
#define DS_CPU_M68K_CF5307               0x8a

/*
** MCore CPU minor types.
*/

#define DS_CPU_MCORE_200                 0x00
#define DS_CPU_MCORE_340                 0x20

/*
** STARCORE CPU minor types
*/

#define DS_CPU_STARCORE_8101             0x01
#define    DS_CPU_STARCORE_8102          0x02

#if __MWERKS__
    #pragma mark  SupportMask
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSSupportMask
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *                                      <header> total
 *
 *    Reply:
 *
 *        u8     command                - (1) ack/nak MessageCommandID
 *        u8     error                  - (1) CWDS error value
 *        u8     mask[32]               - (32) DSSupportMask
 *        u8     protocolLevel          - (1) value of DS_PROTOCOL (1-3)
 *                                      <34>
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
typedef u8    DSSupportMask[32];                /* 256 bits total */

#define DS_MIN_MSG_SUPPORTMASK_LENGTH           DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_SUPPORTMASK_LENGTH        (DS_MIN_REPLY_LENGTH + sizeof(DSSupportMask) + 1)

#if __MWERKS__
    #pragma mark  ConfigTransport
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSConfigTransport
 *
 *    Send: (if type == TRK_TRANSPORT_TYPE_ETHERNET)
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    general options        - (4) reserved (set to 0)
 *        u32    type                   - (4) TRK_TRANSPORT_TYPE_ETHERNET (see below)
 *        u32    channel#               - (4) reserved (set to 0)
 *        u32    ethernet options       - (4) reserved (set to 0)
 *        u32    address                - (4) IP address of TRK
 *        u32    port                   - (4) port used by TRK
 *        u32    gateway                - (4) gateway IP address (0 if none)
 *        u32    subnet mask            - (4) IP subnet mask (0 if none)
 *                                      <header + 32> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host, configures a transport channel on the target with
 *    appropriate parameters.  The first 3 fields should be common to all
 *    transport types.  The number and format of any remaining fields
 *    depend on the value of the "type" field.  Currently only ethernet
 *    channels can be configured in this way.  Also, since the bare-board
 *    TRK TCP/IP stack only supports a single hardware ethernet channel, the
 *    "channel#" field should be set to 0 for now.
 */
/****************************************************************************/
/* values to be used in the "type" field */
#define TRK_MSG_TRANSPORT_TYPE_ETHERNET        0


/* common segment of message */
#define TRK_MSG_CT_HEADER_COUNT                (3 + TRK_MSG_HEADER_COUNT)
DESCDECL TrkMsgDesc gTrkMsgConfigTransport
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Config Transport")
        &gTrkMsgHeader,                                /* previous fragment/header */
        12,                                            /* # of addl bytes */
        3,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("general options", 4),
            __trk_msg_field("type", 4),
            __trk_msg_field("channel#", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_CT_GENOPT_INDEX                (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_CT_TYPE_INDEX                  (1 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_CT_CHANNEL_INDEX               (2 + TRK_MSG_HEADER_COUNT)


/* ethernet-specific segment of message */
DESCDECL TrkMsgDesc gTrkMsgConfigTransportEthernet
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Config Transport (Ethernet)")
        &gTrkMsgConfigTransport,                    /* previous fragment/header */
        20,                                         /* # of addl bytes */
        5,                                          /* # of addl fields */
        {                                           /* addl field descriptors: */
            __trk_msg_field("ethernet options", 4),
            __trk_msg_field("address", 4),
            __trk_msg_field("port", 4),
            __trk_msg_field("gateway", 4),
            __trk_msg_field("netmask", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_CTE_ETHOPT_INDEX             (0 + TRK_MSG_CT_HEADER_COUNT)
#define TRK_MSG_CTE_ADDRESS_INDEX            (1 + TRK_MSG_CT_HEADER_COUNT)
#define TRK_MSG_CTE_PORT_INDEX               (2 + TRK_MSG_CT_HEADER_COUNT)
#define TRK_MSG_CTE_GATEWAY_INDEX            (3 + TRK_MSG_CT_HEADER_COUNT)
#define TRK_MSG_CTE_NETMASK_INDEX            (4 + TRK_MSG_CT_HEADER_COUNT)

#if __MWERKS__
    #pragma mark  ReadMemory
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSReadMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length
 *        u32    start                  - (4) memory start address
 *
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length
 *        u32    startHigh              - (4) memory start high-order address
 *        u32    startLow               - (4) memory start low-order address
 *                                      <12> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length
 *        u32    start                  - (4) memory start address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <16> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length
 *        u32    startHigh              - (4) memory start high-order address
 *        u32    startLow               - (4) memory start low-order address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <20> total
 *
 *    Reply:
 *
 *        u8     command                - (1) ack/nak MessageCommandID
 *        u8     error                  - (1) CWDS error value
 *        u16    length                 - (2) amount that was read
 *        u8     data[]                 - (0 to 2048) data
 *                                      <4 + length> total
 *
 *    Sent by the host, returns the requested memory contents.  The TRK
 *    will attempt to catch and handle any memory access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/


#define DS_MIN_MSG_READMEMORY_LENGTH          (DS_MIN_MSG_LENGTH + 7)
#define DS_MIN_REPLY_READMEMORY_LENGTH        (DS_MIN_REPLY_LENGTH + 2)

#if __MWERKS__
    #pragma mark  WriteMemory
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSWriteMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length in bytes
 *        u32    start                  - (4) memory start address
 *        u8     data[]                 - (0 to 2048) data
 *                                      <8 + length> total
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length in bytes
 *        u32    startHigh              - (4) memory start high-order address
 *        u32    startLow               - (4) memory start low-order address
 *        u8     data[]                 - (0 to 2048) data
 *                                      <12 + length> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) == 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length in bytes
 *        u32    start                  - (4) memory start address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *        u8     data[]                 - (0 to 2048) data
 *                                      <16 + length> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (header.options & DS_MSG_MEMORYEXTENDED) != 0
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u16    length                 - (2) memory data length in bytes
 *        u32    startHigh              - (4) memory start high-order address
 *        u32    startLow               - (4) memory start low-order address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *        u8     data[]                 - (0 to 2048) data
 *                                      <20 + length> total
 *
 *    Reply:
 *
 *        u8     command                - (1) ack/nak MessageCommandID
 *        u8     error                  - (1) CWDS error value
 *        u16    length                 - (2) amount that was written in bytes
 *                                      <4> total
 *
 *    Sent by the host, writes the specified values to target memory  The TRK
 *    will attempt to catch and handle any memory access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#define DS_MIN_MSG_WRITEMEMORY_LENGTH          (DS_MIN_MSG_LENGTH + 7)
#define DS_MIN_REPLY_WRITEMEMORY_LENGTH        (DS_MIN_REPLY_LENGTH + 2)

#if __MWERKS__
    #pragma mark  FillMemory
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSFillMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u8     fillValue              - (1) value with which to fill memory
 *        u32    length                 - (4) data length in bytes
 *        u32    start                  - (4) memory start address
 *                                      <11> total
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u8     fillValue              - (1) value with which to fill memory
 *        u32    length                 - (4) data length in bytes
 *        u32    start                  - (4) memory start address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <19> total
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

#define DS_MIN_MSG_FILLMEMORY_LENGTH          (DS_MIN_MSG_LENGTH + 10)
#define DS_MIN_REPLY_FILLMEMORY_LENGTH        (DS_MIN_REPLY_LENGTH)

#if __MWERKS__
    #pragma mark  CopyMemory
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSCopyMemory
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u32    length                 - (4) data length in bytes
 *        u32    source                 - (4) source memory start address
 *        u32    destination            - (4) destination memory start address
 *                                      <14> total
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) One of DS_MSG_MEMORY_* below
 *        u32    length                 - (4) data length in bytes
 *        u32    source                 - (4) source memory start address
 *        u32    destination            - (4) destination memory start address
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <22> total
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

#define DS_MIN_MSG_COPYMEMORY_LENGTH          (DS_MIN_MSG_LENGTH + 13)
#define DS_MIN_REPLY_COPYMEMORY_LENGTH        (DS_MIN_REPLY_LENGTH)

                                               /* option flags in DSMessage.options */
#define DS_MSG_MEMORY_SEGMENTED    0x01        /* non-flat addr space */
#define DS_MSG_MEMORY_EXTENDED     0x02        /* > 32-bit data addr */
#define DS_MSG_MEMORY_PROTECTED    0x04        /* non-user memory */
#define DS_MSG_MEMORY_USERVIEW     0x08        /* breakpoints are invisible */

/*
** The desired memory space can be specified using one of
** the following options.  It can be logically OR'ed with
** the flags defined above.
*/

#define DS_MSG_MEMORY_SPACE_PROGRAM    0x00
#define DS_MSG_MEMORY_SPACE_DATA       0x40
#define DS_MSG_MEMORY_SPACE_IO         0x80

/*
** Use this mask to extract the memory space specifier
** from the options field.  The specifier occupies the
** 2 most significant bits.
*/

#define DS_MSG_MEMORY_SPACE_MASK    0xc0

#if __MWERKS__
    #pragma mark  FlushCache
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSFlushCache
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DS_MSG_CACHE_* (see below)
 *        u32    start                  - (4) start of mem region for which to clear cache
 *        u32    end                    - (4) end of mem region for which to clear cache
 *                                      <10> total
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

#define DS_MIN_MSG_FLUSHCACHE_LENGTH      (DS_MIN_MSG_LENGTH + 9)
#define DS_MIN_REPLY_FLUSHCACHE_LENGTH    (DS_MIN_REPLY_LENGTH)

/*
** The following values may be logically OR'ed together in the
** options field of the kDSFlushCache command to specify
** the cache types to be flushed.
*/

#define DS_MSG_CACHE_TYPE_INSTRUCTION      0x01
#define DS_MSG_CACHE_TYPE_DATA             0x02
#define DS_MSG_CACHE_TYPE_SECONDARY        0x04


#if __MWERKS__
    #pragma mark  ReadRegisters
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSReadRegisters
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageRegisterOptions
 *        u16    firstRegister          - (2) first register ID
 *        u16    lastRegister           - (2) last register ID
 *                                      <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageRegisterOptions
 *        u16    firstRegister          - (2) first register ID
 *        u16    lastRegister           - (2) last register ID
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <14>
 *
 *    Reply:
 *
 *        u8     command                - (1) ack/nak MessageCommandID
 *        u8     error                  - (1) CWDS error value
 *               register data          - (0-2048)
 *                                      <2 + register data>
 *
 *    Sent by the host, reads the requested register contents.  The TRK
 *    will attempt to catch and handle any access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#define DS_MIN_MSG_READREGISTERS_LENGTH        (DS_MIN_MSG_LENGTH + 5)
#define DS_MIN_REPLY_READREGISTERS_LENGTH       DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  WriteRegisters
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSWriteRegisters
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageRegisterOptions
 *        u16    firstRegister          - (2) first register ID
 *        u16    lastRegister           - (2) last register ID
 *               register data          - (0-2048)
 *                                      <6 + register data>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageRegisterOptions
 *        u16    firstRegister          - (2) first register ID
 *        u16    lastRegister           - (2) last register ID
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *               register data          - (0-2048)
 *                                      <14 + register data>
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host, writes the specified values to target registers.
 *    The TRK will attempt to catch and handle any access exceptions that
 *    occur during this operation.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#define DS_MIN_MSG_WRITEREGISTERS_LENGTH      (DS_MIN_MSG_LENGTH + 5)
#define DS_MIN_REPLY_WRITEREGISTERS_LENGTH    DS_MIN_REPLY_LENGTH

                                               /* option flags for reading registers */

#define DS_MSG_REGISTERS_TYPE_MASK     0x07    /* "register type" mask */
#define DS_MSG_REGISTERS_TYPE_SHIFT    0       /* # bits to shift mask from lsb. */
#define DS_MSG_REGISTERS_TYPE(_x_)     ((_x_ & DS_MSG_REGISTERS_TYPE_MASK) << DS_MSG_REGISTERS_TYPE_SHIFT)

/*
 *    kDSReadRegisters & kDSWriteRegister options
 */

typedef enum DSMessageRegisterOptions
{
    kDSRegistersDefault   = 0x0,          /* Default register block */
    kDSRegistersFP        = 0x1,          /* floating point registers */
    kDSRegistersExtended1 = 0x2,          /* Extended register set 1 */
    kDSRegistersExtended2 = 0x3           /* Extended register set 2 */
} DSMessageRegisterOptions;


#if __MWERKS__
    #pragma mark  Continue
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSContinue
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *                                      <header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <header + 8> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
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
 *    have stopped again, CodeWarriorTRK will once again accept other commands
 *    which access them.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#define DS_MIN_MSG_CONTINUE_LENGTH            DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_CONTINUE_LENGTH          DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  Step
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSStep
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoCount || options == kDSStepOverCount)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStepOptions
 *        u8     count                  - (1) # of instructions
 *                                      <3>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoCount || options == kDSStepOverCount)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStepOptions
 *        u8     count                  - (1) # of instructions
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <11>
 *
 *    Sent by the host.  Steps over the specified number of instructions.
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoRange || options == kDSStepOverRange)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStepOptions
 *        u32    rangeStart             - (4) Start address of memory range
 *        u32    rangeEnd               - (4) End address of memory range
 *                                      <10>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStepIntoRange || options == kDSStepOverRange)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStepOptions
 *        u32    rangeStart             - (4) Start address of memory range
 *        u32    rangeEnd               - (4) End address of memory range
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <18>
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
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
 *    have stopped again, CodeWarriorTRK will once again accept other commands
 *    which access them.
 *
 *    This command can only be used on a stopped system (protocol level 2)
 *    or stopped thread (protocol level 3).
 */
/****************************************************************************/

#define DS_MIN_MSG_STEP_COUNT_LENGTH      (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_MSG_STEP_RANGE_LENGTH      (DS_MIN_MSG_LENGTH + 9)
#define DS_MIN_MSG_STEP_LENGTH            (DS_MIN_MSG_STEP_COUNT_LENGTH)
#define DS_MIN_REPLY_STEP_LENGTH          DS_MIN_REPLY_LENGTH

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


#if __MWERKS__
    #pragma mark  Stop
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSStop
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *                                      <header> total
 *
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopSystem)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStopOptions
 *                                      <2>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopProcess)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStopOptions
 *        u32    processID              - (4) unique process ID
 *                                      <6>
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS &&
 *             (options == kDSStopThread)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageStopOptions
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <10>
 *
 *    Sent by the host.  Forces the target system/process/thread to stop.
 *  Replies    with an ACK.
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_STOP_LENGTH            (DS_MIN_MSG_LENGTH + 2)
#define DS_MIN_REPLY_STOP_LENGTH          DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  SetBreak
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSSetBreak
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     options                - (1) DSMessageBreakOptions
 *        u8     targetOptions          - (1) target-defined
 *        u32    address                - (4) Start of breakpoint memory range
 *        u32    length                 - (4) Length of breakpoint memory range
 *        u32    count                  - (4) Only stop after triggered this many times (currently not used, set to 0)
 *                                      <14 + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     options                - (1) DSMessageBreakOptions
 *        u8     targetOptions          - (1) target-defined
 *        u32    address                - (4) Start of breakpoint memory range
 *        u32    length                 - (4) Length of breakpoint memory range
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *        u32    count                  - (4) Only stop after triggered this many times (currently not used, set to 0)
 *                                      <22 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u32    id                     - (4) ID of the new breakpoint
 *                                      <4 + header> total
 *    Sent by the host.  Sets a breakpoint at the specified location.
 *    Replies with an ACK.
 *
 *  For software execution breakpoints, the length field should be 1.
 *
 *    Note: This command should not be implemented unless there is a
 *    compelling reason to do so.  In most cases, both software and
 *    hardware breakpoints can be implemented via memory and register reads
 *    and writes.
 *
 *
 */
/****************************************************************************/
#define TRK_MSG_REPLY_HEADER_LENGTH    (1 + TRK_MSG_HEADER_LENGTH)    /* 1 addl bytes  */
#define TRK_MSG_REPLY_HEADER_COUNT     (1 + TRK_MSG_HEADER_COUNT)     /* 1 addl fields */

DESCDECL TrkMsgDesc gTrkMsgSetBreak
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Set Breakpoint")
        &gTrkMsgHeader,                                /* previous fragment/header */
        14,                                            /* # of addl bytes */
        5,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("options", 1),
            __trk_msg_field("target options", 1),
            __trk_msg_field("address", 4),
            __trk_msg_field("length", 4),
            __trk_msg_field("count", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* field indices */
#define TRK_MSG_SETBREAK_OPTIONS_INDEX     (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_TOPTIONS_INDEX    (1 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_ADDRESS_INDEX     (2 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_LENGTH_INDEX      (3 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_COUNT_INDEX       (4 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_PID_INDEX         (5 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_SETBREAK_TID_INDEX         (6 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgSetBreakReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Set Breakpoint Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        4,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
           __trk_msg_field("breakpoint ID", 4),
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_SETBREAKREPLY_BID_INDEX    (0 + TRK_MSG_REPLY_HEADER_COUNT)

#if __MWERKS__
    #pragma mark  ClearBreak
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSClearBreak
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    id                     - (4) ID of the breakpoint to clear
 *                                      <4 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host.  Clears the specified breakpoint.  If the specified
 *    id is DS_MSG_BREAK_ID_ALL, clears all breakpoints.
 *
 */
/****************************************************************************/
DESCDECL TrkMsgDesc gTrkMsgClearBreak
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Clear Breakpoint")
        &gTrkMsgHeader,                               /* previous fragment/header */
        8,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("breakpoint ID", 4),
            __trk_msg_field("breakpoint Address", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_CLEARBREAK_BID_INDEX      (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_CLEARBREAK_BADDR_INDEX    (1 + TRK_MSG_HEADER_COUNT)

/*
** The following values should be logically OR'ed with one
** of the DSMessageBreakOptions values to specify the
** type of access which will cause the target program to
** stop.  More than one stop type can be specified.
** the kDSSetBreak command will fail if one of the specified
** conditions is not supported on the target, but there is
** one exception:
**
** NOTE: If both the write and modify conditions are
** specified, the kDSSetBreak command will succeed if
** one of the two is supported.
**
*/

#define DS_MSG_BREAK_CONDITION_READ        0x10    /* stop on read access */
#define DS_MSG_BREAK_CONDITION_WRITE       0x20    /* stop on write access */
#define DS_MSG_BREAK_CONDITION_MODIFY      0x40    /* stop on modify access */
#define DS_MSG_BREAK_CONDITION_EXECUTE     0x80    /* stop on execution */

/*
** Use the following mask to extract the breakpoint condition flags
** from the options.
*/

#define DS_MSG_BREAK_CONDITION_MASK        0xf0

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageBreakOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSBreakSoftware    = 0x00,        /* Set a software breakpoint */
    kDSBreakHardware    = 0x01,        /* Set a hardware breakpoint */
    kDSBreakAuto        = 0x02         /* Set a SW BP if possible, otherwise HW */
} DSMessageBreakOptions;

/*
** If the following is provided as the breakpoint ID in the kDSClearBreak
** command, all breakpoints will be cleared.
*/

#define DS_MSG_BREAK_ID_ALL                0xffffffff

/*
** If the following is provided as the thread ID in the kDSSetBreak
** command, the operation will apply to all threads in the process.
*/

#define DS_MSG_BREAK_THREAD_ID_ALL         0xffffffff

#if __MWERKS__
    #pragma mark  ModifyBreakThread
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSModifyBreakThread
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    id                     - (4) ID of the breakpoint to modify
 *        u32    threadID               - (4) new thread ID
 *                                      <8 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host.  Associates an existing breakpoint with the
 *    specified thread.  The thread ID can be for a specific thread or
 *    it can be DS_MSG_BREAK_THREAD_ID_ALL.
 *
 *    This command is only valid if DS_PROTOCOL == DS_PROTOCOL_RTOS.
 *
 */
/****************************************************************************/
DESCDECL TrkMsgDesc gTrkMsgModifyBreakThread
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Modify Breakpoint Thread")
        &gTrkMsgHeader,                               /* previous fragment/header */
        8,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("breakpoint ID", 4),
            __trk_msg_field("thread ID", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_MBT_BID_INDEX        (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_MBT_TID_INDEX        (1 + TRK_MSG_HEADER_COUNT)


#if __MWERKS__
    #pragma mark  Download
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSDownload
 *
 *    Send: if (DSMessageDownloadOptions == kDSMessageDownloadTFTP)
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     options                - (1) DSMessageDownloadOptions (see below)
 *        u32    IP address             - (4) IP address of the target system
 *        u32    TFTP address           - (4) IP address of the TFTP server
 *        u32    length                 - (4) file size in bytes
 *        u32    start address          - (4) load address of the file
 *        u16    name_length            - (2) length of file name in bytes
 *        u8[]   file name              (0 - 2048) name of the file to download
 *                                      <20 + name_length>
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host.  Downloads a file to the target system according
 *    to the specified parameters.  This command is intended to allow
 *    downloading via communication channels other than the TRK's command
 *    channel.
 *    Replies with an ACK.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_DOWNLOAD_LENGTH        (DS_MIN_MSG_LENGTH + 19)
#define DS_MIN_REPLY_CLEARBREAK_LENGTH    DS_MIN_REPLY_LENGTH

/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargMessageDownloadOptions enum (see msgtarg.h).
*/

typedef enum
{
    kDSMessageDownloadTFTP        = 0x0
} DSMessageDownloadOptions;

#if __MWERKS__
    #pragma mark  NotifyStopped
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSNotifyStopped
 *
 *    Send:
 *
 *        u8     command                 - (1) MessageCommandID
 *                target-defined info (PC, instr @PC, exception word)
 *                                       <1 + target info> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>          ... (see gTrkMsgReplyHeader)
 *                                       <header> total
 *
 *    Sent by the target.  This message will only be sent when the target
 *    is executing (after a kDSContinue or kDSStep command).  It indicates
 *    that the target stopped running.  On a bare-board system (no OS), this
 *    typically means the target stopped due to an exception.  The
 *  target-defind info should provide additional details about why
 *    the target stopped.
 *    Expects an ACK in response.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_NOTIFYSTOPPED_LENGTH        DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_NOTIFYSTOPPED_LENGTH      DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  NotifyInternalError
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSNotifyInternalError
 *
 *    Send:
 *
 *        u8     command                - (1) MessageCommandID
 *        u8     error code             - (1) DSInternalError
 *                                      <2> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the target.  It indicates that an internal error occurred
 *    in CodeWarriorTRK.
 *    Expects an ACK in response.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_NOTIFYINTERNALERROR_LENGTH     (DS_MIN_MSG_LENGTH + 1)
#define DS_MIN_REPLY_NOTIFYINTERNALERROR_LENGTH   DS_MIN_REPLY_LENGTH

#if __MWERKS__
    #pragma mark  OSNotifyCreated / OSNotifyDeleted
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOSNotifyCreated / kDSOSNotifyDeleted
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u16    itemType               - (2) DSOSItemTypes
 *                target-defined info (e.g., process ID for kDSOSDLLItem)
 *                                      <2 + header + target info> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                target-defined info (e.g., code & data offset for
 *                                     kDSOSProcessItem)
 *                                      <header + target info> total
 *
 *    Sent by the target.  This message indicates that an item of the
 *    given type has been created/deleted.
 *    Expects an ACK in response.
 *
 */
/****************************************************************************/

#if __MWERKS__
    #pragma mark  OSCreateItem / OSDeleteItem Header
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOSCreateItem / kDSOSDeleteItem
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u16    itemType               - (2) DSOSItemTypes
 *                target-defined info (e.g., DLL name for kDSOSDLLItem)
 *                                      <2 + header + target info> total
 *
 *    Reply
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                target-defined info (e.g., code & data offset for
 *                                     kDSOSProcessItem)
 *                                      <header + target info> total
 *
 *    Sent by the host.  Creates/deletes an item of the specified type in
 *    the system.  It allows the host to create/delete processes (load),
 *    threads, DLL's, and any other system resources.  This command may
 *    either operate synchronously or asynchronously.  The behavior may be
 *    differ on an item-by-item basis.  The two scenarios:
 *
 *        synchronous: The host sends a OSCreateItem/OSDeleteItem command.
 *            The target system creates the item and then replies with an
 *            ACK containing specific information about the
 *            created/deleted item.
 *
 *        asynchronous: The host sends a OSCreateItem/OSDeleteItem command.
 *            The target system queues the request and replies with
 *            an ACK.  At some later time, when the item is
 *            actually created/deleted, the target system sends an
 *            OSNotifyCreated/OSNotifyDeleted message to the host
 *            containing specific information about the created/deleted
 *            item.
 *
 *    Expects an ACK in response.  The reply may contain specific
 *    information about the created/deleted item, such as code and data
 *    offsets for relocatable processes (if the operation is
 *    synchronous).
 *
 */
/****************************************************************************/

#define TRK_MSG_ITEM_HEADER_LENGTH    (2 + TRK_MSG_HEADER_LENGTH)    /* 2 addl bytes  */
#define TRK_MSG_ITEM_HEADER_COUNT     (1 + TRK_MSG_HEADER_COUNT)     /* 1 addl fields */

DESCDECL TrkMsgDesc gTrkMsgItemHeader
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Item Header")
        &gTrkMsgHeader,                               /* previous fragment/header */
        2,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("item type", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

#define TRK_MSG_ITEM_HEADER_TYPE_INDEX        (0 + TRK_MSG_HEADER_COUNT)


/*
** The following option values are predefined.  Desired
** target-specific options can be added to the
** DSTargOSItemTypes enum (see msgtarg.h).
*/

typedef enum
{
    kDSOSProcessItem      = 0x0000,
    kDSOSThreadItem       = 0x0001,
    kDSOSDLLItem          = 0x0002,
    kDSOSAppItem          = 0x0003,
    kDSOSMemBlockItem     = 0x0004,
    kDSOSProcAttachItem   = 0x0005,
    kDSOSThreadAttachItem = 0x0006,
    kDSOSProcAttach2Item  = 0x0007,
    kDSOSProcRunItem      = 0x0008
    /* 0x0008 - 0x00ff reserved for general expansion */
    /* 0x0100 - 0xffff available for target-specific use */
} DSOSItemTypes;

#if __MWERKS__
    #pragma mark  OSReadInfo / OSWriteInfo Header
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOSReadInfo / KDSOSWriteInfo (Common Header)
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u16    infoType               - (2) DSOSInfoTypes
 *                target-defined info (e.g., process ID for
 *                                     kDSOSProcessStateInfo)
 *                                      <2 + header + target info> total
 *
 *    Reply
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                target-defined info (e.g., process state for
 *                                     kDSOSProcessStateInfo)
 *                                     <header + target info> total
 *
 *    Sent by the host.  Reads/writes the requested info from/to the
 *    target system.  It allows the host to get/set information about the
 *    internal system state.  For ReadInfo, there are additional fields
 *    specified in a secondary header (see below).
 *
 */
/****************************************************************************/

#define TRK_MSG_INFO_HEADER_LENGTH   (2 + TRK_MSG_HEADER_LENGTH)   /* 2 addl bytes  */
#define TRK_MSG_INFO_HEADER_COUNT    (1 + TRK_MSG_HEADER_COUNT)    /* 1 addl fields */

DESCDECL TrkMsgDesc gTrkMsgInfoHeader
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Info Header")
        &gTrkMsgHeader,                                /* previous fragment/header */
        2,                                             /* # of addl bytes */
        1,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("info type", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

#define TRK_MSG_INFO_HEADER_TYPE_INDEX        (0 + TRK_MSG_HEADER_COUNT)

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
    /* 0x0100 - 0xffff available for target-specific use */
} DSOSInfoTypes;

#if __MWERKS__
    #pragma mark  OSReadInfo Header
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOSReadInfo (header)
 *
 *    Send:
 *
 *        ....   <Info Header>          ... (see gTrkMsgInfoHeader)
 *        u32    startIndex             - (4) starting index of returned info
 *                target-defined info (e.g., process ID for
 *                                     kDSOSProcessStateInfo)
 *                                      <4 + header + target info> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u32    returnedCount          - (4) # of items in this reply
 *        u32    totalCount             - (4) total # of items in complete response
 *                target-defined info (e.g., process state for
 *                                     kDSOSProcessStateInfo)
 *                                      <8 + header + target info> total
 *
 *
 *    This is the header to be used by all kDSReadInfo requests.  The
 *    complete info may not fit in a single CodeWarriorTRK message, so it is
 *    possible to read the info in segments.  When reading the info, the
 *    startIndex should be 0 the first time.  If, in the reply,
 *    totalCount > (startIndex + returnedCount), then the next segment of
 *    the info can be read by sending another request with the new
 *    startIndex set to the previous (startIndex + returnedCount).
 *
 *    startIndex, returnedCount, and totalCount all refer to the number of
 *    "items" (the definition of which depends on the info type), not
 *    necessarily the number of bytes.
 *
 */
/****************************************************************************/
#define TRK_MSG_RI_HEADER_LENGTH       (4 + TRK_MSG_INFO_HEADER_LENGTH)    /* 4 addl bytes  */
#define TRK_MSG_RI_HEADER_COUNT        (1 + TRK_MSG_INFO_HEADER_COUNT)     /* 1 addl fields */


DESCDECL TrkMsgDesc gTrkMsgReadInfoHeader
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Read Info Header")
        &gTrkMsgInfoHeader,                            /* previous fragment/header */
        4,                                             /* # of addl bytes */
        1,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("start index", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_RI_HEADER_START_INDEX    (0 + TRK_MSG_INFO_HEADER_COUNT)


#define TRK_MSG_RIR_HEADER_LENGTH        (8 + TRK_MSG_REPLY_HEADER_LENGTH)   /* 4 addl bytes  */
#define TRK_MSG_RIR_HEADER_COUNT         (2 + TRK_MSG_REPLY_HEADER_COUNT)    /* 1 addl fields */

DESCDECL TrkMsgDesc gTrkMsgReadInfoReplyHeader
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Read Info Reply Header")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        8,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("returned count", 4),
            __trk_msg_field("total count", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_RIR_HEADER_RETURNED_INDEX    (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_RIR_HEADER_TOTAL_INDEX       (1 + TRK_MSG_REPLY_HEADER_COUNT)


#if __MWERKS__
    #pragma mark  WriteFile
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSWriteFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) DSFileHandles (see below)
 *        u16    length                 - (2) file data length in bytes
 *               file data              - (0 to 2048)
 *                                      <6 + length + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>      ... (see gTrkMsgHeader)
 *        u32    file_handle           - (4) DSFileHandles (see below)
 *        u16    length                - (2) file data length in bytes
 *        u32    processID             - (4) unique process ID
 *        u32    threadID              - (4) unique thread ID
 *               file data             - (0 to 2048)
 *                                     <14 + length + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>        ... (see gTrkMsgReplyHeader)
 *        u8     io_result             - (1) DSIOResult (see below)
 *        u16    length                - (2) amount that was written in bytes
 *                                     <3 + header> total
 *
 *    kDSWriteFile: Sent by the target.  Instructs the host to write the
 *    specified data to the file with the given handle.  Assumes that stdout
 *    and stderr are always open for writing.
 *    Expects a reply from the host in the above format.
 *
 *    Note: The startup welcome message is sent as a kDSWriteFile
 *  message, but it is a special case and does not require a reply.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_WRITEFILE_LENGTH        (DS_MIN_MSG_LENGTH + 6)
#define DS_MIN_REPLY_WRITEFILE_LENGTH      (DS_MIN_REPLY_LENGTH + 3)

#if __MWERKS__
    #pragma mark  ReadFile
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSReadFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) DSFileHandles (see below)
 *        u16    length                 - (2) file data length in bytes
 *                                      <6 + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) DSFileHandles (see below)
 *        u16    length                 - (2) file data length in bytes
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <14 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u8     io_result              - (1) DSIOResult (see below)
 *        u16    length                 - (2) amount that was read in bytes
 *               file data              - (0 to 2048)
 *                                      <3 + length + header> total
 *
 *    Sent by the target.  Instructs the host to read the specified
 *    amount of data from the file with the given handle.  Assumes
 *    that stdin is always open for reading.
 *    Expects a reply from the host in the above format.
 */
/****************************************************************************/

#define DS_MIN_MSG_READFILE_LENGTH        (DS_MIN_MSG_LENGTH + 6)
#define DS_MIN_REPLY_READFILE_LENGTH      (DS_MIN_REPLY_LENGTH + 3)

#if __MWERKS__
    #pragma mark  NotifyFileInput
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSNotifyFileInput
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) DSFileHandles (see below)
 *                                      <4 + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) DSFileHandles (see below)
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <12 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host.  Notifies CodeWarriorTRK that new input is available for
 *    the given file.  This is intended to be used for stdin, but can be
 *    used for other files as well.
 *
 */
/****************************************************************************/

/*** kDSNotifyFileInput ***/

DESCDECL TrkMsgDesc gTrkMsgNotifyFileInput
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Notify File Input")
        &gTrkMsgHeader,                                /* previous fragment/header */
        4,                                             /* # of addl bytes */
        1,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("file handle", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_NFI_HANDLE_INDEX         (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_NFI_PID_INDEX            (1 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_NFI_TID_INDEX            (2 + TRK_MSG_HEADER_COUNT)


#if __MWERKS__
    #pragma mark  BlockFileIo
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSBlockFileIo
 *
 *    Send:
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8        block               - (1) if 0, OK to send file IO requests, otherwise no
 *                                      <1 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *                                      <header> total
 *
 *    Sent by the host.  Blocks/unblocks all file IO requests from the target.
 *    If the host becomes overwhelmed with file IO processing, it can send this
 *    message with a block value of 1.  When it is able to handle more file IO
 *    requests it can send this message again with a block value of 0.  This
 *    affects target->host IO requests from all files on all processes and threads.
 *
 */
/****************************************************************************/

/*** kDSBlockFileIo ***/

DESCDECL TrkMsgDesc gTrkMsgBlockFileIo
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("Block File IO")
        &gTrkMsgHeader,                                /* previous fragment/header */
        1,                                             /* # of addl bytes */
        1,                                             /* # of addl fields */
        {                                              /* addl field descriptors: */
            __trk_msg_field("block", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;


/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_BFIO_BLOCK_INDEX        (0 + TRK_MSG_HEADER_COUNT)


#if __MWERKS__
    #pragma mark  OpenFile
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOpenFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     modes                  - (1) DSFileOpenModes (see below)
 *        u16    name_length            - (2) file name length in bytes
 *        u8[]   file_name              - (0 to 2048) file name (no string
 *                                                  terminator)
 *                                      <3 + name_length + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     modes                  - (1) DSFileOpenModes (see below)
 *        u16    name_length            - (2) file name length in bytes
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *        u8[]   file_name              - (0 to 2048) file name (no string
 *                                                  terminator)
 *                                      <11 + name_length + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u8     io_result              - (1) DSIOResult (see below)
 *        u32    file_handle            - (4) handle of the newly opened file
 *        u32    timestamp              - (4) timestamp of file on open (replyer-dependent format)
 *                                      <9 + header> total
 *
 *    Sent by the target.  Instructs the host to open a file with the
 *    given name and modes.  The resulting file_handle can then be
 *    used by kDSWriteFile, kDSReadFile, kDSCloseFile, and kDSPositionFile.
 *    Expects a reply from the host in the above format.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_OPENFILE_LENGTH          (DS_MIN_MSG_LENGTH + 3)
#define DS_MIN_REPLY_OPENFILE_LENGTH        (DS_MIN_REPLY_LENGTH + 5)

#if __MWERKS__
    #pragma mark  CloseFile
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSCloseFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) handle of the file to be closed
 *        u32    timestamp              - (4) new timestamp value to be set for the file (receiver-dependent format)
 *                                      <8 + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u32    file_handle            - (4) handle of the file to be closed
 *        u32    timestamp              - (4) new timestamp value to be set for the file (receiver-dependent format)
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <16 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u8     io_result              - (1) DSIOResult (see below)
 *                                      <1 + header> total
 *
 *    Sent by the target.  Instructs the host to close the file with the
 *    specified file_handle.  Note: the timestamp value will only affect
 *    the file if it was opened in write mode.
 *    Expects a reply from the host in the above format.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_CLOSEFILE_LENGTH          (DS_MIN_MSG_LENGTH + 8)
#define DS_MIN_REPLY_CLOSEFILE_LENGTH        (DS_MIN_REPLY_LENGTH + 1)

#if __MWERKS__
    #pragma mark  PositionFile
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSPositionFile
 *
 *    Send: if DS_PROTOCOL < DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     mode                   - (1) DSFilePositionModes (see below)
 *        u32    file_handle            - (4) handle of the file to be positioned
 *        u32    offset                 - (4) position offset
 *                                      <9 + header> total
 *
 *    Send: if DS_PROTOCOL == DS_PROTOCOL_RTOS
 *
 *        ....   <Message Header>       ... (see gTrkMsgHeader)
 *        u8     mode                   - (1) DSFilePositionModes (see below)
 *        u32    file_handle            - (4) handle of the file to be positioned
 *        u32    offset                 - (4) position offset
 *        u32    processID              - (4) unique process ID
 *        u32    threadID               - (4) unique thread ID
 *                                      <17 + header> total
 *
 *    Reply:
 *
 *        ....   <Reply Header>         ... (see gTrkMsgReplyHeader)
 *        u8     io_result              - (1) DSIOResult (see below)
 *                                      <1 + header> total
 *
 *    Sent by the target.  Instructs the host to set the file position
 *    of the specified file.  The new position will be at the specified
 *    offset from one of three reference points in the file, as specified
 *    by the mode.
 *    Expects a reply from the host in the above format.
 *
 */
/****************************************************************************/

#define DS_MIN_MSG_POSITIONFILE_LENGTH      (DS_MIN_MSG_LENGTH + 5)
#define DS_MIN_REPLY_POSITIONFILE_LENGTH    (DS_MIN_REPLY_LENGTH + 1)

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
**    read    write    append    binary    create        fopen mode
**    -------------------------------------------------------
**    yes                                                "r"
**            yes*                         yes           "w"
**            yes*     yes                 yes           "a"
**    yes     yes                                        "r+"
**    yes     yes*                         yes           "w+"
**    yes     yes*     yes                 yes           "a+"
**    yes                        yes                     "rb"
**            yes*               yes       yes           "wb"
**            yes*     yes       yes       yes           "ab"
**    yes     yes                yes                     "r+b"
**    yes     yes*               yes       yes           "w+b"
**    yes     yes*     yes       yes       yes           "a+b"
**
**
** * Note that when the append or create modes are specified,
** the write mode is always assumed.
**
** If the exec mode is specified, the resulting file will
** be created as an executable file (with execute permissions).
** This is only meaningful if the create mode is also specified.
*/

typedef enum DSFileOpenModes
{
    kDSFileOpenRead        = 0x01,
    kDSFileOpenWrite       = 0x02,
    kDSFileOpenAppend      = 0x04,
    kDSFileOpenBinary      = 0x08,
    kDSFileOpenCreate      = 0x10,
    kDSFileOpenExec        = 0x20
} DSFileOpenModes;

/*
** Use one of the following values to specify a file positioning mode.
*/

typedef enum DSFilePositionModes
{
    kDSFileSeekSet         = 0x00,        /* Position is offset from file start */
    kDSFileSeekCur         = 0x01,        /* Position is offset from current pos. */
    kDSFileSeekEnd         = 0x02         /* Position is offset from file end */
} DSFilePositionModes;

#if __MWERKS__
    #pragma mark  OS File Operations
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    kDSOSWriteFile
 *    kDSOSReadFile
 *    kDSOSOpenFile
 *    kDSOSCloseFile
 *    kDSOSPositionFile
 *    kDSOSInstallFile
 *    kDSOSInstallFile2
 *
 *    The format and function of these commands is identical to
 *    that of the kDSWriteFile, kDSReadFile, kDSOpenFile,
 *    kDSCloseFile, and kDSPositionFile commands described above.
 *
 *    The difference is that these commands are sent from the host to
 *    the target, they operate on files on the target system, and they
 *    do not include the processId and threadId fields (i.e., they are
 *    formatted as described above as if DS_PROTOCOL < DS_PROTOCOL_RTOS).
 */
/****************************************************************************/

/*** kDSOSWriteFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsWriteFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Write File")
        &gTrkMsgHeader,                                /* previous fragment/header */
        6,                                             /* # of addl bytes */
        2,                                             /* # of addl fields */
        {                                              /* addl field descriptors (data handled separately): */
            __trk_msg_field("file handle", 4),
            __trk_msg_field("length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OWF_HANDLE_INDEX        (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OWF_LENGTH_INDEX        (1 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsWriteFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Write File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        3,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors (data handled separately): */
            __trk_msg_field("io result", 1),
            __trk_msg_field("length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OWFR_IORESULT_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_OWFR_LENGTH_INDEX          (1 + TRK_MSG_REPLY_HEADER_COUNT)


/*** kDSOSReadFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsReadFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Read File")
        &gTrkMsgHeader,                               /* previous fragment/header */
        6,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("file handle", 4),
        _    _trk_msg_field("length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_ORF_HANDLE_INDEX        (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_ORF_LENGTH_INDEX        (1 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsReadFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Read File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        3,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors (data handled separately): */
            __trk_msg_field("IO result", 1),
            __trk_msg_field("length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_ORFR_IORESULT_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_ORFR_LENGTH_INDEX          (1 + TRK_MSG_REPLY_HEADER_COUNT)


/*** kDSOSOpenFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsOpenFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Open File")
        &gTrkMsgHeader,                               /* previous fragment/header */
        3,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors (file_name handled separately): */
            __trk_msg_field("modes", 1),
            __trk_msg_field("name length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OOF_MODES_INDEX        (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OOF_LENGTH_INDEX       (1 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsOpenFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Open File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        9,                                            /* # of addl bytes */
        3,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("IO result", 1),
            __trk_msg_field("file handle", 4),
            __trk_msg_field("timestamp", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OOFR_IORESULT_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_OOFR_HANDLE_INDEX          (1 + TRK_MSG_REPLY_HEADER_COUNT)
#define TRK_MSG_OOFR_TIMESTAMP_INDEX       (2 + TRK_MSG_REPLY_HEADER_COUNT)


/*** kDSOSCloseFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsCloseFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Close File")
        &gTrkMsgHeader,                               /* previous fragment/header */
        8,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("file handle", 4),
            __trk_msg_field("timestamp", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OCF_HANDLE_INDEX       (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OCF_TIMESTAMP_INDEX    (1 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsCloseFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Close File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        1,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("IO result", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OCFR_IORESULT_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)


/*** kDSOSPositionFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsPositionFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Position File")
        &gTrkMsgHeader,                               /* previous fragment/header */
        9,                                            /* # of addl bytes */
        3,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("modes", 1),
            __trk_msg_field("file handle", 4),
            __trk_msg_field("offset", 4)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OPF_MODES_INDEX     (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OPF_HANDLE_INDEX    (1 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OPF_OFFSET_INDEX    (2 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsPositionFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Position File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        1,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("IO result", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OPFR_IORESULT_INDEX        (0 + TRK_MSG_REPLY_HEADER_COUNT)


/*** kDSOSInstallFile ***/
DESCDECL TrkMsgDesc gTrkMsgOsInstallFile
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Install File")
        &gTrkMsgHeader,                               /* previous fragment/header */
        3,                                            /* # of addl bytes */
        2,                                            /* # of addl fields */
        {                                             /* addl field descriptors (file_name handled separately): */
            __trk_msg_field("drive", 1),
            __trk_msg_field("name length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OIF_DRIVE_INDEX        (0 + TRK_MSG_HEADER_COUNT)
#define TRK_MSG_OIF_LENGTH_INDEX       (1 + TRK_MSG_HEADER_COUNT)


DESCDECL TrkMsgDesc gTrkMsgOsInstallFileReply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Install File Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        1,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("IO result", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/*** kDSOSInstallFile2 ***/
DESCDECL TrkMsgDesc gTrkMsgOsInstallFile2
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Install File2")
        &gTrkMsgHeader,                               /* previous fragment/header */
        2,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors (file_name handled separately): */
        __trk_msg_field("name length", 2)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;

/* "Index" of fields.  Take # of header fields into account. */
#define TRK_MSG_OIF2_LENGTH_INDEX    (0 + TRK_MSG_HEADER_COUNT)

DESCDECL TrkMsgDesc gTrkMsgOsInstallFile2Reply
#ifdef TRK_MSGCMD_INIT
    = {
        __debug_only_parm("OS Install File2 Reply")
        &gTrkMsgReplyHeader,                          /* previous fragment/header */
        1,                                            /* # of addl bytes */
        1,                                            /* # of addl fields */
        {                                             /* addl field descriptors: */
            __trk_msg_field("IO result", 1)
        }
    }
#endif /* #ifdef TRK_MSGCMD_INIT */
;


#if __MWERKS__
    #pragma mark  Message Error Values
#endif /* __MWERKS__ */
/****************************************************************************/
/*
 *    Error values returned in ACK/NAK packets
 */
/****************************************************************************/
typedef enum
{
    kDSReplyNoError                 = 0x00,    /* no error */
    kDSReplyError                   = 0x01,    /* generic error in CWDS message */
    kDSReplyPacketSizeError         = 0x02,    /* unexpected pkt size in send msg */
    kDSReplyCWDSError               = 0x03,    /* internal error occurred in CWDS */
    kDSReplyEscapeError             = 0x04,    /* escape followed by frame flag */
    kDSReplyBadFCS                  = 0x05,    /* bad FCS in packet */
    kDSReplyOverflow                = 0x06,    /* packet too long */
    kDSReplySequenceMissing         = 0x07,    /* sequence ID != expected (gap in sequence) */

    kDSReplyUnsupportedCommandError = 0x10,    /* command not supported */
    kDSReplyParameterError          = 0x11,    /* command param out of range */
    kDSReplyUnsupportedOptionError  = 0x12,    /* an option was not supported */
    kDSReplyInvalidMemoryRange      = 0x13,    /* read/write to invalid memory */
    kDSReplyInvalidRegisterRange    = 0x14,    /* read/write invalid registers */
    kDSReplyCWDSException           = 0x15,    /* exception occurred in CWDS */
    kDSReplyNotStopped              = 0x16,    /* targeted system or thread is running */
    kDSReplyBreakpointsFull         = 0x17,    /* bp resources (HW or SW) exhausted */
    kDSReplyBreakpointConflict      = 0x18,    /* requested bp conflicts w/existing bp */

    kDSReplyOsError                 = 0x20,    /* general OS-related error */
    kDSReplyInvalidProcessId        = 0x21,    /* request specified invalid process */
    kDSReplyInvalidThreadId         = 0x22     /* request specified invalid thread */

} DSReplyError;


#endif /* __MSGCMD_H__ */
