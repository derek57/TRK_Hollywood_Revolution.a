//##########################################################################
//
//    FILE
//
//    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/reset.h,v 1.3 2006/07/11 10:06:34 r04474 Exp $
//
//    DESCRIPTION
//
//    Header file containing declarations specific to the PPC reset
//    code section, ".init".
//
//
//    COPYRIGHT
//
//    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
//    All other product or service names are the property of their respective owners.
//    (C) Freescale Semiconductor, Inc. 2000-2006
//
//    HISTORY
//
//    $History: reset.h $
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 7/30/99    Time: 5:25p
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//    Header file containing declarations specific to the PPC reset
//    code section, ".init".
//
//
//    2002.04.17: changed by Jan Kobler, jkobler@Freescale.com
//                search for Kobler to find changes
//
//
//###########################################################################
//
//
//###########################################################################
#pragma once


#ifndef __RESET_H__
#define __RESET_H__


#include "trk_prefix.h"


__extern_c

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #ifdef __MWERKS__
        #pragma push
        #pragma section code_type ".init" data_mode=far_abs code_mode=far_abs
    #endif
#endif

    void __TRKreset(void);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    #ifdef __MWERKS__
        #pragma pop
    #endif
#endif

__end_extern_c

#ifdef __MWERKS__
    #pragma push
    #pragma section code_type ".init" data_mode=far_abs code_mode=far_abs
#endif

// Kobler: code_mode set to far_abs, was pc_rel

#if defined(__MWERKS__)
    extern const s8 gTRKInterruptVectorTable[];
    extern const s8 gTRKInterruptVectorTableEnd[];
    extern const s8 ROMStart[];
#elif defined(__GNUC__)
    __attribute__((section (".init"))) extern const s8 gTRKInterruptVectorTable[];
    __attribute__((section (".init"))) extern const s8 gTRKInterruptVectorTableEnd[];
    __attribute__((section (".init"))) extern const s8 ROMStart[];
#endif

#ifdef __MWERKS__
    #pragma pop
#endif


// Kobler: mem_TRK.h has its own #pragma section
#include "mem_TRK.h"


#ifdef __MWERKS__
    #pragma push
    #pragma section code_type ".init" data_mode=far_abs code_mode=far_abs
    #pragma pop
#endif


#endif // __RESET_H__
