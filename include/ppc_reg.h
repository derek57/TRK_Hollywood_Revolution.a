/****************************************************************************/
/*

DESCRIPTION

    PPC instruction constants & macros.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009

*/
/****************************************************************************/


#ifndef __PPC_REG_H__
#define __PPC_REG_H__


#include "trktypes.h"


/*
 *    Default register block (all registers typically requested by debugger)
 */

typedef u32       DefaultType;
typedef u32       Extended1Type;
typedef u64       FloatType;
typedef FloatType FPType;    /* TRK core calls it FPType */


typedef struct Default_PPC
{
                    // Table size: 128 (Default: 4 bytes (u32))
    DefaultType   GPR[32];
    DefaultType   PC;            // starts at 128
    DefaultType   LR;            // 132
    DefaultType   CR;            // 136
    DefaultType   CTR;           // 140 = "'0' of Default.XER"
    DefaultType   XER;           // 144 = Next Table (Float)

} Default_PPC;

/*
** Register indices
*/
#define TRK_DEFAULT_GPR(x)            (x)
#define TRK_DEFAULT_PC                32
#define TRK_DEFAULT_LR                33
#define TRK_DEFAULT_CR                34
#define TRK_DEFAULT_CTR               35
#define TRK_DEFAULT_XER               36

#define TRK_DEFAULT_MIN_REGISTER      0
#define TRK_DEFAULT_MAX_REGISTER      (sizeof(Default_PPC) / sizeof(DefaultType) - 1)

#define TRK_DEFAULT_SIZE              (sizeof(DefaultType))


typedef struct Float_PPC
{
                        // Table size: 256 (Float: 8 bytes (u64))
    FloatType FPR[32];  // "FPR[0]" = Pos. 148 (byte '0')
                        // "FPR[1]" = Pos. 156 (byte '0')
                        // so... "FPSCR" = 256 + 156 = 412
    FloatType FPSCR;    // 412 = first byte of "Float.FPSCR"
    FloatType FPECR;    /* only supported on some processors */

} Float_PPC;            // 427 = last byte

/*
** Register indices
*/
#define TRK_FLOAT_FPR(x)               (x)
#define TRK_FLOAT_FPSCR                32
#define TRK_FLOAT_FPECR                33

#define TRK_FLOAT_MIN_REGISTER         0
#define TRK_FLOAT_MAX_REGISTER         (sizeof(Float_PPC) / sizeof(FloatType) - 1)

#define TRK_FLOAT_SIZE                 (sizeof(FloatType))


/*
 * Info stored in NotifyStopped message.
 */

typedef DefaultType    PCType;
typedef u32            InstructionType;
typedef u16            ExceptionCauseType;

typedef struct StopInfo_PPC
{
    PCType             PC;
    InstructionType    PCInstruction;  // instruction at PC
    ExceptionCauseType exceptionID;    // same as vector #, e.g. 0x0200

    // CORRECTED ALIGNMENT
    u8                 pad[2];
} StopInfo_PPC;


//
// Info stored in NotifyException message.
//

typedef StopInfo_PPC ExceptionInfo_PPC;

/*
** Info used by breakpoint code to restore (unset)
** breakpoints.  For software breakpoints it is just
** the saved instruction.
*/

typedef InstructionType BreakpointRestore;


#define TRK_EXTENDED2_SIZE            (sizeof(Extended2Type))

//    Special purpose register numbers

// MOVED TO ASM_REGS.H FOR COMPATIBILITY __MWERKS__ <-> __GNUC__
/*
#define SPR_XER           1
#define SPR_LR            8
#define SPR_CTR           9
#define SPR_DSISR         18
#define SPR_DAR           19
#define SPR_DEC           22
#define SPR_SRR0          26
#define SPR_SRR1          27
#define SPR_CSRR0         58
#define SPR_CSRR1         59

#define SPR_PVR           287
#define SPR_SPRG0         272
#define SPR_SPRG1         273
#define SPR_SPRG2         274
#define SPR_SPRG3         275
#define TBR_TBL_READ      268
#define TBR_TBU_READ      269
#define TBR_TBL_WRITE     284
#define TBR_TBU_WRITE     285
#define SPR_FPECR         1022
*/


#if 0 // NOT IMPLEMENTED ON THE WII / NDEV
    #define _555_VERSION      0x00020020
    #define _7XX_VERSION      0x0008
    #define _7400_VERSION     0x000c
    #define _7410_VERSION     0x800c
    #define _7455_VERSION     0x8001          // bits 0..15 of PVR on 7455
    #define _8260_VERSION     0x0081
    #define _PQ38_VERSION     0x8021


    /* e500 definitions */
    #define SPR_DBCR0         308
    #define SPR_DBSR          304
    #define SPR_DEAR          61
    #define SPR_ESR           62
    #define SPR_L1CSR0        0x3f2           /* L1 Data Cache Control and Status Register 0 */
    #define L1CSR0_CPE        0x00010000      /* Data Cache Parity Enable */
    #define L1CSR0_DCFI       0x00000002      /* Data Cache Flash Invalidate */
    #define L1CSR0_DCE        0x00000001      /* Data Cache Enable */
    #define SPR_L1CSR1        0x3f3           /* L1 Instruction Cache Control and Status Register 1 */
    #define L1CSR1_CPE        0x00010000      /* Instruction Cache Parity Enable */
    #define L1CSR1_ICFI       0x00000002      /* Instruction Cache Flash Invalidate */
    #define L1CSR1_ICE        0x00000001      /* Instruction Cache Enable */

    #define SPR_MMUCSR0       0x3f4   /* MMU control and status register 0 */
    #define SPR_MAS0          0x270   /* MMU Assist Register 0 */
    #define SPR_MAS1          0x271   /* MMU Assist Register 1 */
    #define SPR_MAS2          0x272   /* MMU Assist Register 2 */
    #define SPR_MAS3          0x273   /* MMU Assist Register 3 */
    #define SPR_MAS4          0x274   /* MMU Assist Register 4 */
    #define SPR_MAS5          0x275   /* MMU Assist Register 5 */
    #define SPR_MAS6          0x276   /* MMU Assist Register 6 */
    #define SPR_MAS7          0x3B0   /* MMU Assist Register 7 */

    #define SPR_IVOR32        0x210   /* Interrupt Vector Offset Register 32 */
    #define SPR_IVOR33        0x211   /* Interrupt Vector Offset Register 33 */
    #define SPR_IVOR34        0x212   /* Interrupt Vector Offset Register 34 */
    #define SPR_IVOR35        0x213   /* Interrupt Vector Offset Register 35 */
    #define SPR_SPEFSCR       0x200   /* SPE & Embedded FP Status & Control */

    #define SPR_MCSRR0        0x23a   /* Machine Check Save and Restore Register 0 */
    #define SPR_MCSRR1        0x23b   /* Machine Check Save and Restore Register 1 */
    #define SPR_BUCSR         0x3f5   /* Branch Control and Status Register */
    #define SPR_BBEAR         0x201   /* Branch Buffer Entry Address Register */
    #define SPR_BBTAR         0x202   /* Branch Buffer Target Address Register */
    #define SPR_PID1          0x279   /* Process ID Register 1 */
    #define SPR_PID2          0x27a   /* Process ID Register 2 */
    #define SPR_MCSR          0x23c   /* Machine Check Syndrome register */
    #define SPR_MCAR          0x23d   /* Machine Check Address register */
#endif


/*
 * Convenient assembly values.
 */

// MOVED TO ASM_REGS.H FOR COMPATIBILITY __MWERKS__ <-> __GNUC__
//#define COND_EQ           2

#ifndef MSR_DE
    #define MSR_DE            0x0200      /* DE bit of MSR (bit 22) - only for e500 */
#endif

#ifndef MSR_SE
    #define MSR_SE            0x0400      /* SE bit of MSR (bit 21) */
#endif

#ifndef MSR_EE
    #define MSR_EE            0x8000      /* EE bit of MSR (bit 16) */
#endif

#ifndef MSR_RI
    #define MSR_RI            0x0002      /* RI bit of MSR (bit 30) */
#endif

#ifndef MSR_DR
    #define MSR_DR            0x0010      /* DR bit of MSR (bit 27) */
#endif

#ifndef MSR_IR
    #define MSR_IR            0x0020      /* IR bit of MSR (bit 26) */
#endif

#ifndef MSR_FP
    #define MSR_FP            0x2000      /* FP bit of MSR (bit 18) */
#endif

#ifndef MSR_VEC
    #define MSR_VEC           0x02000000  /* VEC bit of MSR (bit 6) */ /* AltiVec support */
#endif

#define DBCR0_IDM         0x40000000
#define DBCR0_ICMP        0x08000000
#define DBCR0_TRAP        0x01000000


#endif    // #ifndef __PPC_REG_H__
