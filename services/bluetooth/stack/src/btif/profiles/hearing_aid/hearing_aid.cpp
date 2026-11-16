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

#include "btif/profiles/hearing_aid/hearing_aid.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

class BtifHearingAidPlaceholder : public HearingAidInterface {
public:
    void Init(HearingAidCallbacks* callbacks) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void Connect(const RawAddress& address) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void Disconnect(const RawAddress& address) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void AddToAcceptlist(const RawAddress& address) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetVolume(int8_t volume) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetVolumeByAddr(const RawAddress& address, int8_t volume) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void StartSendingAudio(const RawAddress& address) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void Cleanup(void) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void RemoveDevice(const RawAddress& address) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }
};

} // namespace

const void *ProfileInterfaces::GetHearingAid()
{
    static BtifHearingAidPlaceholder instance;
    return &instance;
}
