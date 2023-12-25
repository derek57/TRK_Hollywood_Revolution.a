/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project



COPYRIGHT

    2019 - 2023,        nitr8

*/
/****************************************************************************/


#ifndef __OSREPORT_H__
#define __OSREPORT_H__


#include "trktypes.h"
#include "trk_prefix.h"


__extern_c

#if defined(__MWERKS__)
    extern void OSReport(const s8 *msg, ...);
#elif defined(__GNUC__) // Disable ridiculous warning about signedness...
    extern void OSReport(const char *msg, ...);
#endif

__end_extern_c


#endif /* __OSREPORT_H__ */
