/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Generic/copy_vectors.h,v 1.2 2006/07/11 10:01:28 r04474 Exp $

DESCRIPTION

    Routines to copy the exception handler table to its
    RAM location.

HISTORY


    $History: copy_vectors.h $
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 4/20/99    Time: 2:01p
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Generic
//    Added support for the 8260 processor and the MPC8260 VADS evaluation
//    board.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2006

*/
/****************************************************************************/
#pragma once


#ifndef __COPY_VECTORS_H__
#define __COPY_VECTORS_H__


#include "trk_prefix.h"


#if defined(__MWERKS__)
    extern const s8 gTRKInterruptVectorTable[];
    extern const s8 gTRKInterruptVectorTableEnd[];
#elif defined(__GNUC__)
    __attribute__((section(".init"))) extern const s8 gTRKInterruptVectorTable[];
    __attribute__((section(".init"))) extern const s8 gTRKInterruptVectorTableEnd[];
#endif

__extern_c

    ASM_PREFIX void InitMetroTRK(void);
    ASM_PREFIX void InitMetroTRK_BBA(void);

    void TRK_copy_vector(u32 *src_addr);
    void __TRK_copy_vectors(void);

__end_extern_c


#endif // __COPY_VECTORS_H__
