/****************************************************************************/
/*

FILE
    usr_put.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/usr_put.c,v 1.2 1999/08/16 20:15:04 smoore Exp $

DESCRIPTION

    Target-specific implementation for debugging output.

    Development support functions for debugging output.  These functions
    can be used to write to a buffer in memory and later examined using
    a memory dump view, or by using a UART.

    The UART interface is defined in UART.h and linked in as a separate
    library.

    See the header file <usr_put.h>, which is a portable API for this file.

HISTORY

    $History: usr_put.c $
//
//    *****************  Version 11  *****************
//    User: Smoore       Date: 8/16/99    Time: 2:18p
//    Updated in $/Embedded/MetroTRK/Portable
//
//    *****************  Version 9  *****************
//    User: Smoore       Date: 7/30/99    Time: 3:44p
//    Updated in $/Embedded/MetroTRK/Portable
//    - Enhanced console dump debug option.
//
//    *****************  Version 8  *****************
//    User: Smoore       Date: 6/25/99    Time: 4:47p
//    Updated in $/Embedded/MetroTRK/Portable
//    - Moved all reset-handling code into a separate section so that the
//    main
//      body of TRK code can be copied to RAM.
//    - Added a new self-debugging mode which periodically dumps debug
//      memory buffer contents to the debugger console window.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 8/31/98    Time: 3:27p
//    Updated in $/Embedded/MetroTRK/Portable
//    Altered source to support non-Metrowerks tools:
//       - Eliminated use of '#pragma once'
//       - Eliminated need for a prefix file.
//       - Protected MW-specific pragmas with '#if __MWERKS__'
//
//    Fixed bug in mem.c.
//
//    *****************  Version 6  *****************
//    User: Smoore       Date: 1/21/98    Time: 5:47p
//    Updated in $/Embedded/MetroTRK/Portable
//    Merging in changes from VR TRK.
//
//    *****************  Version 7  *****************
//    User: Smoore       Date: 1/16/98    Time: 11:52a
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Portable
//    Changed debug buffer code to place the end-of-buffer pointer at the
//    beginning of the buffer so that it is easy to find when viewing the
//    buffer.
//
//    *****************  Version 5  *****************
//    User: Smoore       Date: 10/15/97   Time: 11:52a
//    Updated in $/Embedded/MetroTRK/Portable
//    Some non-static functions were being declared as static, causing
//    compile
//    errors when C++ enabled.  Fixed.
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 9/02/97    Time: 3:15p
//    Updated in $/Embedded/MetroTRK/Portable
//    Fixed compile problem when both DEBUGIO_RAM and DEBUGIO_SERIAL
//    are undefined.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 8/28/97    Time: 4:49p
//    Updated in $/Embedded/MetroTRK/Portable
//    Moved RAM buffer address and RAM/Serial debug options to board-
//    specific header usr_put_config.h.  Removed '\r' filter.  Modified to
//    support
//    compilation with neither RAM nor serial debug options on.  Renamed
//    __puts() -> __do_puts() and the other __put* functions similarly.

COPYRIGHT

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    NO HEX-BINARY MATCH (due to compiler version used)

*/
/****************************************************************************/


#include "trk_prefix.h"
#include "msgbuf.h"
#include "target.h"
#include "msgcmd.h"
#include "msghndlr.h"
#include "support.h"
#include "usr_put.h"
#include "UART.h"
#include "usr_put_config.h"


#if 0 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

void usr_put_initialize(void);
static void usr_put_initialize_ram(void);
static DSError usr_putchar_ram(s32 c);
static DSError usr_puts_ram(const s8 *s);


static s8 **mpp;            /* memory pointer */


/*
** During the process of dumping debug RAM contents to the
** console, the debug RAM must be locked so that it is not
** affected by any additional debug output that is generated.
** It is unlocked as soon as the console dump is complete.
*/

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP)
    static BOOL db_ram_locked = FALSE;
#endif


/****************************************************************************/
/*
 *    usr_putchar_serial
 *
 *    Write one character out to serial
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
DSError usr_putchar_serial(s32 c)
{
    BOOL result;
    s8 string[2];

    result = GetTRKConnected();

    string[0] = (s8)c;
    string[1] = 0;

    SetTRKConnected(FALSE);
    OSReport(string);
    SetTRKConnected(result);

    return kNoError;
}

/****************************************************************************/
/*
 *    usr_puts_serial
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError usr_puts_serial(const s8 *s)
{
    DSError result = kNoError;
    s8 c;

    while (((result == 0) && (c = *s++)) != 0)
    {
        // c = (c == '\n') ? '\r' : c;
        result = usr_putchar_serial(c);
    }

    return result;
}


/****************************************************************************/
/*
 *    usr_put_initialize_ram
 *
 *    100 % DONE
 */
/****************************************************************************/
static void usr_put_initialize_ram(void)
{
#ifdef DEBUGIO_RAM
    s8 *cp;

    /*
    ** The beginning of the buffer always holds a pointer to the
    ** current buffer position.  The initial buffer position
    ** immediately follows that pointer.
    */

    mpp = (s8 **)DB_START;
    *mpp = (DB_START + sizeof(*mpp));

    for (cp = *mpp; cp < DB_END; cp++)
    {
        *cp = 0;
    }

#endif
}

/****************************************************************************/
/*
 *    usr_putchar_ram
 *
 *    100 % DONE
 */
/****************************************************************************/
static DSError usr_putchar_ram(s32 c)
{
#ifdef DEBUGIO_RAM

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP)
    if (!db_ram_locked)
#endif
    {
        *((*mpp)++) = c;

        if (*mpp >= (s8 *)(s32)DB_END)
            *mpp = (s8 *)((s32)DB_START + sizeof(mpp));
    }
#else
    #if defined(__MWERKS__)
        #pragma unused(c)
    #elif defined(__GNUC__)
        UNUSED(c);
    #endif
#endif

    return kNoError;
}

/****************************************************************************/
/*
 *    usr_puts_ram
 *
 *    100 % DONE
 */
/****************************************************************************/
static DSError usr_puts_ram(const s8 *p)
{
    s32 c;

    while ((c = *p++) != 0)
    {
        usr_putchar_ram(c);
    }

    return kNoError;
}


/****************************************************************************/
/*
 *    usr_put_initialize
 *
 *    Initialize the serial/memory output buffer
 *
 *    100 % DONE
 */
/****************************************************************************/
void usr_put_initialize(void)
{
#ifdef DEBUGIO_RAM
    usr_put_initialize_ram();
#endif

#ifdef DEBUGIO_SERIAL
    // already initialized
#endif
}

/****************************************************************************/
/*
 *    usr_putchar
 *
 *    Put one character to serial or to memory
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError usr_putchar(s32 c)
{
#if !defined(DEBUGIO_RAM) && !defined(DEBUGIO_SERIAL)
    #if defined(__MWERKS__)
        #pragma unused(c)
    #elif defined(__GNUC__)
        UNUSED(c);
    #endif
#endif

    DSError result = kNoError;

#ifdef DEBUGIO_RAM
    result = usr_putchar_ram(c);
#endif

#ifdef DEBUGIO_SERIAL
    if (result == 0)
        usr_putchar_serial(c);
#endif

    return result;
}

/****************************************************************************/
/*
 *    usr_puts
 *
 *    Put a character string to serial or to memory.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError usr_puts(const s8 *s)
{
#if !defined(DEBUGIO_RAM) && !defined(DEBUGIO_SERIAL)
    #if defined(__MWERKS__)
        #pragma unused(s)
    #elif defined(__GNUC__)
        UNUSED(s);
    #endif
#endif

    DSError result = kNoError;

#ifdef DEBUGIO_RAM
    result = usr_puts_ram(s);
#endif

#ifdef DEBUGIO_SERIAL
    if (result == kNoError)
        result = usr_puts_serial(s);
#endif

    return result;
}

/****************************************************************************/
/*
 *    __do_console_dump
 *
 *    100 % DONE
 */
/****************************************************************************/
#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP)


#define MAX_CONSOLE_DUMP_SIZE    250


void __do_console_dump(void)
{
    s32 total, current, idx, temp;
    DSIOResult io_result;

    /*
    ** Dump everything from the start of the RAM buffer to the
    ** current position and reset the current position pointer
    ** to the start.
    */

    total = (*mpp - (DB_START + sizeof(*mpp)));
    *mpp = (DB_START + sizeof(*mpp));

    /*
    ** Send it as multiple WriteFile requests, which should show up in the
    ** debugger console window.  It attempts to send the output
    ** one line at a time.
    **
    ** Note: in some places where this is called, TRK is already
    ** using one message buffer, the WriteFile requires another,
    ** and the reply would require a third.  By default TRK only
    ** has two buffers.  When this feature is enabled, TRK must
    ** have at least 3 buffers.  This is taken care of with
    ** #ifdef's in "msgbuf.h".
    */

    db_ram_locked = TRUE;

    for (idx = 0; idx < total; idx += current)
    {
        /*
        ** Get the size of the next thing to write.  Start with
        ** the max dump size and work backwards to find the last
        ** line ending in it.  We want a line-ending to be the
        ** last character in the message because some debuggers
        ** will add it anyway.  If we send random chunks of output,
        ** we may get random line endings sprinkled throughout.
        ** If there is no line ending, send the whole chunk.
        */

        current = ((MAX_CONSOLE_DUMP_SIZE < (total - idx)) ?
            MAX_CONSOLE_DUMP_SIZE : (total - idx));

        for (temp = (current - 2); temp >= 0; --temp)
        {
            if ((*mpp)[idx + temp] == '\n')
            {
                current = (temp + 1);
                break;
            }
        }

        temp = current;
        TRK_SuppAccessFile(kDSStdout, (u8 *)(*mpp + idx), (size_t *)&temp, &io_result, TRUE, FALSE);
    }

    db_ram_locked = FALSE;
}

#endif

/****************************************************************************/
/*
 *    __do_putchar
 *
 *    Put one character to serial or to memory.  Don't call this function
 *    directly.  Use the __putchar macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_putchar(s8 c)
{
    return usr_putchar(c);
}

/****************************************************************************/
/*
 *    __do_puts
 *
 *    Put a character string to serial or to memory.  Don't call this function
 *    directly.  Use the __puts macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_puts(const s8 *s)
{
    return usr_puts(s);
}


static const char gHexChars[] = "0123456789ABCDEF";


/****************************************************************************/
/*
 *    __do_puthex32
 *
 *    Put a numeric value to serial or to memory in 32-digit hex format.
 *    Don't call this function directly.  Use the __puthex32 macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_puthex32(void *x)
{
    s32 i;
    s8 s[33];
    u8 copy[16];
    u32 *p;

    // deep copy the content of x to copy array
    for (i = 15; i >= 0; i--)
        copy[i] = (u8)((s8 *)x)[i];

    p = (u32 *)copy;

    for (i = 31; i >= 0; i--)
    {
        if ((i == 23) || (i == 15) || (i == 7))
            p += 1;

        s[i] = gHexChars[*p & 0xF];      /* convert binary to ASCII */
        *p >>= 4;
    }

    s[32] = 0;                           /* terminating zero (cstring) */

    return usr_puts(s);
}


/****************************************************************************/
/*
 *    __do_puthex8
 *
 *    Put a numeric value to serial or to memory in 8-digit hex format.
 *    Don't call this function directly.  Use the __puthex8 macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_puthex8(u32 x)
{
    s32 i;
    s8 s[9];

    for (i = 7; i >= 0; i--)
    {
        s[i] = gHexChars[x & 0xF];       /* convert binary to ASCII */
        x >>= 4;
    }

    s[8] = 0;                            /* terminating zero (cstring) */

    return usr_puts(s);
}

/****************************************************************************/
/*
 *    __do_puthex4
 *
 *    Put a numeric value to serial or to memory in 4-digit hex format.
 *    Don't call this function directly.  Use the __puthex4 macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_puthex4(u16 x)
{
    s32 i;
    s8 s[5];

    for (i = 3; i >= 0; i--)
    {
        s[i] = gHexChars[x & 0xF];
        x >>= 4;
    }

    s[4] = 0;

    return usr_puts(s);
}

/****************************************************************************/
/*
 *    __do_puthex2
 *
 *    Put a numeric value to serial or to memory in 2-digit hex format.
 *    Don't call this function directly.  Use the __puthex2 macro instead.
 *
 *    100 % DONE
 */
/****************************************************************************/
DSError __do_puthex2(u8 x)
{
    s32 i;
    s8 s[3];

    for (i = 1; i >= 0; i--)
    {
        s[i] = gHexChars[x & 0xF];
        x >>= 4;
    }

    s[2] = 0;

    return usr_puts(s);
}

#endif
