##############################################################################
#
#    FILE
#        __exception.s
#
#    DESCRIPTION
#        Exception/interrupt handler table.
#
#    COPYRIGHT
#
#    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
#    All other product or service names are the property of their respective owners.
#    © Freescale Semiconductor, Inc. 2000-2006
#
#    (c) 2019 - 2023,        nitr8
#
#    HISTORY
#        97 MAR 20 LLY    Created.
#        98 JUL 23 SCM    Renamed and re-organized.
#        99 APR 01 SCM    Moved 1st 0x100 bytes to a separate file
#                         for the processors for which that space
#                         contains board-specific boot configuration
#                         data.
#        00 APR 24 SCM    Now uses 'rfi' instruction to jump to
#                         InterruptHandler rather than 'ba'.  Also,
#                         optionally enables bits in the MSR during
#                         the jump (e.g., IR & DR).  This removes
#                         all restrictions on the location of
#                         InterruptHandler in memory.
#        02 APR 14 Jan Kobler, jkobler@freescale.com
#                         to find changes search for Kobler
#
#
#   STATUS
#        100 % DECOMPILED
#        100 % HEX-BINARY MATCH
#
##############################################################################
#
#    This must be the first file in the link order in order to set .org
#    correctly for the exception vector (0x00000100 from start of ROM)
#
##############################################################################


#include "target_asm.h"


    .global gTRKInterruptVectorTable
    .global gTRKInterruptVectorTableEnd


    .extern TRK_InterruptHandler    # void TRK_InterruptHandler(u32 exceptNumber)
    .extern __TRKreset


#ifdef __GNUC__
    .section  .init,"ax",@progbits  # put this code in .init section
    .align 4
#else
    .section  .init,4,1,6           # put this code in .init section
#endif


    #
    # On some PPC variants, the first 0x100 bytes of ROM contain
    # boot configuration data that is board-specific.  For those
    # processors, get that data from "__boot_donfig.s" in the
    # board directory.
    #
    # On all others, put an ID string at the start of ROM.
    #

    # put this code in .init section
#ifdef __GNUC__
    .section  .init,"ax",@progbits
    .align 4
#else
    .section  .init,4,1,6
#endif

    .org      0x0

gTRKInterruptVectorTable:

    # add boot config contents
#if __PPC_BOOT_CONFIG__
    .include  "__boot_config.i"
#else // __PPC_BOOT_CONFIG__
    .asciz    "Metrowerks Target Resident Kernel for PowerPC"
#endif // __PPC_BOOT_CONFIG

    # put this code in .init section
#ifdef __GNUC__
    .section  .init,"ax",@progbits
    .align 4
#else
    .section  .init,4,1,6
#endif


##############################################################################
#
#    __ex_handler (macro)
#
#    This is the code used by most of the exception handlers.
#    It uses 'rfi' to jump to InterruptHandler while setting
#    any needed bits in the MSR (normally just IR & DR, if
#    any).  On entry to InterruptHandler, the registers will
#    be modified as follows:
#
#        r2              -> SPRG1
#        r3              -> SPRG2
#        r4              -> SPRG3
#        exception id    -> r3
#        srr0            -> r2
#        srr1            -> r4
#
##############################################################################
__ex_handler:    .macro    id
#ifdef __GNUC__
    mtspr     273, r2                           # r2 -> sprg1
    mtspr     274, r3                           # r3 -> sprg2
    mtspr     275, r4                           # r4 -> sprg3
#else
    mtspr     sprg1, r2                         # r2 -> sprg1
    mtspr     sprg2, r3                         # r3 -> sprg2
    mtspr     sprg3, r4                         # r4 -> sprg3
#endif

    mfsrr0    r2                                # srr0 -> r2
    mfsrr1    r4                                # srr1 -> r4
    mfmsr     r3
    ori       r3, r3, EXCEPTION_SET_MSR_BITS    # enable address xlation?
    mtsrr1    r3                                # put MSR s32 srr1

    # put InterruptHandler
#ifdef __GNUC__
    lis       r3, TRK_InterruptHandler@h
#else
    addis     r3, r0, TRK_InterruptHandler@h
#endif
    ori       r3, r3, TRK_InterruptHandler@l    # in srr0

    mtsrr0    r3

#ifndef __GNUC__
    addi      r3, r0, id                        # exception ID -> r3
    rfi                                         # jump to InterruptHandler
#endif

    .endm

##############################################################################
#
#    __disable_tgpr (macro)
#
#    If MSR:TGPR is set, clear it.
#    Modifies SPRG1 and SPRG2 only.
#
##############################################################################
__disable_tgpr:  .macro

#if __PPC_TGPR__
    #ifdef __GNUC__
        mtspr     273, r2
    #else
        mtspr     sprg1, r2
    #endif

        /* If TGPR is set, clear it */
        mfcr      r2

    #ifdef __GNUC__
        mtspr     274, r2
    #else
        mtspr     sprg2, r2
    #endif

        mfmsr     r2
        andis.    r2, r2, 0x0002
        beq       no_tgpr\@
        mfmsr     r2
        xoris     r2, r2, 0x0002
        sync
        mtmsr     r2
        sync

    #ifdef __GNUC__
        mtspr     273, r2
    #else
        mtspr     sprg1, r2
    #endif

no_tgpr\@:

    #ifdef __GNUC__
        mfspr     r2, 274
    #else
        mfspr     r2, sprg2
    #endif

        mtcr      r2

    #ifdef __GNUC__
        mfspr     r2, 273
    #else
        mfspr     r2, sprg1
    #endif
#endif

    .endm

##############################################################################
#
#    0x0100 System Reset
#
#    This is the first instruction to be executed after the hardware power-on
#    reset.
#
##############################################################################
    .org      0x100

    b         __TRKreset

##############################################################################
#
#    0x0200 Machine Check
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x200

    ### FLUSH AND INVALIDATE THE CACHES ###
    # These instructions are only valid on some PPC
    # processors.

#ifdef __GNUC__
    mtspr     273, r2
#else
    mtspr     sprg1, r2
#endif

#if __PPC_ICACHE__
    mfspr     r2, srr0                 # get the faulting instruction address
    icbi      0, r2                    # invalidate the cache block
#endif

#if __PPC_DCACHE__
    mfspr     r2, 19                   # get the faulting data from DAR -> r2
    dcbi      0, r2                    # invalidate the data cache block
#endif

#ifdef __GNUC__
    mfspr     r2, 273
#else
    mfspr     r2, sprg1
#endif

    __ex_handler 0x0200

#ifdef __GNUC__
    li        r3, 0x200                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0300    Data Storage
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x300

    __ex_handler 0x0300

#ifdef __GNUC__
    li        r3, 0x300                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0400    Instruction Storage
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x400

    __ex_handler 0x0400

#ifdef __GNUC__
    li        r3, 0x400                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0500    External Interrupt
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x500

    __ex_handler 0x0500

#ifdef __GNUC__
    li        r3, 0x500                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0600    Alignment
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x600

    __ex_handler 0x0600

#ifdef __GNUC__
    li        r3, 0x600                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0700    Program
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x700

    __ex_handler 0x0700

#ifdef __GNUC__
    li        r3, 0x700                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0800    Floating Point Unavailable
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x800

    __ex_handler 0x0800

#ifdef __GNUC__
    li        r3, 0x800                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0900    Decrementer Interrupt
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x900

    __ex_handler 0x0900

#ifdef __GNUC__
    li        r3, 0x900                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0C00    System Call
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0xC00

    __ex_handler 0x0c00

#ifdef __GNUC__
    li        r3, 0xC00                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0D00    Trace
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0xD00

    __ex_handler 0x0d00

#ifdef __GNUC__
    li        r3, 0xD00                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0E00    FP Assist
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0xE00

    __ex_handler 0x0e00

#ifdef __GNUC__
    li        r3, 0xE00                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x0F00    PPC740 and PPC750:      Performance Monitor
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0xF00
    b         __handler_0f00_cont  # not enough space here, continue elsewhere

##############################################################################
#
#    0x0F20    PPC7400:                AltiVec Unavailable Exception
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0xF20

    __ex_handler 0x0f20

#ifdef __GNUC__
    li        r3, 0xF20                # exception ID
    rfi                                # jump to InterruptHandler
#endif

__handler_0f00_cont:

    __ex_handler 0x0f00

#ifdef __GNUC__
    li        r3, 0xF00                # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1000     MPC8xx and MPC505:     Software Emulation
#               PPC603e:               Instruction TLB Miss
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1000

    __disable_tgpr
    __ex_handler 0x1000

#ifdef __GNUC__
    li        r3, 0x1000               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1100     MPC8xx:                Instruction TLB Miss
#               PPC603e:               Data Load TLB Miss
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1100

    __disable_tgpr
    __ex_handler 0x1100

#ifdef __GNUC__
    li        r3, 0x1100               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1200    MPC8xx:                 Data TLB Miss
#              PPC603e:                Data Store TLB Miss
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1200

    __disable_tgpr
    __ex_handler 0x1200

#ifdef __GNUC__
    li        r3, 0x1200               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1300    MPC8xx:                 Instruction TLB Error
#              PPC7xx and PPC603e:     Instruction address breakpoint
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1300

    __ex_handler 0x1300

#ifdef __GNUC__
    li        r3, 0x1300               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1400    MPC8xx:                 Data TLB Error
#              PPC7xx and PPC603e:     System management
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1400

    __ex_handler 0x1400

#ifdef __GNUC__
    li        r3, 0x1400               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1600    MPC7400 (Max processor): Denorm Dectection in Java mode.
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1600

    __ex_handler 0x1600

#ifdef __GNUC__
    li        r3, 0x1600               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1700    MPC7xx:                 Thermal Management
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1700

    __ex_handler 0x1700

#ifdef __GNUC__
    li        r3, 0x1700               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1C00    MPC8xx and MPC505:      Data breakpoint
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1C00

    __ex_handler 0x1c00

#ifdef __GNUC__
    li        r3, 0x1C00               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1D00    MPC8xx and MPC505:      Instruction breakpoint
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1D00

    __ex_handler 0x1d00

#ifdef __GNUC__
    li        r3, 0x1D00               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1E00    MPC8xx and MPC505:      Peripheral breakpoint
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1E00

    __ex_handler 0x1e00

#ifdef __GNUC__
    li        r3, 0x1E00               # exception ID
    rfi                                # jump to InterruptHandler
#endif

##############################################################################
#
#    0x1F00    MPC8xx and MPC505:      Non-maskable development port
#
#    Override the behavior by over-writing the handler code in RAM.
#
##############################################################################
    .org      0x1F00

    __ex_handler 0x1f00

#ifdef __GNUC__
    li        r3, 0x1F00               # exception ID
    rfi                                # jump to InterruptHandler
#endif

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    // Kobler
    // MGT5100: this section must have the size 0x3000-0x100
    // in order to ensure the correct layout of the ROM image
    .org      0x3000-1
    .byte     0
#endif

gTRKInterruptVectorTableEnd:

#ifdef __GNUC__
    .long     0, 0, 0
#endif
