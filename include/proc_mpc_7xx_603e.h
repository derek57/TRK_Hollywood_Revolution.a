//##########################################################################
//
//    FILE
//
//    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/proc_mpc_7xx_603e.h,v 1.2 2006/07/11 10:06:33 r04474 Exp $
//
//    DESCRIPTION
//
//    Header file containing macros and definitions specific to the
//    Freescale MPC7xx and MPC603e processor models.
//
//
//    COPYRIGHT
//
//    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
//    All other product or service names are the property of their respective owners.
//    (C) Freescale Semiconductor, Inc. 2000-2006
//
//    HISTORY
//
//    $History: proc_mpc_7xx_603e.h $
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 6/08/99    Time: 9:20p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//    Added attribute for processors with no instruction cache.
//    Added attribute for up to 4 extra instruction-skip exceptions.
//    Added attribute for implementation of MSR:TGPR bit.
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 4/20/99    Time: 2:05p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//       - Created target_asm.h files to contain target-specific defines that
//         can be included by both assembly and C files.
//       - Added support for the 8260 processor and the MPC8260 VADS board.
//       - Updated __ppc_eabi_init.c from the latest runtime sources.
//       - __copy_vectors is now called from InitializeTarget().
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 4/01/99    Time: 9:17a
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//    Merged common code for the different models and moved model-specific
//    code to separate files.
//
//
//
//
//###########################################################################
//
//
//###########################################################################
#pragma once


#ifndef __PROC_MPC_7XX_603E_H__
#define __PROC_MPC_7XX_603E_H__


/*
** Define __PPC_FPU__ to 1 if the processor has an FPU.
*/

#ifndef __PPC_FPU__
    #define __PPC_FPU__                  (1) /* set to 1 if FPU present */
#endif

/*
** Define __PPC_FPU_FPECR__ to 1 if the processor has the
** FPECR register.  Note that if __PPC_FPU__ is 0, then
** the value of __PPC_FPU_FPECR__ is ignored.
*/

#define __PPC_FPU_FPECR__            (0) /* set to 1 if FPECR register present */

/*
** Define __PPC_DCACHE__ to 1 if the processor has a data cache.
*/
#ifndef __PPC_DCACHE__
    #define __PPC_DCACHE__               (1) /* set to 1 if data cache present */
#endif
/*
** Define __PPC_ICACHE__ to 1 if the processor has an instruction cache.
*/
#ifndef __PPC_ICACHE__
    #define __PPC_ICACHE__               (1) /* set to 1 if inst. cache present */
#endif
/*
** Define __PPC_BOOT_CONFIG__ to 1 if the processor gets boot
** configuration data from the first 0x100 bytes of ROM.
*/

#define __PPC_BOOT_CONFIG__          (0) /* set to 1 boot config in ROM 0..0x100 */


/*
** When CodeWarriorTRK gets an internal exception, it "recovers" by
** restarting execution at the next instruction.  Some exception
** types will return to the same instruction that caused the
** exception.  For those types, the instruction must be skipped
** in order for the restart to work.  By default, CodeWarriorTRK
** does the instruction skip for exception ID's 0x200, 0x300,
** 0x400, 0x600, 0x700, and 0x800.  Additional ID's which
** require instruction skip for this processor model should
** be specified below.  If additional ID's are not required,
** leave these macros undefined.
**
** Note: The exception ID is the offset of the corresponding
** handler for that exception in the handler table.
*/

#define PPC_INSTR_SKIP_EXCEPTION1    (0x1000)
#define PPC_INSTR_SKIP_EXCEPTION2    (0x1100)
#define PPC_INSTR_SKIP_EXCEPTION3    (0x1200)
#define PPC_INSTR_SKIP_EXCEPTION4    (0x1300)

/*
** Define __PPC_TGPR__ to 1 if the processor implements TGPR in
** bit 14 of the MSR.
**
** Note: It is safe to set this to one if bit 14 of the MSR is
** a reserved bit.  It is not safe if bit 14 of the MSR implements
** some function other than TGPR.
*/
#ifndef __PPC_TGPR__
    #define __PPC_TGPR__                 (1)
#endif


#endif // __PROC_MPC_7XX_603E_H__
