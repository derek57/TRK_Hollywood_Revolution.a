/****************************************************************************/
/*

FILE

    targimpl.c

    $Header: /u/debugger/embedded/MetroTRK/Processor/M68K/GENERIC/targimpl.c,v 1.7 2000/07/06 20:41:00 smoore Exp $

DESCRIPTION

    Target-specific implementation for M 683xx (DragonBall)

HISTORY

    $History: $

    97 FEB 16 LLY    Created.
    97 JUN 09 SCM    Added support for exception detection.
                     Added support for M821-specific registers.
                     Added structure for DS state since it is much
                     smaller than the target program state.
    97 NOV xx SIJ    Customized for M 683xx ADS board.

AUTHORS

    (c) 1997-2000 Metrowerks Corporation
    All rights reserved.

COPYRIGHT

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


/*
 *    TRK includes
 */
#include "targimpl_ppc.h"
#include "nubassrt.h"
#include "notify.h"
#include "mslsupp.h"
#include "msg.h"
#include "msgxtrct.h"
#include "UART.h"
#include "support.h"
#include "memmap.h"
#include "mem_TRK.h"
#include "flush_cache.h"
#include "rvl_mem.h"
#include "asm_regs.h"
#include "trkstring.h"
#include "serpoll.h"

#if defined(HAVE_RVL_SDK) && !defined(__GNUC__)
    #include <revolution/os/OSThread.h>
#elif defined(__GNUC__)
    #include <ogc/lwp_threads.h>
#endif

// This is only required to match up with the hex binary data of the original object file
#include "dummy_d.h"


#define DEBUG_TARGIMPL  0


ASM_PREFIX void TRKInterruptHandlerEnableInterrupts();

ASM_PREFIX void TRKExceptionHandler(/*ui16 exceptionNumber*/);

void TRKPostInterruptEvent(void);

u32 ConvertAddress(u32 address);

DSError TRKPPCAccessFPRegister(void *value,
                        u32 register_num,
                        BOOL read);

DSError TRKPPCAccessSpecialReg(void *value,
                        u32 *access_instruction,
                        BOOL read);

DSError TRKPPCAccessPairedSingleRegister(void *value, u32 register_num, BOOL read);


static BOOL TRKTargetCheckStep(void);
static void GetThreadInfo(u32 *result, u32 *a2);


/****************************************************************************/
/*
 *    Target program's processor state (exception state)
 */
/****************************************************************************/

ProcessorState_PPC     gTRKCPUState;

/*
** Some registers are not normally restored to their pre-exception states
** by the TRK.  Typically all timer/counter registers fall into this
** category, since they will continue to change as the TRK runs.  They
** are only restored if they have been explicitly modified by the
** debugger.
*/

/****************************************************************************/
/*
 *    100 % DONE - Corrected initializer
 */
/****************************************************************************/
ProcessorRestoreFlags_PPC gTRKRestoreFlags =
{
    FALSE,
    FALSE,
    {
        0, 0, 0, 0, 0, 0, 0
    }
};

/* ProcessorRestoreFlags_PPC */
/****************************************************************************/
/*
 *    TRK processor state (prior to entering user code)
 */
/****************************************************************************/

TRKState_PPC             gTRKState;

/****************************************************************************/
/*
 *    Storage for info about exceptions that occur within the TRK
 *
 *    100 % DONE - Corrected alignment
 */
/****************************************************************************/
typedef struct
{
    ExceptionInfo_PPC    exceptionInfo;
    u8                   inTRK;
    u8                   exceptionDetected;

    //
    // Added to get around the compiler warning about padding
    //
    // The MetroWerks C-Compiler would still add 2 bytes of padding here
    //
    u8                   pad[2];
} ExceptionStatus;

/****************************************************************************/
/*
 *    100 % DONE - Corrected initializer
 */
/****************************************************************************/
#if defined(__MWERKS__)
static ExceptionStatus gTRKExceptionStatus =
#elif defined(__GNUC__) // externally used by GCC
ExceptionStatus gTRKExceptionStatus =
#endif
{
    { 0, 0, 0, { 0, 0 }, },
    TRUE,
    FALSE,
    { 0, 0 }
};

/****************************************************************************/
/*
 *    100 % DONE - Corrected alignment
 */
/****************************************************************************/
typedef struct
{
    BOOL active;
    DSMessageStepOptions type;
    u32 count;
    u32 rangeStart;
    u32 rangeEnd;

    //
    // Added to get around the compiler warning about padding
    //
    // The MetroWerks C-Compiler would still add 4 bytes of padding here
    //
    u8 pad[4];
} PpcStepStatus;

/****************************************************************************/
/*
 *    100 % DONE - Corrected initializer
 */
/****************************************************************************/
static PpcStepStatus gTRKStepStatus =
{
    FALSE,
    kDSStepIntoCount,
    0,
    0,
    0,
    { 0, 0, 0, 0 }
};


/*
** Shifter
*/
#define SHIFT_LEFT_X_BY_Y_BITS(x, y)             (x << y)
#define SHIFT_RIGHT_X_BY_Y_BITS(x, y)            (x >> y)

/*
** Instruction Builder
*/

/*
** Instruction   :        mfspr    rD, SPR
** (Move from Special Purpose Register)
**
** Bit  0..5     :        31         (decimal)
** Bit  6..10    :        rD
** Bit 11..20    :        SPR
** Bit 21..30    :        339        (decimal)
** Bit 31        :        0
*/
#define INSTR_MFSPR(rD, SPR)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rD, 21))            |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0xfe0), 6))  |    \
                                                 (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0x1f), 16))  |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(339, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction    :       mtspr    SPR, rS
** (Move to Special Purpose Register)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        rS
** Bit 11..20    :        SPR
** Bit 21..30    :        467        (decimal)
** Bit 31        :        0
*/
#define INSTR_MTSPR(SPR, rS)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rS, 21))            |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0xfe0), 6))  |    \
                                                 (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0x1f), 16))  |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(467, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        stw        rS, d(rA)
** (Store Word)
**
** Bit 0..5      :        36         (decimal)
** Bit 6..10     :        rS
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_STW(rS, d, rA)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(36, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rS, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        lwz        rD, d(rA)
** (Load Word and Zero)
**
** Bit 0..5      :        32         (decimal)
** Bit 6..10     :        rD
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_LWZ(rD, d, rA)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(32, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rD, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        stfd       frS, d(rA)
** (Store Floating-Point Double)
**
** Bit 0..5      :        54         (decimal)
** Bit 6..10     :        frS
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_STFD(frS, d, rA)  /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(54,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frS, 21))           |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA,  16))           |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        lfd        frD, d(rA)
** (Load Floating-Point Double)
**
** Bit 0..5      :        50         (decimal)
** Bit 6..10     :        frD
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_LFD(frD, d, rA)   /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(50,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frD, 21))           |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA,  16))           |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        mffs       frD
** (Move from FPSCR)
**
** Bit 0..5      :        63         (decimal)
** Bit 6..10     :        frD
** Bit 11..20    :        0
** Bit 21..30    :        583        (decimal)
** Bit 31        :        0
*/
#define INSTR_MFFS(frD)         /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(63, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frD, 21))           |    \
                                /* Bit 11..20 */ 0                                           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(583, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        mtfsf      FM, frB
** (Move to FPSCR fields)
**
** Bit 0..5      :        63         (decimal)
** Bit 6         :        0
** Bit 7..14     :        FM
** Bit 15        :        0
** Bit 16..20    :        frB
** Bit 21..30    :        711        (decimal)
** Bit 31        :        0
*/
#define INSTR_MTFSF(FM,frB)     /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(63, 26))            |    \
                                /* Bit  6     */ 0                                           |    \
                                /* Bit  7..14 */ (SHIFT_LEFT_X_BY_Y_BITS(FM, 17))            |    \
                                /* Bit  15    */ 0                                           |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(frB, 11))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(711, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        stvx       vS, rA, rB
** (Store Vector Indexed)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        vS
** Bit 11..15    :        rA
** Bit 16..20    :        rB
** Bit 21..30    :        231
** Bit 31        :        0
*/
#define INSTR_STVX(vS, rA, rB)  /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vS, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(rB, 11))            |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(231, 1))            |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        lvx        vD, rA, rB
** (Load Vector Indexed)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        vS
** Bit 11..15    :        rA
** Bit 16..20    :        rB
** Bit 21..30    :        103
** Bit 31        :        0
*/
#define INSTR_LVX(vD, rA, rB)   /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vD, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(rB, 11))            |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(103, 1))            |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        mfvscr     vD
** (Move from Vector Status and Control Register)
**
** Bit 0..5      :        4          (decimal)
** Bit 6..10     :        vD
** Bit 11..20    :        0
** Bit 21..30    :        1540       (decimal)
** Bit 31        :        0
*/
#define INSTR_MFVSCR(vD)        /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS( 4,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vD,  21))           |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS( 0,   0))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(1540, 0))           |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        mtvscr     vB
** (Move to Vector Status and Control Register)
**
** Bit 0..5      :        4          (decimal)
** Bit 6..10     :        vB
** Bit 11..20    :        0
** Bit 21..30    :        1604       (decimal)
** Bit 31        :        0
*/
#define INSTR_MTVSCR(vB)        /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS( 4,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vB,  21))           |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS( 0,   0))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(1604, 0))           |    \
                                /* Bit 31     */ 0

/*
** Instruction    :        blr
** (Branch to Link Register)
**
*/
#define INSTR_BLR                0x4E800020

/*
** Instruction    :        nop
** (No op instruction)
**
*/
#define INSTR_NOP                0x60000000


// MOVED TO TARGIMPL_ASM.S FOR GCC
#ifdef __MWERKS__

/*
** Functions to get SPR values from C.
*/

/****************************************************************************/
/*
 *    __TRK_get_MSR
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_MSR(void)
{
    nofralloc
    mfmsr    r3
    blr
}

/****************************************************************************/
/*
 *    __TRK_set_MSR
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX void __TRK_set_MSR(register u32 val)
{
    nofralloc
    mtmsr    val
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_PVR
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_PVR(void)
{
    nofralloc
    mfspr    r3, SPR_PVR
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT0U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT0U(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT0U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT0L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT0L(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT0L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT1U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT1U(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT1U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT1L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT1L(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT1L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT2U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT2U(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT2U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT2L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT2L(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT2L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT3U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT3U(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT3U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_IBAT3L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_IBAT3L(void)
{
    nofralloc
    mfspr    r3, SPR_IBAT3L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT0U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT0U(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT0U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT0L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT0L(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT0L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT1U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT1U(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT1U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT1L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT1L(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT1L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT2U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT2U(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT2U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT2L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT2L(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT2L
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT3U
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT3U(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT3U
    blr
}

/****************************************************************************/
/*
 *    __TRK_get_DBAT3L
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX u32 __TRK_get_DBAT3L(void)
{
    nofralloc
    mfspr    r3, SPR_DBAT3L
    blr
}
#endif

/****************************************************************************/
/*
 *    TRKValidMemory32
 *
 *    this should to a memory test without crashing
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKValidMemory32(const void *addr, size_t length, ValidMemoryOptions readWriteable)
{
    DSError err = kInvalidMemory;            /* assume range is invalid */

    const u8 *start;
    const u8 *end;

    s32 i;

    /*
    ** Get start and end addresses for the memory range and
    ** verify that they are reasonable.
    */

    start = (const u8 *)addr;
    end = ((const u8 *)addr + (length - 1));

    if (end < start)
        return kInvalidMemory;

    /*
    ** Iterate through the gTRKMemMap array to determine if the requested
    ** range falls within the valid ranges in the map.
    */

    for (i = 0; (i < (s32)(sizeof(gTRKMemMap) / sizeof(memRange))); i++)
    {
        /*
        ** If the requested range is not completely above
        ** the valid range AND it is not completely below
        ** the valid range then it must overlap somewhere.
        ** If the requested range overlaps with one of the
        ** valid ranges, do some additional checking.
        **
        */

        if ((start <= (const u8 *)gTRKMemMap[i].end) && (end >= (const u8 *)gTRKMemMap[i].start))
        {
            /*
            ** First, verify that the read/write attributes are
            ** acceptable.  If so, then recursively check any
            ** part of the requested range that falls before or
            ** after the valid range.
            */

            if (((readWriteable == kValidMemoryReadable) && !gTRKMemMap[i].readable) ||
                ((readWriteable == kValidMemoryWriteable) && !gTRKMemMap[i].writeable))
            {
                err = kInvalidMemory;
            }
            else
            {
                err = kNoError;

                /*
                ** If a portion of the requested range falls before
                ** the current valid range, then recursively
                ** check it.
                */

                if (start < (const u8 *)gTRKMemMap[i].start)
                    err = TRKValidMemory32(start, (u32)((const u8 *)gTRKMemMap[i].start - start), readWriteable);

                /*
                ** If a portion of the requested range falls after
                ** the current valid range, then recursively
                ** check it.
                ** Note: Only do this step if the previous check
                ** did not detect invalid access.
                */

                if ((err == kNoError) && (end > (const u8 *)gTRKMemMap[i].end))
                    err = TRKValidMemory32((const u8 *)gTRKMemMap[i].end, (u32)(end - (const u8 *)gTRKMemMap[i].end), readWriteable);

            }

            break;
        }
    }

    return err;
}


/****************************************************************************/


#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    #include "ppc_mem.h"
#endif


/****************************************************************************/
/*
 *    TRK_ppc_memcpy
 *
 *    Performs the same function as the standard memcpy(), but uses
 *    the src_MSR value in the MSR register when reading from src
 *    memory and the dest_MSR value in the MSR register when writing
 *    to dest memory.
 *
 *    The purpose is to allow reading and writing of target program
 *    memory when the target program uses address translation.
 *
 *    r3 <- dest
 *    r4 <- src
 *    r5 <- len
 *    r6 <- dest_MSR
 *    r7 <- src_MSR
 *
 *    NOTE: This function originally came as pure PPC assembly code and has
 *          now been converted to native C code instead for portability for
 *          the Wii / NDEV.
 *
 *    100 % DONE
 */
/****************************************************************************/
static void TRK_ppc_memcpy(void *dst, const void *src, u32 length, u32 dst_MSR, u32 src_MSR)
{
    u32 msr;
    u8 *srcTmp = (u8 *)src;
    u8 *dstTmp = (u8 *)dst;

    msr = __TRK_get_MSR();

    while (length != 0)
    {
        u8 byte;

        __TRK_set_MSR(src_MSR);
        byte = ppc_readbyte1(srcTmp);
        asm("sync");

        __TRK_set_MSR(dst_MSR);
        ppc_writebyte1(dstTmp, byte);
        asm("sync");

        srcTmp++;
        dstTmp++;
        length--;
    }

    __TRK_set_MSR(msr);
}

/****************************************************************************/
/*
 *    TRKTargetAccessMemory
 *
 *    Read/write memory, testing for accessability
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetAccessMemory(void *Data,
                        void *virtualAddress,
                        size_t *memorySize,
                        MemoryAccessOptions accessOptions,
                        BOOL read)
{
    DSError err;
    ExceptionStatus savedException;
    void *transAddress;

    /*
    ** Save any existing exception status and clear the exception flag.
    ** This allows detection of exceptions that occur ONLY within this
    ** function.
    */

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    /*
    ** Need to translate virtual address to useable address on
    ** the target.
    */

    transAddress = TRKTargetTranslate((u32 *)virtualAddress);

    err = TRKValidMemory32(transAddress, *memorySize,
                        read ? kValidMemoryReadable : kValidMemoryWriteable);

    if (err != kNoError)
    {
        *memorySize = 0;                        /* no data access */
    }
    else
    {
        /*
        ** The TRK buffer should be accessed using the current TRK
        ** MSR.  The target program memory should be accessed using
        ** the target-program's value of the DR bit in the MSR.
        ** If the target-program is using address translation, then
        ** that translation will apply to the transAddress.
        */

        u32 trk_msr = __TRK_get_MSR();
        u32 target_msr = (trk_msr | (gTRKCPUState.Extended1.MSR & MSR_DR));  // MSR

        if (read)
        {
            TRK_ppc_memcpy(Data, (const void *)transAddress, (size_t)*memorySize,
                        trk_msr, target_msr);
        }
        else
        {
            TRK_ppc_memcpy(transAddress, (const void *)Data, (u32)*memorySize,
                        target_msr, trk_msr);

            TRK_flush_cache((u32)transAddress, (u32)*memorySize);

            if (virtualAddress != transAddress)
                TRK_flush_cache((u32)virtualAddress, (u32)(*memorySize));
        }

        if (accessOptions == kUserMemory)
        {
            /* make breakpoints invisible */

            /*
             *    if TRK handles breakpoints, then put the original
             *    instructions back
             */
        }

#if DEBUG_TARGIMPL // Kobler: provides debug information in case of exception
        __puts("Internal Exception ");
        __puthex8(gTRKExceptionStatus.exceptionInfo.exceptionID);
        __puts(" at PC ");
        __puthex8(gTRKExceptionStatus.exceptionInfo.PC);
        __puts("\r\n");
#endif
    }

    /*
    ** Check for exceptions and restore the previous exception status.
    */

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *memorySize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetReadInstruction
 *
 *    Read instruction from memory, determining instruction size.  The
 *    storage pointed to by readData must be big enough to store the
 *    largest possible instruction.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetReadInstruction(InstructionType *readData,
                                void *virtualAddress)
{
    DSError err = kNoError;
    size_t length = sizeof(InstructionType);

    /*
     * Determine instruction size and store in destination.
     * Instructions on PPC are 32 bits.
     */

    err = TRKTargetAccessMemory(readData, virtualAddress, &length, kUserMemory, TRUE);

    /*
     * Verify that an entire instruction was read.
     */

    if ((err == kNoError) && (length != sizeof(InstructionType)))
        err = kInvalidMemory;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetAccessDefault
 *
 *    R/W a range in the default register block.  W/R the registers in the
 *    range directly to/from the given message buffer. Returns the size of the
 *    requested range in bytes.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister,
                          MessageBuffer *b, size_t *registerStorageSize,
                          BOOL read)
{
    DSError err;
    DefaultType *start;
    u32 count;
    ExceptionStatus savedException;

    if (lastRegister > TRK_DEFAULT_XER)
        return kInvalidRegister;

    /*
    ** Save any existing exception status and clear the exception flag.
    ** This allows detection of exceptions that occur ONLY within this
    ** function.
    */

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    /*
    ** Get a pointer to the start of the requested register range and
    ** calculate the size of the range in bytes.
    **
    ** The default regiters for the PPC are all the same type, so
    ** it is OK to use C pointer arithmetic.
    */

    start = (((DefaultType *)&gTRKCPUState.Default) + firstRegister);
    count = (lastRegister - firstRegister + 1);
    *registerStorageSize = (count * sizeof(DefaultType));

    if (read)
        err = MessageAddBlock_ui32(b, start, (s32)count);
    else
        err = MessageGetBlock_ui32(b, start, (s32)count);

    /*
    ** Check for exceptions and restore the previous exception status.
    */

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetAccessFP
 *
 *    R/W a range in the float register block.  W/R the registers in the
 *    range directly to/from the given message buffer. Returns the size of the
 *    requested range in bytes.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister,
                          MessageBuffer *b, size_t *registerStorageSize,
                          BOOL read)
{
#if __PPC_FPU__

    DSError err;
    u32 current;
    FloatType value;
    ExceptionStatus savedException;

    if (lastRegister > TRK_DEFAULT_LR)
        return kInvalidRegister;

    /*
    ** Save any existing exception status and clear the exception flag.
    ** This allows detection of exceptions that occur ONLY within this
    ** function.
    */

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    /*
    ** Before accessing the floating point registers, need to make
    ** sure that floating point is enabled (in MSR register).
    */

    __TRK_set_MSR(__TRK_get_MSR() | MSR_FP);

    /*
    ** The floating point registers are not stored in memory, so
    ** they must be accessed one at a time from the CPU.
    **
    */

    for (current = firstRegister, *registerStorageSize = 0, err = kNoError;
         (current <= lastRegister) && (err == kNoError);
         *registerStorageSize += sizeof(FloatType), current++)
    {
        /*
        ** Read/write the register value to/from the message buffer.
        */

        if (read)
        {
            err = TRKPPCAccessFPRegister((void *)&value, current, read);

            err = MessageAdd_ui64(b, value);
        }
        else
        {
            err = MessageGet_ui64(b, &value);

            err = TRKPPCAccessFPRegister((void *)&value, current, read);
        }
    }

    /*
    ** Check for exceptions and restore the previous exception status.
    */

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;

#else /* ! __PPC_FPU__ */

#if defined(__MWERKS__)
    #pragma unused(firstRegister, lastRegister, b, read)
#elif defined(__GNUC__)
    UNUSED(firstRegister);
    UNUSED(lastRegister);
    UNUSED(b);
    UNUSED(read);
#endif

    *registerStorageSize = 0;

    return kUnsupportedError;

#endif /* __PPC_FPU__ */
}

/****************************************************************************/
/*
 *    TRKTargetAccessExtended1
 *
 *    R/W a range in the extended1 register block.  W/R the registers in the
 *    range directly to/from the given message buffer. Returns the size of the
 *    requested range in bytes.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister,
                             MessageBuffer *b, size_t *registerStorageSize,
                             BOOL read)
{
    DSError err;
    DefaultType *start;
    s32 count;
    ExceptionStatus savedException;

    if (lastRegister > DS_EXTENDED1_MAX_REGISTER_6xx_7xx)
        return kInvalidRegister;

    /*
    ** Save any existing exception status and clear the exception flag.
    ** This allows detection of exceptions that occur ONLY within this
    ** function.
    */

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    *registerStorageSize = 0;

    /*
    ** Get a pointer to the start of the requested register range and
    ** calculate the size of the range in bytes.
    **
    ** All but one of the extended1 registers are the same type or
    ** padded so that it is OK to use C pointer arithmetic.
    */

    if (firstRegister <= lastRegister)
    {
        start = (((Extended1Type *)&gTRKCPUState.Extended1) + firstRegister);
        count = (s32)(lastRegister - firstRegister + 1);

        *registerStorageSize += (count * sizeof(Extended1Type));

        if (read)
        {
            err = MessageAddBlock_ui32(b, start, (s32)count);
        }
        else
        {
            if ((start <= (Extended1Type *)&gTRKCPUState.Extended1.TBU) &&   // TBU
                ((start + count - 1) >=
                    (Extended1Type *)&gTRKCPUState.Extended1.TBL))  // TBL
                gTRKRestoreFlags.TBR = TRUE;

            if ((start <= (Extended1Type *)&gTRKCPUState.Extended1.DEC) &&    // DEC
                ((start + count - 1) >=
                    (Extended1Type *)&gTRKCPUState.Extended1.DEC)) // DEC
                gTRKRestoreFlags.DEC = TRUE;

            err = MessageGetBlock_ui32(b, start, (s32)count);
        }
    }

    /*
    ** Check for exceptions and restore the previous exception status.
    */

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetAccessExtended2
 *
 *    R/W a range in the extended2 register block.  W/R the registers in the
 *    range directly to/from the given message buffer. Returns the size of the
 *    requested range in bytes.
 *
 *    NOTE: This function is very special as it usually was only thought to be
 *          of use for accessing PPC ALTIVEC registers but instead (for the
 *          Wii / NDEV) it was used to access registers in the "Extended 2"
 *          block.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
#if __PPC_ALTIVEC__
    #pragma altivec_model on
    vector u32 value128;
    vector u32 TRKvalue128_temp;
#else
    u8 TRKvalue128_temp[16];
#endif


/*
** Useful defines.
*/
#define ALTIVEC_VECTOR_REG_ACCESS_MIN    0
#define ALTIVEC_VECTOR_REG_ACCESS_MAX    31
#define ALTIVEC_VSCR_ACCESS              32
#define ALTIVEC_VRSAVE_SPR_ACCESS        33


DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister,
                             MessageBuffer *b, size_t *registerStorageSize,
                             BOOL read)
{
    DSError err;
    FloatType value64;  // (64 bits)
    u32 value32[2];     // (32 bits)
    ExceptionStatus savedException;

    if (lastRegister > ALTIVEC_VECTOR_REG_ACCESS_MAX)
        return kInvalidRegister;

    /*
    ** Save any existing exception status and clear the exception flag.
    ** This allows detection of exceptions that occur ONLY within this
    ** function.
    */

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    TRKPPCAccessSPR((void *)value32, SPR_HID2, TRUE);

    value32[0] |= 0xA0000000;
    TRKPPCAccessSPR((void *)value32, SPR_HID2, FALSE);

    value32[0] = 0;
    TRKPPCAccessSPR((void *)value32, SPR_GQR0, FALSE);

    *registerStorageSize = 0;
    err = kNoError;

    while ((firstRegister <= lastRegister) && !err)
    {
        if (read)
        {
            TRKPPCAccessPairedSingleRegister((void *)&value64, firstRegister, read);
            err = MessageAdd_ui64(b, value64);
        }
        else
        {
            MessageGet_ui64(b, &value64);
            err = TRKPPCAccessPairedSingleRegister((void *)&value64, firstRegister, read);
        }

        ++firstRegister;
        *registerStorageSize += sizeof(FloatType);
    }

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetVersions
 *
 *    Fill in version struct for this target.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetVersions(DSVersions *versions)
{
    versions->kernelMajor = DS_KERNEL_MAJOR_VERSION;
    versions->kernelMinor = DS_KERNEL_MINOR_VERSION;
    versions->protocolMajor = DS_PROTOCOL_MAJOR_VERSION;
    versions->protocolMinor = DS_PROTOCOL_MINOR_VERSION;

    return kNoError;
}

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKTargetSupportMask
 *
 *    Fill in support mask for this target.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetSupportMask(DSSupportMask *mask)
{
    (*mask)[0] = DS_SUPPORT_MASK_00_07;
    (*mask)[1] = DS_SUPPORT_MASK_08_0F;
    (*mask)[2] = DS_SUPPORT_MASK_10_17;
    (*mask)[3] = DS_SUPPORT_MASK_18_1F;
    (*mask)[4] = DS_SUPPORT_MASK_20_27;
    (*mask)[5] = DS_SUPPORT_MASK_28_2F;
    (*mask)[6] = DS_SUPPORT_MASK_30_37;
    (*mask)[7] = DS_SUPPORT_MASK_38_3F;
    (*mask)[8] = DS_SUPPORT_MASK_40_47;
    (*mask)[9] = DS_SUPPORT_MASK_48_4F;
    (*mask)[10] = DS_SUPPORT_MASK_50_57;
    (*mask)[11] = DS_SUPPORT_MASK_58_5F;
    (*mask)[12] = DS_SUPPORT_MASK_60_67;
    (*mask)[13] = DS_SUPPORT_MASK_68_6F;
    (*mask)[14] = DS_SUPPORT_MASK_70_77;
    (*mask)[15] = DS_SUPPORT_MASK_78_7F;
    (*mask)[16] = DS_SUPPORT_MASK_80_87;
    (*mask)[17] = DS_SUPPORT_MASK_88_8F;
    (*mask)[18] = DS_SUPPORT_MASK_90_97;
    (*mask)[19] = DS_SUPPORT_MASK_98_9F;
    (*mask)[20] = DS_SUPPORT_MASK_A0_A7;
    (*mask)[21] = DS_SUPPORT_MASK_A8_AF;
    (*mask)[22] = DS_SUPPORT_MASK_B0_B7;
    (*mask)[23] = DS_SUPPORT_MASK_B8_BF;
    (*mask)[24] = DS_SUPPORT_MASK_C0_C7;
    (*mask)[25] = DS_SUPPORT_MASK_C8_CF;
    (*mask)[26] = DS_SUPPORT_MASK_D0_D7;
    (*mask)[27] = DS_SUPPORT_MASK_D8_DF;
    (*mask)[28] = DS_SUPPORT_MASK_E0_E7;
    (*mask)[29] = DS_SUPPORT_MASK_E8_EF;
    (*mask)[30] = DS_SUPPORT_MASK_F0_F7;
    (*mask)[31] = DS_SUPPORT_MASK_F8_FF;

    return kNoError;
}
#endif

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKTargetCPUType
 *
 *    Fill in cpu type struct for this target.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetCPUType(DSCPUType *cpuType)
{
    cpuType->cpuMajor = DS_CPU_MAJOR_TYPE;
    cpuType->cpuMinor = DS_CPU_MINOR_TYPE;
    cpuType->bigEndian = gTRKBigEndian;
    cpuType->defaultTypeSize = sizeof(DefaultType);
    cpuType->fpTypeSize = sizeof(FloatType);
    cpuType->extended1TypeSize = sizeof(Extended1Type);
#ifdef TRK_EXTENDED2_SIZE
    cpuType->extended2TypeSize = TRK_EXTENDED2_SIZE;
#else
    cpuType->extended2TypeSize = 0;
#endif

    return kNoError;
}
#endif

/****************************************************************************/
/*
 *    TRKTargetCheckException
 *
 *    Determine whether an exception has occurred within the DS.  Clears the
 *    state so that successive calls will report no exceptions until another
 *    one occurs.
 *
 *    This function can be used to reset the exception status.
 *
 *    Returns:
 *      kNoError: No exception has occurred
 *      kCWDSException: Exception has occurred.  In this case, a successive
 *        call to TargetAddExceptionInfo will place the information pertaining
 *        to the exception in a message buffer.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetCheckException(void)
{
    DSError err = kNoError;

    if (gTRKExceptionStatus.exceptionDetected)
    {
        gTRKExceptionStatus.exceptionDetected = FALSE;

        err = kCWDSException;
        __puts("Internal Exception ");
        __puthex8(gTRKExceptionStatus.exceptionInfo.exceptionID);
        __puts(" at PC ");
        __puthex8(gTRKExceptionStatus.exceptionInfo.PC);
        __puts("\r\n");
    }

    return err;
}

#if defined(__MWERKS__)
    static u16 TRK_saved_exceptionID = 0;
#elif defined(__GNUC__) // externally used by GCC
    u16 TRK_saved_exceptionID = 0;
#endif

Default_PPC gTRKSaveState;

// MOVED TO TARGIMPL_ASM.S FOR GCC
#ifdef __MWERKS__
/****************************************************************************/
/*
 *    TRK_InterruptHandler
 *
 *
 *    Assumes that some registers have been saved:
 *        r2                 -> SPRG1
 *        r3                 -> SPRG2
 *        r4                 -> SPRG3
 *        exception id       -> r3
 *        srr0               -> r2
 *        srr1               -> r4
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *          It takes 1 argument (ui16 exceptionNumber)
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX void TRK_InterruptHandler(/*ui16 exceptionNumber*/)
{
    nofralloc

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

    mfcr      r2                           /* save CR */
    mtspr     SPR_SPRG3, r2

    /*
    ** Restore the TRK MSR with the EE and RI bits cleared.  This
    ** ensures that any fields needed by the TRK are set correctly
    ** (especially the IR and DR fields), but it does not yet enable
    ** interrupts.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r2, TRKState_PPC.MSR(r2)     /* load TRK MSR value */
    ori       r2, r2, (MSR_EE | MSR_RI)    /* set EE and RI bits */
    xori      r2, r2, (MSR_EE | MSR_RI)    /* change (clear) EE and RI bits */
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
    stw       r3, ProcessorState_PPC.transport_handler_saved_ra(r2)   // transport_handler_saved_ra

    bl        TRKUARTInterruptHandler          /* call the transport handler */

    addis     r2, r0, gTRKCPUState@h           /* restore ra after calling handler */
    ori       r2, r2, gTRKCPUState@l
    lwz       r3, ProcessorState_PPC.transport_handler_saved_ra(r2)   // transport_handler_saved_ra
    mtlr      r3

    addis     r2, r0, gTRKState@h              /* get base addr of TRK state */
    ori       r2, r2, gTRKState@l
    lwz       r2, TRKState_PPC.inputPendingPtr(r2) /* get input pending ptr */
    lbz       r2, 0(r2)                        /* get input pending value */
    cmpwi     r2, FALSE
    beq       TransportReturn                  /* if no input, return from int */

    addis     r2, r0, gTRKExceptionStatus@h    /* base pointer to reentrance flag */
    ori       r2, r2, gTRKExceptionStatus@l
    lbz       r2, ExceptionStatus.inTRK(r2)    /* get re-entrance flag */
    cmpwi     r2, TRUE
    beq       TransportReturn                  /* if TRK already active, return from int */

    /*
    ** Input is pending and the TRK is not currently active.
    ** Activate it, but set the inputActivated flag so that it
    ** does not treat this like an unhandled interrupt in the
    ** target program.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    li        r3, TRUE                         /* set inputActivated flag true */
    stb       r3, TRKState_PPC.inputActivated(r2)
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

    lwz       r3, Default_PPC.CR(r2)
    mtcr      r3                        // restore CR

    lwz       r3, Default_PPC.GPR[3](r2)
    lwz       r2, Default_PPC.GPR[2](r2)

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

    lbz       r2, ExceptionStatus.inTRK(r2)    /* get re-entrance flag */
    cmpwi     r2, FALSE                        /* false indicates no re-entrance */

    bf        COND_EQ, TRKExceptionHandler

    /*
     * No re-entrance.  Continue with normal interrupt handler.
     */

    addis     r2, r0, gTRKCPUState@h        // base pointer to CPU state
    ori       r2, r2, gTRKCPUState@l

    // Save target program state

    // DEFAULT BLOCK
    stw       r0, ProcessorState_PPC.Default.GPR[0](r2)    // save r0
    stw       r1, ProcessorState_PPC.Default.GPR[1](r2)    // save r1
    mfspr     r0, SPR_SPRG1
    stw       r0, ProcessorState_PPC.Default.GPR[2](r2)    // save r2
    sth       r3, ProcessorState_PPC.Extended1.exceptionID(r2)    // save exceptionID (in r3)
    sth       r3, ProcessorState_PPC.Extended1.exceptionID+2(r2)  // in both upper & lower half

    mfspr     r0, SPR_SPRG2
    stw       r0, ProcessorState_PPC.Default.GPR[3](r2)    // save r3
    stmw      r4, ProcessorState_PPC.Default.GPR[4](r2)    // r4-r31

    mfspr     r27, SPR_SRR0           // get pre-exception PC
    mfspr     r28, SPR_LR             // get LR
    mfspr     r29, SPR_SPRG3          // get saved CR
    mfspr     r30, SPR_CTR            // get CTR
    mfspr     r31, SPR_XER            // get XER
    stmw      r27, ProcessorState_PPC.Default.PC(r2)       // store PC thru XER

    // Extended1 block is processor-specific, so call external func
    bl        TRKSaveExtended1Block

    /*
     * Set the re-entrance flag to indicate that CWDS is currently running.
     */

    addis     r2, r0, gTRKExceptionStatus@h /* Base ptr of exception data */
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, TRUE                  /* set re-entrance flag to true */
    stb       r3, ExceptionStatus.inTRK(r2)

    // Restore the TRK state
    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l

    /*
    ** Note that external and recoverable interrupts are enabled
    ** once the TRK MSR value is restored.
    */

    lwz       r0, TRKState_PPC.MSR(r2)
    sync
    mtmsr     r0
    sync

    lwz       r0, TRKState_PPC.LR(r2)
    mtlr      r0

    lwz       r0, TRKState_PPC.CTR(r2)
    mtctr     r0

    lwz       r0, TRKState_PPC.XER(r2)
    mtxer     r0

    lwz       r0, TRKState_PPC.DSISR(r2)
    mtspr     SPR_DSISR, r0

    lwz       r0, TRKState_PPC.DAR(r2)
    mtspr     SPR_DAR, r0

    lmw       r3, TRKState_PPC.GPR[3](r2)        // restore GPR3-31
    lwz       r0, TRKState_PPC.GPR[0](r2)
    lwz       r1, TRKState_PPC.GPR[1](r2)
    lwz       r2, TRKState_PPC.GPR[2](r2)

    b         TRKPostInterruptEvent        // call, then returning to TargetContinue
}

/****************************************************************************/
/*
 *    TRKExceptionHandler
 *
 *    Assumes that some registers have been saved:
 *        r2 -> SPRG1
 *        r3 -> SPRG2
 *        CR -> SPRG3
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX void TRKExceptionHandler(/* u16 exceptNumber */)
{
    nofralloc                        /* no frame */

    /*
     * Should do something here that gives external sign we are in the
     * exception handler.  This code is minimal.  If another exception
     * occurs within it, recovery is most likely not possible.  It will
     * repeat the same exception eternally.
     */

    /*
     * Save off relevant info about the exception.
     */

    addis     r2, r0, gTRKExceptionStatus@h          /* base ptr of exception info */
    ori       r2, r2, gTRKExceptionStatus@l

    sth       r3, ExceptionStatus.exceptionInfo.exceptionID(r2)    /* store exception id */

    mfspr     r3, SPR_SRR0                           /* get pre-exception PC */
    stw       r3, ExceptionStatus.exceptionInfo.PC(r2)

    /*
     * Skip over the offending instruction if necessary.  It depends on the
     * cause of the exception.
     */

    lhz       r3, ExceptionStatus.exceptionInfo.exceptionID(r2)    /* retrieve exception id */

    cmpwi     r3, PPC_MACHINECHECK                   /* machine check - must skip instr */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_DATAACCESSERROR                /* data storage - must skip instr */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_INSTACCESSERROR                /* instruction storage - skip instr */
                                                     /* although it probably will not help */
    bt        COND_EQ, skip_instruction

    cmpwi     r3, PPC_ALIGNMENTERROR                 /* alignment - must skip instr */
    bt        COND_EQ, skip_instruction
                                                     /* program exception - skip instr */
    cmpwi     r3, PPC_PROGRAMERROR                   /* must do more checking if the target */
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

    b         set_exception_flag      /* for all others do not skip an instruction */

skip_instruction:

    mfspr     r3, SPR_SRR0            /* get pre-exception PC */
    addi      r3, r3, 4               /* instruction size is 4 bytes - add to PC */
    mtspr     SPR_SRR0, r3            /* new PC for exception return */

set_exception_flag:

    addis     r2, r0, gTRKExceptionStatus@h             /* base ptr of except flag */
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, TRUE
    stb       r3, ExceptionStatus.exceptionDetected(r2) /* set except flag to true */

    mfspr     r3, SPR_SPRG3            /* restore CR */
    mtcrf     0xFF, r3

    mfspr     r2, SPR_SPRG1            /* restore saved r2 */
    mfspr     r3, SPR_SPRG2            /* restore saved r3 */

    rfi
}
#endif

/****************************************************************************/
/*
 *    TRKPostInterruptEvent
 *
 *    Post an "interrupt/task stopped" event to the event queue.  This
 *    function can be called at any time, allowing concurrent execution to be
 *    synchronized through the event loop.
 *
 *    100 % DONE
 */
/****************************************************************************/


#define SUPPORT_TRAP 0x0FE00000


void TRKPostInterruptEvent(void)
{
    NubEvent        event;
    NubEventType    eventType;
    u32 inst;
    BOOL post;

    __puts("\nExceptionID = ");
    __puthex8(gTRKCPUState.Extended1.exceptionID);
    __putchar('\r\n');

    /*
    ** If CodeWarriorTRK was activated by input, then don't post an
    ** interrupt event.  Events may be generated while processing
    ** the input (in the main loop), but none will be generated
    ** here.  Once the inputActivated flag has been checked,
    ** reset it.
    */

#if TRK_TRANSPORT_INT_DRIVEN
    if (gTRKState.inputActivated)
    {
        gTRKState.inputActivated = FALSE;
        return;
    }
#endif /* TRK_TRANSPORT_INT_DRIVEN */

    post = TRUE;

    switch ((u16)gTRKCPUState.Extended1.exceptionID & 0xffff) // exceptionID
    {
        case PPC_PROGRAMERROR:
        case PPC_TRACE:
            /*
            ** assume "trap" means breakpoint.
            **
            ** Trace exceptions are used for step and are
            ** treated just like breakpoints.  The SUPPORT_TRAP
            ** is a special case.  It is used to invoke
            ** support functions in CodeWarriorTRK.  It triggers
            ** a support event.
            */

            TRKTargetReadInstruction((void *)&inst, (void *)gTRKCPUState.Default.PC);

            if (inst == SUPPORT_TRAP)
                eventType = kSupportEvent;
            else
                eventType = kBreakpointEvent;
            break;

        default:
            eventType = kExceptionEvent;
            break;
    }

    if (post)
    {
        TRKConstructEvent(&event, eventType);

        /*
         *    At this point you could add any kind of interrupt data to the
         *    event's buffer.
         */

        /*
         *    Post an event to be handled in the main loop
         */

        TRKPostEvent(&event);
    }

    /*
     *    return to mainloop (or exception handler)
     */
}

// MOVED TO TARGIMPL_ASM.S FOR GCC
#ifdef __MWERKS__
/****************************************************************************/
/*
 *    TRKSwapAndGo
 *
 *    Swap out TRK CPU state, swap in the target program state, and
 *    continue executing from the "program counter".
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX void TRKSwapAndGo()
{
    nofralloc            // no stack frame

    // Save TRK state in global "gTRKState"

    addis     r3, r0, gTRKState@h
    ori       r3, r3, gTRKState@l

    stmw      r0, TRKState_PPC.GPR[0](r3)

    mfmsr     r0
    stw       r0, TRKState_PPC.MSR(r3)

    mflr      r0
    stw       r0, TRKState_PPC.LR(r3)

    mfctr     r0
    stw       r0, TRKState_PPC.CTR(r3)

    mfxer     r0
    stw       r0, TRKState_PPC.XER(r3)

    mfspr     r0, SPR_DSISR
    stw       r0, TRKState_PPC.DSISR(r3)

    mfspr     r0, SPR_DAR
    stw       r0, TRKState_PPC.DAR(r3)

    /*
     * Disable external and recoverable interrupts while restoring
     * the user-code state.
     */

    addi      r1, r0, (MSR_EE | MSR_RI)         /* get OR mask w/RI and EE bits on */
    nor       r1, r1, r1                        /* complement mask */
    mfmsr     r3
    and       r3, r3, r1                        /* AND w/MSR to turn off RI and EE bits */
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

    addis     r2, r0, gTRKState@h          /* get addr of TRK state */
    ori       r2, r2, gTRKState@l
    lwz       r2, TRKState_PPC.inputPendingPtr(r2)    /* get input pending ptr */
    lbz       r2, 0(r2)                    /* get input pending value */
    cmpwi     r2, FALSE
    beq       NoOutgoingInput              /* if no input, continue return */
                                           /* to target program */
    /*
    ** Input is pending, so the TRK needs to be re-activated.
    ** Activate it, but set the inputActivated flag so that it
    ** does not treat this like an unhandled interrupt in the
    ** target program.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    li        r3, TRUE
    stb       r3, TRKState_PPC.inputActivated(r2)

    b         TRKInterruptHandlerEnableInterrupts    /* re-activate TRK */

NoOutgoingInput:
#endif /* TRK_TRANSPORT_INT_DRIVEN */

    /*
     * clear re-entrance flag before returning to user code
     */

    addis     r2, r0, gTRKExceptionStatus@h
    ori       r2, r2, gTRKExceptionStatus@l

    addi      r3, r0, FALSE                    /* clear flag */
    stb       r3, ExceptionStatus.inTRK(r2)

    bl        TRKRestoreExtended1Block

    // restore target program state from global "gTRKCPUState"

    addis     r2, r0, gTRKCPUState@h           // get addr of stored regs
    ori       r2, r2, gTRKCPUState@l

    // Restore all non-GPR's from the default block.

    lmw       r27, ProcessorState_PPC.Default.PC(r2)        // store PC thru XER
    mtspr     SPR_SRR0, r27            // restore as pre-exception PC
    mtspr     SPR_LR, r28              // restore LR
    mtcr      r29                      // restore CR
    mtspr     SPR_CTR, r30             // restore CTR
    mtspr     SPR_XER, r31             // restore XER

    // Restore all GPR's

    lmw       r3, ProcessorState_PPC.Default.GPR[3](r2)     // restore GPR3-31
    lwz       r0, ProcessorState_PPC.Default.GPR[0](r2)
    lwz       r1, ProcessorState_PPC.Default.GPR[1](r2)
    lwz       r2, ProcessorState_PPC.Default.GPR[2](r2)

    rfi
}

/****************************************************************************/
/*
 *    TRKInterruptHandlerEnableInterrupts
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *
 *    100 % DONE
 */
/****************************************************************************/
ASM_PREFIX void TRKInterruptHandlerEnableInterrupts()
{
    nofralloc                // no stack frame

    /*
    ** Enable exceptions by restoring the MSR value from the TRK
    ** state, since exceptions are enabled during normal TRK
    ** operation.
    */

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r0, TRKState_PPC.MSR(r2)
    sync
    mtmsr     r0
    sync

    lwz       r0, TRKState_PPC.LR(r2)
    mtlr      r0

    lwz       r0, TRKState_PPC.CTR(r2)
    mtctr     r0

    lwz       r0, TRKState_PPC.XER(r2)
    mtxer     r0

    lwz       r0, TRKState_PPC.DSISR(r2)
    mtspr     SPR_DSISR, r0

    lwz       r0, TRKState_PPC.DAR(r2)
    mtspr     SPR_DAR, r0

    lmw       r3, TRKState_PPC.GPR[3](r2)  // restore GPR3-31
    lwz       r0, TRKState_PPC.GPR[0](r2)
    lwz       r1, TRKState_PPC.GPR[1](r2)
    lwz       r2, TRKState_PPC.GPR[2](r2)

    b         TRKPostInterruptEvent        // call, then returning to TargetContinue
}
#endif

/****************************************************************************/
/*
 *    TRKTargetInterrupt
 *
 *    Interrupt/breakpoint occurred.  Event was posted by the exception handler.
 *    Service the event, sending notification to the host debugger
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetInterrupt(NubEvent *event)
{
    DSError err = kNoError;

    switch (event->fType)
    {
        case kBreakpointEvent:
        case kExceptionEvent:

            /*
            ** Don't notify the debugger of the individual trace
            ** breakpoints that occur during stepping - just the
            ** last one.
            **
            ** Just check the step status.  If it is active, don't
            ** notify the debugger.  If it is not, then either we
            ** are not stepping at all or we just finished, so
            ** send out the notification.
            */

            if (!TRKTargetCheckStep())
            {
                /*
                ** Notify the debugger that the target process is
                ** stopped.  Also, set the target stop status
                ** to indicate that it is stopped.
                */

                TRKTargetSetStopped(TRUE);
                err = TRKDoNotifyStopped(kDSNotifyStopped);
            }
            break;

        default:
#if DEBUG_ASSERT
            ASSERT(0);            /* not a valid exception */
#endif
            break;
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetAddStopInfo
 *
 *    Building NotifyStopped message to send to the host.  Add target-specific
 *    block to the message buffer.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/


#define CURRENT_CONTEXT_ADDR    0xD4
#define FPU_CONTEXT_ADDR        0xD8
#define ROOT_THREAD_ADDR        0xDC    // First thread we were running in
#define PREVIOUS_THREAD_ADDR    0xE0    // Previous thread we were running in
#define CURRENT_THREAD_ADDR     0xE4    // Thread we're currently running in


DSError TRKTargetAddStopInfo(MessageBuffer *buffer)
{
    u32 instruction;
    DSError err;
    u32 buf[16];
    u32 v12;
    u32 v13;

    err = kNoError;

    // Reset Buffer
    TRK_memset(&buf[0], kZero, TRK_MSG_HEADER_LENGTH);

    buf[0] = 1256;    // ???

    DSFetch_u8(&buf[1]) = kDSNotifyStopped;

    buf[2] = gTRKCPUState.Default.PC;

    GetThreadInfo(&v12, &v13);

    buf[5] = v12;
    buf[6] = DSFetch_u32(ConvertAddress(CURRENT_THREAD_ADDR));

    TRKTargetReadInstruction((void *)&instruction, (void *)gTRKCPUState.Default.PC);

    buf[3] = instruction;
    buf[4] = (gTRKCPUState.Extended1.exceptionID & 0xffff); // exceptionID

    err = MessageAddBlock_ui8(buffer, (const u8 *)buf, TRK_MSG_HEADER_LENGTH);

    if (!err)
    {
        s32 i;

        for (i = 0; i < 32; i++)
            MessageAdd_ui32(buffer, gTRKCPUState.Default.GPR[i]);

        MessageAdd_ui32(buffer, gTRKCPUState.Default.PC);
        MessageAdd_ui32(buffer, gTRKCPUState.Default.LR);
        MessageAdd_ui32(buffer, gTRKCPUState.Default.CR);
        MessageAdd_ui32(buffer, gTRKCPUState.Default.CTR);

        err = MessageAdd_ui32(buffer, gTRKCPUState.Default.XER);
    }

    if (!err)
    {
        u32 address[5];

        address[0] = DSFetch_u32(ConvertAddress(CURRENT_CONTEXT_ADDR));
        MessageAdd_ui32(buffer, address[0]);

        address[1] = DSFetch_u32(ConvertAddress(FPU_CONTEXT_ADDR));
        MessageAdd_ui32(buffer, address[1]);

        address[2] = DSFetch_u32(ConvertAddress(ROOT_THREAD_ADDR));
        MessageAdd_ui32(buffer, address[2]);

        address[3] = DSFetch_u32(ConvertAddress(PREVIOUS_THREAD_ADDR));
        MessageAdd_ui32(buffer, address[3]);

        address[4] = DSFetch_u32(ConvertAddress(CURRENT_THREAD_ADDR));

        err = MessageAdd_ui32(buffer, address[4]);
    }

    if (!err)
    {
        s8 writeData[1024];

        s32 length = sizeof(writeData);

        err = TRKTargetAccessMemory(writeData, (void *)(gTRKCPUState.Default.PC & 0xFFFFFC00), (size_t *)&length, kUserMemory, TRUE);

        MessageAddBlock(buffer, writeData, sizeof(writeData));
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetAddExceptionInfo
 *
 *  Building NotifyException message to send to the host.  Add target-specific
 *  block to the message buffer.  The added information pertains to the
 *  last exception which occurred in TRK.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKTargetAddExceptionInfo(MessageBuffer *buffer)
{
    u32 instruction;
    DSError err;
    u32 buf[16];

    err = kNoError;

    /*
    ** Append the PC.
    */

    // Reset Buffer
    TRK_memset(&buf, kZero, sizeof(buf));

    buf[0] = sizeof(buf);
    DSFetch_u8(&buf[1]) = kDSNotifyException;
    buf[2] = gTRKExceptionStatus.exceptionInfo.PC;

    /*
    ** Get the instruction at the PC and append that.
    */

    if (err == kNoError)
        err = TRKTargetReadInstruction((void *)&instruction,
                                    (void *)gTRKExceptionStatus.exceptionInfo.PC);

    buf[3] = instruction;
    buf[4] = gTRKExceptionStatus.exceptionInfo.exceptionID;

    err = MessageAddBlock_ui8(buffer, (const u8 *)&buf[0], sizeof(buf));

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetEnableTrace
 *
 *    Enables/disables the SE single-trace enable bit in the MSR of the
 *    target program.  When enabled, it will cause a trace exception after
 *    one instruction is executed.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
static DSError TRKTargetEnableTrace(BOOL enable)
{
    if (enable)
        gTRKCPUState.Extended1.MSR = ((gTRKCPUState.Extended1.MSR | MSR_SE) & (0xFFFFFFFF ^ MSR_EE));
    else
        gTRKCPUState.Extended1.MSR = ((gTRKCPUState.Extended1.MSR & (0xFFFFFFFF ^ MSR_SE)) | MSR_EE);

    return kNoError;
}

#ifdef __MWERKS__
/****************************************************************************/
/*
 *    TRKTargetStepDone
 *
 *    Determines whether trace stop conditions are met.
 *
 *    100 % DONE
 */
/****************************************************************************/
static BOOL TRKTargetStepDone(void)
{
    BOOL err = TRUE;

    /*
    ** If the current exception is not a trace exception, then
    ** there was some other problem with execution and the
    ** trace should be aborted.
    **
    ** If it was a trace exception, then check the stop conditions.
    */

    if (gTRKStepStatus.active &&
        ((u16)gTRKCPUState.Extended1.exceptionID == PPC_TRACE)) // exceptionID
    {
        switch (gTRKStepStatus.type)
        {
            /*
            ** For single step, the trace is not done as long as the count
            ** is larger than 0.
            */

            case kDSStepIntoCount:
                __puts("Remaining step count: ");
                __puthex2((u8)gTRKStepStatus.count);
                __puts("\r\n");

                if (gTRKStepStatus.count > 0)
                    err = FALSE;

                break;

            /*
            ** For stepping out of a memory range, the trace is not done
            ** as long as the PC is in the memory range, where the
            ** range includes the start and end points.
            */

            case kDSStepIntoRange:
                if ((gTRKCPUState.Default.PC >= gTRKStepStatus.rangeStart) &&
                    (gTRKCPUState.Default.PC <= gTRKStepStatus.rangeEnd))
                    err = FALSE;

                break;

            default:
                break;
        }
    }

    return err;
}
#endif

/****************************************************************************/
/*
 *    TRKTargetDoStep
 *
 *    General-purpose step handler.  Sets up the instruction trace and
 *    returns control to the target program.  Used to initiate the step
 *    and to continue the stepping as long as needed.
 *
 *    100 % DONE
 */
/****************************************************************************/
static DSError TRKTargetDoStep(void)
{
    DSError err = kNoError;

    gTRKStepStatus.active = TRUE;

    /*
    ** Once the trace is enabled, just continue.  The target program
    ** will break after each instruction.
    */

    TRKTargetEnableTrace(TRUE);

    if ((gTRKStepStatus.type == kDSStepIntoCount) ||
        (gTRKStepStatus.type == kDSStepOverCount))
        gTRKStepStatus.count--;

    /*
    ** To continue, just clear the stopped flag and return.  The
    ** main loop will begin execution once it runs out of messages
    ** to process.
    */

    TRKTargetSetStopped(FALSE);

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetCheckStep
 *
 *    General-purpose step handler - checks status and continues the
 *    instruction trace if the step is not complete.  Should be called
 *    whenever a breakpoint is hit.
 *
 *    Returns true if the step is still active, false otherwise.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
BOOL TRKTargetCheckStep(void)
{
    BOOL err;

    /*
    ** We hit a breakpoint.  If a step is in-process, continue it
    ** or stop it.
    */
    if (gTRKStepStatus.active)
    {
        /*
        ** The trace must be cleaned up regardless of the error.
        */

        TRKTargetEnableTrace(FALSE);

        /*
        ** If the current exception is not a trace exception, then
        ** there was some other problem with execution and the
        ** trace should be aborted.
        **
        ** If it was a trace exception, then check the stop conditions.
        */

        err = TRUE;

        if (gTRKStepStatus.active &&
            ((u16)gTRKCPUState.Extended1.exceptionID == PPC_TRACE)) // exceptionID
        {
            switch (gTRKStepStatus.type)
            {
                /*
                ** For single step, the trace is not done as long as the count
                ** is larger than 0.
                */

                case kDSStepIntoCount:
                    __puts("Remaining step count: ");
                    __puthex2((u8)gTRKStepStatus.count);
                    __puts("\r\n");

                    if (gTRKStepStatus.count > 0)
                        err = FALSE;

                    break;

                /*
                ** For stepping out of a memory range, the trace is not done
                ** as long as the PC is in the memory range, where the
                ** range includes the start and end points.
                */

                case kDSStepIntoRange:
                    if ((gTRKCPUState.Default.PC >= gTRKStepStatus.rangeStart) &&
                        (gTRKCPUState.Default.PC <= gTRKStepStatus.rangeEnd))
                        err = FALSE;

                    break;

                default:
                    break;
            }
        }

        /*
        ** If we're finished or aborting, de-activate the step.
        ** Otherwise, continue with the next trace.
        */

        if (err)
            gTRKStepStatus.active = FALSE;
        else
            TRKTargetDoStep();
    }

    return gTRKStepStatus.active;
}

/****************************************************************************/
/*
 *    TRKTargetSingleStep
 *
 *    Single step through <count> instructions.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetSingleStep(u32 count, BOOL stepOver)
{
    DSError err = kNoError;

    /*
    ** Just set up the global data and call the general-purpose routine.
    */

    if (stepOver)
    {
        err = kUnsupportedError;
    }
    else
    {
        gTRKStepStatus.type = kDSStepIntoCount;
        gTRKStepStatus.count = count;

        err = TRKTargetDoStep();
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetStepOutOfRange
 *
 *    Step until PC is out of specified memory range.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetStepOutOfRange(u32 start, u32 end, BOOL stepOver)
{
    DSError err = kNoError;

    /*
    ** Just set up the global data and call the general-purpose routine.
    */

    if (stepOver)
    {
        err = kUnsupportedError;
    }
    else
    {
        gTRKStepStatus.type = kDSStepIntoRange;
        gTRKStepStatus.rangeStart = start;
        gTRKStepStatus.rangeEnd = end;

        err = TRKTargetDoStep();
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetGetPC
 *
 *    Return the target PC.
 *
 *    100 % DONE
 */
/****************************************************************************/
PCType TRKTargetGetPC(void)
{
    return (PCType)gTRKCPUState.Default.PC;
}

/****************************************************************************/
/*
 *    TRKTargetSupportRequest
 *
 *    Service support request from the target program and resume target program
 *    execution.  Currently supports only file read/write requests.
 *
 *    Expects the following input parameters in the target program registers:
 *        r3: command (kDSWriteFile or kDSReadFile)
 *        r4: file_handle (One of the values from DSFileHandle enum)
 *        r5: length * (ptr to # of bytes to read/write)
 *        r6: buffer * (ptr to buffer containing the data to read/write)
 *
 *    Returns the following result values in the target program registers:
 *        r3: result (one of the values from DSIOResult enum)
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/


#define SUPP_COMMAND_REG        3
#define SUPP_HANDLE_REG         4
#define SUPP_IN_LEN_REG         5
#define SUPP_BUFFER_REG         6
#define SUPP_RESULT_REG         3


DSError TRKTargetSupportRequest(void)
{
    size_t *length;
    DSIOResult io_result;
    s32 msg_length;
    MessageCommandID command;
    NubEvent event;

    DSError err = kNoError;

    __puts("TargetSupportRequest registers:");
    __puts("\r\n   command (a0) = ");
    __puthex8(gTRKCPUState.Default.GPR[SUPP_COMMAND_REG]);
    __puts("\r\n   handle  (a1) = ");
    __puthex8(gTRKCPUState.Default.GPR[SUPP_HANDLE_REG]);
    __puts("\r\n   length  (a2) = ");
    __puthex8(gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG]);
    __puts("\r\n   buffer  (a3) = ");
    __puthex8(gTRKCPUState.Default.GPR[SUPP_BUFFER_REG]);
    __puts("\r\n");

    /*
    ** Only the kDSWRiteFile and kDSReadFile commands are supported
    ** at this time.
    */

    command = (MessageCommandID)gTRKCPUState.Default.GPR[SUPP_COMMAND_REG];

#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    if ((command - kDSWriteFile) > (u32)kDSConnect)
    {
        /*
        ** Re-post invalid support-requests as an exception
        ** to be reported by the debugger.
        */

        switch (command)
        {
            default:
                TRKConstructEvent(&event, kExceptionEvent);
                TRKPostEvent(&event);
                return kNoError;

            case kDSOpenFile:
                err = HandleOpenFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], (u8)gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG], (u32 *)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG], &io_result);

                if ((io_result == kDSIONoError) && (err != kNoError))
                    io_result = kDSIOError;

                gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
                break;

            case kDSCloseFile:
                err = HandleCloseFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], &io_result);

                if ((io_result == kDSIONoError) && (err != kNoError))
                    io_result = kDSIOError;

                gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
                break;

            case kDSPositionFile:
                msg_length = DSFetch_s32(gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG]);
                err = HandlePositionFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], &msg_length, (u8)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG], &io_result);

                if ((io_result == kDSIONoError) && (err != kNoError))
                    io_result = kDSIOError;

                gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
                DSFetch_s32(gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG]) = msg_length;
                break;
        }
    }
#else
    if ((command != kDSReadFile) && (command != kDSWriteFile) && (command != kDSOpenFile) && (command != kDSCloseFile) && (command != kDSPositionFile))
    {
        TRKConstructEvent(&event, kExceptionEvent);
        TRKPostEvent(&event);
        return err;
    }

    if (command == kDSOpenFile)
    {
        err = HandleOpenFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], (u8)gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG], (u32 *)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG], &io_result);

           if (io_result == kDSIONoError && err != kNoError)
            io_result = kDSIOError;

        gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
    }
    else if (command == kDSCloseFile)
    {
        err = HandleCloseFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], &io_result);

        if (io_result == kDSIONoError && err != kNoError)
            io_result = kDSIOError;

           gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
    }
    else if (command == kDSPositionFile)
    {
        msg_length = DSFetch_u32(gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG]);
        err = HandlePositionFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG], &msg_length, (u8)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG], &io_result);

           if (io_result == kDSIONoError && err != kNoError)
            io_result = kDSIOError;

        gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;
        DSFetch_s32(gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG]) = msg_length;
    }
#endif
    else
    {
        /*
        ** Call the portable routine to handle these requests, pulling
        ** out the relevant parameters from the target registers.
        */

        length = (size_t *)gTRKCPUState.Default.GPR[SUPP_IN_LEN_REG];

        err = TRK_SuppAccessFile((DSFileHandle)gTRKCPUState.Default.GPR[SUPP_HANDLE_REG],
                              (u8 *)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG],
                              (s32 *)length, &io_result, TRUE, (command == kDSReadFile));

        /*
        ** If there was some general error during processing,
        ** translate that into an io error.
        */

        if ((io_result == kDSIONoError) && (err != kNoError))
            io_result = kDSIOError;

        /*
        ** Put the result values in the target registers.
        */

        gTRKCPUState.Default.GPR[SUPP_RESULT_REG] = (DefaultType)io_result;

        /*
        ** If reading, then the buffer memory has been modified.  Flush the
        ** associated cache memory.
        */

        if (command == kDSReadFile)
        {
            TRK_flush_cache((u32)gTRKCPUState.Default.GPR[SUPP_BUFFER_REG],
                        *length);
        }
    }

    /*
    ** Return control to the main loop.  Unless the 'gTRKState.stopped'
    ** flag becomes true, the main loop will continue executing the
    ** target program as soon as it runs out of messages to process.
    **
    ** Note:  On entry from the target program, the
    ** PC points to the breakpoint instruction that
    ** was used to signal CodeWarriorTRK.  For support requests, the
    ** target program needs to continue executing at the
    ** instruction after the breakpoint.  It is necessary to
    ** adjust the PC before continuing.
    **
    ** Using the support request breakpoint instruction in
    ** a branch delay slot is NOT supported.
    */

    gTRKCPUState.Default.PC += 4;

    return err;
}

/****************************************************************************/
/*
 *    TRKTargetFlushCache
 *
 *    Flushes the cache corresponding to the specified memory range using
 *    the given options.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetFlushCache(u8 options,
                          void *start, void *end)
{
#if defined(__MWERKS__)
    #pragma unused(options)
#elif defined(__GNUC__)
    UNUSED(options);
#endif

    if (start < end)
    {
        TRK_flush_cache((u32)start, (u32)((u8 *)end - (u8 *)start));

        return kNoError;
    }
    else
    {
        return kInvalidMemory;
    }
}

/****************************************************************************/
/*
 *    TRKTargetStopped
 *
 *    Returns true if the target process is stopped, false if it is
 *    temporarily suspended.
 *
 *    100 % DONE
 */
/****************************************************************************/
BOOL TRKTargetStopped(void)
{
    return (BOOL)gTRKState.stopped;
}

/****************************************************************************/
/*
 *    TRKTargetSetStopped
 *
 *    Sets the stopped status of the target process.  Set to true if
 *    the target process is stopped, false if it is temporarily
 *    suspended.
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKTargetSetStopped(BOOL stop_state)
{
    gTRKState.stopped = stop_state;
}

/****************************************************************************/
/*
 *    TRKTargetStop
 *
 *    Stops the target.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKTargetStop(void)
{
    /*
    ** For bare-board systems, just set the stopped state to true.
    ** This will prevent the TRK from returning control to the
    ** target program.
    */

    TRKTargetSetStopped(TRUE);

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKPPCAccessSPR
 *
 *    Read/write PPC SPR (Special purpose register)
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKPPCAccessSPR(void *value, u32 spr_register_num, BOOL read)
{
    /* Initialize instruction array with nop */

    InstructionType access_func[10] = {INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP};
    /*
    ** Construct a small assembly function to perform the
    ** requested access and call it.  The read/write function
    ** is in the form:
    **
    ** read:
    **        mfspr    r4, spr_register_num
    **        stw      r4, 0(r3)
    **        blr
    **
    ** write:
    **        lwz      r4, 0(r3)
    **        mtspr    spr_register_num, r4
    **        blr
    **
    */

    if (read)
    {
        access_func[0] = INSTR_MFSPR(4, spr_register_num);
        access_func[1] = (u32)INSTR_STW(4, 0, 3);
    }
    else
    {
        access_func[0] = (u32)INSTR_LWZ(4, 0, 3);
        access_func[1] = INSTR_MTSPR(spr_register_num, 4);
    }

    return TRKPPCAccessSpecialReg(value, access_func, read);
}


#include "copy_vectors.h"
#include "dispatch.h"


/****************************************************************************/
/*
 *    TRKPPCAccessPairedSingleRegister
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError TRKPPCAccessPairedSingleRegister(void *value, u32 register_num, BOOL read)
{
    /* Initialize instruction array with nop */

    InstructionType access_func[10] = {INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP};

    if (read)
        access_func[0] = (SHIFT_LEFT_X_BY_Y_BITS(register_num, 21) | 0xF0030000);
    else
        access_func[0] = (SHIFT_LEFT_X_BY_Y_BITS(register_num, 21) | 0xE0030000);

    return TRKPPCAccessSpecialReg(value, access_func, read);
}

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
/****************************************************************************/
/*
 *    PPCAccessAltiVecRegister
 *
 *    R/W a value from/to a altivec vector, vrsave or vscr register.
 */
/****************************************************************************/
#if __PPC_ALTIVEC__
DSError PPCAccessAltiVecRegister(void *value, u32 register_num,
                                BOOL read)
{
    /*
    ** Initialize access instruction array with nop instruction
    */

    InstructionType access_func[5] = {INSTR_NOP,
                                      INSTR_NOP,
                                      INSTR_NOP,
                                      INSTR_NOP,
                                      INSTR_NOP};

    /*
    ** Construct a function to read/write the specified
    ** AltiVec vector register. Then call PPCAccessVectorReg()
    ** with that instruction.
    */

    /*
    ** Verify values before building the instruction.
    */

    if (register_num > DS_EXTENDED2_MAX_REGISTER_6xx_7xx)
    {
        __puts("\r\nPPCAccessAltiVecRegister bad parameter:");
        __puts("\r\n   register_num: ");
        __puthex8( register_num);
        __puts("\r\n");

        return kInternalError;
    }

    /*
    ** Build access instruction
    */

    if (register_num == ALTIVEC_VRSAVE_SPR_ACCESS)
    {
        if (read)
        {
            /*
            ** Build the following access instructions.
            **
            ** access_func[0]:    mfspr    r4, spr256
            ** access_func[1]:    stw      r4, 0(r3)
            **
            */

            access_func[0] = INSTR_MFSPR(4, SPR_VRSAVE);
            access_func[1] = INSTR_STW(4, 0, 3);
        }
        else
        {
            /*
            ** Build the following access instructions.
            **
            **                    mtspr    spr256, r3
            **
            */

            access_func[0] = INSTR_LWZ(4,0,3);
            access_func[1] = INSTR_MTSPR(SPR_VRSAVE, 4);
        }
    }
    else if (register_num == ALTIVEC_VSCR_ACCESS)
    {
        if (read)
        {
            /*
            ** Build the following access instructions.
            **
            **                    stvx     v0, 0, r4
            **                    mfvscr   v0
            **                    stvx     v0, 0, r3
            **                    lvx      v0, 0, r4
            **
            **
            */

            access_func[0] = INSTR_STVX(0, 0, 4);
            access_func[1] = INSTR_MFVSCR(0);
            access_func[2] = INSTR_STVX(0, 0, 3);
            access_func[3] = INSTR_LVX(0, 0, 4);
        }
        else
        {
            /*
            ** Build the following access instructions.
            **
            **                    stvx     v0, 0, r4
            **                    lvx      v0, 0, r3
            **                    mtvscr   v0
            **                    lvx      v0, 0, r4
            **
            **
            */

            access_func[0] = INSTR_STVX(0, 0, 4);
            access_func[1] = INSTR_LVX(0, 0, 3);
            access_func[2] = INSTR_MTVSCR(0);
            access_func[3] = INSTR_LVX(0, 0, 4);
        }
    }
    else
    {
        /* Vector register access */

        /*
        ** Build the following access instructions.
        **
        **                       stvx      register_number, 0, r3
        **
        **                       or
        **
        **                       lvx       register_numer, 0, r3
        **
        */

        if (read)
            access_func[0] = INSTR_STVX(register_num, 0, 3);
        else
            access_func[0] = INSTR_LVX(register_num, 0, 3);
    }

    /*
    ** Call PPCAccessVectorReg, which will build a small
    ** assembly function around access_func to
    ** r/w the register.
    */

    return TRKPPCAccessSpecialReg(value, access_func, read);
}
#endif /* #if __PPC_ALTIVEC__ */
#endif

#if __PPC_FPU__

/*
** Useful defines.
*/

#define FP_FPSCR_ACCESS     32
#define FP_FPECR_ACCESS     33


/****************************************************************************/
/*
 *    ReadFPSCR
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/

// MOVED TO TARGIMPL_ASM.S FOR GCC
#ifdef __MWERKS__

ASM_PREFIX s32 ReadFPSCR()
{
    nofralloc

    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    mffs      f31
    stfd      f31, 0(r3)
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr
}

/****************************************************************************/
/*
 *    WriteFPSCR
 *
 *    NOTE: This function is DEFINITELY written in pure PPC assembly
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
ASM_PREFIX void WriteFPSCR()
{
    nofralloc

    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    lfd       f31, 0(r3)
    mtfsf     0xff, f31
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr
}
#else
extern s32 ReadFPSCR();
extern void WriteFPSCR();
#endif


#include "mainloop.h"
#include "string_TRK.h"


/****************************************************************************/
/*
 *    TRKPPCAccessFPRegister
 *
 *    Read/write PPC paired floating point register
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKPPCAccessFPRegister(void *value, u32 register_num, BOOL read)
{
    DSError err = kNoError;

    /* Initialize instruction array with nop */

    InstructionType access_func[10] = {INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP,
                                       INSTR_NOP};

    if (register_num < FP_FPSCR_ACCESS)
    {
        /*
        ** Construct a small assembly function to perform the
        ** requested access and call it.  The read/write function
        ** is in the form:
        **
        ** read:
        **        stfd    register_num,0(r3)
        **
        ** write:
        **        lfd     register_num,0(r3)
        **
        */

        if (read)
        {
            access_func[0] = INSTR_STFD(register_num, 0, 3);
        }
        else
        {
            access_func[0] = INSTR_LFD(register_num, 0, 3);
        }

        err = TRKPPCAccessSpecialReg(value, access_func, read);
    }
    else if (register_num == FP_FPSCR_ACCESS)
    {
        /*
        ** Construct a small assembly function to perform the
        ** requested access and call it.  The read/write function
        ** is in the form:
        **
        ** read:
        **        stfd    f1,0(r4)
        **        mffs    f1
        **        stfd    f1,0(r3)
        **        lfd     f1,0(r4)
        **
        ** write:
        **        stfd    f1,0(r4)
        **        lfd     f1,0(r3)
        **        mtfsf   0xff,f1
        **        lfd     f1,0(r4)
        **
        */

        if (read)
        {
            ReadFPSCR();
        }
        else
        {
            WriteFPSCR();
        }

        /*
        ** The upper 32 bits are undefined, so clear them for
        ** consistency.
        */

        *(u64 *)value &= 0xffffffffLL;
    }
    else if (register_num == FP_FPECR_ACCESS)
    {
        /*
        ** For FPECR, use the normal SPR access function.  However,
        ** that function deals with 32-bit values, and this one
        ** deals with 64-bit values.  Be sure to do the appropriate
        ** conversion.
        */

        if (!read)
        {
            DSFetch_u32(value) = (u32)DSFetch_u64(value);
        }

        err = TRKPPCAccessSPR(value, SPR_THRM3, read);

        if (read)
        {
            DSFetch_u64(value) = (((u64)(DSFetch_u32(value))) & 0xffffffffLL);
        }
    }

    return err;
}
#endif /* #if __PPC_FPU__ */

/****************************************************************************/
/*
 *    TRKPPCAccessSpecialReg
 *
 *    R/W a value from/to a PPC register using the provided
 *    access_function.  It should be a 5-instruction array
 *    w/the last one empty.
 *
 *    This function can also be used to access AltiVec vector registers.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/


#define DEBUG_VECTORREG_ACCESS    0


DSError TRKPPCAccessSpecialReg(void *value, u32 *access_func, BOOL read)
{
#if defined(__MWERKS__)
    #pragma unused(read)
#elif defined(__GNUC__)
    UNUSED(read);
#endif

    typedef void (*asm_access_type)(void *, void *);

    asm_access_type asm_access;

    /*
    ** Construct a small assembly function to perform the
    ** requested access and call it.  The read/write function
    ** is in the form:
    **
    **        <access_func>
    **        blr
    */

    /*
    ** Put blr instruction at the end of access function (it should be
    ** a 5-instruction array w/the last one empty).
    */

    access_func[9] = INSTR_BLR;

    /*
    ** Now that the instruction array is built, get a function pointer to it.
    */

    asm_access = (asm_access_type)access_func;

#if DEBUG_VECTORREG_ACCESS

    __puts("\r\nasm_access: ");
    __puthex8((u32)asm_access);
    __puts("   access_func: ");
    __puthex8((u32)access_func);

    for (i = 0; i < 10; i++)
    {
        __puts("\r\ninst[");
        __puthex2(i);
        __puts("]: ");
        __puthex8(access_func[i]);
        __puts("  ;  ");
        __puthex8(*((u32 *)asm_access + i));
    }

    __puts("\r\n");

#endif

    // Flush cache
    TRK_flush_cache((u32)access_func, (sizeof(access_func) * 10));
    (*asm_access)(value, (void *)&TRKvalue128_temp);

    return kNoError;
}

#if TRK_TRANSPORT_INT_DRIVEN
/****************************************************************************/
/*
 *    TRKTargetSetInputPendingPtr
 *
 *    Sets the ptr to the input pending status flag for the interrupt-driven
 *    transport.
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKTargetSetInputPendingPtr(volatile u8 *inputPendingPtr)
{
    gTRKState.inputPendingPtr = inputPendingPtr;
}
#endif

/****************************************************************************/
/*
 *    SetInputState
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
void SetInputState(u32 mode)
{
    gTRKState.inputActivated = mode;
}

/****************************************************************************/
/*
 *    TRKGetInTRKFlag
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
s32 TRKGetInTRKFlag(void)
{
    return (s32)gTRKExceptionStatus.inTRK;
}

/****************************************************************************/
/*
 *    ConvertAddress
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
u32 ConvertAddress(u32 address)
{
    return (address | BOOTINFO);
}


/****************************************************************************/
/*
 *    GetThreadInfo
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/


#define ACTIVE_THREAD_QUEUE (BOOTINFO + ROOT_THREAD_ADDR)       // 8 bytes
#define CURRENT_THREAD      (BOOTINFO + CURRENT_THREAD_ADDR)    // 4 bytes


#if defined(HAVE_RVL_SDK) && !defined(__GNUC__)

static void GetThreadInfo(u32 *result, u32 *a2)
{
    u32 v3;
    OSThread *current;

    *result = 1;
    *a2 = 0;
    current = (OSThread *)DSFetch_u32(ACTIVE_THREAD_QUEUE);

    if (!(((u32)(&current->context.gpr[0]) != 0xFFFFFFFF) &&
        ((u32)(&current->context.gpr[0])) &&
        ((u32)(&current->context.gpr[0]) != BOOTINFO)))
        return;

    v3 = 0;

    while ((u32)(&current->context.gpr[0]) != 0)
    {
        if ((u32)(&current->context.gpr[0]) == DSFetch_u32(CURRENT_THREAD))
            *a2 = v3;

        ++v3;
        current = (OSThread *)ConvertAddress((u32)(current->linkActive.next));

        if (((u32)(&current->context.gpr[0]) == 0xFFFFFFFF) ||
            (!(u32)(&current->context.gpr[0])) ||
            ((u32)(&current->context.gpr[0]) == BOOTINFO))
            break;
    }

    *result = v3;
}

#else

#if defined(__MWERKS__)

static void GetThreadInfo(u32 *result, u32 *a2)
{
    u32 v3;
    u32 active_thread_data;

    *result = 1;
    *a2 = 0;
    active_thread_data = DSFetch_u32(ACTIVE_THREAD_QUEUE);

    if (!((active_thread_data != 0xFFFFFFFF) &&
        (active_thread_data) &&
        (active_thread_data != BOOTINFO)))
        return;

    v3 = 0;

    while (active_thread_data != 0)
    {
        if (active_thread_data == DSFetch_u32(CURRENT_THREAD))
            *a2 = v3;

        ++v3;
        active_thread_data = ConvertAddress(*(u32 *)(active_thread_data + 764));

        if ((active_thread_data == 0xFFFFFFFF) ||
            (!active_thread_data) ||
            (active_thread_data == BOOTINFO))
            break;
    }

    *result = v3;
}

#elif defined(__GNUC__)

static void GetThreadInfo(u32 *result, u32 *a2)
{
    u32 v3;
    lwp_cntrl *thread;

    // catch the running threads from libOGC
    extern lwp_cntrl *_thr_main;        // ROOT_THREAD_ADDR
    extern lwp_cntrl *_thr_executing;   // CURRENT_THREAD_ADDR

    *result = 1;
    *a2 = 0;
    thread = _thr_main;

    if (!(((u32)thread->object.node.next != 0xFFFFFFFF) &&
        ((u32)thread->object.node.next) &&
        ((u32)thread->object.node.next != BOOTINFO)))
        return;

    v3 = 0;

    while ((u32)thread->object.node.next != 0)
    {
        if (thread == _thr_executing)
            *a2 = v3;

        ++v3;
        thread = (lwp_cntrl *)ConvertAddress((u32)(thread->object.node.next));

        if (((u32)thread->object.node.next == 0xFFFFFFFF) ||
            (!(u32)thread->object.node.next) ||
            ((u32)thread->object.node.next == BOOTINFO))
            break;
    }

    *result = v3;
}

#endif

#endif
