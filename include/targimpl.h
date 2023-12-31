/****************************************************************************/
/*

DESCRIPTION

    API for target-specific implementation.  Implementing these basic
    functions will allow a primitive version of CWDS (CWDS L1) to
    become operational.

    Functions may only work for 32-bit (or smaller) processors.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __TARGIMPL_H__
#define __TARGIMPL_H__


#include "trktypes.h"
#include "nubevent.h"
#include "trk_prefix.h"


typedef enum ValidMemoryOptions
{
    kValidMemoryReadable,
    kValidMemoryWriteable
} ValidMemoryOptions;

typedef enum MemoryAccessOptions
{
    kUserMemory,                    /* modify user memory (invisible bp) */
    kDebuggerMemory                 /* modify real memory (visible bp) */
} MemoryAccessOptions;

enum
{                                   /* DSError; see dserror.h */
    kInvalidMemory = 0x0700,
    kInvalidRegister,
    kCWDSException,
    kUnsupportedError,
    kInvalidProcessId,
    kInvalidThreadId,
    kOsError
};

typedef struct DSVersions
{
    u8            kernelMajor;                /* (1) */
    u8            kernelMinor;                /* (1) */
    u8            protocolMajor;              /* (1) */
    u8            protocolMinor;              /* (1) */
} DSVersions;

typedef struct DSCPUType
{
    u8            cpuMajor;                   /* (1) */
    u8            cpuMinor;                   /* (1) */
    u8            bigEndian;                  /* (1) */
    u8            defaultTypeSize;            /* (1) */
    u8            fpTypeSize;                 /* (1) */
    u8            extended1TypeSize;          /* (1) */
    u8            extended2TypeSize;          /* (1) */
} DSCPUType;

__extern_c

    DSError TRKValidMemory32(const void *addr, size_t length,
                            ValidMemoryOptions readWriteable);

    DSError TRKTargetAccessMemory(void *Data,
                            void *virtualAddress,
                            size_t *memorySize,
                            MemoryAccessOptions accessOptions,
                            BOOL read);

    DSError OsTargetAccessMemory(void *Data,
                            void *virtualAddress,
                            size_t *memorySize,
                            MemoryAccessOptions accessOptions,
                            BOOL read,
                            u32 processId,
                            u32 threadId);

    DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError OsTargetAccessDefault(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read,
                            u32 processID,
                            u32 threadID);

    DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError OsTargetAccessFP(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read,
                            u32 processId,
                            u32 threadId);

    DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError OsTargetAccessExtended1(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read,
                            u32 processId,
                            u32 threadId);

    DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError OsTargetAccessExtended2(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read,
                            u32 processId,
                            u32 threadId);

    DSError TRKTargetVersions(DSVersions *versions);
    DSError TRKTargetSupportMask(DSSupportMask *mask);
    DSError TRKTargetCPUType(DSCPUType *cpuType);
    DSError TRKTargetCheckException(void);

    DSError TRKInitializeTarget(void);

    DSError TRKTargetContinue(void);
    DSError OsTargetContinue(u32 processId, u32 threadId);

    DSError TRKTargetInterrupt(NubEvent *event);

    DSError TRKTargetAddStopInfo(MessageBuffer *buffer);
    DSError OsTargetAddStopInfo(MessageBuffer *buffer);

    DSError TRKTargetAddExceptionInfo(MessageBuffer *buffer);
    DSError OsTargetAddExceptionInfo(MessageBuffer *buffer);

    DSError TRKTargetSingleStep(u32 count, BOOL stepOver);
    DSError OsTargetSingleStep(u32 count, BOOL stepOver,
                            u32 processId,
                            u32 threadId);

    DSError TRKTargetStepOutOfRange(u32 start, u32 end, BOOL stepOver);
    DSError OsTargetStepOutOfRange(u32 start, u32 end,
                            BOOL stepOver,
                            u32 processId,
                            u32 threadId);

    PCType TRKTargetGetPC(void);
    PCType OsTargetGetPC(u32 processId, u32 threadId);

    DSError TRKTargetSupportRequest(void);
    DSError TRKTargetFlushCache(u8 options, void *start, void *end);

    BOOL TRKTargetStopped(void);
    BOOL OsTargetStopped(u32 processId, u32 threadId);

    void TRKTargetSetStopped(BOOL stop_state);

    DSError TRKTargetStop(void);
    DSError OsTargetStop(u8 options, u32 processId, u32 threadId);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    u8 TRKTargetCPUMinorType(void);
#endif

    void *TRKTargetTranslate(u32 *address);

    DSError OsTargetValidStep(u32 processId, u32 threadId);

    ASM_PREFIX void TRK_InterruptHandler(/*ui16 exceptionNumber*/);

#if TRK_TRANSPORT_INT_DRIVEN
    void TRKTargetSetInputPendingPtr(volatile u8 *inputPendingPtr);
#endif

__end_extern_c

/*
** Define aliases for the functions which have both OS and non-OS
** variants.  Each alias references the variant which is appropriate
** for the current protocol level.
*/
#if DS_PROTOCOL < DS_PROTOCOL_RTOS

    #define XTargetAccessMemory       TRKTargetAccessMemory
    #define XTargetAccessDefault      TRKTargetAccessDefault
    #define XTargetAccessFP           TRKTargetAccessFP
    #define XTargetAccessExtended1    TRKTargetAccessExtended1
    #define XTargetAccessExtended2    TRKTargetAccessExtended2
    #define XTargetContinue           TRKTargetContinue
    #define XTargetSingleStep         TRKTargetSingleStep
    #define XTargetStepOutOfRange     TRKTargetStepOutOfRange
    #define XTargetGetPC              TRKTargetGetPC
    #define XTargetStopped            TRKTargetStopped
    #define XTargetStop               TRKTargetStop
    #define XTargetAddStopInfo        TRKTargetAddStopInfo
    #define XTargetAddExceptionInfo   TRKTargetAddExceptionInfo

#else /* #if DS_PROTOCOL < DS_PROTOCOL_RTOS */

    #define XTargetAccessMemory       OsTargetAccessMemory
    #define XTargetAccessDefault      OsTargetAccessDefault
    #define XTargetAccessFP           OsTargetAccessFP
    #define XTargetAccessExtended1    OsTargetAccessExtended1
    #define XTargetAccessExtended2    OsTargetAccessExtended2
    #define XTargetContinue           OsTargetContinue
    #define XTargetSingleStep         OsTargetSingleStep
    #define XTargetStepOutOfRange     OsTargetStepOutOfRange
    #define XTargetGetPC              OsTargetGetPC
    #define XTargetStopped            OsTargetStopped
    #define XTargetStop               OsTargetStop
    #define XTargetAddStopInfo        OsTargetAddStopInfo
    #define XTargetAddExceptionInfo   OsTargetAddExceptionInfo

#endif /* #if DS_PROTOCOL < DS_PROTOCOL_RTOS */


#endif /* __TARGIMPL_H__ */
