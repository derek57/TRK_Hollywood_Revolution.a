/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __MSLSUPP_H__
#define __MSLSUPP_H__


#include "trktypes.h"
#include "trk_prefix.h"


/****************************************************************************/

typedef s32 IOError;
typedef void * __ref_con;

enum
{
    kIONoError,
    kIOError,
    kIOEOF
};

/****************************************************************************/

__extern_c

    s32 __open_file(u32 handle, u8 *buffer, size_t *count);
    s32 __position_file(u32 handle, u32 *position, s32 mode);
    s32 __close_file(u32 handle);

__end_extern_c


#endif /* __MSLSUPP_H__ */
