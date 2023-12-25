/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __CC_GDEV_H__
#define __CC_GDEV_H__


#ifdef __GNUC__
    #include <ogc/irq.h>
#endif

#include "dbgdrv.h"
#include "trk_prefix.h"
#include "UART.h"


/****************************************************************************/

typedef s32 GDEVError;

enum
{
    kGDEVDataError = -1,
    kGDEVNoError = 0
};

/****************************************************************************/

__extern_c

    void OutputData(u8 *data, s32 length);

#if defined(__MWERKS__)
    GDEVError gdev_cc_initialize(volatile u8 **inputPendingPtrRef, DBCallback cb);
#elif defined(__GNUC__)
    GDEVError gdev_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb);
#endif

    GDEVError gdev_cc_initinterrupts(void);
    GDEVError gdev_cc_shutdown(void);
    GDEVError gdev_cc_peek(void);
    GDEVError gdev_cc_read(u8 *buffer, s32 data);
    GDEVError gdev_cc_write(u8 *buffer, s32 data);
    GDEVError gdev_cc_open(void);
    GDEVError gdev_cc_close(void);
    GDEVError gdev_cc_pre_continue(void);
    GDEVError gdev_cc_post_stop(void);

__end_extern_c


#endif // __CC_GDEV_H__
