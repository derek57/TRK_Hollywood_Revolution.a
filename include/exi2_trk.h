/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __EXI2_TRK_H__
#define __EXI2_TRK_H__


#ifdef __GNUC__
    #include <ogc/context.h>
#endif

#include "trktypes.h"
#include "trk_prefix.h"


// EXI callback function pointer type
#if defined(__MWERKS__)
    typedef void (*EXICB)(void *context);
#elif defined(__GNUC__)
    typedef void (*EXICB)(frame_context *context);
#endif


// EXI error codes
typedef enum
{
    EXI_NO_ERROR    = 0,
    EXI_UNSELECTED
} ExiError;

__extern_c

#if defined(__MWERKS__)
    void EXI2_Init(volatile u8 **inputPendingPtrRef, EXICB monitorCallback);
#elif defined(__GNUC__)
    void EXI2_Init(volatile u8 **inputPendingPtrRef, raw_irq_handler_t monitorCallback);
#endif

    void EXI2_EnableInterrupts(void);
    s32 EXI2_Poll(void);
    s32 EXI2_ReadN(void *bytes, u32 length);
    u32 EXI2_WriteN(const void *bytes, u32 length);
    void EXI2_Reserve(void);
    void EXI2_Unreserve(void);
    BOOL IsStub(void);
    u32 __EXISwap32(u32 swap);

__end_extern_c


#endif  // __EXI2_TRK_H__
