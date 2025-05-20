/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef REMOTE_OBSERVER_LIST_H
#define REMOTE_OBSERVER_LIST_H

#include <functional>
#include <map>
#include <mutex>
#include <tuple>

#include "refbase.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "bluetooth_log.h"

namespace OHOS {
namespace Bluetooth {
template <typename T, typename... Args>
class RemoteObserverList final {
    using DrCallbackFunc = std::function<void(const sptr<T> &observer, Args... args)>;
public:
    RemoteObserverList() = default;
    ~RemoteObserverList();
    bool Register(const sptr<T> &observer, DrCallbackFunc func, Args... args);
    bool Deregister(const sptr<T> &observer);
    void ForEach(const std::function<void(sptr<T>)> &observer);

    class ObserverDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        ObserverDeathRecipient(const sptr<T> &observer, RemoteObserverList<T, Args...> *owner);
        sptr<T> GetObserver() const
        {
            return observer_;
        };
        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;

    private:
        sptr<T> observer_ {};
        RemoteObserverList<T, Args...> *owner_ {};
    };
    std::mutex lock_ {};
    using ObserverMap = std::map<sptr<T>, sptr<ObserverDeathRecipient>>;
    ObserverMap observers_ {};

    struct RtOberserInfo {
        DrCallbackFunc callbackFunc;
        std::tuple<Args...> args;
    };
    using BtServerMap = std::map<sptr<T>, RtOberserInfo> ;
    BtServerMap btServers_ {};

    RemoteObserverList(const RemoteObserverList &) = delete;
    RemoteObserverList &operator=(const RemoteObserverList &) = delete;

private:
    bool UnregisterInternal(typename ObserverMap::iterator iter);
};

template <typename T, typename... Args>
RemoteObserverList<T, Args...>::~RemoteObserverList()
{
    HILOGI("RemoteObserverList<T, Args...>::~RemoteObserverList() called");
    std::lock_guard<std::mutex> lock(lock_);
    for (auto it = observers_.begin(); it != observers_.end(); ++it) {
        sptr<ObserverDeathRecipient> dr = it->second;
        if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
            HILOGE("Failed to unlink death recipient from observer");
        }
    }
    observers_.clear();
    btServers_.clear();
}

template <typename T, typename... Args>
bool RemoteObserverList<T, Args...>::Register(const sptr<T> &observer, DrCallbackFunc func, Args... args)
{
    HILOGI("RemoteObserverList<T, Args...>::Register called");
    std::lock_guard<std::mutex> lock(lock_);
    bool isMatch = false;
    for (const auto &it : observers_) {
        if (it.first != nullptr && it.first->AsObject() == observer->AsObject()) {
            HILOGW("Observer list already contains given observer");
            isMatch = true;
            break;
        }
    }
    if (!isMatch) {
        sptr<ObserverDeathRecipient> dr(new ObserverDeathRecipient(observer, this));
        if (!observer->AsObject()->AddDeathRecipient(dr)) {
            HILOGE("Failed to link death recipient to observer");
        }
        observers_[observer] = dr;
    }

    bool isExist = false;
    for (const auto &it : btServers_) {
        if (it.first != nullptr && it.first->AsObject() == observer->AsObject()) {
            HILOGW("btServers_ list already contains given observer");
            isExist = true;
            break;
        }
    }
    if (!isExist) {
        struct RtOberserInfo rtInfo;
        rtInfo.callbackFunc = func;
        rtInfo.args = std::make_tuple(args...);
        btServers_[observer] = rtInfo;
    }

    return true;
}

template <typename T, typename... Args>
bool RemoteObserverList<T, Args...>::Deregister(const sptr<T> &observer)
{
    HILOGI("RemoteObserverList<T, Args...>::Deregister called");
    for (auto it = observers_.begin(); it != observers_.end();) {
        if (it->first != nullptr && it->first->AsObject() == observer->AsObject()) {
            UnregisterInternal(it++);
        } else {
            it++;
        }
    }
    HILOGW("Given observer not registered with this list");
    return false;
}

template <typename T, typename... Args>
void RemoteObserverList<T, Args...>::ForEach(const std::function<void(sptr<T>)> &observer)
{
    std::lock_guard<std::mutex> lock(lock_);
    for (const auto &it : observers_) {
        if (it.first != nullptr) {
            observer(it.first);
        }
    }
}

template <typename T, typename... Args>
RemoteObserverList<T, Args...>::ObserverDeathRecipient::ObserverDeathRecipient(
    const sptr<T> &observer, RemoteObserverList<T, Args...> *owner)
    : observer_(observer), owner_(owner)
{
    HILOGI("RemoteObserverList<T>::ObserverDeathRecipient::ObserverDeathRecipient called");
}

template <typename T, typename... Args>
void RemoteObserverList<T, Args...>::ObserverDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &object)
{
    // Remove the observer but no need to call unlinkToDeath.
    std::lock_guard<std::mutex> lock(owner_->lock_);
    HILOGD("Enter OnRemoteDied");
    for (auto it = owner_->btServers_.begin(); it != owner_->btServers_.end();) {
        if (it->first != nullptr && it->first->AsObject() == object) {
            if (it->second.callbackFunc) {
                std::apply(it->second.callbackFunc, std::tuple_cat(std::make_tuple(it->first), it->second.args));
            }
            it = owner_->btServers_.erase(it);
        } else {
            it++;
        }
    }

    HILOGI("Callback from dead process unregistered");
}

template <typename T, typename... Args>
bool RemoteObserverList<T, Args...>::UnregisterInternal(typename ObserverMap::iterator iter)
{
    HILOGI("RemoteObserverList<T, Args...>::UnregisterInternal called");
    sptr<ObserverDeathRecipient> dr = iter->second;

    if (!dr->GetObserver()->AsObject()->RemoveDeathRecipient(dr)) {
        HILOGE("Failed to unlink death recipient from observer");
    }

    observers_.erase(iter);
    return true;
}
}  // namespace Bluetooth
}  // namespace OHOS
#endif  // REMOTE_OBSERVER_LIST_H
