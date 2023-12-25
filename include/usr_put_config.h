/****************************************************************************/
/*

FILE
    usr_put_config.h

    $Header: /u/debugger/embedded/MetroTRK/Processor/ppc/Board/motorola/sandpoint/usr_put_config.h,v 1.1 2000/04/07 19:49:58 sutanto Exp $

DESCRIPTION

    Target-specific implementation info for debugging output.

    See the header files <usr_put.h>/<usr_put.c>, which contain
    the portable debug output routines and API.

HISTORY

    $History: usr_put_config.h $

COPYRIGHT

    (c) 1996-2000 Metrowerks Corporation
    All rights reserved.

*/
/****************************************************************************/

#ifndef __USR_PUT_CONFIG_H__
#define __USR_PUT_CONFIG_H__


#include "trktypes.h"


/*
** Define any combination of these two.  Note that serial debug output
** uses the same serial channel as the TRK commands themselves.  It is
** useful for manual debugging, but is not likely to work with the host
** plugin.
*/

#undef DEBUGIO_SERIAL            /* define for writing out to serial port */
#undef DEBUGIO_RAM               /* define for writing to memory */

/*
** When DEBUGIO_RAM is defined, then defining DB_RAM_CONSOLE_DUMP will
** cause the debug output in the RAM buffer to be periodically
** dumped to the console (stdout) via the kDSWriteFile command.  This
** is useful when using the host plugin to drive MetroTRK, since the
** debug output will appear in the debugger's console window.  Note,
** however, that there are only certain times that the console dump
** can reliably be done.  Currently it is only done when the debugger tells
** MetroTRK to step or execute the target program.  Debug output
** in between these times is queued up until the next dump opportunity.
*/
#ifdef DEBUGIO_RAM
    #undef DB_RAM_CONSOLE_DUMP    /* define for dumping debug buffer to console */
#endif

/*
** Define a buffer to receive debugging output when DEBUGIO_RAM is
** defined.
*/

#ifdef DEBUGIO_RAM
    #define    DB_START ((s8 *)0x07E0000)        /* start of debug buffer */
    #define    DB_END   ((s8 *)0x07F0000)        /* end of debug buffer */
#endif

#endif // __USR_PUT_CONFIG_H__
