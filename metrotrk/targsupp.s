/*****************************************************************************
#
# FILE
#
#    targsupp.s
#
# DESCRIPTION
#
#    Low-level, target-specific interface to the support functions
#    provided by CodeWarriorTRK.  This interface is designed to be linked
#    with mslsupp.c from the Portable directory to create a separate
#    library with a high-level MSL interface.  It is not recommended
#    that target programs call the functions in this module directly.
#
#    NOTE: CodeWarriorTRK must be running on the target system in order for
#    these routines to function.
#
#****************************************************************************#
#
#    Low-level interface:  The CodeWarriorTRK file access support can be
#        invoked directly with the 'twi    31,r0,0x0' instruction.  The
#        following describes the input parameters which must be set up
#        in registers before execution of the break instruction and
#        the output parameters which will be present in registers
#        after execution of the break instruction.
#
#        Note: No registers will be modified by CodeWarriorTRK except
#        the registers used for output parameters.
#
#    Input parameters:
#        r3: Command          (kDSReadFile or kDSWriteFile)
#        r4: File Handle      (one of DSFileHandle values)
#        r5: Count            (ptr to desired number of characters to read/write)
#        r6: Buffer ptr       (ptr to storage buffer containing data
#                             to be written or into which read data should
#                             be stored)
#
#    Output:
#        r3: IO Result        (one of DSIOResult values)
#
#****************************************************************************#
#
# HISTORY
#
#    98 Jan 28 SCM    Created.
#    98 Mar 18 SCM    Ported from the NEC V8xx to MIPS.
#    98 Mar           Ported from MIPS to PPC.
#
#
# COPYRIGHT
#
#    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
#    All other product or service names are the property of their respective owners.
#    © Freescale Semiconductor, Inc. 2000-2006
#
#    (c) 2019 - 2023,        nitr8
#
# STATUS
#
#    100 % DECOMPILED
#    100 % HEX-BINARY MATCH
#
*****************************************************************************/


.text


    .globl TRKAccessFile
TRKAccessFile:
    twi       31, r0, 0x0        # Signal CodeWarriorTRK
    blr



    .globl TRKOpenFile
TRKOpenFile:
    twi       31, r0, 0x0        # Signal CodeWarriorTRK
    blr



    .globl TRKCloseFile
TRKCloseFile:
    twi       31, r0, 0x0        # Signal CodeWarriorTRK
    blr



    .globl TRKPositionFile
TRKPositionFile:
    twi       31, r0, 0x0        # Signal CodeWarriorTRK
    blr
