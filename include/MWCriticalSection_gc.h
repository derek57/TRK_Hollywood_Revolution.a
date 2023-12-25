/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __MWCRITICALSECTION_GC_H__
#define __MWCRITICALSECTION_GC_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

    void MWEnterCriticalSection(u32 *level);
    u32 MWExitCriticalSection(u32 *level);
    void MWTerminateCriticalSection(u8 *level);
    void MWInitializeCriticalSection(u32 *level);


    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);

__end_extern_c


#endif // __MWCRITICALSECTION_GC_H__
