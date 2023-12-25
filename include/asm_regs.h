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


#ifndef __ASM_REGS_H__
#define __ASM_REGS_H__


#ifndef TRK_TRANSPORT_INT_DRIVEN
    #define TRK_TRANSPORT_INT_DRIVEN     (1)
#endif

#define TRK_TRANSPORT_INT_KEY        0x500

#define SPR_XER                      1

#define COND_EQ                      2

#define SPR_LR                       8
#define SPR_CTR                      9

#define SPR_DSISR                    18
#define SPR_DAR                      19

#define SPR_DEC                      22

#define SPR_SDR1                     25
#define SPR_SRR0                     26
#define SPR_SRR1                     27

#define SPR_CSRR0                    58
#define SPR_CSRR1                    59

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_VRSAVE                   256

#endif

#define TBR_TBL_READ                 268
#define TBR_TBU_READ                 269

#define SPR_SPRG0                    272
#define SPR_SPRG1                    273
#define SPR_SPRG2                    274
#define SPR_SPRG3                    275

#define SPR_EAR                      282

#define TBR_TBL_WRITE                284
#define TBR_TBU_WRITE                285

#define SPR_PVR                      287

#define SPR_IBAT0U                   528
#define SPR_IBAT0L                   529
#define SPR_IBAT1U                   530
#define SPR_IBAT1L                   531
#define SPR_IBAT2U                   532
#define SPR_IBAT2L                   533
#define SPR_IBAT3U                   534
#define SPR_IBAT3L                   535

#define SPR_DBAT0U                   536
#define SPR_DBAT0L                   537
#define SPR_DBAT1U                   538
#define SPR_DBAT1L                   539
#define SPR_DBAT2U                   540
#define SPR_DBAT2L                   541
#define SPR_DBAT3U                   542
#define SPR_DBAT3L                   543

#define SPR_IBAT4U                   560
#define SPR_IBAT4L                   561
#define SPR_IBAT5U                   562
#define SPR_IBAT5L                   563
#define SPR_IBAT6U                   564
#define SPR_IBAT6L                   565
#define SPR_IBAT7U                   566
#define SPR_IBAT7L                   567
#define SPR_DBAT4U                   568
#define SPR_DBAT4L                   569
#define SPR_DBAT5U                   570
#define SPR_DBAT5L                   571
#define SPR_DBAT6U                   572
#define SPR_DBAT6L                   573
#define SPR_DBAT7U                   574
#define SPR_DBAT7L                   575

#define SPR_DCNV0                    912
#define SPR_DCNV1                    913
#define SPR_DCNV2                    914
#define SPR_DCNV3                    915
#define SPR_DCTV0                    916
#define SPR_DCTV1                    917
#define SPR_DCTV2                    918
#define SPR_DCTV3                    919
#define SPR_DCVL                     920
#define SPR_ICVL                     921
#define SPR_DMAU                     922
#define SPR_DMAL                     923

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_UMMCR2                   928 /* read only */
    #define SPR_UBAMR                    935 /* read only */

#endif

#define SPR_UMMCR0                   936
#define SPR_UPMC1                    937
#define SPR_UPMC2                    938
#define SPR_USIA                     939
#define SPR_UMMCR1                   940
#define SPR_UPMC3                    941
#define SPR_UPMC4                    942

    #if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_MMCR2                    944
    #define SPR_BAMR                     951

#endif

#define SPR_MMCR0                    952
#define SPR_PMC1                     953
#define SPR_PMC2                     954
#define SPR_SIA                      955
#define SPR_MMCR1                    956
#define SPR_PMC3                     957
#define SPR_PMC4                     958

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_SDA                      959 /* Decoded, but not implemented */
    #define SPR_DMISS                    976
    #define SPR_DCMP                     977
    #define SPR_HASH1                    978
    #define SPR_HASH2                    979
    #define SPR_IMISS                    980
    #define SPR_ICMP                     981
    #define SPR_RPA                      982

#endif

#define SPR_HID0                     1008
#define SPR_HID1                     1009
#define SPR_IABR                     1010
#define SPR_HID2                     1011

#define SPR_DABR                     1013

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_MSSCR0                   1014
    #define SPR_MSSCR1                   1015

#endif

#define SPR_L2CR                     1017

#define SPR_ICTC                     1019

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_THRM1                    1020
    #define SPR_THRM2                    1021
    #define SPR_THRM3                    1022

#else

    #define SPR_FPECR                    1022

#endif


#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

    /*
    ** MPC7400 additional SPRs
    */
    #define SPR_PIR                      1023

#endif

#define SPR_GQR0                     SPR_DCNV0

#define PPC_CACHE_ALIGNMENT          32


#endif // __ASM_REGS_H__
