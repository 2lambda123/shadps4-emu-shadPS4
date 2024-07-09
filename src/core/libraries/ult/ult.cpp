// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"
#include "ult.h"

namespace Libraries::Ult {

bool isUltInitialized = false;
int PS4_SYSV_ABI sceUltInitialize() {
    LOG_INFO(Lib_Ult, "called");
    if (isUltInitialized) {
        return ORBIS_ULT_ERROR_STATE;
    }

    isUltInitialized = true;
    return ORBIS_OK;
}
int PS4_SYSV_ABI _sceUltUlthreadRuntimeCreate(OrbisUltUlthreadRuntime* runtime, const char* name,
                                              uint32_t maxNumUlthread, uint32_t numWorkerThread,
                                              void* workArea,
                                              OrbisUltUlthreadRuntimeOptParam* optParam) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI _sceUltUlthreadCreate(OrbisUltUlthread* ulthread, const char* name,
                                       OrbisUltUlthreadEntry entry, uint64_t arg, void* context,
                                       uint64_t sizeContext, OrbisUltUlthreadRuntime* runtime,
                                       OrbisUltUlthreadOptParam* optParam) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI _sceUltWaitingQueueResourcePoolCreate(
    OrbisUltWaitingQueueResourcePool* pool, const char* name,
    uint32_t numThreads, uint32_t numSyncObjects,
    void* workArea,
    OrbisUltWaitingQueueResourcePoolOptParam* optParam)
{
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    
    if (pool == nullptr)
        return ORBIS_ULT_ERROR_NULL;

    if (name != nullptr)
        LOG_INFO(Lib_Ult, "Creating WaitingQueueResourcePool for {}", name);

    // TODO: Check memory alignment
    // TODO: Set ORBIS_ULT_ERROR_NOT_INITIALIZE

    if (optParam != nullptr)
    {
        // TODO: Check memory alignment
        // TODO: FUN_0100678(optParam)
    }

    // TODO: FUN_01011b10(&pool->field41_0x30,numThreads,numSyncObjects,(long)workArea);

    if (numThreads > 0 && numSyncObjects > 0 && workArea != nullptr)
    {
        pool->workArea = workArea;
        void* w = (void*)((long)workArea + 0x20);
    }

    // IF NO ERROR
    //FUN_0101e800((char *)pool,name)
    pool->field32_0x20 = 0x100; // ??
    pool->field33_0x22 = '\x06'; // ??
    pool->numThreads = numThreads * 2;
    pool->numSyncObjects = numSyncObjects;
    //BG26hBGiNlw(pool, 0x16, &pool->field178_0xc0);
    // ENDIF

    return ORBIS_OK;
}

int PS4_SYSV_ABI _sceUltQueueDataResourcePoolCreate(
    OrbisUltQueueDataResourcePool* pool, const char* name,
    uint32_t numData, uint64_t dataSize,
    uint32_t numQueueObjects,
    OrbisUltWaitingQueueResourcePool* waitingQueueResourcePool,
    void* workArea,
    OrbisUltQueueDataResourcePoolOptParam* optParam)
{

    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceUltQueueTryPush(OrbisUltQueue* queue, void* data) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceUltQueuePush(OrbisUltQueue* queue, void* data) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceUltQueueTryPop(OrbisUltQueue* queue, void* data) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceUltQueuePop(OrbisUltQueue* queue, void* data) {
    LOG_ERROR(Lib_Ult, "(STUBBED) called");
    return ORBIS_OK;
}

void RegisterlibSceUlt(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("hZIg1EWGsHM", "libSceUlt", 1, "libSceUlt", 1, 1, sceUltInitialize);
    LIB_FUNCTION("jw9FkZBXo-g", "libSceUlt", 1, "libSceUlt", 1, 1, _sceUltUlthreadRuntimeCreate);
    LIB_FUNCTION("uZz3ci7XYqc", "libSceUlt", 1, "libSceUlt", 1, 1, sceUltQueueTryPop);
    LIB_FUNCTION("RVSq2tsm2yw", "libSceUlt", 1, "libSceUlt", 1, 1, sceUltQueuePop);
    LIB_FUNCTION("znI3q8S7KQ4", "libSceUlt", 1, "libSceUlt", 1, 1, _sceUltUlthreadCreate);
    LIB_FUNCTION("6Mc2Xs7pI1I", "libSceUlt", 1, "libSceUlt", 1, 1, sceUltQueueTryPush);
    LIB_FUNCTION("dUwpX3e5NDE", "libSceUlt", 1, "libSceUlt", 1, 1, sceUltQueuePush);
    LIB_FUNCTION("YiHujOG9vXY", "libSceUlt", 1, "libSceUlt", 1, 1,
                 _sceUltWaitingQueueResourcePoolCreate);
    LIB_FUNCTION("TFHm6-N6vks", "libSceUlt", 1, "libSceUlt", 1, 1,
                 _sceUltQueueDataResourcePoolCreate);
};

}