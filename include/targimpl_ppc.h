/****************************************************************************/
/*

DESCRIPTION

    PPC-specific implementation function prototypes and structues.

    Functions may only work for 32-bit (or smaller) processors.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009

*/
/****************************************************************************/
#pragma once


#ifndef __TARGIMPL_PPC_H__
#define __TARGIMPL_PPC_H__


#include "trktypes.h"
#include "trk_prefix.h"
#include "targimpl.h"


/****************************************************************************/
/*
 *    Function prototypes
 */
/****************************************************************************/

/****************************************************************************/
/*
 *    TRK processor state (prior to entering user code)
 */
/****************************************************************************/

typedef struct TRKState_PPC
{
    DefaultType      GPR[32];
    DefaultType      LR;
    DefaultType      CTR;
    DefaultType      XER;
    Extended1Type    MSR;
#ifdef _E500
    Extended1Type    DBCR0;
    Extended1Type    DBSR;
#else
    Extended1Type    DAR;
    Extended1Type    DSISR;
#endif

    BOOL             stopped;

#if TRK_TRANSPORT_INT_DRIVEN

    // CHANGED BOOL
    BOOL             inputActivated;

    volatile u8      *inputPendingPtr;
#endif /* TRK_TRANSPORT_INT_DRIVEN */
} TRKState_PPC;

// TRK processor state (prior to entering user code)
extern TRKState_PPC  gTRKState;

/****************************************************************************/
/*
 *    Target program's processor state (exception state)
 */
/****************************************************************************/

// Target program's processor state (exception state)
extern ProcessorState_PPC            gTRKCPUState;

/*
** Some registers are not normally restored to their pre-exception states
** by the TRK.  Typically all timer/counter registers fall into this
** category, since they will continue to change as the TRK runs.  They
** are only restored if they have been explicitly modified by the
** debugger.
*/

typedef struct ProcessorRestoreFlags_PPC
{
    u8 TBR;
    u8 DEC;
    u8 linker_padding[7];      // The linker doesn't like anything smaller!

} ProcessorRestoreFlags_PPC;

extern ProcessorRestoreFlags_PPC gTRKRestoreFlags;

/*
** Separate storage for processor state used by transport
** interrupt handler.
*/
__extern_c

    ASM_PREFIX u32 __TRK_get_MSR(void);
    ASM_PREFIX void __TRK_set_MSR(register u32 val);

    ASM_PREFIX u32 __TRK_get_PVR(void);
    ASM_PREFIX u32 __TRK_get_IBAT0U(void);
    ASM_PREFIX u32 __TRK_get_IBAT0L(void);
    ASM_PREFIX u32 __TRK_get_IBAT1U(void);
    ASM_PREFIX u32 __TRK_get_IBAT1L(void);
    ASM_PREFIX u32 __TRK_get_IBAT2U(void);
    ASM_PREFIX u32 __TRK_get_IBAT2L(void);
    ASM_PREFIX u32 __TRK_get_IBAT3U(void);
    ASM_PREFIX u32 __TRK_get_IBAT3L(void);
    ASM_PREFIX u32 __TRK_get_DBAT0U(void);
    ASM_PREFIX u32 __TRK_get_DBAT0L(void);
    ASM_PREFIX u32 __TRK_get_DBAT1U(void);
    ASM_PREFIX u32 __TRK_get_DBAT1L(void);
    ASM_PREFIX u32 __TRK_get_DBAT2U(void);
    ASM_PREFIX u32 __TRK_get_DBAT2L(void);
    ASM_PREFIX u32 __TRK_get_DBAT3U(void);
    ASM_PREFIX u32 __TRK_get_DBAT3L(void);

    u32 __get_DBCR0();

    DSError TRKTargetVersions(DSVersions *versions);
    DSError TRKTargetCheckException(void);
    ASM_PREFIX s32 ReadFPSCR();
    ASM_PREFIX void WriteFPSCR();
    void SetInputState(u32 mode);

    s32 TRKGetInTRKFlag(void);

    DSError TRKPPCAccessSPR(void *value, u32 spr_register_num, BOOL read);
    void TRKSwapAndGo();

    extern void flush_cache(u32 address, u32 size);

    DSError TRKTargetReadInstruction(InstructionType *readData, void *virtualAddress);
    DSError TargetWriteInstruction(InstructionType *data, void *virtualAddress);

__end_extern_c


#endif /* __TARGIMPL_PPC_H__ */
