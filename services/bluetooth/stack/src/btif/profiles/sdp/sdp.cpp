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

#include "btif/profiles/sdp/sdp.h"

#include <cstring>
#include <memory>
#include <atomic>
#include <vector>

#include "btif/common/btif_constants.h"
#include "btstack.h"
#include "include/sdp.h"
#include "btif/profiles/profile_interfaces.h"
#include "log.h"
#include "securec.h"

namespace {

class SdpCallbacks final {
public:
    static btsdp_callbacks_t *Get() { return callbacks.load(); }
    static void Set(btsdp_callbacks_t *value) { callbacks.store(value); }

private:
    inline static std::atomic<btsdp_callbacks_t *> callbacks { nullptr };
};

struct SdpSearchContext {
    RawAddress address;
    bluetooth::Uuid uuid;
};

BtAddr ToBtAddr(const RawAddress &address)
{
    BtAddr result = {};
    if (memcpy_s(result.addr, sizeof(result.addr), address.address, RawAddress::kLength) != EOK) {
        HILOGE("convert raw address failed");
        return BtAddr{};
    }
    result.type = BT_PUBLIC_DEVICE_ADDRESS;
    return result;
}

BtUuid ToBtUuid(const bluetooth::Uuid &uuid)
{
    BtUuid result = {};
    size_t size = uuid.GetShortestRepresentationSize();
    if (size == bluetooth::Uuid::kNumBytes16) {
        result.type = BT_UUID_16;
        result.uuid16 = uuid.As16Bit();
    } else if (size == bluetooth::Uuid::kNumBytes32) {
        result.type = BT_UUID_32;
        result.uuid32 = uuid.As32Bit();
    } else {
        result.type = BT_UUID_128;
        const auto bytes = uuid.To128BitBE();
        if (memcpy_s(result.uuid128, sizeof(result.uuid128), bytes.data(), sizeof(result.uuid128)) != EOK) {
            HILOGE("convert uuid128 failed");
            return BtUuid{};
        }
    }
    return result;
}

void FillProtocolInfo(const SdpService &service, bluetooth_sdp_record &record)
{
    record.hdr.rfcomm_channel_number = bluetooth::btif::kSdpUnknownChannel;
    record.hdr.l2cap_psm = bluetooth::btif::kSdpUnknownChannel;
    record.hdr.profile_version = 0;

    if (service.descriptor != nullptr) {
        for (uint16_t index = 0; index < service.descriptorNumber; index++) {
            const SdpProtocolDescriptor &descriptor = service.descriptor[index];
            if (descriptor.protocolUuid.type != BT_UUID_16 || descriptor.parameterNumber == 0) {
                continue;
            }
            if (descriptor.protocolUuid.uuid16 == bluetooth::btif::kSdpRfcommProtocolUuid) {
                record.hdr.rfcomm_channel_number = static_cast<int32_t>(descriptor.parameter[0].value);
            } else if (descriptor.protocolUuid.uuid16 == bluetooth::btif::kSdpL2capProtocolUuid) {
                record.hdr.l2cap_psm = static_cast<int32_t>(descriptor.parameter[0].value);
            }
        }
    }

    if (service.profileDescriptor != nullptr && service.profileDescriptorNumber > 0) {
        record.hdr.profile_version = static_cast<int32_t>(service.profileDescriptor[0].versionNumber);
    }
}

void OnSdpSearchAttribute(const BtAddr *addr, const SdpService *serviceArray, uint16_t serviceNum, void *context)
{
    std::unique_ptr<SdpSearchContext> searchContext(static_cast<SdpSearchContext *>(context));
    if (searchContext == nullptr || addr == nullptr) {
        return;
    }

    std::vector<bluetooth_sdp_record> records;
    bt_status_t status = BT_STATUS_SUCCESS;
    if (serviceArray == nullptr || serviceNum == 0) {
        status = BT_STATUS_FAIL;
        records.resize(1);
    } else {
        records.resize(serviceNum);
        for (uint16_t index = 0; index < serviceNum; index++) {
            const SdpService &service = serviceArray[index];
            bluetooth_sdp_record &record = records[index];
            record.hdr.type = SDP_TYPE_RAW;
            record.hdr.uuid = searchContext->uuid;
            record.hdr.service_name = service.serviceName;
            record.hdr.service_name_length = service.serviceName == nullptr ?
                0 : static_cast<uint32_t>(std::strlen(service.serviceName));
            FillProtocolInfo(service, record);
        }
    }

    HILOGI("SDP search complete addr=%{public}s services=%{public}d status=%{public}d",
        searchContext->address.ToString().c_str(), serviceNum, status);
    if (SdpCallbacks::Get() != nullptr && SdpCallbacks::Get()->sdp_search_cb != nullptr) {
        SdpCallbacks::Get()->sdp_search_cb(status, searchContext->address, searchContext->uuid,
            static_cast<int>(records.size()), records.data());
    }
}

bt_status_t BtifSdpInit(btsdp_callbacks_t *callbacks)
{
    if (callbacks == nullptr) {
        HILOGE("SDP init with null callbacks");
        return BT_STATUS_PARM_INVALID;
    }
    SdpCallbacks::Set(callbacks);
    HILOGI("SDP callbacks registered");
    return BT_STATUS_SUCCESS;
}

bt_status_t BtifSdpDeinit()
{
    SdpCallbacks::Set(nullptr);
    HILOGI("SDP callbacks unregistered");
    return BT_STATUS_SUCCESS;
}

bt_status_t BtifSdpSdpSearch(RawAddress *bdAddr, const bluetooth::Uuid &uuid)
{
    if (bdAddr == nullptr || SdpCallbacks::Get() == nullptr) {
        HILOGE("SDP search invalid state addr=%{public}d callback=%{public}d",
            bdAddr != nullptr ? 1 : 0, SdpCallbacks::Get() != nullptr ? 1 : 0);
        return BT_STATUS_PARM_INVALID;
    }

    BtAddr addr = ToBtAddr(*bdAddr);
    BtUuid stackUuid = ToBtUuid(uuid);
    SdpUuid uuidArray = {};
    uuidArray.uuidNum = 1;
    uuidArray.uuid = &stackUuid;

    SdpAttributeIdList attributeIdList = {};
    attributeIdList.type = SDP_TYPE_RANGE;
    attributeIdList.attributeIdRange.start = 0;
    attributeIdList.attributeIdRange.end = bluetooth::btif::kSdpAttributeRangeEnd;

    auto context = std::make_unique<SdpSearchContext>();
    context->address = *bdAddr;
    context->uuid = uuid;

    int result = SDP_ServiceSearchAttribute(&addr, &uuidArray, attributeIdList, context.get(),
        OnSdpSearchAttribute);
    if (result != BT_SUCCESS) {
        HILOGE("SDP search request failed addr=%{public}s result=%{public}d",
            bdAddr->ToString().c_str(), result);
        return BT_STATUS_FAIL;
    }
    (void)context.release();
    HILOGI("SDP search request sent addr=%{public}s uuid=%{public}s",
        bdAddr->ToString().c_str(), uuid.ToString().c_str());
    return BT_STATUS_SUCCESS;
}

bt_status_t BtifSdpCreateSdpRecord(bluetooth_sdp_record *record, int *recordHandle)
{
    (void)record;
    (void)recordHandle;
    HILOGW("%{public}s: SDP record creation not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

bt_status_t BtifSdpRemoveSdpRecord(int sdpHandle)
{
    (void)sdpHandle;
    HILOGW("%{public}s: SDP record removal not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

} // namespace

const void *ProfileInterfaces::GetSdp()
{
    static const btsdp_interface_t table = {
        sizeof(btsdp_interface_t),
        BtifSdpInit,
        BtifSdpDeinit,
        BtifSdpSdpSearch,
        BtifSdpCreateSdpRecord,
        BtifSdpRemoveSdpRecord,
    };
    return &table;
}
