/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "btif/common/btif_task_scheduler.h"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include "log.h"

namespace bluetooth {
namespace btif {
namespace {

constexpr size_t CALLBACK_QUEUE_MAX = 128;
constexpr size_t CONTROL_QUEUE_MAX = 32;

struct TaskEntry {
    std::string key;
    std::function<void()> task;
};

class TaskDispatcher {
public:
    TaskDispatcher(const char *name, size_t capacity) : name_(name), capacity_(capacity)
    {
        worker_ = std::thread(&TaskDispatcher::Run, this);
    }

    ~TaskDispatcher()
    {
        Shutdown();
    }

    bool Post(std::function<void()> task, BtifTaskPolicy policy, const std::string &key)
    {
        if (!task) {
            return false;
        }
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopped_) {
                return false;
            }
            if (policy == BtifTaskPolicy::COALESCE_BY_KEY && !key.empty()) {
                for (auto &entry : queue_) {
                    if (entry.key == key) {
                        entry.task = std::move(task);
                        condition_.notify_one();
                        return true;
                    }
                }
            }
            if (queue_.size() >= capacity_) {
                if (policy == BtifTaskPolicy::DROP_OLDEST) {
                    queue_.pop_front();
                } else {
                    HILOGW("%{public}s task queue full, capacity=%{public}zu", name_, capacity_);
                    return false;
                }
            }
            queue_.push_back(TaskEntry {key, std::move(task)});
        }
        condition_.notify_one();
        return true;
    }

    void Shutdown()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopped_) {
                return;
            }
            stopped_ = true;
            queue_.clear();
        }
        condition_.notify_all();
        if (worker_.joinable()) {
            worker_.join();
        }
    }

private:
    void Run()
    {
        while (!stopped_ || !queue_.empty()) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this] { return stopped_ || !queue_.empty(); });
                if (stopped_ && queue_.empty()) {
                    return;
                }
                task = std::move(queue_.front().task);
                queue_.pop_front();
            }
            task();
        }
    }

    const char *name_;
    size_t capacity_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<TaskEntry> queue_;
    bool stopped_ = false;
    std::thread worker_;
};

TaskDispatcher &GetCallbackDispatcher()
{
    static TaskDispatcher dispatcher("btif-callback", CALLBACK_QUEUE_MAX);
    return dispatcher;
}

TaskDispatcher &GetControlDispatcher()
{
    static TaskDispatcher dispatcher("btif-control", CONTROL_QUEUE_MAX);
    return dispatcher;
}

} // namespace

bool TaskScheduler::Post(BtifTaskLane lane, std::function<void()> task,
    BtifTaskPolicy policy, const std::string &key)
{
    if (lane == BtifTaskLane::CALLBACK) {
        return GetCallbackDispatcher().Post(std::move(task), policy, key);
    }
    return GetControlDispatcher().Post(std::move(task), policy, key);
}

void TaskScheduler::Cleanup()
{
    GetCallbackDispatcher().Shutdown();
    GetControlDispatcher().Shutdown();
}

} // namespace btif
} // namespace bluetooth
