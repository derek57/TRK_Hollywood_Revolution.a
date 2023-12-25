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


#ifdef __GNUC__
    #include <ogc/cache.h>
#endif

#include "osreport.h"
#include "msg.h"
#include "msgxtrct.h"
#include "mem_TRK.h"
#include "dolphin_trk_glue.h"
#include "cc_gdev.h"
#include "targimpl.h"

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #include "UDP_Stubs.h"
#endif

// This is only required to match up with the hex binary data of the original object file
#include "dummy_c.h"


#define COMM_BUF_SIZE    (((DS_MAXMESSAGESIZE + 4) * 2) + 2)


s8 TRK_Use_BBA = FALSE;


extern void PPCHalt(void);

#if defined(__MWERKS__)
    extern void ICInvalidateRange(void *addr, u32 bytes);
    extern void DCFlushRange(void *addr, u32 bytes);
    extern s32 OSEnableScheduler(void);


    static s32 gWritePos = 0;
#elif defined(__GNUC__)
    s32 gWritePos = 0; // externally used
#endif


#ifndef __GNUC__ // stop complaining about unused variables
static s32 gReadPos = 0;
static s32 gReadCount = 0;
#endif


struct gDBCommTable_t
{
#if defined(__MWERKS__)
    s32 (*init) (volatile u8 **inputPendingPtrRef, DBCallback cb);
#elif defined(__GNUC__)
    s32 (*init) (volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb);
#endif

    s32 (*interrupt) (void);
    s32 (*shutdown) (void);
    s32 (*peek) (void);
    s32 (*read) (u8 *buffer, s32 data);
    s32 (*write) (u8 *buffer, s32 data);
    s32 (*open) (void);
    s32 (*close) (void);
    s32 (*pre_continue) (void);
    s32 (*stop) (void);
};

struct gDBCommTable_t gDBCommTable = { 0 };


/****************************************************************************/
/*
 *    TRKLoadContext
 *
 *    100 % DONE (This code was originally written in pure PPC assembly)
 *
 *                Aside from this fact, it's clear that TRKLoadContext
 *                is a modified version of OSLoadContext from the RVL-SDK
 *
 *    r3 (first arg): pointer to the contents of the OSContext structure
 *    r4 (second arg): exception to call for (0x500 -> "External Interrupt")
 */
/****************************************************************************/

// MOVED TO DOLPHIN_TRK_GLUE_ASM.S FOR GCC
#ifdef __MWERKS__

ASM_PREFIX void TRKLoadContext(void *, u32)
{
    nofralloc                   // don't create a frame

    lwz       r0, 0(r3)         // read GPR0 from context
    lwz       r1, 4(r3)         // read GPR1 from context (sp)
    lwz       r2, 8(r3)         // read GPR2 from context (toc)
    lhz       r5, 0x1A2(r3)     // read state from context
    rlwinm.   r6, r5, 0,30,30   // modify state value (r6 = (r5 & 0x2))
    beq       notexc
    rlwinm    r5, r5, 0,31,29   // modify state value (r5 &= ~0x2)
    sth       r5, 0x1A2(r3)     // write new state value
    lmw       r5, 0x14(r3)      // read all GPR's from #5 till #31 of context
    b         misc

notexc:

    lmw       r13, 0x34(r3)     // read all GPR's from #13 till #31 of context

misc:

    mr        r31, r3           // move context to register 31
    mr        r3, r4            // move "External Interrupt" signal (0x500) to register 3
    lwz       r4, 0x80(r31)     // read CR value from context
    mtcr      r4                // write to CR register
    lwz       r4, 0x84(r31)     // read LR value from context
    mtlr      r4                // write to LR register
    lwz       r4, 0x88(r31)     // read CTR value from context
    mtctr     r4                // write to CTR register
    lwz       r4, 0x8C(r31)     // read XER value from context
    mtxer     r4                // write to XER register
    mfmsr     r4                // read MSR register into register 4
    rlwinm    r4, r4, 0,17,15   // modify MSR value (r4 &= ~0x8000) (revokes MSR_EE)
    rlwinm    r4, r4, 0,31,29   // modify MSR value (r4 &= ~0x2) (revokes MSR_RI)
    mtmsr     r4                // write new MSR value in register 4 to MSR register
    mtsprg1   r2                // write GPR2 of context to SPRG1 (toc)
    lwz       r4, 0xC(r31)      // read GPR3 from context
    mtsprg2   r4                // write GPR3 of context to SPRG2
    lwz       r4, 0x10(r31)     // read GPR4 from context
    mtsprg3   r4                // write GPR4 of context to SPRG3
    lwz       r2, 0x198(r31)    // read SRR0 from context (toc)
    lwz       r4, 0x19C(r31)    // read SRR1 from context
    lwz       r31, 0x7C(r31)    // read GPR31 from context
    b         TRK_InterruptHandler
}

#endif

/****************************************************************************/
/*
 *    TRKEXICallBack
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRKEXICallBack(volatile u8 **inputPendingPtrRef, void *cb)
{
#if defined(__MWERKS__)
    #pragma unused(inputPendingPtrRef)

    OSEnableScheduler();

    // TRK_TRANSPORT_INT_KEY = "External Interrupt" exception call (0x500)
    TRKLoadContext(cb, TRK_TRANSPORT_INT_KEY);

#elif defined(__GNUC__)

    UNUSED(inputPendingPtrRef);
    UNUSED(cb);

    // What to do here when it comes to the usage of GCC ???
    // libOGC uses "_cpu_context_restore"
#endif
}

/****************************************************************************/
/*
 *    InitMetroTRKCommTable
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError InitMetroTRKCommTable(s32 reply)
{
    UARTError err;

    OSReport("Devkit set to : %ld\n", reply);
    OSReport("MetroTRK : Sizeof Reply - %ld bytes\n", TRK_MSG_HEADER_LENGTH);

    TRK_Use_BBA = FALSE;

    if (reply == kDSDisconnect)
    {
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        TRK_Use_BBA = TRUE;

        OSReport("MetroTRK : Set to BBA\n");

        gDBCommTable.init = &udp_cc_initialize;
        gDBCommTable.open = &udp_cc_open;
        gDBCommTable.close = &udp_cc_close;
        gDBCommTable.read = &udp_cc_read;
        gDBCommTable.write = &udp_cc_write;
        gDBCommTable.shutdown = &udp_cc_shutdown;
        gDBCommTable.peek = &udp_cc_peek;
        gDBCommTable.pre_continue = &udp_cc_pre_continue;
        gDBCommTable.stop = &udp_cc_post_stop;
        gDBCommTable.interrupt = NULL;
#endif
        err = kUARTNoError;
    }
    else if (reply == kDSConnect)
    {
        OSReport("MetroTRK : Set to NDEV hardware\n");

        gDBCommTable.init = &gdev_cc_initialize;
        gDBCommTable.open = &gdev_cc_open;
        gDBCommTable.close = &gdev_cc_close;
        gDBCommTable.read = &gdev_cc_read;
        gDBCommTable.write = &gdev_cc_write;
        gDBCommTable.shutdown = &gdev_cc_shutdown;
        gDBCommTable.peek = &gdev_cc_peek;
        gDBCommTable.pre_continue = &gdev_cc_pre_continue;
        gDBCommTable.stop = &gdev_cc_post_stop;
        gDBCommTable.interrupt = &gdev_cc_initinterrupts;

        err = kUARTNoError;
    }
    else
    {
        OSReport("MetroTRK : Set to UNKNOWN hardware. (%ld)\n", reply);
        OSReport("MetroTRK : Invalid hardware ID passed from OS\n");
        OSReport("MetroTRK : Defaulting to GDEV Hardware\n");

        err = kUARTUnknownBaudRate;
    }

    return err;
}

/****************************************************************************/
/*
 *    TRKUARTInterruptHandler
 *
 *    100 % DONE - USED, BUT NO CONTENTS...
 */
/****************************************************************************/
void TRKUARTInterruptHandler(void)
{
    return;
}

/****************************************************************************/
/*
 *    TRK_InitializeIntDrivenUART
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError TRK_InitializeIntDrivenUART(u8 intDrivenInput, u8 intDrivenOutput, volatile u8 **inputPendingPtrRef)
{
#if defined(__MWERKS__)
    #pragma unused(intDrivenInput, intDrivenOutput)

    gDBCommTable.init(inputPendingPtrRef, (DBCallback)TRKEXICallBack);
#elif defined(__GNUC__)
    UNUSED(intDrivenInput);
    UNUSED(intDrivenOutput);

    gDBCommTable.init(inputPendingPtrRef, (raw_irq_handler_t)TRKEXICallBack);
#endif

    gDBCommTable.open();

    return kUARTNoError;
}

/****************************************************************************/
/*
 *    EnableEXI2Interrupts
 *
 *    100 % DONE
 */
/****************************************************************************/
void EnableEXI2Interrupts(void)
{
    if (DSFetch_u8(&TRK_Use_BBA) != FALSE)
        return;

    if (gDBCommTable.interrupt == NULL)
        return;

    gDBCommTable.interrupt();

    return;
}

/****************************************************************************/
/*
 *    TRKPollUART
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError TRKPollUART(void)
{
    return gDBCommTable.peek();
}

/****************************************************************************/
/*
 *    TRKReadUARTN
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError TRKReadUARTN(void *bytes, u32 length)
{
    UARTError err = gDBCommTable.read(bytes, (s32)length);

    if (err != kUARTNoError)
        return kUARTDataError;

    return kUARTNoError;
}

/****************************************************************************/
/*
 *    TRK_WriteUARTN
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError TRK_WriteUARTN(const void *bytes, u32 length)
{
    UARTError err = gDBCommTable.write((void *)bytes, (s32)length);

    if (err != kUARTNoError)
        return kUARTDataError;

    return kUARTNoError;
}

/****************************************************************************/
/*
 *    WriteUARTFlush
 *
 *    Imported from the file "linux_comm.c"
 *    Found within "CodeWarrior for Power Architecture"
 *
 *    It's name there is "FlushCommLinux" (though - it's heavily modified...)
 *    It was NEVER USED in the end
 *
 *    100 % DONE
 */
/****************************************************************************/
static u8 gWriteBuf[COMM_BUF_SIZE];

#ifndef __GNUC__ // stop complaining about unused variables
static u8 gReadBuf[COMM_BUF_SIZE];
#endif

UARTError WriteUARTFlush(void)
{
    UARTError err;
    s32 curr_pos = gWritePos;
    u8 *ptr = &gWriteBuf[curr_pos];

    while (curr_pos < DS_MAXREADWRITELENGTH)
    {
        curr_pos++;
        *ptr++ = 0;
    }

    gWritePos = curr_pos;

    if (gDBCommTable.write(gWriteBuf, gWritePos) != kUARTNoError)
        err = kUARTDataError;
    else
        err = kUARTNoError;

    gWritePos = 0;

    return err;
}

/****************************************************************************/
/*
 *    WriteUART1
 *
 *    Imported from the file "linux_comm.c"
 *    Found within "CodeWarrior for Power Architecture"
 *
 *    It's name there is "WriteCommLinux"
 *    It was NEVER USED in the end
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError WriteUART1(u8 c)
{
    gWriteBuf[gWritePos++] = c;

    return kUARTNoError;
}

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRKReadUARTPoll
 *
 *    Imported from the file "linux_comm.c"
 *    Found within "CodeWarrior for Power Architecture"
 *
 *    It's name there is "ReadCommLinux" (though - it's a bit modified...)
 *
 *    100 % DONE
 */
/****************************************************************************/
UARTError TRKReadUARTPoll(s8 *byte)
{
    UARTError ret;
    UARTError err = kUARTNoData;

    if (gReadPos >= gReadCount)
    {
        gReadPos = 0;
        gReadCount = gDBCommTable.peek();

        if (gReadCount > 0)
        {
            if (gReadCount > COMM_BUF_SIZE)
                gReadCount = COMM_BUF_SIZE;

            if (gDBCommTable.read((void *)gReadBuf, gReadCount) != kUARTNoError)
                ret = kUARTDataError;
            else
                ret = kUARTNoError;

            err = ret;

            if (err != kUARTNoError)
                gReadCount = 0;
        }
    }

    if (gReadPos < gReadCount)
    {
        *byte = gReadBuf[gReadPos++];
        err = kUARTNoError;
    }

    return err;
}
#endif

/****************************************************************************/
/*
 *    ReserveEXI2Port
 *
 *    100 % DONE
 */
/****************************************************************************/
void ReserveEXI2Port(void)
{
    gDBCommTable.stop();

    return;
}

/****************************************************************************/
/*
 *    UnreserveEXI2Port
 *
 *    100 % DONE
 */
/****************************************************************************/
void UnreserveEXI2Port(void)
{
    gDBCommTable.pre_continue();

    return;
}

/****************************************************************************/
/*
 *    TRK_board_display
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRK_board_display(s8 *string)
{
    OSReport("%s\n", string);

    return;
}

/****************************************************************************/
/*
 *    InitializeProgramEndTrap
 *
 *    100 % DONE
 */
/****************************************************************************/

#if defined(__MWERKS__)
    BOOL _MetroTRK_Has_Framing __attribute__((force_export)) = FALSE;
#elif defined(__GNUC__)
    BOOL _MetroTRK_Has_Framing __attribute__((used)) = FALSE;
#endif

void InitializeProgramEndTrap(void)
{
    static const s8 EndofProgramInstruction[4] = { 0x00, 'E', 'N', 'D' };
    u32 size = sizeof(EndofProgramInstruction);

    TRK_memcpy((&PPCHalt + size), EndofProgramInstruction, size);
    ICInvalidateRange((&PPCHalt + size), size);
    DCFlushRange((&PPCHalt + size), size);

    return;
}
