/****************************************************************************/
/*

FILE

    main_TRK.c

    $Header: /u/debugger/embedded/CodeWarriorTRK/Portable/main_TRK.c,v 1.3 2006/04/25 08:23:29 tstanescu Exp $

DESCRIPTION

HISTORY

    $History: $
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/CodeWarriorTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//

COPYRIGHT

    (c) 2000-2006 Freescale Semiconductor Inc.
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "main_TRK.h"
#include "nubinit.h"
#include "targimpl_ppc.h"
#include "mainloop.h"


#define DEBUG_MAIN  0


static DSError TRK_mainError;            /* in lieu of an exit() value */


/****************************************************************************/
/*
 *    TRK_main
 *
 *    100 % DONE
 */
/****************************************************************************/
void TRK_main(void)
{
    TRK_mainError = TRK_InitializeNub();

    if (TRK_mainError == kNoError)
    {
#if DEBUG_MAIN
        __puts("Calling NubWelcome\r\n");
#endif

        TRK_NubWelcome();

#if DEBUG_MAIN
        __puts("Calling NubMainLoop\r\n");
#endif

        TRK_NubMainLoop();
    }

#if DEBUG_MAIN
    __puts("Calling TerminateNub\r\n");
#endif

    TRK_mainError = TRK_TerminateNub();

    return;
}
