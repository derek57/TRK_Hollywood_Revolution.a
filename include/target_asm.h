/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/mpc_7410/target_asm.h,v 1.2 2006/07/11 11:10:07 r04474 Exp $

DESCRIPTION

    Assembly-usable defines specific to the Freescale Yelloknife X4 7400 board.

    Note that these defines can also be used by 'C' code.
    In fact, "target.h" typically includes this as a subfile.

HISTORY

    $History: target_asm.h $

    COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2006
*/
/****************************************************************************/
#pragma once


#ifndef __TARGET_ASM_H__
#define __TARGET_ASM_H__


// Note:
//
// It would have been way easier to use and IF/DEF condition
// here but we can't because of CW-Linker's "INABILITY" to
// check for preprocessor defines (as it seems).
//
// What in god's name did Nintendo intend to achieve by using
// this I.D.E. (Integrated Development Environment)...
//
// BTW: The Wii & NDEV both need these MSR bits: MSR:IR | MSR:DR

/*
** When an exception occurs, the following bits are set in the
** MSR before jumping to the main exception handler.  In general,
** if CodeWarriorTRK uses address translation then MSR:IR and MSR:DR
** should be set.  Otherwise, no bits should be set.
*/
#define EXCEPTION_SET_MSR_BITS        (0x30)    // MSR:IR | MSR:DR

/*
** The assembly code needs some of the processor-specific
** defines.  Nothing else is required.
*/


#include "proc_mpc_7xx_603e.h"


#endif // __TARGET_ASM_H__
