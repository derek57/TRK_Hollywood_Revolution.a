/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __CC_DDH_H__
#define __CC_DDH_H__


#ifdef __GNUC__
    #include <ogc/irq.h>
#endif

#include "dbgdrv.h"
#include "exi2_trk.h"
#include "trk_prefix.h"
#include "UART.h"


/****************************************************************************/

typedef s32 DDHError;

enum
{
    kDDHDataError = -1,
    kDDHNoError = 0
};

/****************************************************************************/

__extern_c

#if defined(__MWERKS__)
    DDHError ddh_cc_initialize(volatile u8 **inputPendingPtrRef, EXICB cb);
#elif defined(__GNUC__)
    DDHError ddh_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb);
#endif

    DDHError ddh_cc_initinterrupts(void);
    DDHError ddh_cc_shutdown(void);
    DDHError ddh_cc_peek(void);
    DDHError ddh_cc_read(u8 *buffer, u32 length);
    DDHError ddh_cc_write(u8 *buffer, s32 length);
    DDHError ddh_cc_open(void);
    DDHError ddh_cc_close(void);
    DDHError ddh_cc_pre_continue(void);
    DDHError ddh_cc_post_stop(void);

__end_extern_c


#endif // __CC_DDH_H__
