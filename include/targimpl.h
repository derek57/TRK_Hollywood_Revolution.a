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
{                              /* DSError; see dserror.h */
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

    DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError TargetAccessDefaultSingle(u32 reg, DefaultType *value, BOOL read);

    DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError TargetAccessFPSingle(u32 reg, FPType *value, BOOL read);

    DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError TargetAccessExtended1Single(u32 reg, Extended1Type *value, BOOL read);

    DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister,
                            MessageBuffer *b, size_t *registerStorageSize,
                            BOOL read);

    DSError TRKTargetSupportMask(DSSupportMask *mask);
    DSError TRKTargetCPUType(DSCPUType *cpuType);

    DSError TRKInitializeTarget(void);
    DSError CleanupTarget(void);

    DSError TRKTargetContinue(void);

    DSError TRKTargetInterrupt(NubEvent *event);

    DSError TRKTargetAddStopInfo(MessageBuffer *buffer);

    DSError TRKTargetAddExceptionInfo(MessageBuffer *buffer);

    DSError TRKTargetSingleStep(u32 count, BOOL stepOver);

    DSError TRKTargetStepOutOfRange(u32 start, u32 end, BOOL stepOver);

    PCType TRKTargetGetPC(void);

    DSError TRKTargetSupportRequest(void);
    DSError TRKTargetFlushCache(u8 options, void *start, void *end);

    void TRKTargetSetInputPendingPtr(volatile u8 *inputPendingPtr);

    BOOL TRKTargetStopped(void);

    void TRKTargetSetStopped(BOOL stop_state);

    DSError TRKTargetStop(void);

#if 0 // ONLY AVAILABLE IN V0.1 OF THE TRK TO THE WII / NDEV
    u8 TRKTargetCPUMinorType(void);
#endif

    void *TRKTargetTranslate(u32 *address);

    ASM_PREFIX void TRK_InterruptHandler(/*ui16 exceptionNumber*/);

    DSError TargetAccessInstruction(InstructionType *readData,
                                void *virtualAddress,
                                size_t maxSize,
                                MemoryAccessOptions accessOptions,
                                BOOL read);

    DSIOResult TargetWriteFile(u32 handle, u8 *data, size_t *length);
    DSIOResult TargetReadFile(u32 handle, u8 *data, size_t *length);

    DSIOResult TargetOpenFile(DSFileOpenModes modes, u8 *name,
            u32 *handle, u32 *timestamp);

    DSIOResult TargetCloseFile(u32 handle, u32 timestamp);
    DSIOResult TargetPositionFile(DSFilePositionModes mode, u32 handle, u32 offset);

    DSError TargetNotifyFileInput(u32 handle);

    DSError TargetBlockFileIo(BOOL block);

    DSError TargetConnect();
    DSError TargetDisconnect();

__end_extern_c


#endif /* __TARGIMPL_H__ */
