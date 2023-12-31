/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases by MetroWerks / Freescale Semiconductor / NXP.

COPYRIGHT

    2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "trktypes.h"
#include "dolphin_trk.h"
#include "dolphin_trk_glue.h"
#include "targimpl_ppc.h"
#include "flush_cache.h"
#include "main_TRK.h"
#include "copy_vectors.h"
#include "reset.h"
#include "rvl_mem.h"
#include "ppc_reg.h"
#include "asm_regs.h"


#define EXCEPTION_SIZE  0x100
#define NUM_EXCEPTIONS  15


static u32 TRK_ISR_OFFSETS[NUM_EXCEPTIONS] =
{
    PPC_SYSTEMRESET,
    PPC_MACHINECHECK,
    PPC_DATAACCESSERROR,
    PPC_INSTACCESSERROR,
    PPC_EXTERNALINTERRUPT,
    PPC_ALIGNMENTERROR,
    PPC_PROGRAMERROR,
    PPC_FPUNAVAILABLE,
    PPC_DECREMENTERINTERRUPT,
    PPC_SYSTEMCALL,
    PPC_TRACE,
    PPC_PERFORMANCE_MONITOR,
    PPC7xx_603E_INSTR_ADDR_BREAK,
    PPC7xx_603E_SYS_MANAGE,
    PPC_THERMAL_MANAGE
};

// TRKLoadContext base ???
static u32 *lc_base;


#if defined(__MWERKS__)
    extern void OSResetSystem(s32 state, u32 val, BOOL menu);
#elif defined(__GNUC__)
    extern void SYS_ResetSystem(s32 reset, u32 reset_code, s32 force_menu);
#endif


extern u32 _db_stack_addr;


/****************************************************************************/
/*
 *    InitMetroTRK
 *
 *    100 % DONE (This code was originally written in pure PPC assembly)
 */
/****************************************************************************/

// MOVED TO DOLPHIN_TRK_ASM.S FOR GCC
#ifdef __MWERKS__

ASM_PREFIX void InitMetroTRK(void)
{
    nofralloc

    subi      r1, r1, 4
    stw       r3, 0(r1)
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l
    stmw      r0, ProcessorState_PPC.Default.GPR[0](r3)
    lwz       r4, 0(r1)
    addi      r1, r1, 4
    stw       r1, ProcessorState_PPC.Default.GPR[1](r3)
    stw       r4, ProcessorState_PPC.Default.GPR[3](r3)
    mfspr     r4, LR
    stw       r4, ProcessorState_PPC.Default.LR(r3)
    stw       r4, ProcessorState_PPC.Default.PC(r3)
    mfcr      r4
    stw       r4, ProcessorState_PPC.Default.CR(r3)
    mfmsr     r4
    ori       r3, r4, MSR_EE
    xori      r3, r3, MSR_EE
    mtmsr     r3
    mtspr     SRR1, r4
    bl        TRKSaveExtended1Block
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l

    // For some reason, LMW operation on r0 is problematic to GCC:
    lmw       r0, ProcessorState_PPC.Default.GPR[0](r3)

    li        r0, 0

    // Instruction address breakpoint register
    mtspr     SPR_IABR, r0

    // Data address breakpoint register
    mtspr     SPR_DABR, r0

    lis       r1, _db_stack_addr@h
    ori       r1, r1, _db_stack_addr@l
    mr        r3, r5
    bl        InitMetroTRKCommTable
    cmpwi     r3, 1
    bne       main
    lwz       r4, ProcessorState_PPC.Default.LR(r3)
    mtlr      r4

    // For some reason, LMW operation on r0 is problematic to GCC:
    lmw       r0, ProcessorState_PPC.Default.GPR[0](r3)

    blr

main:

    b         TRK_main
    blr
}

/****************************************************************************/
/*
 *    InitMetroTRK_BBA
 *
 *    100 % DONE (This code was originally written in pure PPC assembly)
 */
/****************************************************************************/
ASM_PREFIX void InitMetroTRK_BBA(void)
{
    nofralloc

    addi      r1, r1, -4
    stw       r3, 0(r1)
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l
    stmw      r0, ProcessorState_PPC.Default.GPR[0](r3)
    lwz       r4, 0(r1)
    addi      r1, r1, 4
    stw       r1, ProcessorState_PPC.Default.GPR[1](r3)
    stw       r4, ProcessorState_PPC.Default.GPR[3](r3)
    mflr      r4
    stw       r4, ProcessorState_PPC.Default.LR(r3)
    stw       r4, ProcessorState_PPC.Default.PC(r3)
    mfcr      r4
    stw       r4, ProcessorState_PPC.Default.CR(r3)
    mfmsr     r4
    ori       r3, r4, MSR_EE
    mtmsr     r3
    mtsrr1    r4
    bl        TRKSaveExtended1Block
    lis       r3, gTRKCPUState@h
    ori       r3, r3, gTRKCPUState@l

    // For some reason, LMW operation on r0 is problematic to GCC:
    lmw       r0, ProcessorState_PPC.Default.GPR[0](r3)

    li        r0, 0

    // Instruction address breakpoint register
    mtspr     SPR_IABR, r0

    // Data address breakpoint register
    mtspr     SPR_DABR, r0

    lis       r1, _db_stack_addr@h
    ori       r1, r1, _db_stack_addr@l
    li        r3, 2
    bl        InitMetroTRKCommTable
    cmpwi     r3, 1
    bne       main
    lwz       r4, ProcessorState_PPC.Default.LR(r3)
    mtlr      r4

    // For some reason, LMW operation on r0 is problematic to GCC:
    lmw       r0, ProcessorState_PPC.Default.GPR[0](r3)

    blr

main:

    b         TRK_main
    blr
}
#endif

/****************************************************************************/
/*
 *    EnableMetroTRKInterrupts
 *
 *    100 % DONE
 */
/****************************************************************************/
void EnableMetroTRKInterrupts(void)
{
    EnableEXI2Interrupts();

    return;
}

/****************************************************************************/
/*
 *    TRKTargetTranslate
 *
 *    100 % DONE
 */
/****************************************************************************/
void *TRKTargetTranslate(u32 *addr)
{
    if ((addr < lc_base) || (addr >= &lc_base[0x1000]) || ((gTRKCPUState.Extended1.DBAT2L & 3) == 0)
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        && (((u32)addr < 0x7E000000) || ((u32)addr > BOOTINFO)))
#endif
        )
    {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        if ((u32)addr < 0x3000000)
        {
            addr = (void *)(((u32)addr & 0x3FFFFFFF) | BOOTINFO);
        }
        else
#endif
        {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
            if (((u32)addr >= 0x10000000) && ((u32)addr < 0x1C000000))
                addr = (void *)(((u32)addr & 0x3FFFFFFF) | MEM2_CACHED);
#else
                addr = (void *)(((u32)addr & 0x3FFFFFFF) | BOOTINFO);
#endif
        }
    }

    return addr;
}

/****************************************************************************/
/*
 *    __TRK_copy_vector
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRK_copy_vector(u32 *src_addr)
{
    void *dest;

    if ((src_addr >= lc_base) && (src_addr < &lc_base[0x1000]) && ((gTRKCPUState.Extended1.DBAT2L & 3) != 0))
    {
        dest = src_addr;
    }
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    else if ((u32)src_addr < 0x3000000)
#else
    else if (((u32)src_addr >= 0x7E000000) && ((u32)src_addr <= BOOTINFO))
#endif
    {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | BOOTINFO);
#else
        dest = src_addr;
#endif
    }
    else
    {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
        if (((u32)src_addr >= 0x10000000) && ((u32)src_addr < 0x1C000000))
            dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | MEM2_CACHED);
        else
            dest = src_addr;
#else
        dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | BOOTINFO);
#endif
    }

    TRK_memcpy(dest, (void *)((u32)&gTRKInterruptVectorTable[(u32)src_addr]), EXCEPTION_SIZE);
    TRK_flush_cache((u32)dest, EXCEPTION_SIZE);

    return;
}

/****************************************************************************/
/*
 *    __TRK_copy_vectors
 *
 *    100 % DONE
 */
/****************************************************************************/
void __TRK_copy_vectors(void)
{
    void *src_addr;
    u32 *isr_ptr;
    s32 i = 0;
    u32 *data_ptr;
    u32 data;
    void *dest;

    if (((u32)lc_base <= DB_EXCEPTION_MASK) && ((u32)&lc_base[0x1000] > DB_EXCEPTION_MASK) && ((gTRKCPUState.Extended1.DBAT2L & 3) != 0))
        data_ptr = (u32 *)DB_EXCEPTION_MASK;
    else
        data_ptr = (u32 *)(BOOTINFO + DB_EXCEPTION_MASK);

    data = *data_ptr;
    isr_ptr = (u32 *)TRK_ISR_OFFSETS;

    for (i = 0; i <= (s32)((sizeof(TRK_ISR_OFFSETS) / sizeof(s32)) - 1); i++)
    {
        if (((data & (1 << i)) != 0) && (i != 4))
        {
            src_addr = (void *)*isr_ptr;

            if (((u32)src_addr >= (u32)lc_base) && ((u32)src_addr < (u32)&lc_base[0x1000]) && ((gTRKCPUState.Extended1.DBAT2L & 3) != 0))
            {
                dest = src_addr;
            }
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
            else if (((u32)src_addr >= 0x7E000000) && ((u32)src_addr <= BOOTINFO))
#else
            else if ((u32)src_addr < 0x3000000)
#endif
            {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
                dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | BOOTINFO);
#else
                dest = src_addr;
#endif
            }
            else
            {
#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
                if (((u32)src_addr >= 0x10000000) && ((u32)src_addr < 0x1C000000))
                    dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | MEM2_CACHED);
                else
                    dest = src_addr;
#else
                dest = (void *)(((u32)src_addr & 0x3FFFFFFF) | BOOTINFO);
#endif
            }

            TRK_memcpy(dest, (void *)((u32)&gTRKInterruptVectorTable[(u32)src_addr]), EXCEPTION_SIZE);
            TRK_flush_cache((u32)dest, EXCEPTION_SIZE);
        }

        isr_ptr++;
    }

    return;
}

/*
** Get 7xx-specific registers from C.
*/

/* This code is to used to create a stack frame for a function, if the function: 1. calls */
/* other functions, 2. uses more than 224 bytes of local variables, and 3. declares local */
/* register variables. */

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    __TRK_get_HID0
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_HID0(void)  /* Inline Assembly code uses the following syntax: asm function definition */
{
    mfspr     r3, SPR_HID0  /* Move from special purpose register HID0 to general purpose r3 */
    frfree                  /* This will free the stack frame and restores the registers that mfspr has */
                            /* reserved. */
    blr                     /* unconditional branch to link to register */
}

/****************************************************************************/
/*
 *    __TRK_get_MMCR0
 *
 *    100 % DONE (NEVER USED)
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_MMCR0(void) /* Inline Assembly code uses the following syntax: asm function definition */
{
    mfspr     r3, SPR_MMCR0 /* Move from special purpose register MMCR0 to general purpose r3 */
    frfree                  /* This will free the stack frame and restores the registers that mfspr has */
                            /* reserved. */
    blr                     /* unconditional branch to link to register */
}
#endif

/****************************************************************************/
/*
 *    TRKInitializeTarget
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKInitializeTarget(void)
{
#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    BOOL read = TRUE;
#endif

    //
    // The call to "__TRK_copy_vectors" right here is OBSOLETE!
    //
    // For the Wii / NDEV, Freescale rewrote the function
    // completely into a native C-function and then moved
    // it's call into the message handler function
    // "TRKDoOverride". Maybe this was the old way to
    // do it, but no older releases than the TRK API
    // v1.10 were seen compared to those found within NXP's
    // IDE's...
    //

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV

    // Copy the interrupt vectors into RAM.
    __TRK_copy_vectors();

#endif

    // Initialize state flags.
    //
    // The target is initially stopped
    gTRKState.stopped = TRUE;

    // This will be initialized eventually in SwapAndGo.  However,
    // need to initialize gTRKState.MSR right away because it is
    // used at the beginning of InterruptHandler and we may be
    // getting communication interrupts long before SwapAndGo gets
    // called.
    gTRKState.MSR = __TRK_get_MSR();

    // On the Wii / NDEV, the MSR stuff below was NOT done
    // this way...

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV

    // Initialize target-state registers to sensible values where
    // necessary.
#if TRK_TRANSPORT_INT_DRIVEN

    // If interrupt-driven, make sure external interrupts are enabled
    // when the target program initially begins execution.
    gTRKCPUState.Extended1.MSR = (__TRK_get_MSR() | MSR_EE | MSR_RI);
#else
    gTRKCPUState.Extended1.MSR = __TRK_get_MSR();
#endif

    gTRKCPUState.Extended1.HID0 = (u32)__TRK_get_HID0();
    gTRKCPUState.Extended1.IBAT0U = __TRK_get_IBAT0U();
    gTRKCPUState.Extended1.IBAT0L = __TRK_get_IBAT0L();
    gTRKCPUState.Extended1.IBAT1U = __TRK_get_IBAT1U();
    gTRKCPUState.Extended1.IBAT1L = __TRK_get_IBAT1L();
    gTRKCPUState.Extended1.IBAT2U = __TRK_get_IBAT2U();
    gTRKCPUState.Extended1.IBAT2L = __TRK_get_IBAT2L();
    gTRKCPUState.Extended1.IBAT3U = __TRK_get_IBAT3U();
    gTRKCPUState.Extended1.IBAT3L = __TRK_get_IBAT3L();
    gTRKCPUState.Extended1.DBAT0U = __TRK_get_DBAT0U();
    gTRKCPUState.Extended1.DBAT0L = __TRK_get_DBAT0L();
    gTRKCPUState.Extended1.DBAT1U = __TRK_get_DBAT1U();
    gTRKCPUState.Extended1.DBAT1L = __TRK_get_DBAT1L();
    gTRKCPUState.Extended1.DBAT2U = __TRK_get_DBAT2U();
    gTRKCPUState.Extended1.DBAT2L = __TRK_get_DBAT2L();
    gTRKCPUState.Extended1.DBAT3U = __TRK_get_DBAT3U();
    gTRKCPUState.Extended1.DBAT3L = __TRK_get_DBAT3L();

    gTRKCPUState.Extended1.IABR = 0;

    /*
    ** Some init is different for the 7xx and 603e models.
    */

    switch (TRKTargetCPUMinorType())
    {
        case DS_CPU_PPC_7400:
        case DS_CPU_PPC_7XX:
        case DS_CPU_PPC_740:
        case DS_CPU_PPC_750:
            gTRKCPUState.Extended1.MMCR0 = __TRK_get_MMCR0();
            gTRKCPUState.Extended1.MMCR1 = 0;
            gTRKCPUState.Extended1.ICTC = 0;
            gTRKCPUState.Extended1.DABR = 0;
            gTRKCPUState.Extended1.THRM1 = 0;
            gTRKCPUState.Extended1.THRM2 = 0;
            gTRKCPUState.Extended1.THRM3 = 0;
            gTRKCPUState.Extended1.L2CR = 0;
            break;
    }

    if (TRKTargetCPUMinorType() == DS_CPU_PPC_7400)
    {
        /* PPC7400 has additional SPRs */
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UMMCR2, SPR_UMMCR2, read);

#if !defined(__PPC7450__)
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UBAMR,  SPR_UBAMR,  read);  // doesn't exist in 7455 // Kobler 7455
#endif

        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UMMCR0, SPR_UMMCR0, read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UPMC1,  SPR_UPMC1,  read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UPMC2,  SPR_UPMC2,  read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.USIA,   SPR_USIA,   read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UMMCR1, SPR_UMMCR1, read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UPMC3,  SPR_UPMC3,  read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.UPMC4,  SPR_UPMC4,  read);

#if !defined(__PPC7450__)
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.USDA,   SPR_USDA,   read);  // not allowed in 7455 // Kobler 7455
#endif

        TRKPPCAccessSPR(&gTRKCPUState.Extended1.MMCR2,  SPR_MMCR2,  read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.BAMR,   SPR_BAMR,   read);

#if !defined(__PPC7450__)
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.SDA,    SPR_SDA,    read);  // not allowed in 7455 // Kobler 7455
#endif

        TRKPPCAccessSPR(&gTRKCPUState.Extended1.MSSCR0, SPR_MSSCR0, read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.MSSCR1, SPR_MSSCR1, read);
        TRKPPCAccessSPR(&gTRKCPUState.Extended1.PIR,    SPR_PIR,    read);
    }
#endif

    lc_base = (u32 *)0xE0000000;

    return kNoError;
}

/****************************************************************************/
/*
 *    __TRKreset
 *
 *    100 % DONE
 */
/****************************************************************************/

#define RESTART 0

void __TRKreset(void)
{
#if defined(__MWERKS__)
    OSResetSystem(RESTART, 0, FALSE);
#elif defined(__GNUC__)
    SYS_ResetSystem(RESTART, 0, FALSE);
#endif

    return;
}
