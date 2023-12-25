/****************************************************************************/
/*
FILE
    mutex_TRK.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/mutex_TRK.c,v 1.1 1999/08/11 21:53:27 harper Exp $

DESCRIPTION

    Synchronization primitives

    Target-dependent implementation for a mutual-exclusion synchronization
    primitive.

MODIFICATIONS

    $History: mutex_TRK.c $
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
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 10/06/97   Time: 3:05p
//    Updated in $/Embedded/MetroTRK/Portable
//    Renamed from "mutex.h" to avoid MSL conflicts.

COPYRIGHT

    (c) 1996-8 Metrowerks Corporation
    All rights reserved.

    2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    NO HEX-BINARY MATCH (due to compiler version used)

*/
/****************************************************************************/


#include "trk_prefix.h"
#include "dserror.h"
#include "mutex_TRK.h"


#if 0 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

/****************************************************************************/
/*
 *    TRKInitializeMutex - no contents
 *
 *    100 % DONE (Function was added for v0.1 of the TRK to the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKInitializeMutex(DSMutex *mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
    UNUSED(mutex);
#endif

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKAcquireMutex - no contents
 *
 *    100 % DONE (Function was added for v0.1 of the TRK to the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKAcquireMutex(DSMutex *mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
    UNUSED(mutex);
#endif

    return kNoError;
}

/****************************************************************************/
/*
 *    TRKReleaseMutex - no contents
 *
 *    100 % DONE (Function was added for v0.1 of the TRK to the Wii / NDEV)
 */
/****************************************************************************/
DSError TRKReleaseMutex(DSMutex *mutex)
{
#if defined(__MWERKS__)
    #pragma unused(mutex)
#elif defined(__GNUC__)
    UNUSED(mutex);
#endif

    return kNoError;
}

#endif
