/****************************************************************************/
/*

DESCRIPTION



COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __TARGET_OPTIONS_H__
#define __TARGET_OPTIONS_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

    u8 GetUseSerialIO(void);
    void SetUseSerialIO(u8 result);

__end_extern_c


#endif // __TARGET_OPTIONS_H__
