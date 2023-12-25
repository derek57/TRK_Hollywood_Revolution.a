/****************************************************************************/
/*

FILE

    mslsupp.c

    $Header: /u/debugger/embedded/MetroTRK/Export/mslsupp.c,v 1.2 1999/08/11 21:53:26 harper Exp $

DESCRIPTION

    C MSL interface to the support functions provided by MetroTRK.
    This interface is designed to be built as a separate library
    that can be linked with a target program.  The target program
    can make calls into this interface to obtain support services
    from MetroTRK.  The interface is designed to be used directly
    by MSL, without requiring any #includes from the MetroTRK
    sources.

    NOTE: MetroTRK must be running on the target system in order for
    these routines to function.

    Public functions:

        __read_console() - Read a specified number of characters
            from the console into a buffer.

        __write_console() - Read a specified number of characters
            from a buffer to the console.

        __close_console() - Close the console.

        __read_file() - Read a specified number of characters
            from a file into a buffer.

        __write_file() - Read a specified number of characters
            from a buffer to a file.

        __close_file() - Close the specified file.

        Function parameters:

            handle (__file_handle): Specifies the console or
                disk file on which to operate.  Disk file
                handles are assigned dynamically.  Console
                file handles are:

                    stdin:    0
                    stdout:   1
                    stderr:   2

            buffer (unsigned char *): For writes, points to
                the data which is to be written.  For reads,
                points to storage for the data which will
                be read.

            count (size_t *): The desired number of characters
                to read/write.  The value after calling
                a particular function indicates the actual
                number of characters read/written.

            idle_proc (__idle_proc): A function to be called
                while the current operation is idle.  Not used
                in the current implementation.

        Return value:  All of the public functions here return
            a value of type 'enum __io_results' as defined
            in stdio.h in the Metrowerks MSL.

HISTORY

    $History: mslsupp.c $
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 7/30/99    Time: 3:38p
//    Updated in $/Embedded/MetroTRK/Export
//       - Hard-coded the console file handles to work around
//         an embedded MSL problem: all console handles are 0
//         for current embedded MSL.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 5/18/98    Time: 6:12p
//    Updated in $/Embedded/MetroTRK/Export
//    Merging in changes from latest MIPS TRK: MIPS release 2.
//
//    *****************  Version 3  *****************
//    User: Smoore       Date: 3/31/98    Time: 5:42p
//    Updated in $/Embedded/TRK_In_Progress/NEC_VR_TRK/MetroTRK/Export
//    Modified to use fixed-length data types that don't require any of the
//    non-exported TRK headers.  This allows the code to be compiled
//    as an external library.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 2/02/98    Time: 5:58p
//    Created in $/Embedded/MetroTRK/Portable
//    Merging changes from NEC V8xx TRK.
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 2/02/98    Time: 3:18p
//    Updated in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Portable
//    Added more detailed documentation of the functions in this file.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 2/02/98    Time: 10:48a
//    Created in $/Embedded/TRK_In_Progress/NEC_V8xx_TRK/MetroTRK/Portable
//    Interface between MSL and the TRK support routines.

AUTHORS

    (c) 1998 Metrowerks Corporation
    All rights reserved.

COPYRIGHT

    (c) 2019 - 2023,        nitr8

STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "usr_put.h"        // must be included before msgcmd.h
#include "msg.h"
#include "msgxtrct.h"
#include "mslsupp.h"
#include "msghndlr.h"
#include "target_options.h"
#include "targsupp.h"


static IOError __access_file(u32 handle, u8 *buffer,
                          size_t *count, __ref_con ref_con, u8 read);


static u8 convertFileMode(u8 *a1);

IOError __read_file(u32 handle, u8 *buffer,
                 size_t *count, __ref_con ref_con);


IOError __write_file(u32 handle, u8 *buffer,
                  size_t *count, __ref_con ref_con);

u8 convertFileMode(u8 *a1);

IOError __read_console(u32 handle, u8 *buffer,
                    size_t *count, __ref_con ref_con);

IOError __TRK_write_console(u32 handle, u8 *buffer,
                     size_t *count, __ref_con ref_con);

IOError __close_Console(u32 handle);

IOError __open_temp_file(void);


/****************************************************************************/
/*
 *    __read_console
 *
 *    Read the specified amount of data from the console file specified by
 *    handle into the given buffer.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
IOError __read_console(u32 handle, u8 *buffer,
                    size_t *count, __ref_con ref_con)
{
#if defined(__MWERKS__)
    #pragma unused(handle)
#elif defined(__GNUC__)
    UNUSED(handle);
#endif

    if ((GetUseSerialIO() & 0xFF) == 0)
        return kIOError;

    return __read_file(0, buffer, count, ref_con);
}

/****************************************************************************/
/*
 *    __TRK_write_console
 *
 *    Write the specified amount of data to the console file specified by
 *    handle from the given buffer.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
IOError __TRK_write_console(u32 handle, u8 *buffer,
                     size_t *count, __ref_con ref_con)
{
#if defined(__MWERKS__)
    #pragma unused(handle)
#elif defined(__GNUC__)
    UNUSED(handle);
#endif

    if ((GetUseSerialIO() & 0xFF) == 0)
        return kIOError;

    return __write_file(1, buffer, count, ref_con);
}

/****************************************************************************/
/*
 *    __close_Console
 *
 *    Close the console file specified by handle from the given buffer.
 *
 *    100 % DONE (The function name has changed - the second "C" is uppercase)
 */
/****************************************************************************/
IOError __close_Console(u32 handle)
{
    return __close_file(handle);
}

/****************************************************************************/
/*
 *    __read_file
 *
 *    Read the specified amount of data from the file specified by
 *    handle into the given buffer.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
IOError __read_file(u32 handle, u8 *buffer,
                 size_t *count, __ref_con ref_con)
{
    return __access_file(handle, buffer, count, ref_con, kDSReadFile);
}

/****************************************************************************/
/*
 *    __write_file
 *
 *    Write the specified amount of data to the file specified by
 *    handle from the given buffer.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
IOError __write_file(u32 handle, u8 *buffer,
                  size_t *count, __ref_con ref_con)
{
    return __access_file(handle, buffer, count, ref_con, kDSWriteFile);
}

/****************************************************************************/
/*
 *    __open_file
 *
 *    This function was NEVER found within any of the official
 *    MetroTRK source code releases by NXP.
 *
 *    100 % DONE (NEVER USED BUT ADDED FOR THE WII / NDEV)
 */
/****************************************************************************/
IOError __open_file(u32 handle, u8 *buffer, size_t *length)
{
    IOError err;
    u8 lenptr;

    if (GetTRKConnected() == FALSE)
        return kIOError;

    lenptr = convertFileMode(buffer);

    err = (IOError)TRKOpenFile((u8)kDSOpenFile,
                                handle,
                                                    // stop GCC complaining
                                (u32 *)(u32)lenptr, // about int-to-ptr cast
                                                    // of different size
                                (u8 *)length);

    /*
    ** Convert the result into the MSL equivalent.
    */

    switch (err)
    {
        case kDSIONoError:
            err = kIONoError;
            break;

        case kDSIOEOF:
            err = kIOEOF;
            break;

        default:
            err = kIOError;
            break;
    }

    return err;
}

/****************************************************************************/
/*
 *    __close_file
 *
 *    Close the file specified by handle from the given buffer.
 *
 *    100 % DONE (Contents were completely rewritten for the Wii / NDEV)
 */
/****************************************************************************/
IOError __close_file(u32 handle)
{
    IOError err;

    if (GetTRKConnected() == FALSE)
        return kIOError;

    err = (IOError)TRKCloseFile((u8)kDSCloseFile, (u32)handle);

    /*
    ** Convert the result into the MSL equivalent.
    */

    switch (err)
    {
        case kDSIONoError:
            err = kIONoError;
            break;

        case kDSIOEOF:
            err = kIOEOF;
            break;

        default:
            err = kIOError;
            break;
    }

    return err;
}

/****************************************************************************/
/*
 *    __position_file
 *
 *    This function was NEVER found within any of the official
 *    MetroTRK source code releases by NXP.
 *
 *    100 % DONE (NEVER USED BUT ADDED FOR THE WII / NDEV)
 */
/****************************************************************************/
IOError __position_file(u32 handle, u32 *position, s32 mode)
{
    IOError err;
    s32 bufptr = 0;

    if (GetTRKConnected() == FALSE)
        return kIOError;

    if (!mode)
        bufptr = 0;
    else
    {
        if (mode == 1)
            bufptr = 1;
        else if (mode == 2)
            bufptr = 2;
    }

    err = (IOError)TRKPositionFile((u8)kDSPositionFile, handle, position, (u8 *)bufptr);

    /*
    ** Convert the result into the MSL equivalent.
    */

    switch (err)
    {
        case kDSIONoError:
            err = kIONoError;
            break;

        case kDSIOEOF:
            err = kIOEOF;
            break;

        default:
            err = kIOError;
            break;
    }

    return err;
}

/****************************************************************************/
/*
 *    convertFileMode
 *
 *    This function was NEVER found within any of the official
 *    MetroTRK source code releases by NXP.
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
static u8 convertFileMode(u8 *buffer)
{
    u8 result = 0;
    u8 group = (u8)(DSFetch_u32(buffer) >> 30);
    u8 other = (u8)((DSFetch_u32(buffer) >> 27) & 7);
    u8 owner = (u8)((DSFetch_u32(buffer) >> 19) & 1);

    switch (group)
    {
        case 0: // 0x00000000 (no idea)
            result |= 1;
            break;

        case 2: // 0x80000000 (write permission "group" class ???)
            result |= 2;
            break;

        case 1: // 0x40000000 (exec permission "group" class ???)
            result |= 4;

        default:
            break;
    }

    switch (other)
    {
        case 1: // 0x08000000 (exec permission "other" class ???)
            result |= 1;
            break;

        case 2: // 0x10000000 (write permission "other" class ???)
            result |= 2;
            break;

        case 6: // 0x30000000 (read and write permission "other" class ???)
            result |= 4;
            break;

        case 3: // 0x18000000 (write and exec permission "other" class ???)
            result |= 0x12;
            break;

        case 7: // 0x38000000 (generic permission for "other" class ???)
            result |= 7;
            break;
    }

    if (owner == 1) // 0x00080000 (read permission for "owner" class ???)
        result |= 8;

    return result;
}

/****************************************************************************/
/*
 *    __access_file
 *
 *    Read/Write the specified amount of data from/to the file specified by
 *    handle to/from the given buffer.
 *
 *    100 % DONE (Contents were partially rewritten for the Wii / NDEV)
 */
/****************************************************************************/
static IOError __access_file(u32 handle, u8 *buffer,
                   size_t *count, __ref_con ref_con, u8 read)
{
#if defined(__MWERKS__)
    #pragma unused(ref_con)
#elif defined(__GNUC__)
    UNUSED(ref_con);
#endif

    u8 err;
    u32 local_count;

    if (GetTRKConnected() == FALSE)
        return kIOError;

    /*
    ** Convert MSL types (loosely specified sizes and forms) into
    ** types with well-defined sizes and forms.
    */

    local_count = (u32)*count;

    err = TRKAccessFile((u8)(read),
                            (u32)handle, &local_count,
                            (u8 *)buffer);

    *count = (size_t)local_count;

    /*
    ** Convert the result into the MSL equivalent.
    */

    switch (err)
    {
        case kDSIONoError:
            err = kIONoError;
            break;

        case kDSIOEOF:
            err = kIOEOF;
            break;

        default:
            err = kIOError;
            break;
    }

    return err;
}

/****************************************************************************/
/*
 *    __open_temp_file
 *
 *    This function was NEVER found within any of the official
 *    MetroTRK source code releases by NXP.
 *
 *    Still unsolved for real (native) shifting C-code but the current
 *    solution should help when it comes to cross-compiling things...
 *
 *    In the end it was NEVER USED but added for the Wii / NDEV
 *
 *    100 % DONE with help of mono21400 @ decomp.me (Discord)
 */
/****************************************************************************/

typedef struct bf
{
    s32 bits_0: 2;
    s32 bits_2: 3;
    s32 bits_5: 2;
    s32 bits_7: 3;
    s32 bits_10: 2;
    s32 bits_12: 1;
} bf;

#ifndef __GNUC__
    extern s8 *tmpnam(s8 *name);
#endif

IOError __open_temp_file(void)
{
    IOError err;
    u32 handle;
    bf bitfields;
    u32 bitfield_data;

    handle = (u32)tmpnam("tmp");

    if (handle == 0)
        return kIOError;

    bitfields.bits_2 = 3;               //   28 (0x001C) ???
    bitfields.bits_0 = 2;               //    3 (0x0003) ???
    bitfields.bits_5 = 0;               //   32 (0x0020) ???
    bitfields.bits_7 = 1;               //  128 (0x0080) ???
    bitfields.bits_12 = 1;              // 4096 (0x1000) ???

    bitfield_data = DSFetch_u32(&bitfields); // contents of the bitfield struct

    // spC = 3;                         // spC = r0

                                        // there's assembler instruction "lwz     r4,0xc(r1)"     missing here...

    // spC |= (handle & 7) << 27;       // creates assembler instruction "rlwimi  r0,r3,0x1b,2,4" (but it should be "rlwimi  r4,r0,0x1b,2,4")

                                        // there's assembler instruction "li      r3,1"           missing here...
                                        // there's assembler instruction "li      r0,2"           missing here...

    // spC |= (handle) << 30;           // creates assembler instruction "rlwimi  r0,r3,0x1e,0,1" (but it should be "rlwimi  r4,r0,0x1e,0,1")
    // sp8 = DSFetch_u32(&sp) & ~0x60000000; // creates assembler instruction "rlwinm  r0,r0,0,3,0"    (but it should be "rlwinm  r0,r4,0,7,4")
    // sp8 |= (handle & 7) << 22;       // creates assembler instruction "rlwimi  r0,r3,0x16,7,9" (MATCH)
    // sp8 |= 0x80000;                  // creates assembler instruction "oris    r0, r0, 8"      (MATCH)

    /*

    REGARDING THE ORING / SHIFTING / ANDING CODE ABOVE:

    ??? - There should be all the byte shifting and ORing stuff right here, similar to this...:

    val[0] = ((val[1] & 0xC7FFFFFF | 0x18000000) & 0x3FFFFFFF | 0x80000000) & 0xF83FFFFF | 0x480000;

    ...but the disassembly of the original object binary doesn't match the assembler code once
    the code provided above is compiled.

    */

    if (GetTRKConnected() == FALSE)
        return kIOError;

    err = (IOError)TRKOpenFile((u8)kDSOpenFile,
                                handle,

                                // stop GCC complaining about int-to-ptr cast of different size
                                (u32 *)(u32)convertFileMode((u8 *)&bitfield_data),

                                NULL);

    /*
    ** Convert the result into the MSL equivalent.
    */

    switch (err)
    {
        case kDSIONoError:
            err = kIONoError;
            break;

        case kDSIOEOF:
            err = kIOEOF;
            break;

        case kDSIOError:
        default:
            err = kIOError;
            break;
    }

    return err;
}
