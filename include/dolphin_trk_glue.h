/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __DOLPHIN_TRK_GLUE_H__
#define __DOLPHIN_TRK_GLUE_H__


#include "trktypes.h"
#include "UART.h"
#include "trk_prefix.h"


__extern_c

    ASM_PREFIX void TRKLoadContext(void *, u32);
    void TRKEXICallBack(volatile u8 **inputPendingPtrRef, void *context);
    UARTError InitMetroTRKCommTable(s32 reply);
    void EnableEXI2Interrupts(void);
    void ReserveEXI2Port(void);
    void UnreserveEXI2Port(void);
    void InitializeProgramEndTrap(void);

__end_extern_c


#endif // __DOLPHIN_TRK_GLUE_H__
