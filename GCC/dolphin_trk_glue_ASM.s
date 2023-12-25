/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED

*/
/****************************************************************************/


#include "asm.h"


.text


#ifdef __GNUC__

.extern TRK_InterruptHandler
.extern gDBCommTable
.extern gWriteBuf


########################################################################
# RLWINM (r)otate (l)eft (w)ord (i)mmediate "then" a(n)d "with" (m)ask #
########################################################################

#
# 1.) second argument to rlwinm is the source register
# 2.) it get's rotated left by argument 3
#
# 3.) before saving into target argument 1, it's "AND"ed (&)
#     with the interval between argument 4 and 5 whereby an
#     interval between 0 and 31 would be a whole 32-bit value.
#
#     Interval starts backwards from argument 5 to argument 4.
#

#
# "TRKLoadContext" is a stripped-down version of "OSLoadContext"
# without interrupt stuff
#

.globl TRKLoadContext
TRKLoadContext:
    lwz       r0, 0(r3)
    lwz       r1, 4(r3)
    lwz       r2, 8(r3)
    lhz       r5, 0x1A2(r3)
    rlwinm.   r6, r5, 0,30,30
    beq       notexc
    rlwinm    r5, r5, 0,31,29
    sth       r5, 0x1A2(r3)
    lmw       r5, 0x14(r3)
    b         misc

notexc:

    lmw       r13, 0x34(r3)

misc:

    mr        r31, r3
    mr        r3, r4
    lwz       r4, 0x80(r31)
    mtcr      r4
    lwz       r4, 0x84(r31)
    mtlr      r4
    lwz       r4, 0x88(r31)
    mtctr     r4
    lwz       r4, 0x8C(r31)
    mtxer     r4
    mfmsr     r4
    rlwinm    r4, r4, 0,17,15
    rlwinm    r4, r4, 0,31,29
    mtmsr     r4
    mtsprg1   r2
    lwz       r4, 0xC(r31)
    mtsprg2   r4
    lwz       r4, 0x10(r31)
    mtsprg3   r4
    lwz       r2, 0x198(r31)
    lwz       r4, 0x19C(r31)
    lwz       r31, 0x7C(r31)
    b         TRK_InterruptHandler

#endif