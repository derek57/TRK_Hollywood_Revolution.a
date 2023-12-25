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
#include "ppc_except.h"
#include "proc_mpc_7xx_603e.h"


.text


#ifdef __GNUC__

.extern gTRKState
.extern gTRKCPUState
.extern gTRKSaveState
.extern gTRKExceptionStatus
.extern TRKSaveExtended1Block
.extern TRKRestoreExtended1Block
.extern TRKPostInterruptEvent
.extern TRKUARTInterruptHandler
.extern TRK_saved_exceptionID


.globl __TRK_get_MSR
__TRK_get_MSR:
    mfmsr     r3
    blr



.globl __TRK_set_MSR
__TRK_set_MSR:
    mtmsr     r3
    blr



.globl __TRK_get_PVR
__TRK_get_PVR:
    mfspr     r3, SPR_PVR
    blr



.globl __TRK_get_IBAT0U
__TRK_get_IBAT0U:
    mfspr     r3, SPR_IBAT0U
    blr



.globl __TRK_get_IBAT0L
__TRK_get_IBAT0L:
    mfspr     r3, SPR_IBAT0L
    blr



.globl __TRK_get_IBAT1U
__TRK_get_IBAT1U:
    mfspr     r3, SPR_IBAT1U
    blr



.globl __TRK_get_IBAT1L
__TRK_get_IBAT1L:
    mfspr     r3, SPR_IBAT1L
    blr



.globl __TRK_get_IBAT2U
__TRK_get_IBAT2U:
    mfspr     r3, SPR_IBAT2U
    blr



.globl __TRK_get_IBAT2L
__TRK_get_IBAT2L:
    mfspr     r3, SPR_IBAT2L
    blr



.globl __TRK_get_IBAT3U
__TRK_get_IBAT3U:
    mfspr     r3, SPR_IBAT3U
    blr



.globl __TRK_get_IBAT3L
__TRK_get_IBAT3L:
    mfspr     r3, SPR_IBAT3L
    blr



.globl __TRK_get_DBAT0U
__TRK_get_DBAT0U:
    mfspr     r3, SPR_DBAT0U
    blr



.globl __TRK_get_DBAT0L
__TRK_get_DBAT0L:
    mfspr     r3, SPR_DBAT0L
    blr



.globl __TRK_get_DBAT1U
__TRK_get_DBAT1U:
    mfspr     r3, SPR_DBAT1U
    blr



.globl __TRK_get_DBAT1L
__TRK_get_DBAT1L:
    mfspr     r3, SPR_DBAT1L
    blr



.globl __TRK_get_DBAT2U
__TRK_get_DBAT2U:
    mfspr     r3, SPR_DBAT2U
    blr



.globl __TRK_get_DBAT2L
__TRK_get_DBAT2L:
    mfspr     r3, SPR_DBAT2L
    blr



.globl __TRK_get_DBAT3U
__TRK_get_DBAT3U:
    mfspr     r3, SPR_DBAT3U
    blr



.globl __TRK_get_DBAT3L
__TRK_get_DBAT3L:
    mfspr     r3, SPR_DBAT3L
    blr



.globl TRK_InterruptHandler
TRK_InterruptHandler:
    /*
     * Restore srr0/srr1 from r2/r4 & restore r4 from SPRG3
     */

    mtsrr0    r2
    mtsrr1    r4
    mfspr     r4, SPR_SPRG3

    /*
     * Be careful not to affect the CR until after it has been
     * saved.  The same applies once it has been restored.
     */

    mfcr      r2                /* save CR */
    mtspr     SPR_SPRG3, r2

    /*
    ** Restore the TRK MSR with the EE and RI bits cleared.  This
    ** ensures that any fields needed by the TRK are set correctly
    ** (especially the IR and DR fields), but it does not yet enable
    ** interrupts.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r2, 0x8C(r2)      /* load TRK MSR value */
    ori       r2, r2, 0x8002    /* set EE and RI bits */
    xori      r2, r2, 0x8002    /* change (clear) EE and RI bits */
    sync
    mtmsr     r2
    sync

#if TRK_TRANSPORT_INT_DRIVEN

    /****************************************************************
    ** If communication is interrupt-driven, check for the comm
    ** interrupt first.
    ****************************************************************/

    addis     r2, r0, TRK_saved_exceptionID@h
    ori       r2, r2, TRK_saved_exceptionID@l
    sth       r3, 0(r2)                        /* saved exception ID */

    cmpwi     r3, TRK_TRANSPORT_INT_KEY        /* compare with the key */
    bne       NonTransportInterrupt            /* if not equal, do standard */
                                               /* interrupt processing */

    addis     r2, r0, gTRKCPUState@h           /* save ra before calling handler */
    ori       r2, r2, gTRKCPUState@l
    mflr      r3
    stw       r3, 0x42C(r2)

    bl        TRKUARTInterruptHandler          /* call the transport handler */

    addis     r2, r0, gTRKCPUState@h           /* restore ra after calling handler */
    ori       r2, r2, gTRKCPUState@l
    lwz       r3, 0x42C(r2)
    mtlr      r3

    addis     r2, r0, gTRKState@h              /* get base addr of TRK state */
    ori       r2, r2, gTRKState@l
    lwz       r2, 0xA0(r2)                     /* get input pending ptr */
    lbz       r2, 0(r2)                        /* get input pending value */
    cmpwi     r2, 0
    beq       TransportReturn                  /* if no input, return from int */


    addis     r2, r0, gTRKExceptionStatus@h    /* base pointer to reentrance flag */
    ori       r2, r2, gTRKExceptionStatus@l
    lbz       r2, 0xC(r2)                      /* get re-entrance flag */
    cmpwi     r2, 1
    beq       TransportReturn                  /* if TRK already active, return from int */

    /*
    ** Input is pending and the TRK is not currently active.
    ** Activate it, but set the inputActivated flag so that it
    ** does not treat this like an unhandled interrupt in the
    ** target program.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    li        r3, 1                            /* set inputActivated flag true */
    stb       r3, 0x9C(r2)
    b         NonTransportInterrupt

    /*
    ** Return from the transport interrupt.
    */

TransportReturn:

    /*
    ** restore cr, r2 and r3
    */

    addis     r2, r0, gTRKSaveState@h
    ori       r2, r2, gTRKSaveState@l

    lwz       r3, 0x88(r2)
    mtcr      r3                               // restore CR

    lwz       r3, 12(r2)
    lwz       r2, 8(r2)

    rfi

    /****************************************************************
    ** This is not a comm interrupt.  Proceed with standard
    ** interrupt handler.
    ****************************************************************/

NonTransportInterrupt:

    /* Restore exception ID */

    addis     r2, r0, TRK_saved_exceptionID@h
    ori       r2, r2, TRK_saved_exceptionID@l
    lhz       r3, 0(r2)

#endif /* TRK_TRANSPORT_INT_DRIVEN */

    /*
     * Check for re-entrance.  If it occurs, go to separate exception
     * handler.
     */

    addis     r2, r0, gTRKExceptionStatus@h    /* base pointer to reentrance flag */
    ori       r2, r2, gTRKExceptionStatus@l

    lbz       r2, 0xC(r2)                      /* get re-entrance flag */
    cmpwi     r2, 0                            /* false indicates no re-entrance */

    bf        COND_EQ, TRKExceptionHandler

    /*
     * No re-entrance.  Continue with normal interrupt handler.
     */

    addis     r2, r0, gTRKCPUState@h           // base pointer to CPU state
    ori       r2, r2, gTRKCPUState@l

    // Save target program state

    // DEFAULT BLOCK

    stw       r0, 0(r2)                        // save r0
    stw       r1, 4(r2)                        // save r1
    mfspr     r0, SPR_SPRG1
    stw       r0, 8(r2)                        // save r2
    sth       r3, 0x2F8(r2)                    // save exceptionID (in r3)
    sth       r3, 0x2FA(r2)                    // in both upper & lower half

    mfspr     r0, SPR_SPRG2
    stw       r0, 12(r2)                       // save r3
    stmw      r4, 16(r2)                       // r4-r31

    mfspr     r27, SPR_SRR0                    // get pre-exception PC
    mfspr     r28, SPR_LR                      // get LR
    mfspr     r29, SPR_SPRG3                   // get saved CR
    mfspr     r30, SPR_CTR                     // get CTR
    mfspr     r31, SPR_XER                     // get XER
    stmw      r27, 0x80(r2)                    // store PC thru XER

    // Extended1 block is processor-specific, so call external func

    bl        TRKSaveExtended1Block

    /*
     * Set the re-entrance flag to indicate that CWDS is currently running.
     */

    addis     r2, r0, gTRKExceptionStatus@h    /* Base ptr of exception data */
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, 1                        /* set re-entrance flag to true */
    stb       r3, 0xC(r2)

    // Restore the TRK state

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l

    /*
    ** Note that external and recoverable interrupts are enabled
    ** once the TRK MSR value is restored.
    */

    lwz       r0, 0x8C(r2)
    sync
    mtmsr     r0
    sync

    lwz       r0, 0x80(r2)
    mtlr      r0

    lwz       r0, 0x84(r2)
    mtctr     r0

    lwz       r0, 0x88(r2)
    mtxer     r0

    lwz       r0, 0x94(r2)
    mtspr     SPR_DSISR, r0

    lwz       r0, 0x90(r2)
    mtspr     SPR_DAR, r0

    lmw       r3, 12(r2)                       // restore GPR3-31
    lwz       r0, 0(r2)
    lwz       r1, 4(r2)
    lwz       r2, 8(r2)

    b         TRKPostInterruptEvent            // call, then returning to TargetContinue



.globl TRKExceptionHandler
TRKExceptionHandler:
    /*
     * Should do something here that gives external sign we are in the
     * exception handler.  This code is minimal.  If another exception
     * occurs within it, recovery is most likely not possible.  It will
     * repeat the same exception eternally.
     */

    /*
     * Save off relevant info about the exception.
     */

    addis     r2, r0, gTRKExceptionStatus@h    /* base ptr of exception info */
    ori       r2, r2, gTRKExceptionStatus@l

    sth       r3, 8(r2)                        /* store exception id */

    mfspr     r3, SPR_SRR0                     /* get pre-exception PC */
    stw       r3, 0(r2)

    /*
     * Skip over the offending instruction if necessary.  It depends on the
     * cause of the exception.
     */

    lhz       r3, 8(r2)                        /* retrieve exception id */

    cmpwi     r3, PPC_MACHINECHECK             /* machine check - must skip instr */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_DATAACCESSERROR          /* data storage - must skip instr */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_INSTACCESSERROR          /* instruction storage - skip instr */
                                               /* although it probably will not help */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_ALIGNMENTERROR           /* alignment - must skip instr */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_PROGRAMERROR             /* program exception - skip instr */
                                               /* must do more checking if the target */
                                               /* supports floating point */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_FPUNAVAILABLE            /* floating pt unavailable - must skip instr */
    bt        COND_EQ, skip_instruction

#ifdef PPC_INSTR_SKIP_EXCEPTION1
    cmpwi     r3, PPC_INSTR_SKIP_EXCEPTION1    /* additional exception requiring instr skip */
    bt        COND_EQ, skip_instruction
#endif

#ifdef PPC_INSTR_SKIP_EXCEPTION2
    cmpwi     r3, PPC_INSTR_SKIP_EXCEPTION2    /* additional exception requiring instr skip */
    bt        COND_EQ, skip_instruction
#endif

#ifdef PPC_INSTR_SKIP_EXCEPTION3
    cmpwi     r3, PPC_INSTR_SKIP_EXCEPTION3    /* additional exception requiring instr skip */
    bt        COND_EQ, skip_instruction
#endif

#ifdef PPC_INSTR_SKIP_EXCEPTION4
    cmpwi     r3, PPC_INSTR_SKIP_EXCEPTION4    /* additional exception requiring instr skip */
    bt        COND_EQ, skip_instruction
#endif

    b         set_exception_flag               /* for all others do not skip an instruction */

skip_instruction:

    mfspr     r3, SPR_SRR0                     /* get pre-exception PC */
    addi      r3, r3, 4                        /* instruction size is 4 bytes - add to PC */
    mtspr     SPR_SRR0, r3                     /* new PC for exception return */

set_exception_flag:

    addis     r2, r0, gTRKExceptionStatus@h    /* base ptr of except flag */
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, 1
    stb       r3, 0xD(r2)                      /* set except flag to true */

    mfspr     r3, SPR_SPRG3                    /* restore CR */
    mtcrf     0xFF, r3

    mfspr     r2, SPR_SPRG1                    /* restore saved r2 */
    mfspr     r3, SPR_SPRG2                    /* restore saved r3 */

    rfi



.globl TRKSwapAndGo
TRKSwapAndGo:
    // Save TRK state in global "gTRKState"

    addis     r3, r0, gTRKState@h
    ori       r3, r3, gTRKState@l

    stmw      r0, 0(r3)

    mfmsr     r0
    stw       r0, 0x8C(r3)

    mflr      r0
    stw       r0, 0x80(r3)

    mfctr     r0
    stw       r0, 0x84(r3)

    mfxer     r0
    stw       r0, 0x88(r3)

    mfspr     r0, SPR_DSISR
    stw       r0, 0x94(r3)

    mfspr     r0, SPR_DAR
    stw       r0, 0x90(r3)

    /*
     * Disable external and recoverable interrupts while restoring
     * the user-code state.
     */

    addi      r1, r0, -0x7FFE                  /* get OR mask w/RI and EE bits on */
    nor       r1, r1, r1                       /* complement mask */
    mfmsr     r3
    and       r3, r3, r1                       /* AND w/MSR to turn off RI and EE bits */
    mtmsr     r3

#if TRK_TRANSPORT_INT_DRIVEN

    /*
    ** If any input has arrived while the TRK was in the process of
    ** returning to the target program, it needs to be re-activated.
    ** Otherwise, the TRK won't get that input until some other
    ** exception or interrupt occurs.  This is the appropriate place
    ** to check, since interrupts are masked now.  There will not
    ** be any more transport interrupts (and hence no more input
    ** arriving) until after the return to the target program.
    **
    ** Transport interrupts which occur after returning to the
    ** target program will automatically re-activate the TRK,
    ** so there is no need to worry about those.
    */

    addis     r2, r0, gTRKState@h              /* get addr of TRK state */
    ori       r2, r2, gTRKState@l
    lwz       r2, 0xA0(r2)                     /* get input pending ptr */
    lbz       r2, 0(r2)                        /* get input pending value */
    cmpwi     r2, 0
    beq       NoOutgoingInput                  /* if no input, continue return */
                                               /* to target program */

    /*
    ** Input is pending, so the TRK needs to be re-activated.
    ** Activate it, but set the inputActivated flag so that it
    ** does not treat this like an unhandled interrupt in the
    ** target program.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    li        r3, 1
    stb       r3, 0x9C(r2)

    b         TRKInterruptHandlerEnableInterrupts    /* re-activate TRK */

NoOutgoingInput:

#endif /* TRK_TRANSPORT_INT_DRIVEN */

    /*
     * clear re-entrance flag before returning to user code
     */

    addis     r2, r0, gTRKExceptionStatus@h
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, 0                        /* clear flag */
    stb       r3, 0xC(r2)

    bl        TRKRestoreExtended1Block

    // restore target program state from global "gTRKCPUState"

    addis     r2, r0, gTRKCPUState@h           // get addr of stored regs
    ori       r2, r2, gTRKCPUState@l

    // Restore all non-GPR's from the default block.

    lmw       r27, 0x80(r2)                    // store PC thru XER
    mtspr     SPR_SRR0, r27                    // restore as pre-exception PC
    mtspr     SPR_LR, r28                      // restore LR
    mtcr      r29                              // restore CR
    mtspr     SPR_CTR, r30                     // restore CTR
    mtspr     SPR_XER, r31                     // restore XER

    // Restore all GPR's

    lmw       r3, 12(r2)                       // restore GPR3-31
    lwz       r0, 0(r2)
    lwz       r1, 4(r2)
    lwz       r2, 8(r2)

    rfi



.globl TRKInterruptHandlerEnableInterrupts
TRKInterruptHandlerEnableInterrupts:
    /*
    ** Enable exceptions by restoring the MSR value from the TRK
    ** state, since exceptions are enabled during normal TRK
    ** operation.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r0, 0x8C(r2)
    sync
    mtmsr     r0
    sync

    lwz       r0, 0x80(r2)
    mtlr      r0

    lwz       r0, 0x84(r2)
    mtctr     r0

    lwz       r0, 0x88(r2)
    mtxer     r0

    lwz       r0, 0x94(r2)
    mtspr     SPR_DSISR, r0

    lwz       r0, 0x90(r2)
    mtspr     SPR_DAR, r0

    lmw       r3, 12(r2)                       // restore GPR3-31
    lwz       r0, 0(r2)
    lwz       r1, 4(r2)
    lwz       r2, 8(r2)

    b         TRKPostInterruptEvent            // call, then returning to TargetContinue



.globl ReadFPSCR
ReadFPSCR:
    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    mffs      f31
    stfd      f31, 0(r3)
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr



.globl WriteFPSCR
WriteFPSCR:
    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    lfd       f31, 0(r3)
    mtfsf     0xff, f31
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr

#endif
