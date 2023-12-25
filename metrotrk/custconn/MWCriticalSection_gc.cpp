/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases.



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#ifdef __GNUC__
    #include <ogc/irq.h>
#endif

#include "MWCriticalSection_gc.h"


/****************************************************************************/
/*
 *    MWInitializeCriticalSection
 *
 *    100 % DONE
 */
/****************************************************************************/
void MWInitializeCriticalSection(u32 *level)
{
#if defined(__MWERKS__)
    #pragma unused(level)
#elif defined(__GNUC__)
    UNUSED(level);
#endif

    return;
}

/****************************************************************************/
/*
 *    MWEnterCriticalSection
 *
 *    100 % DONE
 */
/****************************************************************************/
void MWEnterCriticalSection(u32 *level)
{
    u32 ret = 0;

#if defined(__MWERKS__)
    ret = OSDisableInterrupts();
#elif defined(__GNUC__)
    ret = IRQ_Disable();
#endif

    *level = ret;

    return;
}

/****************************************************************************/
/*
 *    MWExitCriticalSection
 *
 *    100 % DONE
 */
/****************************************************************************/
u32 MWExitCriticalSection(u32 *level)
{
#if defined(__MWERKS__)
    return OSRestoreInterrupts(*level);
#elif defined(__GNUC__)
    IRQ_Restore(*level);

    return *level;
#else
    return 0;
#endif
}

/****************************************************************************/
/*
 *    MWTerminateCriticalSection
 *
 *    100 % DONE
 */
/****************************************************************************/
void MWTerminateCriticalSection(u8 *level)
{
#if defined(__MWERKS__)
    #pragma unused(level)
#elif defined(__GNUC__)
    UNUSED(level);
#endif

    return;
}
