/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED
    NO HEX-BINARY MATCH (due to compiler version used)

*/
/****************************************************************************/


#include "trk_prefix.h"

/*
 *    System includes
 */
#include <stdlib.h>

/*
 *    Debugger Nub includes
 */
#include "trktypes.h"
#include "nubassrt.h"
#include "usr_put.h"


#if 0 // THE WHOLE FILE IS ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV

/****************************************************************************/

/*
** If you wish to allow TRK to die (call exit()), define this
** to 1 in target-specific headers.
*/
#ifndef ALLOW_DIE
    #define ALLOW_DIE        1
#endif


/****************************************************************************/
/*
 *    TRKNubAssertionFailed
 */
/****************************************************************************/
void TRKNubAssertionFailed(s8 *condition, s8 *filename, s32 lineno, BOOL die)
{
#if defined(__MWERKS__)

    #pragma unused(condition, filename)

    #ifndef DEBUG_ASSERT
        #pragma unused(lineno)
    #endif

#elif defined(__GNUC__)

    UNUSED(condition);
    UNUSED(filename);

    #ifndef DEBUG_ASSERT
        UNUSED(lineno);
    #endif

#endif

#ifdef DEBUG_ASSERT

    s8 linestr[6];
    s32 order;

    for (order = 4; order >= 0; order--)
    {
        linestr[order] = (s8)('0' + (lineno % 10));
        lineno /= 10;
    }

    linestr[5] = 0;

    __puts("Assert failed: file = ");
    __puts(filename);
    __puts("; line = ");
    __puts(linestr);
    __puts("; ");
    __puts(condition);
    __puts("\r\n");

#endif

#if ALLOW_DIE

    if (die)
    {
        exit(-1);
    }

#endif /* #if ALLOW_DIE */

    return;
}

#endif
