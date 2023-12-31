/****************************************************************************/
/*
FILE

    serframe.h

DESCRIPTION

    Frame Check Sequence checksums (FCS) for 16 and 32 bits.

    The 16-bit version uses a 512-byte table (2 bytes * 256 elements)
    and the 32-bit version uses a 1024-byte table (4 bytes * 256 elements)

    Serial-based HDLC-like framing (RFC 1662) for use with the Metrowerks
    CWDS serial protocol.

    Serial data is transmitted using an HDLC-like framing convention.
    Byte-oriented streams are transmitted with a starting frame byte
    and also followed by an ending frame byte and a checksum (FCS).

    Errors can be detected with high probability using the FCS.
    While this is not intended to be used in an environment that
    generates a large number of errors, nevertheless it can be
    useful to help ensure that packets arrive with a high degree of
    certainty and to allow data to be dropped and still allow
    continuous data.

    Sender and receiver can be resynchronized with the receipt of a new
    framing byte.


    Frame Format

    The general frame format specified in RFC 1662 is described as:

        +----------+----------+----------+
        |   Flag   | Address  | Control  |
        | 01111110 | 11111111 | 00000011 |
        +----------+----------+----------+
        +----------+-------------+---------+
        | Protocol | Information | Padding |
        | 8/16 bits|      *      |    *    |
        +----------+-------------+---------+
        +----------+----------+-----------------
        |   FCS    |   Flag   | Inter-frame Fill
        |16/32 bits| 01111110 | or next Address
        +----------+----------+-----------------

    The routines in this file use a subset and are not negotiated;
    they are simply assumed. These are:

    Address and Control Field Compression is always ON.
    Protocol Field Compression is always ON.
    Protocol value is implied, but undefined.
    Padding is OFF.
    MRU is 1500 bytes.
    FCS is 32 bits.
    Trailing Flag is always transmitted.


    The resulting frame format is:

        +----------+
        |   Flag   |
        | 01111110 |
        +----------+
        +-------------+
        | Information |
        |      *      |
        +-------------+
        +----------+----------+-----------------
        |   FCS    |   Flag   | Inter-frame Fill
        |16/32 bits| 01111110 | or next Address
        +----------+----------+-----------------

    Macros & Typedefs

        FCSBITSIZE

            This header file must first define FCSBITSIZE to be either one of:
            FCS16 or FCS32.  These control the definitions for the rest of the header
            file and the underlying tables.  Selecting FCS16 will cause the header
            file to define 16-bit types, table, and functions for calculating the
            16-bit FCS.  Selecting FCS32 is identical, except it uses 32-bit FCS values.

            FCS16 will use 512 bytes (2 bytes * 256 entries) of constant data space.
            FCS32 will use 1024 bytes.

        FCSType

            The 16- or 32-bit type used for the FCS register and datum.

        PPPINITFCS

            Initialize the FCS register with this value.

        PPPGOODFCS

            On the receiver side, after the FCS compute function is applied
            to all data bytes plus the two or four FCS bytes, the correct FCS
            value will be PPPGOODFCS, a constant.

        PPPCOMPFCS

REFERENCES

    [RFC1662.94]    Simpson, W., Editor, "PPP in HDLC-like Framing,"
                    STD 51, RFC 1662, Daydreamer, July 1994.

COPYRIGHT

    (c) 1997 Metrowerks Corporation
    All rights reserved.

*/
/****************************************************************************/

#ifndef __SERFRAME_H__
#define __SERFRAME_H__


#include "trktypes.h"
#include "trk_prefix.h"


#define FCS8        0            /* SCM: Added for simple 8-bit checksums */
#define FCS16       1
#define FCS32       2

#define FCSBITSIZE  FCS8         /* define as either FCS8, FCS16 or FCS32 */


#if FCSBITSIZE == FCS8

    /*
     *    Definitions for 8-bit simple Frame Check Sequences
     */

    typedef u8 FCSType;    /* 8-bit type */

    #define PPPINITFCS8       0x00        /* Initial FCS value */
    #define PPPGOODFCS8       0xFF        /* Good final FCS value */

    #define PPPINITFCS        PPPINITFCS8
    #define PPPGOODFCS        PPPGOODFCS8
    #define PPPCOMPFCS        0xFF        /* complement FCS value */

    #ifdef __cplusplus
        inline FCSType PPPFCS(FCSType fcs, u8 c)
        {
            return (fcs + c);
        }
    #else
        #define PPPFCS(fcs, c)  (fcs + c)
    #endif


#elif FCSBITSIZE == FCS16

    /*
     *    Definitions for 16-bit Frame Check Sequences
     */

    typedef u16 FCSType;    /* 16-bit type */

    /* extern FCSType fcstab[]; */

    extern const FCSType fcstab[256];

    #define PPPINITFCS16    0xffff        /* Initial FCS value */
    #define PPPGOODFCS16    0xf0b8        /* Good final FCS value */

    #define PPPINITFCS      PPPINITFCS16
    #define PPPGOODFCS      PPPGOODFCS16
    #define PPPCOMPFCS      0xffff        /* complement FCS value */

    #ifdef __cplusplus
        inline FCSType PPPFCS(FCSType fcs, u8 c)
        {
            return (((fcs) >> 8) ^ fcstab[((fcs) ^ (c)) & 0xff]);
        }
    #else
        #define PPPFCS(fcs, c)  (((fcs) >> 8) ^ fcstab[((fcs) ^ (c)) & 0xff])
    #endif

#elif FCSBITSIZE == FCS32

    /*
     *    Definitions for 32-bit Frame Check Sequences
     */

    typedef u32 FCSType;    /* 32-bit FCS */

    /* extern FCSType fcstab_32[]; */

    extern const FCSType fcstab_32[256];

    #define PPPINITFCS32    0xffffffff    /* Initial FCS value */
    #define PPPGOODFCS32    0xdebb20e3    /* Good final FCS value */

    #define PPPINITFCS      PPPINITFCS32
    #define PPPGOODFCS      PPPGOODFCS32
    #define PPPCOMPFCS      0xffffffff    /* complement FCS value */

    #ifdef __cplusplus
        inline FCSType PPPFCS(FCSType fcs, u8 c)
        {
            return (((fcs) >> 8) ^ fcstab_32[((fcs) ^ (c)) & 0xff]);
        }
    #else
        #define PPPFCS(fcs, c)  (((fcs) >> 8) ^ fcstab_32[((fcs) ^ (c)) & 0xff])
    #endif

#endif


/****************************************************************************/
/*
 *    Constants used for framing serial data
 */
/****************************************************************************/

#define PPP_UI          0x03    /* Unnumbered Information */
#define PPP_FLAG        0x7e    /* Flag Sequence */
#define PPP_ESCAPE      0x7d    /* Asynchronous Control Escape */
#define PPP_TRANS       0x20    /* Asynchronous transparency modifier */

#define MAXFRAMESIZE    0x1000  /* maximum unframed/unescaped data size */

#endif /* ndef __SERFRAME_H__ */
