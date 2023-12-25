/****************************************************************************/
/*

DESCRIPTION

    TRK_Hollywood_Revolution.a                  Reverse-Engineering project

    This file is NOT part of any official CodeWarriorTRK / MetroTRK / AppTRK
    source code releases.



COPYRIGHT

    2019 - 2023,        nitr8



STATUS

    100 % DECOMPILED
    100 % HEX-BINARY MATCH

*/
/****************************************************************************/


#include "target_options.h"


static u8 bUseSerialIO;


/****************************************************************************/
/*
 *    SetUseSerialIO
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
void SetUseSerialIO(u8 result)
{
    bUseSerialIO = result;

    return;
}

/****************************************************************************/
/*
 *    GetUseSerialIO
 *
 *    100 % DONE (This function was added for the Wii / NDEV)
 */
/****************************************************************************/
u8 GetUseSerialIO(void)
{
    return bUseSerialIO;
}
