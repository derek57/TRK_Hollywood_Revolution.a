/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED
    NO HEX-BINARY MATCH (due to compiler version used)

*/
/****************************************************************************/


#include "trk_prefix.h"

/*
 *    Debugger Nub includes
 */
#ifdef __GNUC__
    #include <ogc/irq.h>
#endif

#include "trktypes.h"
#include "cc_gdev.h"
#include "UDP_Stubs.h"


#if 0 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

/****************************************************************************/
/*
 *    udp_cc_initialize - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
#if defined(__MWERKS__)
UDPError udp_cc_initialize(volatile u8 **inputPendingPtrRef, DBCallback cb)
#elif defined(__GNUC__)
UDPError udp_cc_initialize(volatile u8 **inputPendingPtrRef, raw_irq_handler_t cb)
#endif
{
#if defined(__MWERKS__)
    #pragma unused(inputPendingPtrRef, cb)
#elif defined(__GNUC__)
    UNUSED(inputPendingPtrRef);
    UNUSED(cb);
#endif

    return kUDPDataError;
}

/****************************************************************************/
/*
 *    udp_cc_shutdown - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_shutdown(void)
{
    return kUDPDataError;
}

/****************************************************************************/
/*
 *    udp_cc_open - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_open(void)
{
    return kUDPDataError;
}

/****************************************************************************/
/*
 *    udp_cc_close - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_close(void)
{
    return kUDPDataError;
}

/****************************************************************************/
/*
 *    udp_cc_read - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_read(u8 *buffer, s32 length)
{
#if defined(__MWERKS__)
    #pragma unused(buffer, length)
#elif defined(__GNUC__)
    UNUSED(buffer);
    UNUSED(length);
#endif

    return kUDPNoError;
}

/****************************************************************************/
/*
 *    udp_cc_write - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_write(u8 *buffer, s32 length)
{
#if defined(__MWERKS__)
    #pragma unused(buffer, length)
#elif defined(__GNUC__)
    UNUSED(buffer);
    UNUSED(length);
#endif

    return kUDPNoError;
}

/****************************************************************************/
/*
 *    udp_cc_peek - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_peek(void)
{
    return kUDPNoError;
}

/****************************************************************************/
/*
 *    udp_cc_pre_continue - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_pre_continue(void)
{
    return kUDPDataError;
}

/****************************************************************************/
/*
 *    udp_cc_post_stop - no contents
 *
 *    NOTE: The original code can be found within "CC_UDP_GC_Common.o"
 *          of "TRK_UDP_Server.a" for Dolphin
 *
 *    100 % DONE (NEVER USED ON THE WII / NDEV BUT FOR DDH (MINNOW) INSTEAD
 */
/****************************************************************************/
UDPError udp_cc_post_stop(void)
{
    return kUDPDataError;
}

#endif
