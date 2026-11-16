/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>

#include "btif/common/btif_task_scheduler.h"

using namespace testing::ext;
using namespace bluetooth::btif;

namespace {
constexpr int CONTROL_QUEUE_CAPACITY = 32;
constexpr auto WAIT_TIMEOUT = std::chrono::seconds(3);

bool WaitFor(const std::function<bool()> &predicate, std::mutex &mutex, std::condition_variable &condition)
{
    std::unique_lock<std::mutex> lock(mutex);
    return condition.wait_for(lock, WAIT_TIMEOUT, predicate);
}
}

class TaskSchedulerLifecycleTest : public testing::Test {};

HWTEST_F(TaskSchedulerLifecycleTest, TaskSchedulerLifecycle_QueueFullFifo_001, TestSize.Level1)
{
    std::mutex mutex;
    std::condition_variable condition;
    bool blockerStarted = false;
    bool releaseBlocker = false;
    std::atomic<int> executed = 0;

    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
        std::unique_lock<std::mutex> lock(mutex);
        blockerStarted = true;
        condition.notify_all();
        condition.wait(lock, [&]() { return releaseBlocker; });
    }));
    ASSERT_TRUE(WaitFor([&]() { return blockerStarted; }, mutex, condition));

    for (int index = 0; index < CONTROL_QUEUE_CAPACITY; index++) {
        ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
            executed++;
            condition.notify_all();
        }));
    }
    EXPECT_FALSE(TaskScheduler::Post(BtifTaskLane::CONTROL, []() {}));

    {
        std::lock_guard<std::mutex> lock(mutex);
        releaseBlocker = true;
        condition.notify_all();
    }
    ASSERT_TRUE(WaitFor([&]() { return executed.load() == CONTROL_QUEUE_CAPACITY; }, mutex, condition));
}

HWTEST_F(TaskSchedulerLifecycleTest, TaskSchedulerLifecycle_DropOldest_002, TestSize.Level1)
{
    std::mutex mutex;
    std::condition_variable condition;
    bool blockerStarted = false;
    bool releaseBlocker = false;
    std::atomic<int> executed = 0;
    std::atomic<bool> oldestExecuted = false;
    std::atomic<int> lastExecuted = -1;

    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
        std::unique_lock<std::mutex> lock(mutex);
        blockerStarted = true;
        condition.notify_all();
        condition.wait(lock, [&]() { return releaseBlocker; });
    }));
    ASSERT_TRUE(WaitFor([&]() { return blockerStarted; }, mutex, condition));

    for (int index = 0; index < CONTROL_QUEUE_CAPACITY; index++) {
        ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&, index]() {
            if (index == 0) {
                oldestExecuted = true;
            }
            lastExecuted = index;
            executed++;
            condition.notify_all();
        }));
    }
    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
        lastExecuted = CONTROL_QUEUE_CAPACITY;
        executed++;
        condition.notify_all();
    }, BtifTaskPolicy::DROP_OLDEST));

    {
        std::lock_guard<std::mutex> lock(mutex);
        releaseBlocker = true;
        condition.notify_all();
    }
    ASSERT_TRUE(WaitFor([&]() { return executed.load() == CONTROL_QUEUE_CAPACITY; }, mutex, condition));
    EXPECT_FALSE(oldestExecuted.load());
    EXPECT_EQ(lastExecuted.load(), CONTROL_QUEUE_CAPACITY);
}

HWTEST_F(TaskSchedulerLifecycleTest, TaskSchedulerLifecycle_CleanupIsIdempotent_003, TestSize.Level1)
{
    TaskScheduler::Cleanup();
    TaskScheduler::Cleanup();
    EXPECT_FALSE(TaskScheduler::Post(BtifTaskLane::CONTROL, []() {}));
}
