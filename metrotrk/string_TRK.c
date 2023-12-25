/****************************************************************************/
/*

FILE

    string_TRK.c

    $Header: /u/debugger/embedded/MetroTRK/Portable/string_TRK.c,v 1.1 2000/04/27 02:28:38 smoore Exp $

DESCRIPTION

    Standalone Standard C Library Implementation for string operations.

    Derived from MSL C (CodeWarrior 11).

COPYRIGHT

    (c) 1996-2000 Metrowerks Corporation
    All rights reserved.

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "string_TRK.h"


#if __MWERKS__
    #pragma warn_possunwant off
#endif /* __MWERKS__ */


/****************************************************************************/
/*
 *    TRK_strlen
 *
 *    100 % DONE
 */
/****************************************************************************/
size_t TRK_strlen(const s8 *str)
{
    size_t len = 0xFFFFFFFF;

#if !__POWERPC__

    do
        len++;
    while (*str++);

#else

    u8 *p = (u8 *)(str - 1);

    do
        len++;
    while (*++p);

#endif

    return len;
}

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
/****************************************************************************/
/*
 *    TRK_strcat
 */
/****************************************************************************/
s8 *TRK_strcat(s8 *dst, const s8 *src)
{
#if !__POWERPC__

    const s8 *p = src;
    s8 *q = dst;

    while (*q++);

    q--;

    while (*q++ = *p++);

#else

    const u8 *p = (u8 *)(src - 1);
    u8 *q = (u8 *)(dst - 1);

    while (*++q);

    q--;

    while (*++q = *++p);

#endif

    return dst;
}
#endif
