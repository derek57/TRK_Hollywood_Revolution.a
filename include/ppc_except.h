/****************************************************************************/
/*

FILE

    $Header: /u/debugger/embedded/CodeWarriorTRK/Processor/ppc/Export/ppc_except.h,v 1.2 2006/07/11 09:51:08 r04474 Exp $

DESCRIPTION

    Exception numbers for PPC 5xx, 603e, 7xx, and 8xx.

HISTORY

    $History: ppc_except.h $
//
//    *****************  Version 2  *****************
//    User: Smoore       Date: 6/08/99    Time: 6:41p
//    Updated in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Added support for the MPC555.
//
//    *****************  Version 1  *****************
//    User: Smoore       Date: 8/14/98    Time: 10:25a
//    Created in $/Embedded/CodeWarriorTRK/Processor/ppc/Export
//    Common exception description file for PPC processors.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2006

*/
/****************************************************************************/


#ifndef __PPC_EXCEPT_H__
#define __PPC_EXCEPT_H__


#define PPC_SYSTEMRESET              0x0100
#define PPC_MACHINECHECK             0x0200
#define PPC_DATAACCESSERROR          0x0300
#define PPC_INSTACCESSERROR          0x0400
#define PPC_EXTERNALINTERRUPT        0x0500
#define PPC_ALIGNMENTERROR           0x0600
#define PPC_PROGRAMERROR             0x0700
#define PPC_FPUNAVAILABLE            0x0800
#define PPC_DECREMENTERINTERRUPT     0x0900
#define PPC_RESERVED_0A00            0x0A00
#define PPC_RESERVED_0B00            0x0B00
#define PPC_SYSTEMCALL               0x0C00
#define PPC_TRACE                    0x0D00
#define PPC_FPASSIST                 0x0E00

#define PPC_PERFORMANCE_MONITOR      0x0F00    /* PPC 740 and 750 */
#define PPC_ALTIVEC_UNAVAILABLE      0x0F20    /* PPC 7400 (Max processor) */

#define PPC5xx_8xx_SOFTWAREEMULATION 0x1000    /* PPC 505, 821, and 860 */

#define PPC8xx_INSTTLBMISS           0x1100    /* PPC 821 and 860 */
#define PPC8xx_DATATLBMISS           0x1200    /* PPC 821 and 860 */
#define PPC8xx_INSTTLBERROR          0x1300    /* PPC 821, 860, and 555 */
#define PPC8xx_DATATLBERROR          0x1400    /* PPC 821, 860, and 555 */
#define PPC_DENORM_DETECT_JAVA_MODE  0x1600    /* PPC 7400 (Max processor) */

#define PPC603E_INSTR_TLB_MISS       0x1000    /* PPC 603e */
#define PPC603E_DATA_LOAD_TLB_MISS   0x1100    /* PPC 603e */
#define PPC603E_DATA_STORE_TLB_MISS  0x1200    /* PPC 603e */

#define PPC7xx_603E_INSTR_ADDR_BREAK 0x1300    /* PPC 603e, 740, 750 */

#define PPC7xx_603E_SYS_MANAGE       0x1400    /* PPC 603e, 740, 750 */

#define PPC_RESERVED_1500            0x1500
#define PPC_RESERVED_1600            0x1600
#define PPC_THERMAL_MANAGE           0x1700    /* PPC 740 and 750 */
#define PPC_RESERVED_1800            0x1800
#define PPC_RESERVED_1900            0x1900
#define PPC_RESERVED_1A00            0x1A00
#define PPC_RESERVED_1B00            0x1B00
#define PPC_DATABREAKPOINT           0x1C00    /* PPC 821, 860, and 505 */
#define PPC_INSTBREAKPOINT           0x1D00    /* PPC 821, 860, and 505 */
#define PPC_PERIPHERALBREAKPOINT     0x1E00    /* PPC 821, 860, and 505 */
#define PPC_NMDEVELOPMENTPORT        0x1F00    /* PPC 821, 860, and 505 */


#endif /* #ifndef __PPC_EXCEPT_H__ */
