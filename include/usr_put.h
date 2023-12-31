/****************************************************************************/
/*

DESCRIPTION

    Portable API for debugging CWDS.

    Development support functions for debugging output.  These functions
    can be used to write to a buffer in memory and later examined using
    a memory dump view, or by using a UART.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __USR_PUT_H__
#define __USR_PUT_H__


#include "trktypes.h"
#include "dserror.h"
#include "trk_prefix.h"
#include "target.h"
#include "msgcmd.h"


#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define TRK_PROCESS_ID      getpid()
    #define TRK_THREAD_ID       0
#endif


__extern_c

    DSError usr_putchar(s32 c);
    DSError usr_puts(const s8 *s);
    void usr_put_initialize(void);

#if defined(DEBUGIO_RAM) || defined(DEBUGIO_SERIAL) || defined(DEBUGIO_CUSTOM)

    // The ((void)0) hushes up warnings about an empty body in an else statement

    #define __putchar(c)    OSReport("%c", c)
    #define __puts(s)       OSReport("%s", s)
    #define __puthex32(v)   OSReport("0x%32x", v)
    #define __puthex20(v)   OSReport("0x%20x", v)
    #define __puthex16(v)   OSReport("0x%16x", v)
    #define __puthex8(v)    OSReport("0x%08x", v)
    #define __puthex4(v)    OSReport("0x%04x", v)
    #define __puthex2(v)    OSReport("0x%02x", v)

    #ifndef DEBUG_ASSERT
        #define DEBUG_ASSERT    1
    #endif

#else

    #define __putchar(c)    ((void)0)
    #define __puts(s)       ((void)0)
    #define __puthex32(v)   ((void)0)
    #define __puthex20(v)   ((void)0)
    #define __puthex16(v)   ((void)0)
    #define __puthex8(v)    ((void)0)
    #define __puthex4(v)    ((void)0)
    #define __puthex2(v)    ((void)0)

    #ifndef DEBUG_ASSERT
        #define DEBUG_ASSERT    0
    #endif

#endif

    /*
        Serial-specific functions
    */
    DSError usr_puts_serial(const s8 *s);
    DSError usr_putchar_serial(s32 c);

__end_extern_c


#endif /* __USR_PUT_H__ */
