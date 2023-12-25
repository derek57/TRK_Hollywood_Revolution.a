/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __CIRCLEBUFFER_H__
#define __CIRCLEBUFFER_H__


#include "trktypes.h"
#include "dserror.h"
#include "trk_prefix.h"


/****************************************************************************/

typedef s32 CBError;

enum
{
    kCBDataError = -1,
    kCBNoError = 0
};

/****************************************************************************/

__extern_c

    u32 CBGetBytesAvailableForRead(u32 *buffer);
    u32 CBGetBytesAvailableForWrite(u32 *buffer);
    void CircleBufferTerminate(u8 *buffer);
    void CircleBufferInitialize(u32 *cbuf, u32 buffer, u32 buffersize);
    DSError CircleBufferReadBytes(u32 *buffer, u8 *data, size_t length);
    DSError CircleBufferWriteBytes(u32 *buffer, u8 *data, size_t length);

__end_extern_c


#endif // __CIRCLEBUFFER_H__
