/****************************************************************************/
/*

DESCRIPTION

    Abstract interface to serial device (UART) for CodeWarriorTRK.
    For maximum portability, do not change this file.

    NOTE: This interface is obsolete.  The new interface is
    define in transport_clayer.h.  This interface is still
    supported in most cases for legacy code.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __UART_H__
#define __UART_H__


/* #pragma once */
#include "trk_prefix.h"


/****************************************************************************/

typedef s32 UARTError;

enum
{
    kUARTDataError = -1,
    kUARTNoError = 0,
    kUARTUnknownBaudRate,
    kUARTConfigurationError,
    kUARTBufferOverflow,                /* specified buffer was too small */
    kUARTNoData                         /* no data available from polling */
};

/****************************************************************************/

typedef enum
{
    kBaudHWSet = -1,                    /* use HW settings such as DIP switches */
    kBaud300 = 300,                     /* valid baud rates */
    kBaud600 = 600,
    kBaud1200 = 1200,
    kBaud1800 = 1800,
    kBaud2000 = 2000,
    kBaud2400 = 2400,
    kBaud3600 = 3600,
    kBaud4800 = 4800,
    kBaud7200 = 7200,
    kBaud9600 = 9600,
    kBaud19200 = 19200,
    kBaud38400 = 38400,
    kBaud57600 = 57600,
    kBaud115200 = 115200,
    kBaud230400 = 230400
} UARTBaudRate;

__extern_c

    UARTError InitializeUART(UARTBaudRate baudRate);

UARTError TRK_InitializeIntDrivenUART(u8 intDrivenInput,
                                   u8 intDrivenOutput,
                                   volatile u8 **inputPendingPtrRef);

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    UARTError TerminateUART(void);
#endif

    UARTError TRKReadUARTPoll(s8 *c);

    UARTError ReadUART1(s8 *c);
    UARTError TRKReadUARTN(void *bytes, u32 length);
    UARTError ReadUARTString(s8 *string, u32 limit, s8 terminateChar);

    UARTError WriteUART1(u8 c);
    UARTError TRK_WriteUARTN(const void *bytes, u32 length);

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    UARTError WriteUARTString(const s8 *string);
#endif

    UARTError WriteUARTFlush(void);

    UARTError TRKPollUART(void);
    void TRKUARTInterruptHandler(void);

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    void TransportIrqHandler(void);
#endif

__end_extern_c


#endif /* __UART_H__ */
