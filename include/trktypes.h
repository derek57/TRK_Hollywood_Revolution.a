/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __TRKTYPES_H__
#define __TRKTYPES_H__


#ifdef __GNUC__
    #include <stdio.h>
    #include <string.h>
#endif


/*! \file trktypes.h
\brief Data type definitions

*/

#ifdef __GNUC__
    #define UNUSED(x)    ((x) = (x))
#endif

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/****************************************************************************/
typedef unsigned char      u8;            ///< 8bit unsigned integer
typedef unsigned short     u16;           ///< 16bit unsigned integer
typedef unsigned long      u32;           ///< 32bit unsigned integer
typedef unsigned long long u64;           ///< 64bit unsigned integer
/****************************************************************************/
typedef signed char        s8;            ///< 8bit signed integer
typedef signed short       s16;           ///< 16bit signed integer
typedef long               s32;           ///< 32bit signed integer
typedef long long          s64;           ///< 64bit signed integer
/****************************************************************************/
typedef volatile u8        vu8;           ///< 8bit unsigned volatile integer
typedef volatile u16       vu16;          ///< 16bit unsigned volatile integer
typedef volatile u32       vu32;          ///< 32bit unsigned volatile integer
typedef volatile u64       vu64;          ///< 64bit unsigned volatile integer
/****************************************************************************/
typedef u8                 u128[16];      ///< 128bit unsigned integer
/****************************************************************************/
typedef volatile s8        vs8;           ///< 8bit signed volatile integer
typedef volatile s16       vs16;          ///< 16bit signed volatile integer
typedef volatile s32       vs32;          ///< 32bit signed volatile integer
typedef volatile s64       vs64;          ///< 64bit signed volatile integer
/****************************************************************************/
// fixed point math typedefs
typedef s16                sfp16;         ///< signed 8:8 fixed point
typedef s32                sfp32;         ///< signed 20:8 fixed point
typedef u16                ufp16;         ///< unsigned 8:8 fixed point
typedef u32                ufp32;         ///< unsigned 24:8 fixed point
/****************************************************************************/
typedef float              f32;
typedef double             f64;
/****************************************************************************/
typedef volatile float     vf32;
typedef volatile double    vf64;
/****************************************************************************/

#if defined(__NEED_BOOL__)
    #ifdef bool
        #undef bool
    #endif

#ifndef __cplusplus

#ifndef BOOL_defined
    #if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        typedef s32 BOOL;
    #else
        typedef u32 BOOL;
    #endif
    #define BOOL_defined    1
#endif

#endif

    #ifdef false
        #undef false
    #endif

    #define false (0)

    #ifdef true
        #undef true
    #endif

    #define true (1)

#else

#ifdef __MWERKS__
    #if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
        typedef s32 BOOL;
    #else
        typedef u32 BOOL;
    #endif
#endif

#endif

/****************************************************************************/
// alias type typedefs
#define FIXED    s32                 ///< Alias type for sfp32
/****************************************************************************/
#ifndef TRUE
    #define TRUE     1                   ///< True
#endif
/****************************************************************************/
#ifndef FALSE
    #define FALSE    0                   ///< False
#endif
/****************************************************************************/
#ifndef NULL
    #ifdef  __cplusplus
        #define NULL                0
    #else   // __cplusplus
        #define NULL                ((void *)0)
    #endif  // __cplusplus
#endif  // NULL
/****************************************************************************/

#ifdef __MWERKS__
    #ifndef size_t_defined
        typedef u32 size_t;
        #define size_t_defined    1
    #endif
#endif

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif /* __TRKTYPES_H__ */

/* END OF FILE */
