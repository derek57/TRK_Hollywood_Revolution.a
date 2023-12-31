/****************************************************************************/
/*

FILE

    mpc_7xx_603e.c

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/mpc_7xx_603e.c,v 1.4 2006/08/25 14:00:08 r04474 Exp $

DESCRIPTION

    Target-specific implementation for PowerPC MPC7xx, MPC603e and MPC7400 (Max)
    models.

HISTORY

    $History: mpc_7xx_603e.c $
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 5/05/99    Time: 3:22p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//       - Added support for the Freescale Excimer board.
//       - Moved copy_vectors() call so that it occurs before application-state
//         registers are initialized.  Otherwise MSR:IP will be set in the
//         application-state and RAM vectors will not be used.
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

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2006

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "targimpl_ppc.h"


// MOVED TO MPC_7XX_603E_ASM.S FOR GCC
#ifdef __MWERKS__

extern ProcessorState_PPC gTRKCPUState;


/****************************************************************************/
/*
 *    TRKSaveExtended1Block
 *
 *    Saves the portion of the target program state that is in the
 *    Extended1 block.  Exception: The exceptionID field has already
 *    been saved when this routine is called.
 *
 *    Note: Exceptions are disabled when this function is called.
 *    Also, it is OK to change any GPR value.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
ASM_PREFIX void TRKSaveExtended1Block(void)
{
    nofralloc

    // Load CPUState address.

    lis       r2, gTRKCPUState@h   // base pointer to CPU state
    ori       r2, r2, gTRKCPUState@l

    // EXTENDED BLOCK 1    (Blockwise save)

    mfsr      r16, 0               // Segment Register 0
    mfsr      r17, 1               // Segment Register 1
    mfsr      r18, 2               // Segment Register 2
    mfsr      r19, 3               // Segment Register 3
    mfsr      r20, 4               // Segment Register 4
    mfsr      r21, 5               // Segment Register 5
    mfsr      r22, 6               // Segment Register 6
    mfsr      r23, 7               // Segment Register 7
    mfsr      r24, 8               // Segment Register 8
    mfsr      r25, 9               // Segment Register 9
    mfsr      r26, 10              // Segment Register 10
    mfsr      r27, 11              // Segment Register 11
    mfsr      r28, 12              // Segment Register 12
    mfsr      r29, 13              // Segment Register 13
    mfsr      r30, 14              // Segment Register 14
    mfsr      r31, 15              // Segment Register 15
    stmw      r16, ProcessorState_PPC.Extended1.SR[0](r2) // store Segment registers

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    mftb      r10, TBR_TBL_READ    // Time Base Read Register (lower)
    mftb      r11, TBR_TBU_READ    // Time Base Read Register (upper)
    mfspr     r12, SPR_HID0        // Hardware Implementation-Dependent Register 0
    mfspr     r13, SPR_HID1        // Hardware Implementation-Dependent Register 1

    // get pre-exception MSR

    mfspr     r14, SPR_SRR1        // Save and Restore Register 1

    mfspr     r15, SPR_PVR         // Processor Version Register
    mfspr     r16, SPR_IBAT0U      // Instruction Block-Address Translation Register 0 (upper)
    mfspr     r17, SPR_IBAT0L      // Instruction Block-Address Translation Register 0 (lower)
    mfspr     r18, SPR_IBAT1U      // Instruction Block-Address Translation Register 1 (upper)
    mfspr     r19, SPR_IBAT1L      // Instruction Block-Address Translation Register 1 (lower)
    mfspr     r20, SPR_IBAT2U      // Instruction Block-Address Translation Register 2 (upper)
    mfspr     r21, SPR_IBAT2L      // Instruction Block-Address Translation Register 2 (lower)
    mfspr     r22, SPR_IBAT3U      // Instruction Block-Address Translation Register 3 (upper)
    mfspr     r23, SPR_IBAT3L      // Instruction Block-Address Translation Register 3 (lower)
    mfspr     r24, SPR_DBAT0U      // Data Block-Address Translation Register 0 (upper)
    mfspr     r25, SPR_DBAT0L      // Data Block-Address Translation Register 0 (lower)
    mfspr     r26, SPR_DBAT1U      // Data Block-Address Translation Register 1 (upper)
    mfspr     r27, SPR_DBAT1L      // Data Block-Address Translation Register 1 (lower)
    mfspr     r28, SPR_DBAT2U      // Data Block-Address Translation Register 2 (upper)
    mfspr     r29, SPR_DBAT2L      // Data Block-Address Translation Register 2 (lower)
    mfspr     r30, SPR_DBAT3U      // Data Block-Address Translation Register 3 (upper)
    mfspr     r31, SPR_DBAT3L      // Data Block-Address Translation Register 3 (lower)
    stmw      r10, ProcessorState_PPC.Extended1.TBL(r2)

    /*
    ** Skip search-table regs - they are 603e-only registers.
    */

    mfspr     r22, SPR_SDR1        // Sample Data Register 1
    mfspr     r23, SPR_DAR         // Data Address Register
    mfspr     r24, SPR_DSISR       // Data Storage Interrupt Status Register
    mfspr     r25, SPR_SPRG0       // Special Purpose Register General 0
    mfspr     r26, SPR_SPRG1       // Special Purpose Register General 1
    mfspr     r27, SPR_SPRG2       // Special Purpose Register General 2
    mfspr     r28, SPR_SPRG3       // Special Purpose Register General 3

    /* skip DEC for now - it is 603e-only */

    li        r29, 0               // Decrement Register

    mfspr     r30, SPR_IABR        // Instruction Address Breakpoint Register
    mfspr     r31, SPR_EAR         // External Address Register
    stmw      r22, ProcessorState_PPC.Extended1.SDR1(r2)

    mfspr     r20, SPR_GQR0        // Quantization Register 0
    mfspr     r21, SPR_GQR1        // Quantization Register 1
    mfspr     r22, SPR_GQR2        // Quantization Register 2
    mfspr     r23, SPR_GQR3        // Quantization Register 3
    mfspr     r24, SPR_GQR4        // Quantization Register 4
    mfspr     r25, SPR_GQR5        // Quantization Register 5
    mfspr     r26, SPR_GQR6        // Quantization Register 6
    mfspr     r27, SPR_GQR7        // Quantization Register 7
    mfspr     r28, SPR_HID2        // Hardware Implementation-Dependent Register 2
    mfspr     r29, SPR_WPAR        // Write Gather Pipe Register
    mfspr     r30, SPR_DMAU        // Direct Memory Access Register (upper)
    mfspr     r31, SPR_DMAL        // Direct Memory Access Register (lower)
    stmw      r20, ProcessorState_PPC.Extended1.GQR(r2)

    b         __7xx_specific_save

    /* PPC7400's additional SPRs */

    mfspr     r16, SPR_UMMCR2      // User Monitor Mode Control Register 2
    mfspr     r17, SPR_UBAMR       // User Breakpoint Address Mask Register
    mfspr     r18, SPR_UMMCR0      // User Monitor Mode Control Register 0
    mfspr     r19, SPR_UPMC1       // User Performance Monitor Counter Register 1
    mfspr     r20, SPR_UPMC2       // User Performance Monitor Counter Register 2
    mfspr     r21, SPR_USIA        // User Sampled Instruction Address Register
    mfspr     r22, SPR_UMMCR1      // User Monitor Mode Control Register 1
    mfspr     r23, SPR_UPMC3       // User Performance Monitor Counter Register 3
    mfspr     r24, SPR_UPMC4       // User Performance Monitor Counter Register 4
    mfspr     r25, SPR_USDA        // User Sampled Data Address Register
    mfspr     r26, SPR_MMCR2       // Monitor Mode Control Register 2
    mfspr     r27, SPR_BAMR        // Breakpoint Address Mask Register
    mfspr     r28, SPR_SDA         // Sampled Data Address Register
    mfspr     r29, SPR_MSSCR0      // Memory Subsystem Control Register 0
    mfspr     r30, SPR_MSSCR1      // Memory Subsystem Control Register 1
    mfspr     r31, SPR_PIR         // Processor ID Register
    stmw      r16, ProcessorState_PPC.Extended1.UMMCR2(r2)

__7xx_specific_save:

    mfspr     r19, SPR_DABR        // Data Address Breakpoint Register
    mfspr     r20, SPR_PMC1        // Performance Monitor Counter Register 1
    mfspr     r21, SPR_PMC2        // Performance Monitor Counter Register 2
    mfspr     r22, SPR_PMC3        // Performance Monitor Counter Register 3
    mfspr     r23, SPR_PMC4        // Performance Monitor Counter Register 4
    mfspr     r24, SPR_SIA         // Sampled Instruction Address Register
    mfspr     r25, SPR_MMCR0       // Monitor Mode Control Register 0
    mfspr     r26, SPR_MMCR1       // Monitor Mode Control Register 1
    mfspr     r27, SPR_THRM1       // Thermal Assist Unit Register 1
    mfspr     r28, SPR_THRM2       // Thermal Assist Unit Register 2
    mfspr     r29, SPR_THRM3       // Thermal Assist Unit Register 3
    mfspr     r30, SPR_ICTC        // Instruction Cache Throttling Control Register
    mfspr     r31, SPR_L2CR        // Level 2 Cache Control Register
    stmw      r19, ProcessorState_PPC.Extended1.DABR(r2)

    blr

    mfspr     r25, SPR_DMISS       // Data TLB Miss Address Register
    mfspr     r26, SPR_DCMP        // Data TLB Compare Register
    mfspr     r27, SPR_HASH1       // Primary Hash Address Register
    mfspr     r28, SPR_HASH2       // Secondary Hash Address Register
    mfspr     r29, SPR_IMISS       // Instruction TLB Miss Register
    mfspr     r30, SPR_ICMP        // Instruction TLB Compare Register
    mfspr     r31, SPR_RPA         // Required Physical Address Register
    stmw      r25, ProcessorState_PPC.Extended1.DMISS(r2)

    mfdec     r31                  // Decrement Register
    stw       r31, ProcessorState_PPC.Extended1.DEC(r2)

#else

    mftb      r27, TBR_TBL_READ    // Time Base Read Lower Register
    mftb      r28, TBR_TBU_READ    // Time Base Read Upper Register
    mfspr     r29, SPR_HID0        // Hardware Implementation-Dependent Register 0
    mfspr     r30, SPR_HID1        // Hardware Implementation-Dependent Register 1
    mfspr     r31, SPR_SRR1        // Save and Restore Register 1
    stmw      r27, ProcessorState_PPC.Extended1.TBL(r2)

    mfspr     r15, SPR_PVR         // Processor Version Register
    mfspr     r16, SPR_IBAT0U      // Instruction Block-Address Translation Register 0 (upper)
    mfspr     r17, SPR_IBAT0L      // Instruction Block-Address Translation Register 0 (lower)
    mfspr     r18, SPR_IBAT1U      // Instruction Block-Address Translation Register 1 (upper)
    mfspr     r19, SPR_IBAT1L      // Instruction Block-Address Translation Register 1 (lower)
    mfspr     r20, SPR_IBAT2U      // Instruction Block-Address Translation Register 2 (upper)
    mfspr     r21, SPR_IBAT2L      // Instruction Block-Address Translation Register 2 (lower)
    mfspr     r22, SPR_IBAT3U      // Instruction Block-Address Translation Register 3 (upper)
    mfspr     r23, SPR_IBAT3L      // Instruction Block-Address Translation Register 3 (lower)
    mfspr     r24, SPR_DBAT0U      // Data Block-Address Translation Register 0 (upper)
    mfspr     r25, SPR_DBAT0L      // Data Block-Address Translation Register 0 (lower)
    mfspr     r26, SPR_DBAT1U      // Data Block-Address Translation Register 1 (upper)
    mfspr     r27, SPR_DBAT1L      // Data Block-Address Translation Register 1 (lower)
    mfspr     r28, SPR_DBAT2U      // Data Block-Address Translation Register 2 (upper)
    mfspr     r29, SPR_DBAT2L      // Data Block-Address Translation Register 2 (lower)
    mfspr     r30, SPR_DBAT3U      // Data Block-Address Translation Register 3 (upper)
    mfspr     r31, SPR_DBAT3L      // Data Block-Address Translation Register 3 (lower)
    stmw      r15, ProcessorState_PPC.Extended1.PVR(r2)

    mfspr     r24, SPR_IBAT4U      // Instruction Block-Address Translation Register 4 (upper)
    mfspr     r25, SPR_IBAT4L      // Instruction Block-Address Translation Register 4 (lower)
    mfspr     r26, SPR_IBAT5U      // Instruction Block-Address Translation Register 5 (upper)
    mfspr     r27, SPR_IBAT5L      // Instruction Block-Address Translation Register 5 (lower)
    mfspr     r28, SPR_IBAT6U      // Instruction Block-Address Translation Register 6 (upper)
    mfspr     r29, SPR_IBAT6L      // Instruction Block-Address Translation Register 6 (lower)
    mfspr     r30, SPR_IBAT7U      // Instruction Block-Address Translation Register 7 (upper)
    mfspr     r31, SPR_IBAT7L      // Instruction Block-Address Translation Register 7 (lower)
    stmw      r24, ProcessorState_PPC.Extended1.IBAT4U(r2)

    /*
    ** Skip search-table regs - they are 603e-only registers.
    */

    mfspr     r22, SPR_SDR1        // Sample Data Register 1
    mfspr     r23, SPR_DAR         // Data Address Register
    mfspr     r24, SPR_DSISR       // Data Storage Interrupt Status Register
    mfspr     r25, SPR_SPRG0       // Special Purpose Register General 0
    mfspr     r26, SPR_SPRG1       // Special Purpose Register General 1
    mfspr     r27, SPR_SPRG2       // Special Purpose Register General 2
    mfspr     r28, SPR_SPRG3       // Special Purpose Register General 3
    mfspr     r29, SPR_DEC         // Decrement Register
    mfspr     r30, SPR_IABR        // Instruction Address Breakpoint Register
    mfspr     r31, SPR_EAR         // External Address Register
    stmw      r22, ProcessorState_PPC.Extended1.SDR1(r2)

    /*
    ** Handle model-specific registers here.  Check the value of
    ** the PVR.  If it indicates 7xx, get the 7xx-specific registers.
    ** Otherwise get the 603e-specific registers.
    */

    mfspr     r24, SPR_DABR        // Data Address Breakpoint Register
    mfspr     r25, SPR_PMC1        // Performance Monitor Counter Register 1
    mfspr     r26, SPR_PMC2        // Performance Monitor Counter Register 2
    mfspr     r27, SPR_PMC3        // Performance Monitor Counter Register 3
    mfspr     r28, SPR_PMC4        // Performance Monitor Counter Register 4
    mfspr     r29, SPR_SIA         // Sampled Instruction Address Register
    mfspr     r30, SPR_MMCR0       // Monitor Mode Control Register 0
    mfspr     r31, SPR_MMCR1       // Monitor Mode Control Register 1
    stmw      r24, ProcessorState_PPC.Extended1.DABR(r2)

    mfspr     r29, SPR_IBAT7L      // Instruction Block-Address Translation Register 7 (lower)
    mfspr     r30, SPR_DBAT4U      // Data Block-Address Translation Register 4 (upper)
    mfspr     r31, SPR_DBAT4L      // Data Block-Address Translation Register 4 (lower)
    stmw      r29, ProcessorState_PPC.Extended1.IBAT7L(r2)

    mfspr     r30, SPR_ICTC        // Instruction Cache Throttling Control Register
    mfspr     r31, SPR_L2CR        // Level 2 Cache Control Register
    stmw      r30, ProcessorState_PPC.Extended1.ICTC(r2)

    mfspr     r16, SPR_SRR0        // Save and Restore Register 0
    stw       r16, ProcessorState_PPC.Extended1.SRR0(r2)

    mfspr     r17, SPR_DBAT5U      // Data Block-Address Translation Register 5 (upper)
    stw       r17, ProcessorState_PPC.Extended1.DBAT5U(r2)

    mfspr     r25, SPR_UMMCR0      // User Monitor Mode Control Register 0
    mfspr     r26, SPR_UPMC1       // User Performance Monitor Counter Register 1
    mfspr     r27, SPR_UPMC2       // User Performance Monitor Counter Register 2
    mfspr     r28, SPR_USIA        // User Sampled Instruction Address Register
    mfspr     r29, SPR_UMMCR1      // User Monitor Mode Control Register 1
    mfspr     r30, SPR_UPMC3       // User Performance Monitor Counter Register 3
    mfspr     r31, SPR_UPMC4       // User Performance Monitor Counter Register 4
    stmw      r25, ProcessorState_PPC.Extended1.UMMCR0(r2)

    mfspr     r25, SPR_DBAT5L      // Data Block-Address Translation Register 5 (lower)
    mfspr     r26, SPR_DBAT6U      // Data Block-Address Translation Register 6 (upper)
    mfspr     r27, SPR_DBAT6L      // Data Block-Address Translation Register 6 (lower)
    mfspr     r28, SPR_DBAT7U      // Data Block-Address Translation Register 7 (upper)
    mfspr     r29, SPR_DBAT7L      // Data Block-Address Translation Register 7 (lower)
    mfspr     r30, SPR_HID2        // Hardware Implementation-Dependent Register 2
    mfspr     r31, SPR_HID4        // Hardware Implementation-Dependent Register 4
    stmw      r25, ProcessorState_PPC.Extended1.DBAT5L(r2)

    mfspr     r20, SPR_GQR0        // Quantization Register 0
    mfspr     r21, SPR_GQR1        // Quantization Register 1
    mfspr     r22, SPR_GQR2        // Quantization Register 2
    mfspr     r23, SPR_GQR3        // Quantization Register 3
    mfspr     r24, SPR_GQR4        // Quantization Register 4
    mfspr     r25, SPR_GQR5        // Quantization Register 5
    mfspr     r26, SPR_GQR6        // Quantization Register 6
    mfspr     r27, SPR_GQR7        // Quantization Register 7
    mfspr     r28, SPR_HID2        // Hardware Implementation-Dependent Register 2
    mfspr     r29, SPR_WPAR        // Write Gather Pipe Register
    mfspr     r30, SPR_DMAU        // Direct Memory Access Register (upper)
    mfspr     r31, SPR_DMAL        // Direct Memory Access Register (lower)
    stmw      r20, ProcessorState_PPC.Extended1.GQR(r2)

#endif

    blr
}

/****************************************************************************/
/*
 *    TRKRestoreExtended1Block
 *
 *    Restores the portion of the target program state that is in the
 *    Extended1 block.  Exception: The exceptionID field is not a true
 *    register and cannot be restored.
 *
 *    Note: Exceptions are disabled when this function is called.
 *    Also, it is OK to change any GPR value.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
ASM_PREFIX void TRKRestoreExtended1Block(void)
{
    nofralloc

    // restore target program state from global "gCPUState"

    lis       r2, gTRKCPUState@h   // get addr of stored regs
    ori       r2, r2, gTRKCPUState@l

    // TBR and DEC get restored depending on these flags

    lis       r5, gTRKRestoreFlags@h // get restore flags
    ori       r5, r5, gTRKRestoreFlags@l
    lbz       r3, ProcessorRestoreFlags_PPC.TBR(r5)            // TBR restore flag
    lbz       r6, ProcessorRestoreFlags_PPC.DEC(r5)            // DEC restore flag

    // Clear the flags in memory.

    li        r0, 0
    stb       r0, ProcessorRestoreFlags_PPC.TBR(r5)            // TBR restore flag
    stb       r0, ProcessorRestoreFlags_PPC.DEC(r5)            // DEC restore flag
    cmpwi     r3, 0                // skip TBR if false
    beq       no_tbr_restore

    lwz       r24, ProcessorState_PPC.Extended1.TBL(r2)
    lwz       r25, ProcessorState_PPC.Extended1.TBU(r2)

    mtspr     TBR_TBL_WRITE, r24   // Time base facility for writing (lower)
    mtspr     TBR_TBU_WRITE, r25   // Time base facility for writing (upper)

no_tbr_restore:

    // EXTENDED BLOCK 1    (Blockwise restore)

    /*
    ** Handle model-specific registers here.  Check the value of
    ** the PVR.  If it indicates 7xx, restore the 7xx-specific registers.
    ** Otherwise restore the 603e-specific registers.
    */

    lmw       r20, ProcessorState_PPC.Extended1.GQR(r2)
    mtspr     SPR_GQR0, r20        // Quantization Register 0
    mtspr     SPR_GQR1, r21        // Quantization Register 1
    mtspr     SPR_GQR2, r22        // Quantization Register 2
    mtspr     SPR_GQR3, r23        // Quantization Register 3
    mtspr     SPR_GQR4, r24        // Quantization Register 4
    mtspr     SPR_GQR5, r25        // Quantization Register 5
    mtspr     SPR_GQR6, r26        // Quantization Register 6
    mtspr     SPR_GQR7, r27        // Quantization Register 7
    mtspr     SPR_HID2, r28        // Hardware Implementation-Dependent Register 2
    mtspr     SPR_DMAU, r30        // Direct Memory Access Register (upper)
    mtspr     SPR_DMAL, r31        // Direct Memory Access Register (lower)

    b         __603e_specific_restore

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /* PPC7455 has additional SPRs */

    lmw       r26, ProcessorState_PPC.Extended1.MMCR2(r2)
    mtspr     SPR_MMCR2, r26       // Monitor Mode Control Register 2
    mtspr     SPR_BAMR, r27        // Breakpoint Address Mask Register

//    mtspr     SPR_SDA, r28         // User Sampled Data Address Register (not allowed in 7455 - Kobler 7455)

    mtspr     SPR_MSSCR0, r29      // Memory Subsystem Control Register 0
    mtspr     SPR_MSSCR1, r30      // Memory Subsystem Control Register 1
    mtspr     SPR_PIR, r31         // Processor ID Register

#endif

__603e_specific_restore:

    lmw       r19, ProcessorState_PPC.Extended1.DABR(r2)
    mtspr     SPR_DABR, r19        // Data Address Breakpoint Register
    mtspr     SPR_PMC1, r20        // Performance Monitor Counter Register 1
    mtspr     SPR_PMC2, r21        // Performance Monitor Counter Register 2
    mtspr     SPR_PMC3, r22        // Performance Monitor Counter Register 3
    mtspr     SPR_PMC4, r23        // Performance Monitor Counter Register 4
    mtspr     SPR_SIA, r24         // Sampled Instruction Address Register
    mtspr     SPR_MMCR0, r25       // Monitor Mode Control Register 0
    mtspr     SPR_MMCR1, r26       // Monitor Mode Control Register 1

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    mtspr     SPR_THRM1, r27       // Thermal Assist Unit Register 1
    mtspr     SPR_THRM2, r28       // Thermal Assist Unit Register 2
    mtspr     SPR_THRM3, r29       // Thermal Assist Unit Register 3

#endif

    mtspr     SPR_ICTC, r30        // Instruction Cache Throttling Control Register
    mtspr     SPR_L2CR, r31        // Level 2 Control Register

    b         __end_specific_restore

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    cmpwi     r6, 0                // skip DEC if false
    beq       no_dec_restore

    lwz       r26, ProcessorState_PPC.Extended1.DEC(r2)
    mtdec     r26                  // Decrement Register

no_dec_restore:

    lmw       r25, ProcessorState_PPC.Extended1.DMISS(r2)
    mtspr     SPR_DMISS, r25       // Data TLB Miss Address Register
    mtspr     SPR_DCMP, r26        // Data TLB Compare Register
    mtspr     SPR_HASH1, r27       // Primary Hash Address Register
    mtspr     SPR_HASH2, r28       // Secondary Hash Address Register
    mtspr     SPR_IMISS, r29       // Instruction TLB Miss Register
    mtspr     SPR_ICMP, r30        // Instruction TLB Compare Register
    mtspr     SPR_RPA, r31         // Required Physical Address Register

#endif

__end_specific_restore:

    lmw       r16, ProcessorState_PPC.Extended1.SR[0](r2)
    mtsr      0, r16               // Segment Register 0
    mtsr      1, r17               // Segment Register 1
    mtsr      2, r18               // Segment Register 2
    mtsr      3, r19               // Segment Register 3
    mtsr      4, r20               // Segment Register 4
    mtsr      5, r21               // Segment Register 5
    mtsr      6, r22               // Segment Register 6
    mtsr      7, r23               // Segment Register 7
    mtsr      8, r24               // Segment Register 8
    mtsr      9, r25               // Segment Register 9
    mtsr      10, r26              // Segment Register 10
    mtsr      11, r27              // Segment Register 11
    mtsr      12, r28              // Segment Register 12
    mtsr      13, r29              // Segment Register 13
    mtsr      14, r30              // Segment Register 14
    mtsr      15, r31              // Segment Register 15

    lmw       r12, ProcessorState_PPC.Extended1.HID0(r2)
    mtspr     SPR_HID0, r12        // Hardware Implementation-Dependent Register 0
    mtspr     SPR_HID1, r13        // Hardware Implementation-Dependent Register 1
    mtspr     SPR_SRR1, r14        // Save and Restore Register 1 (SRR1 gets copied to MSR on rfi)
    mtspr     SPR_PVR, r15         // Processor Version Register
    mtspr     SPR_IBAT0U, r16      // Instruction Block-Address Translation Register 0 (upper)
    mtspr     SPR_IBAT0L, r17      // Instruction Block-Address Translation Register 0 (lower)
    mtspr     SPR_IBAT1U, r18      // Instruction Block-Address Translation Register 1 (upper)
    mtspr     SPR_IBAT1L, r19      // Instruction Block-Address Translation Register 1 (lower)
    mtspr     SPR_IBAT2U, r20      // Instruction Block-Address Translation Register 2 (upper)
    mtspr     SPR_IBAT2L, r21      // Instruction Block-Address Translation Register 2 (lower)
    mtspr     SPR_IBAT3U, r22      // Instruction Block-Address Translation Register 3 (upper)
    mtspr     SPR_IBAT3L, r23      // Instruction Block-Address Translation Register 3 (lower)
    mtspr     SPR_DBAT0U, r24      // Data Block-Address Translation Register 0 (upper)
    mtspr     SPR_DBAT0L, r25      // Data Block-Address Translation Register 0 (lower)
    mtspr     SPR_DBAT1U, r26      // Data Block-Address Translation Register 1 (upper)
    mtspr     SPR_DBAT1L, r27      // Data Block-Address Translation Register 1 (lower)
    mtspr     SPR_DBAT2U, r28      // Data Block-Address Translation Register 2 (upper)
    mtspr     SPR_DBAT2L, r29      // Data Block-Address Translation Register 2 (lower)
    mtspr     SPR_DBAT3U, r30      // Data Block-Address Translation Register 3 (upper)
    mtspr     SPR_DBAT3L, r31      // Data Block-Address Translation Register 3 (lower)

    lmw       r22, ProcessorState_PPC.Extended1.SDR1(r2)
    mtspr     SPR_SDR1, r22        // Sample Data Register 1
    mtspr     SPR_DAR, r23         // Data Address Register
    mtspr     SPR_DSISR, r24       // Data Storage Interrupt Status Register
    mtspr     SPR_SPRG0, r25       // Special Purpose Register General 0
    mtspr     SPR_SPRG1, r26       // Special Purpose Register General 1
    mtspr     SPR_SPRG2, r27       // Special Purpose Register General 2
    mtspr     SPR_SPRG3, r28       // Special Purpose Register General 3

    /* skip DEC here - it is 603e-only */

#if !defined(__PPC7450__)          // MetroTRK crashes - Kobler 7455
    mtspr     SPR_IABR, r30        // Instruction Address Breakpoint Register
    mtspr     SPR_EAR, r31         // External Address Register
#endif

    blr
}
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKTargetCPUMinorType
 *
 *    Determines the CPU minor type.
 *
 *    Currently this is done by checking the PVR value.  However, this
 *    does not distinguish between the 740 and the 750.  Additional
 *    code will be necessary for that, if it is possible.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
u8 TRKTargetCPUMinorType(void)
{
#if 0 // NOT IMPLEMENTED ON THE WII / NDEV

    switch (__TRK_get_PVR() >> 16)
    {
        case _7XX_VERSION:
            return DS_CPU_PPC_7XX;

        case _8260_VERSION:
            return DS_CPU_PPC_8260;

        case _7400_VERSION:
            return DS_CPU_PPC_7400;

        case _7410_VERSION:

#endif
            // v0.1 of MetroTRK for the Wii / NDEV
            // returned a value of 0x54 by default
            return DS_CPU_PPC_7410;

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV

        case _7455_VERSION:          // Kobler 7455
            return DS_CPU_PPC_7400;  // should be DS_CPU_PPC_7455,
                                     // but not yet supported by CW Debugger
        default:
            return DS_CPU_PPC_603E;
    }
#endif
}
#endif
