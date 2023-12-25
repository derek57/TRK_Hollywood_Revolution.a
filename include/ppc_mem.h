/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/
#pragma once

#ifndef __PPC_MEM_H__
#define __PPC_MEM_H__


#include "trktypes.h"


/****************************************************************************/
/*
 *    ppc_readbyte1
 */
/****************************************************************************/

// 100 % DONE
static u8 ppc_readbyte1(u8 *addr)
{
    u32 *byte = (u32 *)((u32)addr & ~3);

    return (u8)(*byte >> ((3 - ((u32)addr - (u32)byte)) << 3));
}

/****************************************************************************/
/*
 *    ppc_writebyte1
 */
/****************************************************************************/

// 100 % DONE
static void ppc_writebyte1(u8 *addr, u8 byte)
{
    u32 *dest = (u32 *)((u32)addr & ~3);
    u32 val = *dest;
    u32 uVar3 = (u32)0xff << ((3 - ((u32)addr - (u32)dest)) << 3);
    u32 iVar1 = (3 - ((u32)addr - (u32)dest)) << 3;

    *dest = (val & ~uVar3) | (uVar3 & (byte << iVar1));
}


#endif // __PPC_MEM_H__
