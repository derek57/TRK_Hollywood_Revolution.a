/****************************************************************************/
/*

DESCRIPTION
    Message buffers for TRK.

    Message buffers are allocated out of a pool of buffers, preallocated
    at initialization.  MessageBuffers are structs that contain a
    synchronization primitive for concurrent access, position and size
    information, as well as the memory for the buffer itself.

    Usage

        Call TRK_InitializeMessageBuffers once to initialize the buffers. When a
        new buffer is desired, call "GetFreeBuffer" and a buffer ID plus a
        pointer to the MessageBuffer struct is returned.  Call "TRKResetBuffer"
        before using the buffer.  When using the MessageBuffer, acquire and
        release the mutex before and after using or modifying it.  Once the
        buffer is no longer being used, call ReleaseBuffer.
        To reset the buffer, clearing its contents, call TRKResetBuffer.  To
        add data to the buffer, advancing the position and size information,
        call TRK_AppendBuffer.

    Initialization

        DSError TRK_InitializeMessageBuffers(void);

            Call once at the beginning of the program.

    Getting and Releasing

        DSError GetFreeBuffer(MessageBufferId *returnedBufferID,
                                    MessageBuffer **buffer);
        MessageBuffer *TRKGetBuffer(MessageBufferId bufferID);


            GetFreeBuffer will acquire a buffer if it is available.
            Returned values:
            kNoError    the returnedBufferID and buffer pointers refer
                        to the allocated buffer

            kMessageBuffersFull
                        no more buffers are available.

            TRKGetBuffer gets a pre-allocated MessageBuffer pointer from the
            buffer's ID.  If the buffer was not allocated or the ID is out
            of range, TRKGetBuffer retuns NULL.


        void ReleaseBuffer(MessageBufferId id);

            When a buffer is no longer needed, it can be returned to
            the storage pool.


    Operations on MessageBuffers

        DSError TRK_SetBufferPosition(MessageBuffer *buffer, size_t position,
                                    BOOL truncate);

            Set the buffer to the given "position". Advances "length" if
            greater than position.  Forces "length" to equal position if
            truncate is true.  If the buffer length is
            exceeded, kMessageBufferOverflow is returned.
            Mutex must be acquired before entering.

        DSError TRK_AppendBuffer(MessageBuffer *buffer, const void *data,
                                size_t length);

            Append "length" bytes, starting at "data".  Buffer is filled until
            full. If the buffer length is exceeded, kMessageBufferOverflow is
            returned. Mutex must be acquired before entering.
        DSError TRK_ReadBuffer(MessageBuffer *buffer, void *data,
                                size_t length);

            Read "length" bytes, into "data".  Buffer is read until
            exhausted. If the request exceeds available data,
            kMessageBufferUnderflow is returned.
            Mutex must be acquired before entering.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MSGBUF_H__
#define __MSGBUF_H__


/****************************************************************************/
/*
 *    Includes
 */
/****************************************************************************/
#include "dserror.h"
#include "target.h"
#include "msgcmd.h"


/*
** Set this to 1 to get debug messages showing the get/release calls
** for the message buffers.  This helps to track down leaks.
*/
#ifndef DEBUG_MSGBUF_USAGE
    #define DEBUG_MSGBUF_USAGE                  (1)
#endif

#if DEBUG_MSGBUF_USAGE && (defined(DEBUGIO_SERIAL) || defined(DEBUGIO_RAM) || defined(DEBUGIO_CUSTOM))
    #define GetFreeBuffer(id, b)            (OSReport("Calling GetFreeBuffer from "), OSReport("%s", __FILE__), OSReport(":"), OSReport("0x%08x", __LINE__), OSReport("\n"), TRK_GetFreeBuffer(id, b))
    #define ReleaseBuffer(id)               (OSReport("Calling ReleaseBuffer from "), OSReport("%s", __FILE__), OSReport(":"), OSReport("0x%08x", __LINE__), OSReport("\n"), TRK_ReleaseBuffer(id))
#else /* #if DEBUG_MSGBUF_USAGE */
    #define GetFreeBuffer(id, b)            TRK_GetFreeBuffer(id, b)
    #define ReleaseBuffer(id)               TRK_ReleaseBuffer(id)
#endif /* #if DEBUG_MSGBUF_USAGE */


/****************************************************************************/
/*
 *    Errors
 */
/****************************************************************************/
enum
{                                      /* DSError; see dserror.h */
    kMessageBuffersFull = 0x0300,
    kMessageBufferOverflow,            /* TRK_AppendBuffer failed */
    kMessageBufferUnderflow            /* TRK_ReadBuffer failed */
};

/****************************************************************************/
/*
 *    Types
 */
/****************************************************************************/

/*
 *    MessageBufferId
 */
typedef s32 MessageBufferId;

enum
{
    kInvalidMessageBufferId = -1
};

/*
** Total number of buffers.  A basic, bare-board implementation
** needs three.
**
** If sequence IDs are used, an additional buffer is required
** to store the previous reply.
**
** Add one buffer when using the debug RAM console dump feature.
**
** If a particular target requires more, it should define
** TRK_EXTRA_BUFFER_COUNT in target.h.
*/
#ifndef TRK_EXTRA_BUFFER_COUNT
    #define TRK_EXTRA_BUFFER_COUNT          0
#endif

#if defined(DEBUGIO_RAM) && defined(DB_RAM_CONSOLE_DUMP)
    #define DB_RAM_BUFFERS                  1
#else
    #define DB_RAM_BUFFERS                  0
#endif

#define kMessageBufferCount                 (3 + DB_RAM_BUFFERS + TRK_EXTRA_BUFFER_COUNT)

/*
 *    struct MessageBuffer
 */
#define DS_MAXREADWRITELENGTH               0x0800                 /* 2K data portion */
#define DS_MAXMESSAGESIZE                   (DS_MAXREADWRITELENGTH + 0x80)
#define kMessageBufferSize                  (DS_MAXMESSAGESIZE)    // 2K + overhead, usually


typedef struct MessageBuffer
{
#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSMutex       fMutex;                   /* synchronization */
#endif

    BOOL          fInUse;                   // true if buffer is not free
    u32           fLength;                  // logical end of buffer
    u32           fPosition;                // current index in buffer
    u8            fData[kMessageBufferSize];
} MessageBuffer;

/*
 *    Options for the TRKResetBuffer function
 */
typedef enum ResetBufferOption
{
    kZero,        /* zero out the buffer */
    kDontZero
} ResetBufferOption;

typedef u8 ui128[16];


/****************************************************************************/
/*
 *    Public Function Prototypes
 */
/****************************************************************************/

__extern_c

    /*
     *    MessageBuffer management
     */

    DSError TRK_InitializeMessageBuffers(void);
    MessageBuffer *TRKGetBuffer(MessageBufferId bufferID);
    DSError TRK_GetFreeBuffer(MessageBufferId *returnedBufferID, MessageBuffer **buffer);
    void TRK_ReleaseBuffer(MessageBufferId id);

    /*
     * operations on MessageBuffers
     */

    void TRKResetBuffer(MessageBuffer *buffer, ResetBufferOption option);
    DSError TRK_SetBufferPosition(MessageBuffer *buffer, u32 bufferPosition);
    DSError TRK_AppendBuffer(MessageBuffer *buffer, const void *data, size_t length);
    DSError TRK_ReadBuffer(MessageBuffer *buffer, void *data, size_t length);

#ifdef VIRTUAL_DISPATCH // NOT IMPLEMENTED ON THE WII / NDEV
    DSError TRKAppendBuffer1_ui8(MessageBuffer *buffer, const u8 data);
#endif

    DSError TRKAppendBuffer1_ui16(MessageBuffer *buffer, const u16 data);
    DSError TRKAppendBuffer1_ui32(MessageBuffer *buffer, const u32 data);
    DSError TRKAppendBuffer1_ui64(MessageBuffer *buffer, const u64 data);
    DSError TRKAppendBuffer1_ui128(MessageBuffer *buffer, const ui128 data);
    DSError TRKAppendBuffer_ui8(MessageBuffer *buffer, const u8 *data, s32 count);
    DSError TRKAppendBuffer_ui16(MessageBuffer *buffer, const u16 *data, s32 count);
    DSError TRKAppendBuffer_ui32(MessageBuffer *buffer, const u32 *data, s32 count);
    DSError TRKAppendBuffer_ui64(MessageBuffer *buffer, const u64 *data, s32 count);
    DSError TRKAppendBuffer_ui128(MessageBuffer *buffer, const ui128 *data, s32 count);
    DSError TRKReadBuffer1_ui8(MessageBuffer *buffer, u8 *data);
    DSError TRKReadBuffer1_ui16(MessageBuffer *buffer, u16 *data);
    DSError TRKReadBuffer1_ui32(MessageBuffer *buffer, u32 *data);
    DSError TRKReadBuffer1_ui64(MessageBuffer *buffer, u64 *data);
    DSError TRKReadBuffer1_ui128(MessageBuffer *buffer, ui128 *data);
    DSError TRKReadBuffer_ui8(MessageBuffer *buffer, u8 *data, s32 count);
    DSError TRKReadBuffer_ui16(MessageBuffer *buffer, u16 *data, s32 count);
    DSError TRKReadBuffer_ui32(MessageBuffer *buffer, u32 *data, s32 count);
    DSError TRKReadBuffer_ui64(MessageBuffer *buffer, u64 *data, s32 count);
    DSError TRKReadBuffer_ui128(MessageBuffer *buffer, ui128 *data, s32 count);

__end_extern_c


#endif /* __MSGBUF_H__ */
