/****************************************************************************/
/*

DESCRIPTION

    Defines a common version number for all PPC CodeWarriorTRK binaries.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
*/
/****************************************************************************/
#pragma once


#ifndef __PPC_VERSION_H__
#define __PPC_VERSION_H__


#include "macro.h"


/****************************************************************************/

#define DS_KERNEL_MAJOR_VERSION     0
#define DS_KERNEL_MINOR_VERSION     4

#define DS_KERNEL_VERSION_STRING    version_str(DS_KERNEL_MAJOR_VERSION, \
                                                DS_KERNEL_MINOR_VERSION)


#endif // __PPC_VERSION_H__
