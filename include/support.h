/****************************************************************************/
/*

DESCRIPTION

    Support commands from TRK to host debugger.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __SUPPORT_H__
#define __SUPPORT_H__


#include "trktypes.h"
#include "trk_prefix.h"


/****************************************************************************/
/*
 *    Globals
 */
/****************************************************************************/
extern BOOL gRequestCached;


/****************************************************************************/
/*
 *    Public Function Prototypes
 */
/****************************************************************************/

__extern_c

    DSError TRK_SuppAccessFile(DSFileHandle file_handle, u8 *data, s32 *count,
                           DSIOResult *io_result, BOOL need_reply,
                           BOOL read);

    DSError TRK_RequestSend(MessageBuffer *buffer, MessageBufferId *replyID
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
                            , size_t min_size, s32 max_retries, BOOL check_timeout
#endif
                            );

    u8 GetNextSendSeqId();
    void ResetSendSeqId(u8 val);

    DSError HandlePositionFileSupportRequest(DSFileHandle handle, s32 *length, u8 data, DSIOResult *io_result);
    DSError HandleOpenFileSupportRequest(DSFileHandle handle, u8 length, u32 *options, DSIOResult *io_result);
    DSError HandleCloseFileSupportRequest(DSFileHandle handle, DSIOResult *io_result);

__end_extern_c


#endif /* ndef __SUPPORT_H__ */
