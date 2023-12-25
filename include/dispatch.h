/****************************************************************************/
/*

DESCRIPTION

    Command dispatcher

    OverrideDispatch
        note: if DispatchEntry* returnedOldEntry is NULL, the old entry is lost

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __DISPATCH_H__
#define __DISPATCH_H__


#include "msgbuf.h"

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    #include "dserror.h"
    #include "target.h"
    #include "msgcmd.h"
#endif


enum
{
    kDispatchErr = 0x500        /* generic dispatcher error */
};

#ifdef VIRTUAL_DISPATCH

    typedef DSError (*DispatchFunctionPtr)(MessageBuffer *b);
        /*
         *    DispatchFunctionPtr       type of function pointer
         *    DSError                   return value's type
         *    MessageBuffer             buffer pointer for the message
         */

    typedef struct DispatchEntry
    {
        DispatchFunctionPtr        fn;
    } DispatchEntry;

    extern u32              gDispatchTableSize;
    extern DispatchEntry    gDispatchTable[];

#endif /* VIRTUAL_DISPATCH */

/****************************************************************************/

__extern_c

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKInitializeDispatcher(void);
#endif

#ifdef VIRTUAL_DISPATCH
    DSError OverrideDispatch(MessageCommandID, const DispatchEntry *newEntry,
                                 DispatchEntry *returnedOldEntry);
#endif

    DSError TRK_DispatchMessage(MessageBuffer *);

__end_extern_c


#endif /* ndef __DISPATCH_H__ */
