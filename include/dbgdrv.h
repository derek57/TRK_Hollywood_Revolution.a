/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __DBGDRV_H__
#define __DBGDRV_H__


#ifdef __GNUC__
    #include <ogc/context.h>
    #include <ogc/irq.h>
#endif

#include "trk_prefix.h"


#if defined(__MWERKS__)
    typedef void (*DBCallback)(s32, void *);

    typedef void DBInitCommType(volatile u8 **inputPendingPtrRef, DBCallback cb);
#elif defined(__GNUC__)
    typedef void DBInitCommType(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb);
#endif

DBInitCommType DBInitComm;

__extern_c

    void DBClose(void);
    void DBOpen(void);
    void DBInitInterrupts(void);
    s32 DBQueryData(void);
    u32 DBRead(void *buffer, u32 data);
    u32 DBWrite(void *buffer, u32 data);

__end_extern_c


#endif // __DBGDRV_H__
