/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ISO_TASK_INTERNAL_H
#define ISO_TASK_INTERNAL_H

#include "event.h"

#define ISO_WAIT_TIME (-1)

typedef struct {
    Event *event;
    void *ctx;
    void (*func)(void *);
    int ref;
} IsoRunTaskBlockInfo;

typedef struct {
    void (*freeCtx)(void *);
    void *ctx;
    void (*func)(void *);
} IsoRunTaskUnBlockInfo;

typedef struct {
    int result;
} IsoGeneralVoidInfo;

typedef struct {
    int result;
    void *pointer;
} IsoGeneralPointerInfo;

typedef struct {
    int result;
    void *callback;
    void *context;
} IsoGeneralCallbackInfo;

int IsoRunTaskBlockProcess(void (*func)(void *), void *ctx);
// |freeCtx| (when non-NULL) is invoked on the ISO queue with |ctx| to release it, so it must be
// callable from the Stack thread. When |freeCtx| is NULL, a heap-allocated |ctx| is released
// with MEM_MALLOC.free by the queued task. If the task could not be queued
// (BTM_RunTaskInProcessingQueue failure, or |func| is NULL), the context is NOT released here:
// ownership stays with the caller and it is responsible for the release.
int IsoRunTaskUnBlockProcess(void (*func)(void *), void *ctx, void (*freeCtx)(void *));
// Non-blocking variant of IsoRunTaskUnBlockProcess (backed by
// BTM_RunTaskInProcessingQueueNoBlock): a full ISO queue drops the task with an error code
// instead of blocking. Only for HCI event dispatch, which runs on the Stack thread that
// drains the queue - a blocking enqueue there would deadlock. The context ownership rules
// above apply unchanged: on failure the caller still owns |ctx| and must release it.
int IsoRunTaskUnBlockProcessNoBlock(void (*func)(void *), void *ctx, void (*freeCtx)(void *));

#endif // ISO_TASK_INTERNAL_H
