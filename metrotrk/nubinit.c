/****************************************************************************/
/*

FILE

    nubinit.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/nubinit.c,v 1.4 2000/06/16 21:52:54 smoore Exp $

DESCRIPTION

    TRK initialization functions.

MODIFICATIONS

    96 JUN 01 TS    Original implementation for PowerTV/XPPC Nub Protocol
    96 SEP 22 LLY    Reformatted source code and comments
    96 OCT 19 LLY    Converted for MIPS
    96 NOV 02 LLY    Converted for Motorola PowerPC MPC821
    97 FEB 12 LLY    Removed msgproto.h from include list.  Removed const
                     pointer passed to Dispatch
    97 FEB 12 LLY    Initialized dispatcher
    97 FEB 15 LLY    Removed target-independent portion, now in serpoll.c
    97 MAR 01 LLY    Removed #include __config.h

    $History: nubinit.c $
//
//    *****************  Version 11  *****************
//    User: Smoore       Date: 1/04/99    Time: 4:24p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merged NEC V8xx portable-code changes into the main repository.
//
//    *****************  Version 10  *****************
//    User: Smoore       Date: 11/10/98   Time: 1:08p
//    Updated in $/Embedded/MetroTRK/Portable
//    Board welcome string is now displayed before the serial welcome
//    message.
//    Otherwise, if hardware flow control were enabled and the board were not
//    connected to the host, the board welcome string would never be seen.
//
//    *****************  Version 9  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 8  *****************
//    User: Smoore       Date: 8/14/98    Time: 10:38a
//    Updated in $/Embedded/MetroTRK/Portable
//    Re-organized PPC TRK code and added support for the Cogent
//    board with a PPC daughtercard.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 6/25/98    Time: 2:35p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in latest changes from VR TRK:
//       - Added support for FlushCache and Stop commands.
//       - Added support for interrupt-driven I/O.
//       - Added several extensions to the MetroTRK API,
//         including Level 3 (OS-level) extensions.
//       - Added support for the Midas RTE-VR5000-PC board.
//
//    *****************  Version 8  *****************
//    User: Smoore       Date: 6/24/98    Time: 2:39p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Added support for interrupt-driven I/O.
//    Added support for the FlushCache command.
//    Made several extensions to the MetroTRK API, including all of the
//    level 3 (OS-level) extensions.  None are currently implemented.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 3/19/98    Time: 12:35a
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Merging in console I/O support from the main repository.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 2/02/98    Time: 6:06p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging changes from NEC V8xx TRK.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 2/02/98    Time: 11:05a
//    Updated in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Portable
//    Changed welcome message from an FPuts message to a WriteFile
//    message.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:35p
//    Updated in $/Embedded/MetroTRK/Portable
//    Added dynamic endian detection.  Removed baud rate specification to
//    board-specific header file.  Removed use of 'goto'.  Changed 'protocol'
//    to 'API' in welcome message.

COPYRIGHT

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "nubinit.h"
#include "targimpl.h"

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    #include "dispatch.h"
#endif

#include "dolphin_trk_glue.h"
#include "serpoll.h"

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #include "usr_put.h"
#endif


#define DEBUG_NUBINIT       0


/*
** Either interrupt-driven input, interrupt-driven output,
** or both may be enabled by setting USE_RXIRQ or USE_TXIRQ,
** respectively, to 1.
*/

/*
** Setting USE_RXIRQ enables interrupt-driven
** UART input.  Setting USE_TXIRQ enables
** interrupt-driven UART output.  Set these
** only if the UART library being tested
** supports them.
*/

/* Kobler: For CodeWarriorTRK value (1) required */
#define USE_TXIRQ           0        /* int-driven transmit? */
#define USE_RXIRQ           1        /* int-driven receive? */


#if TRK_TRANSPORT_INT_DRIVEN
    extern volatile u8 *gTRKInputPendingPtr;
#endif // TRK_TRANSPORT_INT_DRIVEN


/*
** Global endian flag that is initialized on startup.
*/

BOOL gTRKBigEndian;


/******************************************************************************/
/*
 *    TRK_InitializeNub
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/******************************************************************************/
DSError TRK_InitializeNub(void)
{
    DSError err = TRK_InitializeEndian();

    /*
    ** Initialize output buffer for diagnostic output.
    */

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    if (err == kNoError)
        usr_put_initialize();
#endif

    if (err == kNoError)
        err = TRKInitializeEventQueue();

    if (err == kNoError)
        err = TRK_InitializeMessageBuffers();

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    if (err == kNoError)
        err = TRKInitializeDispatcher();
#endif

    InitializeProgramEndTrap();

    /*
        don't try to print anything before initializing the output port
    */

#if DEBUG_NUBINIT
    __puts("Calling InitializeSerialHandler\r\n");
#endif

    if (err == kNoError)
        err = TRKInitializeSerialHandler();

#if DEBUG_NUBINIT
    __puts("Calling InitializeTarget\r\n");
#endif

    if (err == kNoError)
        err = TRKInitializeTarget();

    if (err == kNoError)
    {
#if TRK_TRANSPORT_INT_DRIVEN
        /*
        ** Don't use interrupt-driven output.  The current implementation
        ** operates synchronously with the host, so it would save any
        ** time.  It would just raise issues about when the output must
        ** be flushed.
        */

        // The original call to "TRK_InitializeIntDrivenUART"
        // used to have 4 arguments:
        //
        // - baudRate
        // - intDrivenInput
        // - intDrivenOutput
        // - inputPendingPtr
        //
        // On the Wii / NDEV, they only used 3 arguments instead
        // (according to the disassembly). The baudRate was NEVER
        // being set inside the TRK library. Most likely, it was
        // or IS being set inside the Serial Debug Output library
        // functions ("OSReport").
        UARTError ret = TRK_InitializeIntDrivenUART(USE_RXIRQ, USE_TXIRQ, &gTRKInputPendingPtr);

        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);

        if (ret != kUARTNoError)
            err = (DSError)ret;
#else
        err = (DSError)InitializeUART(TRK_BAUD_RATE);
#endif // TRK_TRANSPORT_INT_DRIVEN
    }

#if DEBUG_NUBINIT
    __puts("Returning ");
    __puthex8(err);
    __puts(" from InitializeNub\r\n");
#endif

    return err;
}

/******************************************************************************/
/*
 *    TRK_TerminateNub
 *
 *    100 % DONE
 */
/******************************************************************************/
DSError TRK_TerminateNub(void)
{
    TRKTerminateSerialHandler();

#if DEBUG_NUBINIT
    __puts("Exiting MetroTRK.\r\n");
#endif

    return kNoError;
}

/******************************************************************************/
/*
 *    TRK_NubWelcome
 *
 *    100 % DONE
 */
/******************************************************************************/
void TRK_NubWelcome(void)
{
#ifndef SUPPRESS_WELCOME_MESSAGE
    s8         str[DS_MAXREADWRITELENGTH];
    size_t     count;
    DSIOResult io_result;
#endif /* #ifndef SUPPRESS_WELCOME_MESSAGE */

    /*
    ** If a board welcome string is defined, display it here.
    */

#ifdef BOARD_WELCOME_STRING
    TRK_board_display((s8 *)BOARD_WELCOME_STRING);
#endif

#ifndef SUPPRESS_WELCOME_MESSAGE
    /*
    ** Build welcome string in buffer.
    */

    str[0] = '\0';

    TRK_strcat(str, "\r\nWelcome to Metrowerks Target Resident Kernel for ");
    TRK_strcat(str, DS_TARGET_NAME);
    TRK_strcat(str, "\r\nVersion ");
    TRK_strcat(str, DS_KERNEL_VERSION_STRING);
    TRK_strcat(str, " implementing MetroTRK API version ");
    TRK_strcat(str, DS_PROTOCOL_VERSION_STRING);
    TRK_strcat(str, "\r\n");

    /*
    ** Send as a WriteFile command to stdout.
    */

    count = (size_t)TRK_strlen(str);

    TRK_SuppAccessFile(kDSStdout, (u8 *)str, &count, &io_result, FALSE, FALSE
#if DS_PROTOCOL == DS_PROTOCOL_RTOS
        , TRK_PROCESS_ID, TRK_THREAD_ID
#endif
        );

#endif /* #ifndef SUPPRESS_WELCOME_MESSAGE */

    return;
}

/******************************************************************************/
/*
 *    TRK_InitializeEndian
 *
 *    Tests the endian-ness of the processor and sets the global
 *    gTRKBigEndian accordingly.
 *
 *    100 % DONE
 */
/******************************************************************************/
DSError TRK_InitializeEndian(void)
{
    DSError    err = kNoError;
    u32        test;
    u8         *byte_alias = (u8 *)&test;

    gTRKBigEndian = TRUE;

    /*
    ** Write a specific 4-byte sequence and then read it as
    ** a 32-bit word.  Big-endian systems yield one value and
    ** little-endian systems yield another.
    */

    byte_alias[0] = 0x12;
    byte_alias[1] = 0x34;
    byte_alias[2] = 0x56;
    byte_alias[3] = 0x78;

    if (test == 0x12345678)
        gTRKBigEndian = TRUE;
    else if (test == 0x78563412)
        gTRKBigEndian = FALSE;
    else
        err = kInternalError;

    return err;
}
