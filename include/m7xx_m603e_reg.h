/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Export/m7xx_m603e_reg.h,v 1.4 2006/08/25 13:59:30 r04474 Exp $

DESCRIPTION

    API for target-specific implementation.

    Register numbers for PPC740, PPC750, and PPC603e.

    These register blocks are accessed via the kDSReadRegisters and
    kDSWriteRegisters messages.  The register number for a specific
    register is determined by its corresponding element number in these
    structures.  The register data in the messages must conform to
    the structures defined here in big-endian format.

HISTORY


    $History: m7xx_m603e_reg.h $
//
//    *****************  Version 4  *****************
//    User: Smoore       Date: 9/22/98    Time: 11:03a
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Moved 5xx FP registers into the FP block.  Fixed FP block typedef.
//
//    *****************  Version 3  *****************
//    User: Clayton      Date: 9/09/98    Time: 2:12p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Fixed a the DS_EXTENDED1_MAX_REGISTER_6xx_7xx define
//
//    *****************  Version 2  *****************
//    User: Clayton      Date: 9/09/98    Time: 12:49p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Extracted all common powerPC stuff into ppc_reg.h and cleaned up so
//    multiple reg.h files for different processors can be included in a
//    debugger plugin.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 8/14/98    Time: 10:24a
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Register description file for 7xx and 603e processors.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2006

*/
/****************************************************************************/
#pragma once


#ifndef __M7XX_M603E_REG_H__
#define __M7XX_M603E_REG_H__


#include "trktypes.h"
#include "ppc_reg.h"    // Generic PowerPC Register Definitions
#include "trk_prefix.h"


/****************************************************************************/
/****************************************************************************/



// PPC740/PP750/MPC603e/MPC7400-specific registers here


/****************************************************************************/

/*
 * Extended registers block 1
 */
#define MWDEBUGGER_PLUGIN (1)


#ifdef SPRG0
    #undef SPRG0
#endif

#ifdef SPRG1
    #undef SPRG1
#endif

#ifdef SPRG2
    #undef SPRG2
#endif

#ifdef SPRG3
    #undef SPRG3
#endif

// Stuff in this struct right here needs to be fixed up...
//
// The offsets of registers being used for the Wii / NDEV
// are basically correct but do not match with what was
// seen in the official source code of protocol version
// v1.10 (kernel v3.37 of the TRK...
typedef struct Extended1_PPC_6xx_7xx
{
    Extended1Type SR[16]; // 0x1E4 (USED)
    Extended1Type TBL;    // 0x1E8 (USED)
    Extended1Type TBU;    // 0x1EC (USED)
    Extended1Type HID0;
    Extended1Type HID1;
    Extended1Type MSR;    // 0x1F8 (USED)
    Extended1Type PVR;    // 0x1FC (USED)
    Extended1Type HID2;   // 0x200 (USED)
    Extended1Type IBAT0U;
    Extended1Type IBAT0L;
    Extended1Type IBAT1U;
    Extended1Type IBAT1L;
    Extended1Type IBAT2U;
    Extended1Type IBAT2L;
    Extended1Type IBAT3U;
    Extended1Type IBAT3L;
    Extended1Type DBAT0U;
    Extended1Type DBAT0L;
    Extended1Type DBAT1U;
    Extended1Type DBAT1L;
    Extended1Type DBAT2U;
    Extended1Type DBAT2L; // 0x238 (USED)
    Extended1Type DBAT3U;
    Extended1Type IBAT4U; // 0x240 (USED)

    Extended1Type DCMP;                     /* PPC603e only */
    Extended1Type HASH1;                    /* PPC603e only */
    Extended1Type HASH2;                    /* PPC603e only */
    Extended1Type IMISS;                    /* PPC603e only */
    Extended1Type ICMP;                     /* PPC603e only */
    Extended1Type RPA;                      /* PPC603e only */
    Extended1Type SDR1;   // 0x25C (USED)
    Extended1Type DAR;    // 0x260 (USED)
    Extended1Type DSISR;  // 0x264 (USED)
    Extended1Type SPRG0;
    Extended1Type SPRG1;
    Extended1Type SPRG2;
    Extended1Type SPRG3;
    Extended1Type DEC;    // 0x278 (USED)   /* PPC603e only */
    Extended1Type IABR;
    Extended1Type EAR;

#if 0 // NOT IMPLEMENTED ON THE WII / NDEV

    Extended1Type PMC1;                     /* PPC740/PPC750 only */
    Extended1Type PMC2;                     /* PPC740/PPC750 only */
    Extended1Type PMC3;                     /* PPC740/PPC750 only */
    Extended1Type PMC4;                     /* PPC740/PPC750 only */
    Extended1Type SIA;                      /* PPC740/PPC750 only */

#endif

#ifdef PROCESSOR_7400_EXTENSION // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    Extended1Type UMMCR2;                   /* PPC7400 (Max) read only */
    Extended1Type UBAMR;                    /* PPC7400 (Max) read only */
    Extended1Type IBAT5U;                   /* PPC7400 (Max) read only */
    Extended1Type UPMC1;                    /* PPC7400 (Max) read only */
    Extended1Type UPMC2;                    /* PPC7400 (Max) read only */
    Extended1Type USIA;                     /* PPC7400 (Max) read only */
    Extended1Type UMMCR1;                   /* PPC7400 (Max) read only */
    Extended1Type UPMC3;                    /* PPC7400 (Max) read only */
    Extended1Type UPMC4;                    /* PPC7400 (Max) read only */
    Extended1Type USDA;                     /* PPC7400 (Max) Decoded, but not implemented */
    Extended1Type MMCR2;                    /* PPC7400 (Max) only */
    Extended1Type BAMR;                     /* PPC7400 (Max) only */
    Extended1Type SDA;                      /* PPC7400 (Max) Decoded, but not implemented */
    Extended1Type MSSCR0;                   /* PPC7400 (Max) only */
    Extended1Type MSSCR1;                   /* PPC7400 (Max) only */
    Extended1Type PIR;                      /* PPC7400 (Max) only */

#endif

#ifdef PROCESSOR_7455_EXTENSION

    Extended1Type DABR;   // 0x284 (USED)
    Extended1Type LDSTCR;
    Extended1Type MSSSR0;
    Extended1Type UPMC5;
    Extended1Type UPMC6;
    Extended1Type PMC5;
    Extended1Type PMC6;
    Extended1Type TLBMISS;
    Extended1Type IBAT7L; // 0x2A4 (USED)
    Extended1Type PTELO;
    Extended1Type L3PM;
    Extended1Type ICTC;   // 0x2B0 (USED)
    Extended1Type L3CR;
    Extended1Type SRR0;   // 0x2B8 (USED)
    Extended1Type DBAT5U; // 0x2BC (USED)
    Extended1Type UMMCR0; // 0x2C0 (USED)
    Extended1Type IBAT5L;
    Extended1Type IBAT6U;
    Extended1Type IBAT6L;
    Extended1Type IBAT7U;
    Extended1Type PTEHI;
    Extended1Type DBAT4U;
    Extended1Type DBAT5L; // 0x2DC (USED)
    Extended1Type IBAT4L;
    Extended1Type DBAT4L;
    Extended1Type DBAT6U;
    Extended1Type DBAT6L;
    Extended1Type DBAT7U;
    Extended1Type DBAT7L;

    Extended1Type exceptionID;
    Extended1Type DCNV0;  // 0x2FC (USED)

#endif // PROCESSOR_7455_EXTENSION

    Extended1Type DBAT3L;

    Extended1Type DMISS;                    /* PPC603e only */

#ifdef PROCESSOR_7455_EXTENSION

    Extended1Type L2CR2;                    /* PPC740/PPC750 only */
    Extended1Type MMCR0;                    /* PPC740/PPC750 only */
    Extended1Type MMCR1;                    /* PPC740/PPC750 only */
    Extended1Type THRM1;                    /* PPC740/PPC750 only */
    Extended1Type THRM2;                    /* PPC740/PPC750 only */
    Extended1Type THRM3;                    /* PPC740/PPC750 only */
    Extended1Type ICCR;                     /* PPC740/PPC750 only */
    Extended1Type L2CR;                     /* PPC750 only */

#endif

    u8            pad[4];
} Extended1_PPC_6xx_7xx;



#define DS_EXTENDED1_MIN_REGISTER_6xx_7xx    0
#define DS_EXTENDED1_MAX_REGISTER_6xx_7xx    (sizeof(Extended1_PPC_6xx_7xx) / sizeof(Extended1Type) - 1)

#define __PPC_ALTIVEC__ 0

/*
 * Extended registers block 2.
 *
 * Extended registers block 2 will contain registers specific
 * to AltiVec technology.
 */

#ifndef MWDEBUGGER_PLUGIN

    #if __PPC_ALTIVEC__
        #ifndef vector
            #define vector
        #endif

        #pragma altivec_model on

        typedef struct Extended2_PPC_6xx_7xx
        {
            vector u32 VR[32];
            vector u32 VSCR;        /* PPC7400 only (Vector Status and Control Register, NOT an spr) */
            DefaultType VRSAVE;     /* PPC7400 only (PPC750 with AltiVec technology) */
        } Extended2_PPC_6xx_7xx;
    #endif /* #if __PPC_ALTIVEC__ */

#else /* #ifdef MWDEBUGGER_PLUGIN */

    typedef struct Extended2_PPC_6xx_7xx
    {
        #if 0 // NOT IMPLEMENTED ON THE WII / NDEV
            u128 VR[32];
            DefaultType VSCR;           // PPC7400 only (Vector Status and Control Register, NOT an spr)
            DefaultType VRSAVE;         // PPC7400 only (PPC750 with AltiVec technology)
        #else
            u128 pad[16];
        #endif
    } Extended2_PPC_6xx_7xx;

#endif

#define DS_EXTENDED2_MIN_REGISTER_6xx_7xx   0
#define DS_EXTENDED2_MAX_REGISTER_6xx_7xx   (33)

//    full processor state
typedef struct ProcessorState_PPC_6xx_7xx
{
    Default_PPC                        Default;   // Default register block

    // ADDED for the Wii / NDEV
    u8                                 pad[4];

    Float_PPC                          Float;
    Extended1_PPC_6xx_7xx              Extended1; // Extended 1 register block

#ifndef MWDEBUGGER_PLUGIN

    #if __PPC_ALTIVEC__
        Extended2_PPC_6xx_7xx          Extended2; // Extended 2 register block
    #endif /* #if __PPC_ALTIVEC__ */

#else

    Extended2_PPC_6xx_7xx              Extended2; // Extended 2 register block

#endif

#if TRK_TRANSPORT_INT_DRIVEN
    DefaultType                        transport_handler_saved_ra;
#endif

} ProcessorState_PPC_6xx_7xx;

#ifndef MWDEBUGGER_PLUGIN
    #define DS_EXTENDED1_MIN_REGISTER  DS_EXTENDED1_MIN_REGISTER_6xx_7xx
    #define DS_EXTENDED1_MAX_REGISTER  DS_EXTENDED1_MAX_REGISTER_6xx_7xx
    #define DS_EXTENDED2_MIN_REGISTER  DS_EXTENDED2_MIN_REGISTER_6xx_7xx
    #define DS_EXTENDED2_MAX_REGISTER  DS_EXTENDED2_MAX_REGISTER_6xx_7xx
    #define FPR_START                  FPR_START_6xx_7xx

    typedef Extended1_PPC_6xx_7xx      Extended1_PPC;

    #if __PPC_ALTIVEC__
        typedef Extended2_PPC_6xx_7xx  Extended2_PPC;
    #endif /* #if __PPC_ALTIVEC__ */

#endif

typedef ProcessorState_PPC_6xx_7xx     ProcessorState_PPC;

__extern_c

    ASM_PREFIX void TRKSaveExtended1Block(void);
    ASM_PREFIX void TRKRestoreExtended1Block(void);

__end_extern_c


#endif // __M7XX_M603E_REG_H__
