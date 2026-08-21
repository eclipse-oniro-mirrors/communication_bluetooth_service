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

#include "hci_evt_le.h"

#include <stddef.h>
#include <securec.h>

#include "btstack.h"
#include "platform/include/allocator.h"
#include "platform/include/list.h"
#include "platform/include/mutex.h"

#include "hci/acl/hci_acl.h"
#include "hci/hci.h"
#include "hci/hci_def.h"
#include "hci/hci_error.h"

#include "hci_evt.h"
#include "log.h"

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E, 7.7.65.2
// A legacy LE Advertising Report data field may contain at most 31 octets.
#define HCI_LE_ADV_REPORT_DATA_LEN_MAX 31

#define BYTE_BIT_WIDTH 8

/* Event parameters (and any nested data pointer) point into the HCI packet buffer
 * owned by the caller. Callbacks must copy any data they need to retain. */
typedef void (*HciLeEventFunc)(const uint8_t *param, size_t length);

static void HciEventOnLeConnectionCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeConnectionCompleteEventParam))) {
        return;
    }

    HciLeConnectionCompleteEventParam *eventParam = (HciLeConnectionCompleteEventParam *)param;

    if (eventParam->status == HCI_SUCCESS) {
        HciAclOnConnectionComplete(eventParam->connectionHandle, TRANSPORT_LE_STACK);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionComplete != NULL) {
        callbacks->leConnectionComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static bool HciParseLeAdvertisingReport(
    HciLeAdvertisingReport *report, const uint8_t *param, size_t length, size_t *offset)
{
    report->eventType = param[*offset];
    *offset += sizeof(uint8_t);

    report->addressType = param[*offset];
    *offset += sizeof(uint8_t);

    if (memcpy_s(report->address.raw, BT_ADDRESS_SIZE, param + *offset, BT_ADDRESS_SIZE) != EOK) {
        return false;
    }
    *offset += BT_ADDRESS_SIZE;

    report->lengthData = param[*offset];
    *offset += sizeof(uint8_t);

    // Legacy advertising reports cannot carry more than 31 bytes of data.
    if (report->lengthData > HCI_LE_ADV_REPORT_DATA_LEN_MAX) {
        LOG_ERROR("%{public}s: invalid data length, offset(%{public}zu), lengthData(%{public}u), max(%{public}u)",
            __FUNCTION__,
            *offset,
            report->lengthData,
            HCI_LE_ADV_REPORT_DATA_LEN_MAX);
        return false;
    }

    if (*offset + report->lengthData + sizeof(uint8_t) > length) {
        LOG_ERROR("%{public}s: truncated data field, offset(%{public}zu), lengthData(%{public}u), "
                  "length(%{public}zu)",
            __FUNCTION__,
            *offset,
            report->lengthData,
            length);
        return false;
    }
    // Zero-copy view into the event packet buffer. The data is read-only
    // and its lifetime is limited to the dispatch of this event: consumers
    // (e.g. GapRecvLeAdvertisingReportEvent) must copy it synchronously in
    // the callback and must not retain or modify it.
    report->data = (uint8_t *)(param + *offset);
    *offset += report->lengthData;

    report->rssi = (int8_t)param[*offset];
    *offset += sizeof(uint8_t);

    return true;
}

static void HciEventOnLeAdvertisingReportEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || length <= 1) {
        return;
    }

    size_t offset = 0;
    HciLeAdvertisingReportEventParam eventParam = {
        .numReports = param[offset],
        .reports = NULL,
    };
    offset += sizeof(uint8_t);

    if (eventParam.numReports == 0) {
        return;
    }

    // Minimum report size: eventType(1) + addressType(1) + address(6) + lengthData(1) + rssi(1).
    const size_t minReportSize = sizeof(uint8_t) + sizeof(uint8_t) + BT_ADDRESS_SIZE + sizeof(uint8_t) +
                                 sizeof(uint8_t);
    if (length < offset + eventParam.numReports * minReportSize) {
        LOG_ERROR("%{public}s: truncated event, length(%{public}zu)", __FUNCTION__, length);
        return;
    }

    HciLeAdvertisingReport *reports = MEM_MALLOC.alloc(sizeof(HciLeAdvertisingReport) * eventParam.numReports);
    if (reports == NULL) {
        return;
    }

    for (uint8_t i = 0; i < eventParam.numReports; i++) {
        if (!HciParseLeAdvertisingReport(reports + i, param, length, &offset)) {
            LOG_ERROR("%{public}s: failed to parse report %{public}u", __FUNCTION__, i);
            MEM_MALLOC.free(reports);
            return;
        }
    }

    eventParam.reports = reports;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAdvertisingReport != NULL) {
        callbacks->leAdvertisingReport(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;

    MEM_MALLOC.free(reports);
}

static void HciEventOnLeConnectionUpdateCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeConnectionUpdateCompleteEventParam))) {
        return;
    }

    HciLeConnectionUpdateCompleteEventParam *eventParam = (HciLeConnectionUpdateCompleteEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionUpdateComplete != NULL) {
        callbacks->leConnectionUpdateComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadRemoteFeaturesCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeReadRemoteFeaturesCompleteEventParam))) {
        return;
    }

    HciLeReadRemoteFeaturesCompleteEventParam *eventParam = (HciLeReadRemoteFeaturesCompleteEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadRemoteFeaturesComplete != NULL) {
        callbacks->leReadRemoteFeaturesComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeLongTermKeyRequestEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeLongTermKeyRequestEventParam))) {
        return;
    }

    HciLeLongTermKeyRequestEventParam *eventParam = (HciLeLongTermKeyRequestEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leLongTermKeyRequest != NULL) {
        callbacks->leLongTermKeyRequest(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoteConnectionParameterRequestEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeRemoteConnectionParameterRequestEventParam))) {
        return;
    }

    HciLeRemoteConnectionParameterRequestEventParam *eventParam =
        (HciLeRemoteConnectionParameterRequestEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoteConnectionParameterRequest != NULL) {
        callbacks->leRemoteConnectionParameterRequest(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadLocalP256PublicKeyCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeReadLocalP256PublicKeyCompleteEventParam))) {
        return;
    }

    HciLeReadLocalP256PublicKeyCompleteEventParam *eventParam = (HciLeReadLocalP256PublicKeyCompleteEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalP256PublicKeyComplete != NULL) {
        callbacks->leReadLocalP256PublicKeyComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeGenerateDHKeyCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeGenerateDHKeyCompleteEventParam))) {
        return;
    }

    HciLeGenerateDHKeyCompleteEventParam *eventParam = (HciLeGenerateDHKeyCompleteEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leGenerateDHKeyComplete != NULL) {
        callbacks->leGenerateDHKeyComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEnhancedConnectionCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeEnhancedConnectionCompleteEventParam))) {
        return;
    }

    HciLeEnhancedConnectionCompleteEventParam *eventParam = (HciLeEnhancedConnectionCompleteEventParam *)param;

    if (eventParam->status == HCI_SUCCESS) {
        HciAclOnConnectionComplete(eventParam->connectionHandle, TRANSPORT_LE_STACK);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedConnectionComplete != NULL) {
        callbacks->leEnhancedConnectionComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static bool HciParseLeDirectedAdvertisingReport(
    HciLeDirectedAdvertisingReport *report, const uint8_t *param, size_t length, size_t *offset)
{
    if (*offset + sizeof(uint8_t) > length) {
        return false;
    }
    report->eventType = param[*offset];
    *offset += sizeof(uint8_t);

    if (*offset + sizeof(uint8_t) > length) {
        return false;
    }
    report->addressType = param[*offset];
    *offset += sizeof(uint8_t);

    if (*offset + BT_ADDRESS_SIZE > length) {
        return false;
    }
    if (memcpy_s(report->address.raw, BT_ADDRESS_SIZE, param + *offset, BT_ADDRESS_SIZE) != EOK) {
        return false;
    }
    *offset += BT_ADDRESS_SIZE;

    if (*offset + sizeof(uint8_t) > length) {
        return false;
    }
    report->directAddressType = param[*offset];
    *offset += sizeof(uint8_t);

    if (*offset + BT_ADDRESS_SIZE > length) {
        return false;
    }
    if (memcpy_s(report->directAddress.raw, BT_ADDRESS_SIZE, param + *offset, BT_ADDRESS_SIZE) != EOK) {
        return false;
    }
    *offset += BT_ADDRESS_SIZE;

    if (*offset + sizeof(uint8_t) > length) {
        return false;
    }
    report->rssi = (int8_t)param[*offset];
    *offset += sizeof(uint8_t);

    return true;
}

static void HciEventOnLeDirectedAdvertisingReportCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || length <= 1) {
        return;
    }

    size_t offset = 0;
    HciLeDirectedAdvertisingReportEventParam eventParam = {
        .numReports = param[offset],
        .reports = NULL,
    };
    offset += sizeof(uint8_t);

    if (eventParam.numReports == 0) {
        return;
    }

    // Minimum report size: eventType(1) + addressType(1) + address(6) + directAddressType(1) +
    // directAddress(6) + rssi(1).
    const size_t minReportSize = sizeof(uint8_t) + sizeof(uint8_t) + BT_ADDRESS_SIZE + sizeof(uint8_t) +
                                 BT_ADDRESS_SIZE + sizeof(uint8_t);
    if (length < offset + eventParam.numReports * minReportSize) {
        LOG_ERROR("%{public}s: truncated event, length(%{public}zu)", __FUNCTION__, length);
        return;
    }

    HciLeDirectedAdvertisingReport *reports =
        MEM_MALLOC.alloc(sizeof(HciLeDirectedAdvertisingReport) * eventParam.numReports);
    if (reports == NULL) {
        return;
    }

    for (uint8_t i = 0; i < eventParam.numReports; i++) {
        if (!HciParseLeDirectedAdvertisingReport(reports + i, param, length, &offset)) {
            LOG_ERROR("%{public}s: failed to parse report %{public}u", __FUNCTION__, i);
            MEM_MALLOC.free(reports);
            return;
        }
    }

    eventParam.reports = reports;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leDirectedAdvertisingReport != NULL) {
        callbacks->leDirectedAdvertisingReport(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;

    MEM_MALLOC.free(reports);
}

static void HciEventOnLePHYUpdateCompleteEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLePhyUpdateCompleteEventParam))) {
        return;
    }

    HciLePhyUpdateCompleteEventParam *eventParam = (HciLePhyUpdateCompleteEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePhyUpdateComplete != NULL) {
        callbacks->lePhyUpdateComplete(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static bool HciParseLeExtendedAdvertisingReport(
    HciLeExtendedAdvertisingReport *report, const uint8_t *param, size_t length, size_t *offset)
{
    const size_t extAdvCommonReportSize = 24;  // A extended advertising report size except 'Data' field.
    if (*offset + extAdvCommonReportSize > length) {
        LOG_ERROR("%{public}s: Error length, offset(%{public}zu), length(%{public}zu)",
            __FUNCTION__, *offset, length);
        return false;
    }

    report->eventType = param[*offset] | ((uint16_t)param[*offset + 1] << BYTE_BIT_WIDTH);
    *offset += sizeof(uint16_t);

    report->addressType = param[*offset];
    *offset += sizeof(uint8_t);

    if (memcpy_s(report->address.raw, BT_ADDRESS_SIZE, param + *offset, BT_ADDRESS_SIZE) != EOK) {
        return false;
    }
    *offset += BT_ADDRESS_SIZE;

    report->primaryPHY = param[*offset];
    *offset += sizeof(uint8_t);

    report->secondaryPHY = param[*offset];
    *offset += sizeof(uint8_t);

    report->advertisingSID = param[*offset];
    *offset += sizeof(uint8_t);

    report->txPower = (int8_t)param[*offset];
    *offset += sizeof(uint8_t);

    report->rssi = (int8_t)param[*offset];
    *offset += sizeof(uint8_t);

    report->periodicAdvertisingInterval = param[*offset] | ((uint16_t)param[*offset + 1] << BYTE_BIT_WIDTH);
    *offset += sizeof(uint16_t);

    report->directAddressType = param[*offset];
    *offset += sizeof(uint8_t);

    if (memcpy_s(report->directAddress.raw, BT_ADDRESS_SIZE, param + *offset, BT_ADDRESS_SIZE) != EOK) {
        return false;
    }
    *offset += BT_ADDRESS_SIZE;

    report->dataLength = param[*offset];
    *offset += sizeof(uint8_t);

    if (*offset + report->dataLength > length) {
        LOG_ERROR("%{public}s: Error data length, offset(%{public}zu), dataLength(%{public}u), "
        "length(%{public}zu)", __FUNCTION__, *offset, report->dataLength, length);
        return false;
    }
    report->data = (uint8_t *)(param + *offset);
    *offset += report->dataLength;
    return true;
}

static void HciEventOnLeExtendedAdvertisingReportEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || length <= 1) {
        return;
    }

    size_t offset = 0;
    HciLeExtendedAdvertisingReportEventParam eventParam = {
        .numReports = param[offset],
        .reports = NULL,
    };
    offset += sizeof(uint8_t);

    if (eventParam.numReports == 0) {
        return;
    }

    // Each report has at least a 24-byte common header before the variable data payload.
    const size_t extAdvCommonReportSize = 24;
    if (length < offset + eventParam.numReports * extAdvCommonReportSize) {
        LOG_ERROR("%{public}s: truncated event, length(%{public}zu)", __FUNCTION__, length);
        return;
    }

    HciLeExtendedAdvertisingReport *reports =
        MEM_MALLOC.alloc(sizeof(HciLeExtendedAdvertisingReport) * eventParam.numReports);
    if (reports != NULL) {
        for (uint8_t i = 0; i < eventParam.numReports; i++) {
            bool ret = HciParseLeExtendedAdvertisingReport(reports + i, param, length, &offset);
            if (!ret) {
                LOG_ERROR("HciParseLeExtendedAdvertisingReport failed");
                MEM_MALLOC.free(reports);
                return;
            }
        }

        eventParam.reports = reports;
    } else {
        return;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leExtendedAdvertisingReport != NULL) {
        callbacks->leExtendedAdvertisingReport(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;

    MEM_MALLOC.free(reports);
}

static void HciEventOnLeChannelSelectionAlgorithmEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeChannelSelectionAlgorithmEventParam))) {
        return;
    }

    HciLeChannelSelectionAlgorithmEventParam *eventParam = (HciLeChannelSelectionAlgorithmEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leChannelSelectionAlgorithm != NULL) {
        callbacks->leChannelSelectionAlgorithm(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeScanTimeoutEvent(const uint8_t *param, size_t length)
{
    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leScanTimeoutComplete != NULL) {
        callbacks->leScanTimeoutComplete();
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeAdvertisingSetTerminatedEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeAdvertisingSetTerminatedEventParam))) {
        return;
    }

    HciLeAdvertisingSetTerminatedEventParam *eventParam = (HciLeAdvertisingSetTerminatedEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAdvertisingSetTerminated != NULL) {
        callbacks->leAdvertisingSetTerminated(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeScanRequestReceivedEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeScanRequestReceivedEventParam))) {
        return;
    }

    HciLeScanRequestReceivedEventParam *eventParam = (HciLeScanRequestReceivedEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leScanRequestReceived != NULL) {
        callbacks->leScanRequestReceived(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLEDataLengthChangeEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLeDataLengthChangeEventParam))) {
        return;
    }

    HciLeDataLengthChangeEventParam *eventParam = (HciLeDataLengthChangeEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leDataLengthChange != NULL) {
        callbacks->leDataLengthChange(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLEPeriodicAdvertisingSyncEstablishedEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLePeriodicAdvertisingSyncEstablishedEventParam))) {
        return;
    }

    HciLePeriodicAdvertisingSyncEstablishedEventParam *eventParam =
        (HciLePeriodicAdvertisingSyncEstablishedEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSyncEstablished != NULL) {
        callbacks->lePeriodicAdvertisingSyncEstablished(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static int HciEvtLeReadUint8(const uint8_t *param, size_t length, size_t *offset, uint8_t *dst)
{
    if (*offset + sizeof(uint8_t) > length) {
        LOG_ERROR("%{public}s: truncated UINT8 at offset %{public}zu", __FUNCTION__, *offset);
        return -1;
    }
    *dst = param[*offset];
    (*offset) += sizeof(uint8_t);
    return 0;
}

static int HciEvtLeReadUint16Le(const uint8_t *param, size_t length, size_t *offset, uint16_t *dst)
{
    if (*offset + sizeof(uint16_t) > length) {
        LOG_ERROR("%{public}s: truncated UINT16 at offset %{public}zu", __FUNCTION__, *offset);
        return -1;
    }
    *dst = (uint16_t)param[*offset] | ((uint16_t)param[*offset + 1] << BYTE_BIT_WIDTH);
    (*offset) += sizeof(uint16_t);
    return 0;
}

static void HciEventOnLEPeriodicAdvertisingReportEvent(const uint8_t *param, size_t length)
{
    // Core Spec 5.0: the fixed-size wire header is
    // Sync_Handle(2) + TX_Power(1) + RSSI(1) + Data_Status(1) + Data_Length(1),
    // i.e. 6 bytes. The CTE_Type byte added in 5.1 must not be parsed here,
    // otherwise dataStatus/dataLength/data shift by one byte on 5.0 controllers.
    // Only the fixed-size header is parsed byte-by-byte from the HCI event; the
    // trailing data payload is referenced through the separate |data| pointer
    // and is valid only for the duration of this callback.
    const size_t fixedLength = sizeof(uint16_t) + (sizeof(uint8_t) * 4);
    if (param == NULL || length < fixedLength) {
        return;
    }

    HciLePeriodicAdvertisingReportEventParam eventParam = {0};
    size_t offset = 0;

    if (HciEvtLeReadUint16Le(param, length, &offset, &eventParam.syncHandle) != 0) {
        return;
    }
    if (HciEvtLeReadUint8(param, length, &offset, (uint8_t *)&eventParam.txPower) != 0) {
        return;
    }
    if (HciEvtLeReadUint8(param, length, &offset, (uint8_t *)&eventParam.rssi) != 0) {
        return;
    }
    if (HciEvtLeReadUint8(param, length, &offset, &eventParam.dataStatus) != 0) {
        return;
    }
    if (HciEvtLeReadUint8(param, length, &offset, &eventParam.dataLength) != 0) {
        return;
    }

    if (eventParam.dataLength > length - offset) {
        LOG_ERROR("%{public}s: malformed length, got %{public}zu, expected at least %{public}zu",
                  __FUNCTION__, length, offset + eventParam.dataLength);
        return;
    }
    // eventParam is stack-local and data points into the raw HCI packet buffer.
    // Both are only valid for the duration of this callback; copy data if it is needed later.
    eventParam.data = (eventParam.dataLength > 0) ? (param + offset) : NULL;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingReport != NULL) {
        callbacks->lePeriodicAdvertisingReport(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLEPeriodicAdvertisingSyncLostEvent(const uint8_t *param, size_t length)
{
    if (param == NULL || (length < sizeof(HciLePeriodicAdvertisingSyncLostEventParam))) {
        return;
    }

    HciLePeriodicAdvertisingSyncLostEventParam *eventParam = (HciLePeriodicAdvertisingSyncLostEventParam *)param;

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSyncLost != NULL) {
        callbacks->lePeriodicAdvertisingSyncLost(eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static HciLeEventFunc g_leEventMap[] = {
    NULL,                                                 // 0x00
    HciEventOnLeConnectionCompleteEvent,                  // 0x01
    HciEventOnLeAdvertisingReportEvent,                   // 0x02
    HciEventOnLeConnectionUpdateCompleteEvent,            // 0x03
    HciEventOnLeReadRemoteFeaturesCompleteEvent,          // 0x04
    HciEventOnLeLongTermKeyRequestEvent,                  // 0x05
    HciEventOnLeRemoteConnectionParameterRequestEvent,    // 0x06
    HciEventOnLEDataLengthChangeEvent,                    // 0x07
    HciEventOnLeReadLocalP256PublicKeyCompleteEvent,      // 0x08
    HciEventOnLeGenerateDHKeyCompleteEvent,               // 0x09
    HciEventOnLeEnhancedConnectionCompleteEvent,          // 0x0A
    HciEventOnLeDirectedAdvertisingReportCompleteEvent,   // 0x0B
    HciEventOnLePHYUpdateCompleteEvent,                   // 0x0C
    HciEventOnLeExtendedAdvertisingReportEvent,           // 0x0D
    HciEventOnLEPeriodicAdvertisingSyncEstablishedEvent,  // 0x0E
    HciEventOnLEPeriodicAdvertisingReportEvent,           // 0x0F
    HciEventOnLEPeriodicAdvertisingSyncLostEvent,         // 0x10
    HciEventOnLeScanTimeoutEvent,                         // 0x11
    HciEventOnLeAdvertisingSetTerminatedEvent,            // 0x12
    HciEventOnLeScanRequestReceivedEvent,                 // 0x13
    HciEventOnLeChannelSelectionAlgorithmEvent,           // 0x14
};

#define LESUBEVENTCODE_MAX 0x14

void HciEventOnLeMetaEvent(Packet *packet)
{
    Buffer *payloadBuffer = PacketContinuousPayload(packet);
    if (payloadBuffer == NULL) {
        return;
    }
    uint8_t *buf = (uint8_t *)BufferPtr(payloadBuffer);
    if (buf == NULL) {
        return;
    }
    size_t length = BufferGetSize(payloadBuffer);
    if (length < 1) {
        return;
    }

    if (buf[0] > LESUBEVENTCODE_MAX) {
        return;
    }

    HciLeEventFunc func = g_leEventMap[buf[0]];
    if (func != NULL) {
        if (length > 1) {
            func(buf + 1, length - 1);
        } else {
            func(NULL, 0);
        }
    }
}
