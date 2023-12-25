/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Board/Freescale/mpc_7410/MEMMAP.H,v 1.2 2006/07/11 11:10:07 r04474 Exp $

DESCRIPTION

    Memory map of the Freescale MPC7400 Yellowknife X4 board.


HISTORY


    $History: memmap.h $

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2006

*/
/****************************************************************************/
#pragma once


#ifndef __MEMMAP_H__
#define __MEMMAP_H__


#include "trktypes.h"


/****************************************************************************/


typedef struct memRange
{
    u8      *start;
    u8      *end;
    BOOL    readable;
    BOOL    writeable;
} memRange;


/*
** Specify valid memory ranges in this array.  When memory
** ranges overlap, they should be specified in order of
** checking priority.  A particular memory access will be
** accepted or rejected according the first entry in the
** table matching its address range.
**
** Examples:
**    1.    To disallow access everywhere except a range of
**        valid memory between addresses x and y, the table
**        would be:
**
**        {
**            {x, y, TRUE, TRUE}
**        }
**
**         Any memory range that falls within x..y would be
**        accepted.  Everything else would be rejected by
**        default.
**
**    2.    To allow access everywhere except a range of invalid
**        memory between addresses x and y, the table would
**        be:
**
**        {
**            {x, y, FALSE, FALSE},
**            {0x00000000, 0xffffffff, TRUE, TRUE}
**        }
**
**        Any memory range that falls within or overlaps x..y
**        would be rejected.  Everything else would fall within
**        0x00000000..0xffffffff and be accepted.
**
**
** On this board, allow all memory accesses except writes
** to TRK memory.
**
** The symbols used here are specific to the CodeWarrior
** linker.  They also assume a certain ordering of
** the sections.  Check the link-map file to verify
** that they are correct.
**
** The assumption is that all read-only sections will
** be grouped between _f_init and _eextabindex, and
** all writeable sections will be grouped between
** _f_data and _e_bss, and that those plus the stack
** are the only three blocks of memory used by the TRK.
**
** A final entry covers the entire memory space so that
** any memory accesses outside the restriced areas will
** be allowed.
**
*/


const memRange gTRKMemMap[] =
{
#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    {
        (u8 *)_f_init, (u8 *)_eextabindex, TRUE, FALSE
    },

    {
        (u8 *)_f_data, (u8 *)_e_bss, TRUE, FALSE
    },

    {
        (u8 *)_stack_end, (u8 *)_stack_addr, TRUE, FALSE
    },
#endif
    {
        (u8 *)0x00000000, (u8 *)0xffffffff, TRUE, TRUE
    }
};


#endif // __MEMMAP_H__
