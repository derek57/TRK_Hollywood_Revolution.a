/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED

*/
/****************************************************************************/


#include "asm_regs.h"


.text


#ifdef __GNUC__

.extern gTRKCPUState
.extern gTRKRestoreFlags


.globl TRKSaveExtended1Block
TRKSaveExtended1Block:

    lis       r2, gTRKCPUState@h
    ori       r2, r2, gTRKCPUState@l

    mfsr      r16, 0
    mfsr      r17, 1
    mfsr      r18, 2
    mfsr      r19, 3
    mfsr      r20, 4
    mfsr      r21, 5
    mfsr      r22, 6
    mfsr      r23, 7
    mfsr      r24, 8
    mfsr      r25, 9
    mfsr      r26, 10
    mfsr      r27, 11
    mfsr      r28, 12
    mfsr      r29, 13
    mfsr      r30, 14
    mfsr      r31, 15

    stmw      r16, 0x1A8(r2)

    mftb      r27, TBR_TBL_READ    // Time Base Read Lower Register
    mftb      r28, TBR_TBU_READ    // Time Base Read Upper Register

                                   // Same values: HID0 or DBSR
    mfspr     r29, SPR_HID0        // Hardware Implementation Register 0
                                   // ...or Debug Status Register

    mfspr     r30, SPR_HID1        // Hardware Implementation Register 1
    mfspr     r31, SPR_SRR1        // Save/Restore Register 1
    stmw      r27, 0x1E8(r2)

    mfspr     r15, SPR_PVR         // Processor Version Register

    mfspr     r16, SPR_IBAT0U      // Instruction BAT register 0 (upper)
    mfspr     r17, SPR_IBAT0L      // Instruction BAT register 0 (lower)
    mfspr     r18, SPR_IBAT1U      // Instruction BAT register 1 (upper)
    mfspr     r19, SPR_IBAT1L      // Instruction BAT register 1 (lower)
    mfspr     r20, SPR_IBAT2U      // Instruction BAT register 2 (upper)
    mfspr     r21, SPR_IBAT2L      // Instruction BAT register 2 (lower)
    mfspr     r22, SPR_IBAT3U      // Instruction BAT register 3 (upper)
    mfspr     r23, SPR_IBAT3L      // Instruction BAT register 3 (lower)
    mfspr     r24, SPR_DBAT0U      // Data BAT register 0 (upper)
    mfspr     r25, SPR_DBAT0L      // Data BAT register 0 (lower)
    mfspr     r26, SPR_DBAT1U      // Data BAT register 1 (upper)
    mfspr     r27, SPR_DBAT1L      // Data BAT register 1 (lower)
    mfspr     r28, SPR_DBAT2U      // Data BAT register 2 (upper)
    mfspr     r29, SPR_DBAT2L      // Data BAT register 2 (lower)
    mfspr     r30, SPR_DBAT3U      // Data BAT register 3 (upper)
    mfspr     r31, SPR_DBAT3L      // Data BAT register 3 (lower)

    stmw      r15, 0x1FC(r2)
    mfspr     r24, SPR_IBAT4U      // Instruction BAT register 4 (upper)
    mfspr     r25, SPR_IBAT4L      // Instruction BAT register 4 (lower)
    mfspr     r26, SPR_IBAT5U      // Instruction BAT register 5 (upper)
    mfspr     r27, SPR_IBAT5L      // Instruction BAT register 5 (lower)
    mfspr     r28, SPR_IBAT6U      // Instruction BAT register 6 (upper)
    mfspr     r29, SPR_IBAT6L      // Instruction BAT register 6 (lower)
    mfspr     r30, SPR_IBAT7U      // Instruction BAT register 7 (upper)
    mfspr     r31, SPR_IBAT7L      // Instruction BAT register 7 (lower)
    stmw      r24, 0x240(r2)

    mfspr     r22, SPR_SDR1        // MMU hash base register (SDR1)
    mfspr     r23, SPR_DAR         // Data Address Register
    mfspr     r24, SPR_DSISR       // Data Storage Interrupt Status Register
    mfspr     r25, SPR_SPRG0       // Special Purpose Register General 0
    mfspr     r26, SPR_SPRG1       // Special Purpose Register General 1
    mfspr     r27, SPR_SPRG2       // Special Purpose Register General 2
    mfspr     r28, SPR_SPRG3       // Special Purpose Register General 3

    mfspr     r29, SPR_DEC         // Decrement Register
                                   // Same values:
    mfspr     r30, SPR_IABR        // Instruction address breakpoint register
                                   // ...or Debug Control Register 0

    mfspr     r31, SPR_EAR         // External address register

    stmw      r22, 0x25C(r2)
                                   // Same values:
                                   // Instruction Address Compare 2
    mfspr     r24, SPR_DABR        // Data address breakpoint register
                                   // Branch Control and Status Register

                                   // Same values:
    mfspr     r25, SPR_PMC1        // Performance Counter Register 1
                                   // ...or storage guarded register

                                   // Same values:
    mfspr     r26, SPR_PMC2        // data cache write-thru register
                                   // ...or Performance Counter Register 2

                                   // Same values:
    mfspr     r27, SPR_PMC3        // Performance Counter Register 3
                                   // ...or Debug Control Register 1

    mfspr     r28, SPR_PMC4        // Performance Counter Register 4
    mfspr     r29, SPR_SIA         // Sampled Instruction Address Register

    // Was hard to find:
    //
    // A Google search listed only 1 japanese website with a
    // result of (decimal) "952" which in turn is 0x3B8 in hex.
    mfspr     r30, SPR_MMCR0       // storage mem-coherent (not implemented?)
                                   // Monitor Mode Control Register 0

    mfspr     r31, SPR_MMCR1       // Monitor Mode Control Register 1
    stmw      r24, 0x284(r2)
    mfspr     r29, SPR_IBAT7L      // Instruction BAT 7 Lower Register
    mfspr     r30, SPR_DBAT4U      // Data BAT 4 Upper Register
    mfspr     r31, SPR_DBAT4L      // Data BAT 4 Lower Register
    stmw      r29, 0x2A4(r2)

    mfspr     r30, SPR_ICTC        // instruction cache control register
    mfspr     r31, SPR_L2CR        // Level 2 Cache Control Register

    stmw      r30, 0x2B0(r2)
    mfspr     r16, SPR_SRR0        // Save/Restore Register 0
    stw       r16, 0x2B8(r2)

                                   // Same values:
    mfspr     r17, SPR_DBAT5U      // Machine Check Save and Restore Register 0
                                   // ...or Data BAT 5 Upper Register

    stw       r17, 0x2BC(r2)
    mfspr     r25, SPR_UMMCR0      // User Monitor Mode Control Register 0
    mfspr     r26, SPR_UPMC1       // User Performance Counter Register 1
    mfspr     r27, SPR_UPMC2       // User Performance Counter Register 2
    mfspr     r28, SPR_USIA        // User Sampled Instruction Address Register
    mfspr     r29, SPR_UMMCR1      // User Monitor Mode Control Register 1
    mfspr     r30, SPR_UPMC3       // User Performance Counter Register 3
    mfspr     r31, SPR_UPMC4       // User Performance Counter Register 4
    stmw      r25, 0x2C0(r2)

                                   // Same values:
    mfspr     r25, SPR_DBAT5L      // Machine Check Save and Restore Register 1
                                   // ...or Data BAT 5 Lower Register

                                   // Same values:
    mfspr     r26, SPR_DBAT6U      // Machine Check Syndrome register
                                   // ...or Data BAT 6 Upper Register

                                   // Same values:
    mfspr     r27, SPR_DBAT6L      // Machine Check Address register
                                   // ...or Data BAT 6 Lower Register

    mfspr     r28, SPR_DBAT7U      // Data BAT 7 Upper Register
    mfspr     r29, SPR_DBAT7L      // Data BAT 7 Lower Register
    mfspr     r30, SPR_DCVL        // Data Cache Victim Limit

                                   // Same values:
    mfspr     r31, SPR_HID2        // Debug Data Register
                                   // L1 Instruction Cache Control and Status Register 1

    stmw      r25, 0x2DC(r2)

    mfspr     r20, SPR_DCNV0       // Data Cache Normal Victim 0
    mfspr     r21, SPR_DCNV1       // Data Cache Normal Victim 1
    mfspr     r22, SPR_DCNV2       // Data Cache Normal Victim 2
    mfspr     r23, SPR_DCNV3       // Data Cache Normal Victim 3
    mfspr     r24, SPR_DCTV0       // Data Cache Transient Victim 0
    mfspr     r25, SPR_DCTV1       // Data Cache Transient Victim 1
    mfspr     r26, SPR_DCTV2       // Data Cache Transient Victim 2
    mfspr     r27, SPR_DCTV3       // Data Cache Transient Victim 3
    mfspr     r28, SPR_DCVL        // Data Cache Victim Limit
    mfspr     r29, SPR_ICVL        // Instruction Cache Victim Limit
    mfspr     r30, SPR_DMAU        // Special Purpose Register DMA Upper
    mfspr     r31, SPR_DMAL        // Special Purpose Register DMA Lower
    stmw      r20, 0x2FC(r2)

    blr



.globl TRKRestoreExtended1Block
TRKRestoreExtended1Block:

    lis       r2, gTRKCPUState@h
    ori       r2, r2, gTRKCPUState@l
    lis       r5, gTRKRestoreFlags@h
    ori       r5, r5, gTRKRestoreFlags@l
    lbz       r3, 0(r5)
    lbz       r6, 1(r5)
    li        r0, 0
    stb       r0, 0(r5)
    stb       r0, 1(r5)
    cmpwi     r3, 0
    beq       no_tbr_restore

    lwz       r24, 0x1E8(r2)       // TBL
    lwz       r25, 0x1EC(r2)       // TBU

    mtspr     TBR_TBL_WRITE, r24   // Time base facility for writing (lower)
    mtspr     TBR_TBU_WRITE, r25   // Time base facility for writing (upper)

no_tbr_restore:

    lmw       r20, 0x2FC(r2)
    mtspr     SPR_DCNV0, r20
    mtspr     SPR_DCNV1, r21
    mtspr     SPR_DCNV2, r22
    mtspr     SPR_DCNV3, r23
    mtspr     SPR_DCTV0, r24
    mtspr     SPR_DCTV1, r25
    mtspr     SPR_DCTV2, r26
    mtspr     SPR_DCTV3, r27
    mtspr     SPR_DCVL, r28
    mtspr     SPR_DMAU, r30
    mtspr     SPR_DMAL, r31
    b         __603e_specific_restore

__603e_specific_restore:

    lmw       r19, 0x284(r2)

    mtspr     SPR_DABR, r19        // Data address breakpoint register
    mtspr     SPR_PMC1, r20        // storage guarded register         (403GC)
    mtspr     SPR_PMC2, r21        // data cache write-thru register   (403GC)

    mtspr     SPR_PMC3, r22
    mtspr     SPR_PMC4, r23
    mtspr     SPR_SIA, r24

    // Was hard to find:
    //
    // A Google search listed only 1 website with a decimal
    // result of "952" which in turn is 0x3B8 in hexadecimal
    mtspr     SPR_MMCR0, r25       // storage mem-coherent (not implemented?)

    mtspr     SPR_MMCR1, r26

    mtspr     SPR_ICTC, r30        // instruction cache control register
    mtspr     SPR_L2CR, r31

    b         __end_specific_restore

__end_specific_restore:

    lmw       r16, 0x1A8(r2)

    mtsr      0, r16               // SR0
    mtsr      1, r17               // SR1
    mtsr      2, r18               // SR2
    mtsr      3, r19               // SR3
    mtsr      4, r20               // SR4
    mtsr      5, r21               // SR5
    mtsr      6, r22               // SR6
    mtsr      7, r23               // SR7
    mtsr      8, r24               // SR8
    mtsr      9, r25               // SR9
    mtsr      10, r26              // SR10
    mtsr      11, r27              // SR11
    mtsr      12, r28              // SR12
    mtsr      13, r29              // SR13
    mtsr      14, r30              // SR14
    mtsr      15, r31              // SR15

    lmw       r12, 0x1F0(r2)

    mtspr     SPR_HID0, r12

    mtspr     SPR_HID1, r13
    mtspr     SPR_SRR1, r14

    mtspr     SPR_PVR, r15         // Processor version register
    mtspr     SPR_IBAT0U, r16      // Instruction BAT register 0 (upper)
    mtspr     SPR_IBAT0L, r17      // Instruction BAT register 0 (lower)
    mtspr     SPR_IBAT1U, r18      // Instruction BAT register 1 (upper)
    mtspr     SPR_IBAT1L, r19      // Instruction BAT register 1 (lower)
    mtspr     SPR_IBAT2U, r20      // Instruction BAT register 2 (upper)
    mtspr     SPR_IBAT2L, r21      // Instruction BAT register 2 (lower)
    mtspr     SPR_IBAT3U, r22      // Instruction BAT register 3 (upper)
    mtspr     SPR_IBAT3L, r23      // Instruction BAT register 3 (lower)
    mtspr     SPR_DBAT0U, r24      // Data BAT register 0 (upper)
    mtspr     SPR_DBAT0L, r25      // Data BAT register 0 (lower)
    mtspr     SPR_DBAT1U, r26      // Data BAT register 1 (upper)
    mtspr     SPR_DBAT1L, r27      // Data BAT register 1 (lower)
    mtspr     SPR_DBAT2U, r28      // Data BAT register 2 (upper)
    mtspr     SPR_DBAT2L, r29      // Data BAT register 2 (lower)
    mtspr     SPR_DBAT3U, r30      // Data BAT register 3 (upper)
    mtspr     SPR_DBAT3L, r31      // Data BAT register 3 (lower)

    lmw       r22, 0x25C(r2)
    mtspr     SPR_SDR1, r22        // SDR1 register
    mtspr     SPR_DAR, r23
    mtspr     SPR_DSISR, r24
    mtspr     SPR_SPRG0, r25
    mtspr     SPR_SPRG1, r26
    mtspr     SPR_SPRG2, r27
    mtspr     SPR_SPRG3, r28

    mtspr     SPR_IABR, r30        // Instruction address breakpoint register
    mtspr     SPR_EAR, r31         // External address register

    blr

#endif
