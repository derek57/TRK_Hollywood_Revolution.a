/****************************************************************************/
/*

DESCRIPTION

    Event notification from CWDS to host debugger.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __NOTIFY_H__
#define __NOTIFY_H__


#include "msgbuf.h"
#include "trk_prefix.h"


enum
{
    kWaitACKError = 0x0800                /* DSError type */
};

__extern_c

    /*
     *    meta command set
     */

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKWaitForACK(void);
#endif

    DSError TRKDoNotifyStopped(MessageCommandID command);

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    DSError TRKDoNotify(MessageBuffer *buffer);
#endif

__end_extern_c


#endif /* __NOTIFY_H__ */
