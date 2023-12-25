/****************************************************************************/
/*

DESCRIPTION



COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __SERPOLL_H__
#define __SERPOLL_H__


#include "trktypes.h"
#include "trk_prefix.h"


#define DS_SERIAL_POLLING       1
#define DS_SERIAL_INTERRUPT     2

#if TRK_TRANSPORT_INT_DRIVEN
    #define DS_SERIAL   DS_SERIAL_INTERRUPT

    extern volatile u8 *gTRKInputPendingPtr;
#else
    #define DS_SERIAL   DS_SERIAL_POLLING
#endif /* TRK_TRANSPORT_INT_DRIVEN */


/****************************************************************************/

typedef u8 FCSType;     /* 8-bit type */

/****************************************************************************/

__extern_c

    void TRKGetInput(void);
    void TRKProcessInput(MessageBufferId Id);
    DSError TRKInitializeSerialHandler(void);
    DSError TRKTerminateSerialHandler(void);
    MessageBufferId TRKTestForPacket(void);

__end_extern_c

#endif // __SERPOLL_H__
