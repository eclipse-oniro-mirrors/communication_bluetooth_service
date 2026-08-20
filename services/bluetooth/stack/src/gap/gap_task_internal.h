/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef GAP_TASK_INTERNAL_H
#define GAP_TASK_INTERNAL_H

#include "event.h"

#define WAIT_TIME (-1)

typedef struct {
    Event *event;
    void *ctx;
    void (*func)(void *);
    uint32_t ref;
} GapRunTaskBlockInfo;

typedef struct {
    void (*free)(void *);
    void *ctx;
    void (*func)(void *);
} GapRunTaskUnBlockInfo;

typedef struct {
    int result;
} GapGeneralVoidInfo;

typedef struct {
    int result;
    void *pointer;
} GapGeneralPointerInfo;

typedef struct {
    int result;
    void *callback;
    void *context;
} GapGeneralCallbackInfo;

void GapInTaskProcess(void *ctx);
// GapRunTaskBlockProcess runs |func(ctx)| synchronously on the GAP processing queue and returns
// after it completes. |ctx| remains owned by the caller; the caller must not release it until
// the function returns.
int GapRunTaskBlockProcess(void (*func)(void *), void *ctx);
// GapRunTaskUnBlockProcess schedules |func(ctx)| asynchronously on the GAP processing queue.
// Ownership of |ctx| is transferred to the task framework. |cleanup| (if non-NULL) is called by the
// framework to release all resources owned by |ctx|, including |ctx| itself. When |cleanup| is NULL,
// the framework releases |ctx| with MEM_MALLOC.free. |cleanup| must be NULL-safe and idempotent.
int GapRunTaskUnBlockProcess(void (*func)(void *), void *ctx, void (*cleanup)(void *));

#endif
