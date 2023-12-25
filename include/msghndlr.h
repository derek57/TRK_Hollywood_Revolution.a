/****************************************************************************/
/*

DESCRIPTION

    Command handler prototypes.

    Handle messages sent from the host to TRK.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    © Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MSGHNDLR_H__
#define __MSGHNDLR_H__


#include "msgbuf.h"


#define NOTIMPLEMENTED  0


/*
** Connected status.
*/

__extern_c

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    void TRKMessageIntoReply(MessageBuffer *b,
                            u8 ackCmd,
                            DSReplyError errSentInAck);
#endif

    /*
     *    Standard ACK/NAK (noncommands)
     */
    DSError TRKSendACK(MessageBuffer *);

    DSError TRKStandardACK(MessageBuffer *b,
                        u8 ackCmd,
                        DSReplyError errSentInAck);

    DSError TRKDoError(MessageBuffer *);
    DSError TRKDoUnsupported(MessageBuffer *b);

    /*
     *    meta command set
     */
    DSError TRK_DoConnect(MessageBuffer *b);
    DSError TRKDoDisconnect(MessageBuffer *b);
    DSError TRKDoReset(MessageBuffer *b);

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKDoVersions(MessageBuffer *);
    DSError TRKDoSupportMask(MessageBuffer *);
    DSError TRKDoCPUType(MessageBuffer *b);
#endif

    /*
     *    target "state" command set
     */

    DSError TRKDoOverride(MessageBuffer *b);
    DSError TRKDoReadMemory(MessageBuffer *);
    DSError TRKDoWriteMemory(MessageBuffer *);
    DSError TRKDoReadRegisters(MessageBuffer *b);
    DSError TRKDoWriteRegisters(MessageBuffer *b);
    DSError TRKDoSetOption(MessageBuffer *b);

#ifdef VIRTUAL_DISPATCH // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    DSError TRKDoFlushCache(MessageBuffer *);
#endif

    /*
     *    target "execution" command set
     */

    DSError TRKDoContinue(MessageBuffer *b);
    DSError TRKDoStep(MessageBuffer *b);
    DSError TRKDoStop(MessageBuffer *b);

    /*
     *    target-specific command set
     */
    BOOL GetTRKConnected(void);
    BOOL SetTRKConnected(BOOL result);

__end_extern_c

#if NOTIMPLEMENTED
    void DoCompareVersions(void);
    void DoResetNub(void);
    void DoLaunchProcess(void);
    void DoSetTeamBreak(void);
    void DoClearTeamBreak(void);
    void DoStep(void);
    void DoSuspendProcess(void);
    void DoGetFileForProcess(void);
    void DoSwitchDebuggers(void);
    void DoKillTeam(void);
    void DoReadRegisters(void);
    void DoWriteRegisters(void);
    void DoGetIndexTaskForProcess(void);
    void DoGetMSections(void);
    void DoGetMContainers(void);
    void DoGetLoaderEvent(void);
    void DoGetTaskInfo(void);
    void DoGatherProcessInfo(void);
    void DoRequestLoaderEvents(void);
    void DoGetTaskHalted(void);
    void DoGetProcessDied(void);
    void DoAckException(void);
    void DoGetProcesses(void);
    void DoGetMProcessTasks(void);
    void DoGetProcessInfo(void);
    void DoGetMProcesses(void);
    void DoCreateFile(void);
    void DoWriteFile(void);
    void DoCloseFile(void);
    void DoAbortFile(void);
    void DoCloseConnection(void);
    void DoGetPrintfMsg(void);
#endif

#endif /* ndef __MSGHNDLR_H__ */
