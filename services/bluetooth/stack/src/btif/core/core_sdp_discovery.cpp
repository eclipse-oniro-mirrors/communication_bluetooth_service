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

#include "btif/core/core_sdp_discovery.h"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "alarm.h"
#include "btif/common/btif_constants.h"
#include "btif/common/btif_task_scheduler.h"
#include "btif/core/core_callbacks.h"
#include "btif/core/core_device_store.h"
#include "btif/core/core_discovery_manager.h"
#include "btif/core/core_pairing_manager.h"
#include "btif/core/core_util.h"
#include "btstack.h"
#include "log.h"
#include "sdp.h"
#include "securec.h"

namespace {

using Uuid = bluetooth::Uuid;
using SdpUuidMap = std::map<std::string, std::vector<Uuid>>;

constexpr size_t SDP_MAX_REMOTE_UUIDS = 32;

// Keep the search order and coverage aligned with the reference stack.
// L2CAP (0x0100) is the final generic fallback after all concrete service IDs.
constexpr uint16_t SDP_PROFILE_QUERY_UUIDS[] = {
    0x1200,  // PnP Information
    0x1101,  // SPP
    0x1103,  // DUN
    0x110A,  // A2DP Source
    0x1102,  // LAN Access over PPP
    0x1108,  // HSP
    0x111E,  // HFP HF
    0x1105,  // OPP
    0x1106,  // FTP
    0x1109,  // CTP
    0x1110,  // ICP
    0x1104,  // IrMC Sync
    0x1118,  // BPP Direct Printing
    0x111B,  // BIP Imaging Responder
    0x1115,  // PANU
    0x1116,  // NAP
    0x1117,  // GN
    0x112D,  // SAP
    0x110B,  // A2DP Sink
    0x110E,  // AVRCP
    0x1124,  // HID
    0x1304,  // Video Sink
    0x112F,  // PBAP PSE
    0x1112,  // HSP AG
    0x111F,  // HFP AG
    0x1132,  // MAP MAS
    0x1133,  // MAP MNS
    0x1400,  // HDP
    0x112E,  // PBAP PCE
    0x0007,  // ATT
    0x0100,  // L2CAP generic fallback
};

struct AlarmDeleter {
    void operator()(Alarm *alarm) const
    {
        if (alarm != nullptr) {
            AlarmCancel(alarm);
            AlarmDelete(alarm);
        }
    }
};

using AlarmPtr = std::unique_ptr<Alarm, AlarmDeleter>;

struct QueryContext {
    RawAddress address;
    uint32_t id = 0;
    AlarmPtr alarm;
    bool finished = false;
    size_t profileIndex = 0;
    std::vector<Uuid> collected;
};

}  // namespace

struct CoreSdpDiscovery::State {
    std::mutex mutex;
    std::map<std::string, std::unique_ptr<QueryContext>> queries;
    std::map<std::string, std::vector<Uuid>> uuidCache;
    bool storeLoaded = false;
    uint32_t nextQueryId = 1;
};

CoreSdpDiscovery::CoreSdpDiscovery() : state_(std::make_unique<State>())
{
}

CoreSdpDiscovery::~CoreSdpDiscovery() = default;

CoreSdpDiscovery &::CoreSdpDiscovery::GetInstance()
{
    static CoreSdpDiscovery instance;
    return instance;
}

CoreSdpDiscovery::State &CoreSdpDiscovery::GetState()
{
    return *state_;
}

namespace {

void SaveSdpStore()
{
    SdpUuidMap snapshot;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        snapshot = ::CoreSdpDiscovery::GetInstance().GetState().uuidCache;
    }

    (void)::CoreDeviceStore::Update([snapshot](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
        std::set<std::string> existing;
        for (auto &record : records) {
            std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
            existing.insert(key);
            record.uuids.clear();
            auto iter = snapshot.find(key);
            if (iter == snapshot.end()) {
                continue;
            }
            for (const Uuid &uuid : iter->second) {
                record.uuids.push_back(uuid.To128BitBE());
            }
        }
        for (const auto &entry : snapshot) {
            if (existing.find(entry.first) != existing.end() || entry.first.size() != RawAddress::kLength) {
                continue;
            }
            bluetooth::btif::DeviceStoreRecord record;
            if (memcpy_s(record.address.data(), record.address.size(), entry.first.data(), RawAddress::kLength) !=
                EOK) {
                HILOGE("copy SDP store address failed");
                continue;
            }
            for (const Uuid &uuid : entry.second) {
                record.uuids.push_back(uuid.To128BitBE());
            }
            records.push_back(record);
        }
    });
}

void LoadSdpStore()
{
    if (::CoreSdpDiscovery::GetInstance().GetState().storeLoaded) {
        return;
    }
    ::CoreSdpDiscovery::GetInstance().GetState().storeLoaded = true;
    auto records = ::CoreDeviceStore::Snapshot();
    SdpUuidMap loaded;
    for (const auto &record : records) {
        if (record.uuids.empty()) {
            continue;
        }
        std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
        for (const auto &uuidBytes : record.uuids) {
            loaded[key].push_back(Uuid::From128BitBE(uuidBytes.data()));
        }
    }
    size_t count = loaded.size();
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        ::CoreSdpDiscovery::GetInstance().GetState().uuidCache = std::move(loaded);
    }
    HILOGI("sdp uuid store loaded count=%{public}d", static_cast<int>(count));
}

void ReportUuids(const RawAddress &address, const std::vector<Uuid> &uuids, bool includeName = true)
{
    if (uuids.empty()) {
        HILOGW("sdp report skipped: empty uuid list");
        return;
    }
    if (!CoreCallbackRegistry::HasRemoteDeviceProperties()) {
        HILOGW("sdp report skipped: callback unavailable");
        return;
    }

    std::array<bt_property_t, bluetooth::btif::kNamePropertyCount> properties = {};
    int propertyCount = 0;
    properties[propertyCount].type = BT_PROPERTY_UUIDS;
    properties[propertyCount].len = static_cast<int>(uuids.size() * sizeof(Uuid));
    properties[propertyCount].val = const_cast<Uuid *>(uuids.data());
    propertyCount++;

    // A UUID-only property update can make the Settings model rebuild the
    // device without a name. Include the cached BDNAME in the same callback.
    std::string name;
    bt_bdname_t bdName = {};
    if (includeName && ::CoreDeviceStore::GetName(address, name) && !name.empty()) {
        size_t nameLength = std::min(name.size(), static_cast<size_t>(GAP_NAME_SIZE_MAX));
        if (memcpy_s(bdName.name, sizeof(bdName.name), name.data(), nameLength) != EOK) {
            HILOGE("copy cached bdname failed");
            return;
        }
        properties[propertyCount].type = BT_PROPERTY_BDNAME;
        properties[propertyCount].len = static_cast<int>(nameLength);
        properties[propertyCount].val = &bdName;
        propertyCount++;
    }

    RawAddress reportAddress = address;
    CoreCallbackRegistry::NotifyRemoteDeviceProperties(
        BT_STATUS_SUCCESS, &reportAddress, propertyCount, properties.data());
    HILOGI(
        "sdp report uuid addr=%{public}s count=%{public}d name=%{public}d", address.ToString().c_str(),
        static_cast<int>(uuids.size()), propertyCount > 1 ? 1 : 0);
}

void QueueReportUuids(const RawAddress &address, const std::vector<Uuid> &uuids, bool includeName = true)
{
    if (uuids.empty()) {
        return;
    }
    RawAddress reportAddress = address;
    std::vector<Uuid> reportUuids = uuids;
    if (!bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CALLBACK,
        [reportAddress, reportUuids, includeName]() { ReportUuids(reportAddress, reportUuids, includeName); })) {
        HILOGW("drop sdp uuid report: callback lane unavailable");
    }
}

std::vector<Uuid> ParseServiceUuids(const SdpService *services, uint16_t serviceNum)
{
    std::vector<Uuid> uuids;
    if (services == nullptr) {
        return uuids;
    }
    for (uint16_t index = 0; index < serviceNum; index++) {
        const SdpService &service = services[index];
        if (service.classId == nullptr) {
            continue;
        }
        for (uint16_t classIndex = 0; classIndex < service.classIdNumber; classIndex++) {
            const BtUuid &classId = service.classId[classIndex];
            Uuid uuid;
            switch (classId.type) {
                case BT_UUID_16:
                    uuid = Uuid::From16Bit(classId.uuid16);
                    break;
                case BT_UUID_32:
                    uuid = Uuid::From32Bit(classId.uuid32);
                    break;
                case BT_UUID_128:
                    uuid = Uuid::From128BitBE(classId.uuid128);
                    break;
                default:
                    continue;
            }
            if (std::find(uuids.begin(), uuids.end(), uuid) == uuids.end()) {
                uuids.push_back(uuid);
            }
            if (uuids.size() >= SDP_MAX_REMOTE_UUIDS) {
                return uuids;
            }
        }
    }
    return uuids;
}

bool ContainsUuid16(const std::vector<Uuid> &uuids, uint16_t uuid16)
{
    Uuid target = Uuid::From16Bit(uuid16);
    return std::find(uuids.begin(), uuids.end(), target) != uuids.end();
}

std::vector<Uuid> FallbackUuids()
{
    if constexpr (bluetooth::btif::kSdpFallbackUuidEnable != 0) {
        return {Uuid::From16Bit(bluetooth::btif::kSdpFallbackUuid16)};
    }
    return {};
}

void DeleteQueryAlarm(QueryContext *context)
{
    if (context == nullptr) {
        return;
    }
    context->alarm.reset();
}

void FinishSdpQuery(QueryContext *context)
{
    if (context == nullptr) {
        return;
    }

    std::vector<Uuid> result;
    bool cacheUpdated = false;
    std::unique_ptr<QueryContext> holder;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        std::string key = CoreUtil::AddressKey(context->address);
        auto iter = ::CoreSdpDiscovery::GetInstance().GetState().queries.find(key);
        if (iter == ::CoreSdpDiscovery::GetInstance().GetState().queries.end() || iter->second.get() != context ||
            context->finished) {
            return;
        }
        context->finished = true;
        DeleteQueryAlarm(context);
        result = context->collected;
        holder = std::move(iter->second);
        ::CoreSdpDiscovery::GetInstance().GetState().queries.erase(iter);
        if (!result.empty()) {
            ::CoreSdpDiscovery::GetInstance().GetState().uuidCache[key] = result;
            cacheUpdated = true;
        } else {
            result = FallbackUuids();
        }
    }

    if (cacheUpdated) {
        SaveSdpStore();
    }
    if (!result.empty()) {
        QueueReportUuids(context->address, result);
    }
}

void OnSdpTimeout(void *parameter)
{
    QueryContext *context = static_cast<QueryContext *>(parameter);
    if (context == nullptr) {
        return;
    }
    HILOGW("sdp query timeout addr=%{public}s", context->address.ToString().c_str());
    FinishSdpQuery(context);
}

void StartSdpRequest(QueryContext *context, uint16_t uuid16);

void ContinueSdpQuery(QueryContext *context)
{
    if (context == nullptr) {
        return;
    }

    uint16_t uuid16 = 0;
    bool active = false;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        std::string key = CoreUtil::AddressKey(context->address);
        auto iter = ::CoreSdpDiscovery::GetInstance().GetState().queries.find(key);
        if (iter != ::CoreSdpDiscovery::GetInstance().GetState().queries.end() && iter->second.get() == context &&
            !context->finished) {
            // BrowseGroupList often contains only PnP/PublicBrowseRoot. Skip
            // profiles already returned by that query and query the rest.
            constexpr size_t profileCount = sizeof(SDP_PROFILE_QUERY_UUIDS) / sizeof(SDP_PROFILE_QUERY_UUIDS[0]);
            while (context->profileIndex < profileCount &&
                   ContainsUuid16(context->collected, SDP_PROFILE_QUERY_UUIDS[context->profileIndex])) {
                context->profileIndex++;
            }
            if (context->profileIndex < profileCount) {
                uuid16 = SDP_PROFILE_QUERY_UUIDS[context->profileIndex];
                active = true;
            }
        }
    }
    if (!active) {
        FinishSdpQuery(context);
        return;
    }
    StartSdpRequest(context, uuid16);
}

bool UpdateSdpQueryFromResult(
    QueryContext *queryContext, const SdpService *serviceArray, uint16_t serviceNum, bool &finish, bool &continueQuery)
{
    std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
    std::string key = CoreUtil::AddressKey(queryContext->address);
    auto iter = ::CoreSdpDiscovery::GetInstance().GetState().queries.find(key);
    if (iter == ::CoreSdpDiscovery::GetInstance().GetState().queries.end() || iter->second.get() != queryContext ||
        queryContext->finished) {
        HILOGW("sdp result for stale query addr=%{public}s", queryContext->address.ToString().c_str());
        return false;
    }

    DeleteQueryAlarm(queryContext);
    std::vector<Uuid> parsed = ParseServiceUuids(serviceArray, serviceNum);
    HILOGI(
        "sdp result serviceNum=%{public}d uuidCount=%{public}d addr=%{public}s", static_cast<int>(serviceNum),
        static_cast<int>(parsed.size()), queryContext->address.ToString().c_str());
    for (const Uuid &uuid : parsed) {
        if (std::find(queryContext->collected.begin(), queryContext->collected.end(), uuid) ==
            queryContext->collected.end()) {
            queryContext->collected.push_back(uuid);
        }
        if (queryContext->collected.size() >= SDP_MAX_REMOTE_UUIDS) {
            break;
        }
    }

    queryContext->profileIndex++;
    if (queryContext->profileIndex < sizeof(SDP_PROFILE_QUERY_UUIDS) / sizeof(SDP_PROFILE_QUERY_UUIDS[0]) &&
        queryContext->collected.size() < SDP_MAX_REMOTE_UUIDS) {
        continueQuery = true;
    } else {
        finish = true;
    }
    return true;
}

void OnSdpResult(const BtAddr *addr, const SdpService *serviceArray, uint16_t serviceNum, void *context)
{
    (void)addr;
    QueryContext *queryContext = static_cast<QueryContext *>(context);
    if (queryContext == nullptr) {
        return;
    }

    bool finish = false;
    bool continueQuery = false;
    if (!UpdateSdpQueryFromResult(queryContext, serviceArray, serviceNum, finish, continueQuery)) {
        return;
    }
    if (finish) {
        FinishSdpQuery(queryContext);
        return;
    }
    if (continueQuery) {
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [queryContext]() {
                ContinueSdpQuery(queryContext);
            })) {
            HILOGW("drop next sdp query: control lane unavailable");
            FinishSdpQuery(queryContext);
        }
    }
}

void StartSdpRequest(QueryContext *context, uint16_t uuid16)
{
    if (context == nullptr) {
        return;
    }

    BtAddr btAddr = CoreUtil::ToBtAddr(context->address);
    BtUuid serviceUuid = {};
    serviceUuid.type = BT_UUID_16;
    serviceUuid.uuid16 = uuid16;
    SdpUuid uuidArray = {};
    uuidArray.uuidNum = 1;
    uuidArray.uuid = &serviceUuid;
    SdpAttributeIdList attributeIdList = {};
    // The reference stack initializes the discovery database with an empty
    // attribute list. Requesting only 0x0001 makes some devices return an empty
    // AttributeLists even when the service record exists.
    attributeIdList.type = SDP_TYPE_RANGE;
    attributeIdList.attributeIdRange.start = 0x0000;
    attributeIdList.attributeIdRange.end = 0xFFFF;

    context->alarm = AlarmPtr(AlarmCreate("sdpUuid", false));
    if (context->alarm != nullptr) {
        (void)AlarmSet(context->alarm.get(), bluetooth::btif::kSdpQueryTimeoutMs, OnSdpTimeout, context);
    }

    HILOGI(
        "sdp query start addr=%{public}s uuid=0x%{public}04x id=%{public}u", context->address.ToString().c_str(),
        uuid16, context->id);
    int result = SDP_ServiceSearchAttribute(&btAddr, &uuidArray, attributeIdList, context, OnSdpResult);
    if (result != BT_SUCCESS) {
        HILOGE(
            "sdp query request failed addr=%{public}s result=%{public}d", context->address.ToString().c_str(), result);
        FinishSdpQuery(context);
    }
}

std::vector<Uuid> LookupCachedUuids(const std::string &key, bool forceRefresh)
{
    std::vector<Uuid> cached;
    std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
    if (!forceRefresh) {
        auto cacheIter = ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.find(key);
        if (cacheIter != ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.end()) {
            cached = cacheIter->second;
        }
    }
    return cached;
}

/* Returns nullptr when a query for the same device is already in flight. */
QueryContext *RegisterQuery(const RawAddress &address, const std::string &key)
{
    auto context = std::make_unique<QueryContext>();
    context->address = address;
    context->id = ::CoreSdpDiscovery::GetInstance().GetState().nextQueryId++;
    QueryContext *rawContext = context.get();
    std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
    if (::CoreSdpDiscovery::GetInstance().GetState().queries.find(key) !=
        ::CoreSdpDiscovery::GetInstance().GetState().queries.end()) {
        HILOGI("sdp query already in flight addr=%{public}s", address.ToString().c_str());
        return nullptr;
    }
    ::CoreSdpDiscovery::GetInstance().GetState().queries[key] = std::move(context);
    return rawContext;
}

}  // namespace

static void CoreSdpQueryRemoteUuids(const RawAddress &address, bool forceRefresh)
{
    std::string key = CoreUtil::AddressKey(address);
    std::vector<Uuid> cached = LookupCachedUuids(key, forceRefresh);
    if (!cached.empty()) {
        QueueReportUuids(address, cached);
        return;
    }

    QueryContext *context = RegisterQuery(address, key);
    if (context == nullptr) {
        return;
    }

    // Start directly with the concrete service UUID table. Relying on
    // PublicBrowseGroup first fails on devices that publish A2DP/HFP/HID but do
    // not publish BrowseGroupList.
    StartSdpRequest(context, SDP_PROFILE_QUERY_UUIDS[0]);
}

static void CoreSdpOnBonded(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    std::vector<Uuid> cached;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        auto iter = ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.find(key);
        if (iter != ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.end()) {
            cached = iter->second;
        }
    }
    if (!cached.empty()) {
        QueueReportUuids(address, cached);
        return;
    }
    CoreSdpQueryRemoteUuids(address, false);
}

static void CoreSdpOnUnpaired(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    bool removed = false;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        removed = ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.erase(key) > 0;
        auto iter = ::CoreSdpDiscovery::GetInstance().GetState().queries.find(key);
        if (iter != ::CoreSdpDiscovery::GetInstance().GetState().queries.end()) {
            iter->second->finished = true;
            DeleteQueryAlarm(iter->second.get());
            ::CoreSdpDiscovery::GetInstance().GetState().queries.erase(iter);
            removed = true;
        }
    }
    if (removed) {
        SaveSdpStore();
    }
}

static void CoreSdpLoadStore()
{
    LoadSdpStore();
}

static void CoreSdpReportCachedUuids()
{
    LoadSdpStore();
    std::vector<std::pair<RawAddress, std::vector<Uuid>>> cachedDevices;
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        for (const auto &entry : ::CoreSdpDiscovery::GetInstance().GetState().uuidCache) {
            if (entry.first.size() != RawAddress::kLength || entry.second.empty()) {
                continue;
            }
            RawAddress address = {};
            if (memcpy_s(address.address, sizeof(address.address), entry.first.data(), RawAddress::kLength) != EOK) {
                continue;
            }
            if (!::CorePairingManager::IsBonded(address)) {
                HILOGW("skip sdp cache for unpaired device");
                continue;
            }
            cachedDevices.emplace_back(address, entry.second);
        }
    }
    for (const auto &entry : cachedDevices) {
        QueueReportUuids(entry.first, entry.second, false);
    }
    HILOGI("sdp cached uuid replay count=%{public}d", static_cast<int>(cachedDevices.size()));
}

static void CoreSdpCacheCleanup()
{
    // Persist the current cache before clearing process memory. A service
    // restart must be able to replay UUIDs even when the peer is not reachable
    // for a fresh SDP query.
    if (::CoreSdpDiscovery::GetInstance().GetState().storeLoaded) {
        SaveSdpStore();
    }
    {
        std::lock_guard<std::mutex> lock(::CoreSdpDiscovery::GetInstance().GetState().mutex);
        ::CoreSdpDiscovery::GetInstance().GetState().uuidCache.clear();
        for (auto &entry : ::CoreSdpDiscovery::GetInstance().GetState().queries) {
            entry.second->finished = true;
            DeleteQueryAlarm(entry.second.get());
        }
        ::CoreSdpDiscovery::GetInstance().GetState().queries.clear();
    }
    ::CoreSdpDiscovery::GetInstance().GetState().storeLoaded = false;
}
void CoreSdpDiscovery::QueryRemoteUuids(const RawAddress &address, bool forceRefresh)
{
    CoreSdpQueryRemoteUuids(address, forceRefresh);
}
void CoreSdpDiscovery::OnBonded(const RawAddress &address)
{
    CoreSdpOnBonded(address);
}
void CoreSdpDiscovery::OnUnpaired(const RawAddress &address)
{
    CoreSdpOnUnpaired(address);
}
void CoreSdpDiscovery::LoadStore()
{
    CoreSdpLoadStore();
}
void CoreSdpDiscovery::ReportCachedUuids()
{
    CoreSdpReportCachedUuids();
}
void CoreSdpDiscovery::Cleanup()
{
    CoreSdpCacheCleanup();
}
