/****************************************************************************/
/*

FILE

    targcont.c

    $Source: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/targcont.c,v $

DESCRIPTION

    Target-specific implementation of continue for PowerPC

HISTORY

    $History: $

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


/*
 *    TRK includes
 */
#include "targimpl_ppc.h"
#include "dolphin_trk_glue.h"


/****************************************************************************/
/*
 *    TRKTargetContinue
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKTargetContinue(void)
{
    /* swap registers and go */

#ifdef DEBUG_REGISTER_DUMP
    OSReport("TargetContinue Outgoing Registers:\n");
    OSReport("PC    = 0x%08x\n", gTRKCPUState.Default.PC);
    OSReport("MSR   = 0x%08x\n", gTRKCPUState.Extended1.MSR);
    OSReport("LR    = 0x%08x\n", gTRKCPUState.Default.LR);
    OSReport("CR    = 0x%08x\n", gTRKCPUState.Default.CR);
    OSReport("CTR   = 0x%08x\n", gTRKCPUState.Default.CTR);
    OSReport("XER   = 0x%08x\n", gTRKCPUState.Default.XER);
    OSReport("DSISR = 0x%08x\n", gTRKCPUState.Extended1.DSISR);
    OSReport("DAR   = 0x%08x\n", gTRKCPUState.Extended1.DAR);
#endif

    /*
    ** Whatever the target process stopped state was, it will now
    ** be false.
    */

    TRKTargetSetStopped(FALSE);

    UnreserveEXI2Port();
    TRKSwapAndGo();

    ReserveEXI2Port();

#ifdef DEBUG_REGISTER_DUMP
    OSReport("TargetContinue Incoming Registers:\n");
    OSReport("PC    = 0x%08x\n", gTRKCPUState.Default.PC);
    OSReport("MSR   = 0x%08x\n", gTRKCPUState.Extended1.MSR);
    OSReport("LR    = 0x%08x\n", gTRKCPUState.Default.LR);
    OSReport("CR    = 0x%08x\n", gTRKCPUState.Default.CR);
    OSReport("CTR   = 0x%08x\n", gTRKCPUState.Default.CTR);
    OSReport("XER   = 0x%08x\n", gTRKCPUState.Default.XER);
    OSReport("DSISR = 0x%08x\n", gTRKCPUState.Extended1.DSISR);
    OSReport("DAR   = 0x%08x\n", gTRKCPUState.Extended1.DAR);
#endif

    /*
     *    After SwapAndGo returns, TRK has gained control because of
     *    either one of these cases:
     *
     *    1.  The target process stopped, an exception was handled, and
     *        our execution state (registers + stack) were restored.
     *
     *    2.  The target process was scheduled for execution in a non-
     *        blocking call, e.g. two tasks/threads are running now.
     *
     *    This function then returns, popping back up to the event loop.
     *    For the two different cases above, two things may happen.
     *
     *    1.  Before SwapAndGo returned control to TargetContinue, an
     *        exception or interrupt occurred.  An "program stopped" event
     *        is posted to the event queue, which will be picked up by
     *        the TRK main event loop.
     *
     *    2.  In the multitasking case, the target program continues to
     *        run as does TargetContinue.  This function returns in the
     *        TRK thread, and the TRK main loop continues to
     *        execute until an event arrives.  Concurrently, the target
     *        program executes and when an exception or interrupt occurs,
     *        the exception handler blocks the target task, creates
     *        a "program stopped" event, and posts it.  Subsequently,
     *        TRK will get the event and handle the "program stopped"
     *        event.
     *
     *    NB: TRK for bare board environments will typically use (1).
     */

    return kNoError;
}
