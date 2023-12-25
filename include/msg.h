/****************************************************************************/
/*

DESCRIPTION

    CWDS messages.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MSG_H__
#define __MSG_H__


/*
 *    Portable includes
 */
#include "trktypes.h"
#include "msgbuf.h"


enum
{                                /* DSError; see dserror.h */
    kMessageOverflow = 0x0400,
    kMessageSizeError,
    kUnsupportedOptionError
};


/*
 *    Function prototypes
 */

__extern_c

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKMessageAdd(MessageBuffer *buffer, u32 data);
#endif

    #define MessageAddBlock       TRK_AppendBuffer

    #define MessageAdd_ui8        TRKAppendBuffer1_ui8
    #define MessageAdd_ui16       TRKAppendBuffer1_ui16
    #define MessageAdd_ui32       TRKAppendBuffer1_ui32
    #define MessageAdd_ui64       TRKAppendBuffer1_ui64
    #define MessageAdd_ui128      TRKAppendBuffer1_ui128

    #define MessageAddBlock_ui8   TRKAppendBuffer_ui8
    #define MessageAddBlock_ui16  TRKAppendBuffer_ui16
    #define MessageAddBlock_ui32  TRKAppendBuffer_ui32
    #define MessageAddBlock_ui64  TRKAppendBuffer_ui64
    #define MessageAddBlock_ui128 TRKAppendBuffer_ui128

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKMessageGet(MessageBuffer *buffer, u32 *data);
#endif

    #define MessageGetBlock       TRK_ReadBuffer

    #define MessageGet_ui8        TRKReadBuffer1_ui8
    #define MessageGet_ui16       TRKReadBuffer1_ui16
    #define MessageGet_ui32       TRKReadBuffer1_ui32
    #define MessageGet_ui64       TRKReadBuffer1_ui64
    #define MessageGet_ui128      TRKReadBuffer1_ui128

    #define MessageGetBlock_ui8   TRKReadBuffer_ui8
    #define MessageGetBlock_ui16  TRKReadBuffer_ui16
    #define MessageGetBlock_ui32  TRKReadBuffer_ui32
    #define MessageGetBlock_ui64  TRKReadBuffer_ui64
    #define MessageGetBlock_ui128 TRKReadBuffer_ui128

    DSError TRK_MessageSend(MessageBuffer *buffer);

__end_extern_c


#endif /* __MSG_H__ */
