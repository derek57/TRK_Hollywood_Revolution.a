/****************************************************************************/
/*

DESCRIPTION

    CWDS Error Constants

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __DSERROR_H__
#define __DSERROR_H__


#include "trktypes.h"


/****************************************************************************/

typedef s32 DSError;

enum
{
    kNoError = 0,                        /* DSError type */
    kInternalError,
    kUserError,
    kNotFoundError,
    kParameterError,
    kOutOfMemory
};

/****************************************************************************/

/*
    DSError error number ranges.  See these header files for
    constants:

    0x0000-0x00FF    dserror.h
    0x0000-0x00FF    uart.h
    0x0100-0x01FF    nubevent.h
    0x0200-0x02FF    mutex_TRK.h
    0x0300-0x03FF    msgbuf.h
    0x0400-0x04FF    msg.h
    0x0500-0x05FF    dispatch.h
    0x0600-0x06FF    serpoll.h
    0x0700-0x07FF    targimpl.h
    0x0800-0x08FF    notify.h
    0x0900-0x09FF    support.h
    0x0a00-0x0aff    transport_clayer.h
    0x0b00-0x0bff    break.h
*/


#endif /* __DSERROR_H__ */
