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

#ifndef __UDP_STUBS_H__
#define __UDP_STUBS_H__


#include "trktypes.h"
#include "dserror.h"

#if defined(__MWERKS__)
    #include "dbgdrv.h"
#elif defined(__GNUC__)
    #include <ogc/lwp_threads.h>
#endif


/****************************************************************************/

typedef s32 UDPError;

enum
{
    kUDPDataError = -1,
    kUDPNoError = 0
};

/****************************************************************************/

__extern_c

#if defined(__MWERKS__)
    __declspec(weak) UDPError udp_cc_initialize(volatile u8 **inputPendingPtrRef, DBCallback exicb);
    __declspec(weak) UDPError udp_cc_shutdown(void);
    __declspec(weak) UDPError udp_cc_peek(void);
    __declspec(weak) UDPError udp_cc_read(u8 *buffer, s32 data);
    __declspec(weak) UDPError udp_cc_write(u8 *buffer, s32 data);
    __declspec(weak) UDPError udp_cc_open(void);
    __declspec(weak) UDPError udp_cc_close(void);
    __declspec(weak) UDPError udp_cc_pre_continue(void);
    __declspec(weak) UDPError udp_cc_post_stop(void);
#elif defined(__GNUC__)
    __attribute__((weak)) UDPError udp_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb);
    __attribute__((weak)) UDPError udp_cc_shutdown(void);
    __attribute__((weak)) UDPError udp_cc_peek(void);
    __attribute__((weak)) UDPError udp_cc_read(u8 *buffer, s32 data);
    __attribute__((weak)) UDPError udp_cc_write(u8 *buffer, s32 data);
    __attribute__((weak)) UDPError udp_cc_open(void);
    __attribute__((weak)) UDPError udp_cc_close(void);
    __attribute__((weak)) UDPError udp_cc_pre_continue(void);
    __attribute__((weak)) UDPError udp_cc_post_stop(void);
#endif

__end_extern_c

#endif
