/****************************************************************************/
/*

FILE

    mem_TRK.c

    $Source: /u/debugger/embedded/CodeWarriorTRK/Portable/mem_TRK.c,v $

DESCRIPTION

    Standalone Standard C Library Implementation for memory operations.
    This is a target-specific implementation for PowerPC.  It will
    probably work for other target processors as well, but this one
    has been improved specifically for PowerPC.

    Derived from MSL C (CodeWarrior 10).

COPYRIGHT

    (c) 2000-2006 Freescale Semiconductor Inc.
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "reset.h" /* forces funcs into reset section, if desired */


#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    #ifdef __POWERPC__
        #pragma push
        #pragma section code_type ".init" data_mode=far_abs code_mode=far_abs

        static void TRK_fill_mem(u8 *dst, s32 val, size_t length);

        #pragma pop
    #else
        static void TRK_fill_mem(u8 *dst, s32 val, size_t length);
    #endif
#endif


/****************************************************************************/


#if 1 // ONLY AVAILABLE IN V0.4 OF THE TRK TO THE WII / NDEV
    #include "ppc_mem.h"
#endif


/****************************************************************************/
/*
 *    TRK_memcpy
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
void *TRK_memcpy(void *dst, const void *src, size_t length)
{
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    const s8 *p;
    s8 *q;

    for (p = ((const s8 *)src - 1), q = ((s8 *)dst - 1), length++; --length;)
        *++q = *++p;
#else
    s32 i;
    u8 *srcTmp = (u8 *)src;
    u8 *dstTmp = (u8 *)dst;

    for (i = 0; i != (s32)length; i++)
    {
        ppc_writebyte1(dstTmp, (u8)ppc_readbyte1(srcTmp));
        srcTmp++;
        dstTmp++;
    }
#endif

    return dst;
}

/****************************************************************************/
/*
 *    TRK_memmove
 *
 *    100 % DONE (USED FOR V0.1 OF THE TRK TO THE WII / NDEV)
 */
/****************************************************************************/

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

void *TRK_memmove(void *dst, const void *src, size_t length)
{
    const s8 *p;
    s8 *q;
    s32 rev = ((u32)src < (u32)dst);

    if (!rev)
    {
        for (p = (const s8 *)(src - 1), q = (s8 *)(dst - 1), length++; --length;)
            *++q = *++p;
    }
    else
    {
        for (p = (const s8 *)(src + length), q = (s8 *)(dst + length), length++; --length;)
            *--q = *--p;
    }

    return dst;
}

#endif

/****************************************************************************/
/*
 *    TRK_fill_mem
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV


#define deref_auto_inc(p)    *++(p)


#endif

static void TRK_fill_mem(u8 *dst, s32 val, u32 length)
{
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    u32    v = (u8)val;
    u32    i;

    union
    {
        u8 *cpd;
        u32 *lpd;
    } dstu;

    dstu.cpd = (((u8 *)dst) - 1);

    if (length >= 32)
    {
        i = ((~ (u32)dstu.cpd) & 3);

        if (i)
        {
            length -= i;

            do
                deref_auto_inc(dstu.cpd) = (u8)v;
            while (--i);
        }

        if (v)
            v |= ((v << 24) | (v << 16) | (v <<  8));

        dstu.lpd = (((u32 *)(dstu.cpd + 1)) - 1);

        i = (length >> 5);

        if (i)
        {
            do
            {
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
                deref_auto_inc(dstu.lpd) = v;
            } while (--i);
        }

        i = ((length & 31) >> 2);

        if (i)
        {
            do
                deref_auto_inc(dstu.lpd) = v;
            while (--i);
        }

        dstu.cpd = (((u8 *)(dstu.lpd + 1)) - 1);

        length &= 3;
    }

    if (length)
    {
        do
            deref_auto_inc(dstu.cpd) = (u8)v;
        while (--length);
    }
#else
    s32 i;
    u8 byte = (u8)val;

    for (i = 0; i != (s32)length; i++)
    {
        ppc_writebyte1(dst, byte);
        dst++;
    }
#endif

    return;
}

/****************************************************************************/
/*
 *    TRK_memset
 *
 *    100 % DONE
 */
/****************************************************************************/
void *TRK_memset(void *dst, s32 val, size_t length)
{
    TRK_fill_mem(dst, val, length);

    return dst;
}
