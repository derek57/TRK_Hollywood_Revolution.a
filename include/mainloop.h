/****************************************************************************/
/*

FILE
    mainloop.h

    $Header: /u/debugger/embedded/MetroTRK/Portable/mainloop.h,v 1.1 1999/08/11 21:53:26 harper Exp $

DESCRIPTION

    Main loop for TRK.

    Repeatedly handle host- and target-generated events.

HISTORY

    97 FEB 12 LLY    Added extern "C"

    $History: mainloop.h $
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.

COPYRIGHT

    (c) 1997-8 Metrowerks Corporation
    All rights reserved.
*/
/****************************************************************************/


#ifndef __MAINLOOP_H__
#define __MAINLOOP_H__


#include "nubevent.h"


__extern_c

    /*
     *    Public API and public implementation
     */
    void TRK_NubMainLoop(void);


    /*
     *    Public API and private implementation
     */

    void TRKHandleSupportEvent(NubEvent *event);
    void TRK_HandleRequestEvent(NubEvent *event);
    void TRK_Idle(void);

__end_extern_c


#endif /* ndef __MAINLOOP_H__ */