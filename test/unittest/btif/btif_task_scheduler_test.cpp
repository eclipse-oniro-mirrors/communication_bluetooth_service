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

#include "btif/common/btif_task_scheduler.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace testing::ext;
using namespace bluetooth::btif;

namespace {
constexpr auto WAIT_TIMEOUT = std::chrono::seconds(3);
}

class TaskSchedulerTest : public testing::Test {};

HWTEST_F(TaskSchedulerTest, TaskScheduler_PostToBothLanes_001, TestSize.Level1)
{
    std::atomic<int> callbackCount = 0;
    std::atomic<int> controlCount = 0;
    std::mutex mutex;
    std::condition_variable condition;
    auto done = [&mutex, &condition]() {
        std::lock_guard<std::mutex> lock(mutex);
        condition.notify_all();
    };

    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CALLBACK, [&]() {
        callbackCount++;
        done();
    }));
    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
        controlCount++;
        done();
    }));

    std::unique_lock<std::mutex> lock(mutex);
    EXPECT_TRUE(condition.wait_for(lock, WAIT_TIMEOUT, [&]() {
        return callbackCount.load() == 1 && controlCount.load() == 1;
    }));
    EXPECT_EQ(callbackCount.load(), 1);
    EXPECT_EQ(controlCount.load(), 1);
}

HWTEST_F(TaskSchedulerTest, TaskScheduler_ControlLaneFifo_002, TestSize.Level1)
{
    std::mutex mutex;
    std::condition_variable condition;
    std::vector<int> order;
    for (int index = 0; index < 5; index++) {
        ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&, index]() {
            std::lock_guard<std::mutex> lock(mutex);
            order.push_back(index);
            if (order.size() == 5) {
                condition.notify_one();
            }
        }));
    }

    std::unique_lock<std::mutex> lock(mutex);
    EXPECT_TRUE(condition.wait_for(lock, WAIT_TIMEOUT, [&]() { return order.size() == 5; }));
    EXPECT_EQ(order, (std::vector<int>{0, 1, 2, 3, 4}));
}

HWTEST_F(TaskSchedulerTest, TaskScheduler_CoalesceByKey_003, TestSize.Level1)
{
    std::mutex mutex;
    std::condition_variable condition;
    bool blockerStarted = false;
    bool releaseBlocker = false;
    int lastValue = 0;

    ASSERT_TRUE(TaskScheduler::Post(BtifTaskLane::CONTROL, [&]() {
        std::unique_lock<std::mutex> lock(mutex);
        blockerStarted = true;
        condition.notify_all();
        condition.wait(lock, [&]() { return releaseBlocker; });
    }));

    {
        std::unique_lock<std::mutex> lock(mutex);
        ASSERT_TRUE(condition.wait_for(lock, WAIT_TIMEOUT, [&]() { return blockerStarted; }));
    }

    ASSERT_TRUE(TaskScheduler::Post(
        BtifTaskLane::CONTROL, [&]() { lastValue = 1; }, BtifTaskPolicy::COALESCE_BY_KEY, "scan-mode"));
    ASSERT_TRUE(TaskScheduler::Post(
        BtifTaskLane::CONTROL, [&]() { lastValue = 2; }, BtifTaskPolicy::COALESCE_BY_KEY, "scan-mode"));
    ASSERT_TRUE(TaskScheduler::Post(
        BtifTaskLane::CONTROL,
        [&]() {
            std::lock_guard<std::mutex> taskLock(mutex);
            lastValue = 3;
            condition.notify_all();
        },
        BtifTaskPolicy::COALESCE_BY_KEY, "scan-mode"));

    {
        std::lock_guard<std::mutex> lock(mutex);
        releaseBlocker = true;
        condition.notify_all();
    }

    std::unique_lock<std::mutex> lock(mutex);
    EXPECT_TRUE(condition.wait_for(lock, WAIT_TIMEOUT, [&]() { return lastValue == 3; }));
    EXPECT_EQ(lastValue, 3);
}

HWTEST_F(TaskSchedulerTest, TaskScheduler_NullTaskRejected_004, TestSize.Level1)
{
    EXPECT_FALSE(TaskScheduler::Post(BtifTaskLane::CONTROL, std::function<void()>()));
}
