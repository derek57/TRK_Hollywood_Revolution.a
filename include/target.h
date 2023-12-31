/****************************************************************************/
/*

FILE

    target.h

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/mpc_7410/TARGET.H,v 1.4 2006/09/13 08:25:46 r04474 Exp $

DESCRIPTION

    Defines specific to the PPC603e and 7xx Yellowknife X4 target.

HISTORY

    98 Sep 21 SCM   Created.

    $History: target.h $
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 6/08/99    Time: 6:40p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/yellowknife_x4
//    Moved to common version # for all PPC TRKs (defined in ppc_version.h).
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 5/05/99    Time: 3:22p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/yellowknife_x4
//       - Added support for the Freescale Excimer board.
//       - Moved copy_vectors() call so that it occurs before application-state
//         registers are initialized.  Otherwise MSR:IP will be set in the
//         application-state and RAM vectors will not be used.
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 4/20/99    Time: 2:05p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/yellowknife_x4
//       - Created target_asm.h files to contain target-specific defines that
//         can be included by both assembly and C files.
//       - Added support for the 8260 processor and the MPC8260 VADS board.
//       - Updated __ppc_eabi_init.c from the latest runtime sources.
//       - __TRK_copy_vectors is now called from InitializeTarget().
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 4/01/99    Time: 9:43a
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/yellowknife_x4
//    Moved PPC board support to Processor\ppc\board subdirectory.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 1/04/99    Time: 4:21p
//    Created in $/Embedded/CodeWarriorTRK/Board/yellowknife/ppc7xx_603e
//    Files and projects specific to the Freescale Yellowknife board.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2006
*/
/****************************************************************************/
#pragma once


#ifndef __TARGET_H__
#define __TARGET_H__


#include "ppc_version.h"


/****************************************************************************/


#define DS_TARGET_NAME               "Revolution"

#define TRK_BAUD_RATE                kBaud38400

#ifndef DS_PROTOCOL
    #define DS_PROTOCOL                  DS_PROTOCOL_BOARD
#endif

#define DS_CPU_MAJOR_TYPE            DS_CPU_MAJOR_PPC
#define DS_CPU_MINOR_TYPE            TRKTargetCPUMinorType()

/*
** The following string is displayed on the LCD panel.  For
** boards which do not have this ability, don't define
** BOARD_WELCOME_STRING.
*/

#define BOARD_WELCOME_STRING         "MetroTRK for Revolution v0.4"


#include "asm_regs.h"


// MOVED TO ASM_REGS.H FOR COMPATIBILITY __MWERKS__ <-> __GNUC__
/*
#ifndef TRK_TRANSPORT_INT_DRIVEN
    #define TRK_TRANSPORT_INT_DRIVEN     (1)
#endif
*/

/*
** A few defines are needed by assembly code, but the assembler
** cannot handle some of the C-specific things included in this
** file.  Any defines needed by assembly code are put in the
** following assembler-friendly header file.  Those defines
** are also useful for C code, so include them here.
*/

#include "target_asm.h"


/*
** Target-specific includes that define things needed by
** portable code.
*/

#include "m7xx_m603e_reg.h"
#include "ppc_except.h"


#endif // __TARGET_H__
