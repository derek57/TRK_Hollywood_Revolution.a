/****************************************************************************/
/*

DESCRIPTION

    Low-level, target-specific interface to the support functions
    provided by CodeWarriorTRK.

COPYRIGHT


    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/
#pragma once


#ifndef __TARGSUPP_H__
#define __TARGSUPP_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

    u8 TRKAccessFile(u8 command, u32 file_handle, u32 *length_ptr, u8 *buffer_ptr);
    u8 TRKOpenFile(u8 command, u32 file_handle, u32 *length_ptr, u8 *buffer_ptr);
    u8 TRKCloseFile(u8 command, u32 file_handle);
    u8 TRKPositionFile(u8 command, u32 file_handle, u32 *length_ptr, u8 *buffer_ptr);

__end_extern_c


#endif // __TARGSUPP_H__
