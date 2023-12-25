/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __STRING_H__
#define __STRING_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

    void *memcpy(void *dst, const void *src, size_t n);
    void *memmove(void *dst, const void *src, size_t n);

#ifndef __GNUC__
    void *memset(void *dst, s32 val, size_t n);
#endif

__end_extern_c


#endif
