/****************************************************************************/
/*

DESCRIPTION

    Nub intitialization routines.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __NUBINIT_H__
#define __NUBINIT_H__


#include "trktypes.h"
#include "trk_prefix.h"
#include "dserror.h"


/****************************************************************************/
/*
 *    Function Prototypes
 */
/****************************************************************************/

__extern_c

    /*
        Abstract function interface for target-specific functions
        (See ProcNub.c for implementation.)
    */

    DSError TRK_InitializeNub(void);
    DSError TRK_TerminateNub(void);
    void TRK_NubWelcome(void);
    DSError TRK_InitializeEndian(void);

    extern BOOL gTRKBigEndian;

__end_extern_c


#endif /* ndef __NUBINIT_H__ */
