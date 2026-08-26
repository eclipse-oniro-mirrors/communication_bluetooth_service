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

#ifndef GAP_LE_IF_5_1_H
#define GAP_LE_IF_5_1_H

#include "gap_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/// BLE 5.1 Constant Tone Extension (CTE) types.
/// See Bluetooth 5.1, Vol 2, Part E, 7.8.78 (LE Receiver Test [v3]).
#define GAP_LE_CTE_TYPE_AOA (0x00)
#define GAP_LE_CTE_TYPE_AOD_1US (0x01)
#define GAP_LE_CTE_TYPE_AOD_2US (0x02)
#define GAP_LE_CTE_TYPE_NONE (0xFF)

/// CTE length in 8us units (0x02-0x14).
#define GAP_LE_CTE_LENGTH_MIN (0x02)
#define GAP_LE_CTE_LENGTH_MAX (0x14)

/// Number of CTEs to switch to when sampling (0x01-0x10).
#define GAP_LE_CTE_COUNT_MIN (0x01)
#define GAP_LE_CTE_COUNT_MAX (0x10)

/// Length of the antenna switching pattern (0x02-0x4B).
#define GAP_LE_SWITCHING_PATTERN_LENGTH_MIN (0x02)
#define GAP_LE_SWITCHING_PATTERN_LENGTH_MAX (0x4B)

/// CTE slot durations.
#define GAP_LE_CTE_SLOT_DURATIONS_1US (0x01)
#define GAP_LE_CTE_SLOT_DURATIONS_2US (0x02)

/// Modulation index for LE Receiver Test [v3].
#define GAP_LE_RECEIVER_TEST_MODULATION_INDEX_STANDARD (0x00)
#define GAP_LE_RECEIVER_TEST_MODULATION_INDEX_STABLE (0x01)

/// Periodic Advertising Sync Transfer (PAST) mode.
#define GAP_LE_PAST_MODE_NO_SYNC (0x00)
#define GAP_LE_PAST_MODE_SYNC_NO_REPORT (0x01)
#define GAP_LE_PAST_MODE_SYNC_REPORT (0x02)

/// Periodic Advertising Sync Transfer CTE type bit mask (bit 0 no AoA, bit 1 no AoD 1us,
/// bit 2 no AoD 2us, bit 4 no CTE). See Bluetooth 5.1, Vol 2, Part E, 7.8.91.
#define GAP_LE_PAST_CTE_TYPE_NO_AOA (0x01)
#define GAP_LE_PAST_CTE_TYPE_NO_AOD_1US (0x02)
#define GAP_LE_PAST_CTE_TYPE_NO_AOD_2US (0x04)
#define GAP_LE_PAST_CTE_TYPE_NO_CTE (0x10)
/// Combined mask of all valid CTE type bits, used to reject reserved bit patterns.
#define GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL \
    (GAP_LE_PAST_CTE_TYPE_NO_AOA | GAP_LE_PAST_CTE_TYPE_NO_AOD_1US | GAP_LE_PAST_CTE_TYPE_NO_AOD_2US | \
        GAP_LE_PAST_CTE_TYPE_NO_CTE)

/// Maximum number of IQ samples per report (Bluetooth 5.1, Vol 2, Part E, 7.7.65,21).
#define GAP_LE_IQ_SAMPLE_COUNT_MAX (0x52)

// Forward declarations for the BLE 5.1 event structures. The full definitions live in
// src/hci/hci_def_le_evt.h (stack internal); only pointers to these types appear in the
// public callback interface, so a tagged-struct forward declaration keeps the types
// visible here without exposing the stack internal header.
typedef struct HciLeConnectionlessIqReportEventParamTag HciLeConnectionlessIqReportEventParam;
typedef struct HciLeConnectionIqReportEventParamTag HciLeConnectionIqReportEventParam;
typedef struct HciLePeriodicAdvertisingSyncTransferReceivedEventParamTag
    HciLePeriodicAdvertisingSyncTransferReceivedEventParam;

/**
 * @brief       BLE 5.1 Constant Tone Extension (CTE) / direction finding callback structure
 *
 * Covers the commands and events introduced by Bluetooth 5.1 direction finding
 * (AoA/AoD) and Periodic Advertising Sync Transfer (PAST):
 *   - command result callbacks: one per HCI command 7.8.78-7.8.92 (except 7.8.93/94)
 *   - event callbacks: 7.7.65.21 Connectionless IQ Report, 7.7.65.22 Connection IQ
 *     Report, 7.7.65.23 CTE Request Failed, 7.7.65.24 PA Sync Transfer Received
 *
 * New fields may be added to the end of this structure in future releases.
 * Callers should use C99 designated initializers or explicit per-member assignment.
 * C++11/14/17 callers must use explicit per-member assignment because designated
 * initializers are only available from C++20 onward.
 * All callbacks in this structure are optional and may be set to NULL.
 * The implementation checks each pointer before invocation.
 *
 * For @a connectionlessIqReport and @a connectionIqReport, the I/Q samples are
 * exposed as a single interleaved (I, Q) pair array (@a iqSamples, 2 * sampleCount
 * entries: I0, Q0, I1, Q1, ...), matching the 7.7.65.21/7.7.65.22 wire format.
 * The array is only valid during the callback. Copy it if needed later.
 */
typedef struct {
    void (*receiverTestV3Result)(uint8_t status, void *context);
    void (*transmitterTestV3Result)(uint8_t status, void *context);
    void (*setConnectionlessCteTransmitParametersResult)(uint8_t status, void *context);
    void (*setConnectionlessCteTransmitEnableResult)(uint8_t status, void *context);
    void (*setConnectionlessIqSamplingEnableResult)(uint8_t status, void *context);
    void (*setConnectionCteReceiveParametersResult)(uint8_t status, void *context);
    void (*setConnectionCteTransmitParametersResult)(uint8_t status, void *context);
    void (*connectionCteRequestEnableResult)(uint8_t status, void *context);
    void (*connectionCteResponseEnableResult)(uint8_t status, void *context);
    void (*readAntennaInformationResult)(uint8_t status, uint8_t supportedSwitchingSamplingRates,
        uint8_t numberOfAntennae, uint8_t maxLengthOfSwitchingPattern, uint8_t maxCteLength, void *context);
    void (*setPeriodicAdvertisingReceiveEnableResult)(uint8_t status, void *context);
    void (*periodicAdvertisingSyncTransferResult)(uint8_t status, void *context);
    void (*periodicAdvertisingSetInfoTransferResult)(uint8_t status, void *context);
    void (*setPeriodicAdvertisingSyncTransferParametersResult)(uint8_t status, void *context);
    void (*setDefaultPeriodicAdvertisingSyncTransferParametersResult)(uint8_t status, void *context);
    void (*connectionlessIqReport)(
        const HciLeConnectionlessIqReportEventParam *eventParam, void *context);
    void (*connectionIqReport)(
        const HciLeConnectionIqReportEventParam *eventParam, void *context);
    void (*cteRequestFailed)(uint8_t status, uint16_t connectionHandle, void *context);
    void (*pastSyncTransferReceived)(
        const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *eventParam, void *context);
} GapLeCteCallback;

/**
 * @brief       Parameters of the LE Receiver Test [v3] command (Bluetooth 5.1, Vol 2, Part E, 7.8.78).
 */
typedef struct {
    uint8_t rxChannel;                  ///< rx channel (0x00-0x27)
    uint8_t phy;                        ///< PHY to be tested (1-3)
    uint8_t modulationIndex;            ///< modulation index (0 standard, 1 stable)
    uint8_t expectedCteLength;          ///< expected CTE length in 8us units (0 or 2-20)
    uint8_t expectedCteType;            ///< expected CTE type (see GAP_LE_CTE_TYPE_*)
    uint8_t slotDurations;              ///< CTE slot durations (1 or 2 us)
    uint8_t lengthOfSwitchingPattern;   ///< length of the antenna switching pattern (2-75)
    const uint8_t *antennaIds;          ///< antenna IDs (may be NULL when length is 0)
} GapLeReceiverTestV3Param;

/**
 * @brief       Parameters of the LE Transmitter Test [v3] command (Bluetooth 5.1, Vol 2, Part E, 7.8.79).
 */
typedef struct {
    uint8_t txChannel;                  ///< tx channel (0x00-0x27)
    uint8_t lengthOfTestData;           ///< length of test data (0-0xFF)
    uint8_t packetPayload;              ///< packet payload type (0-7)
    uint8_t phy;                        ///< PHY to be tested (1-4)
    uint8_t cteLength;                  ///< CTE length in 8us units (0-20)
    uint8_t cteType;                    ///< CTE type (see GAP_LE_CTE_TYPE_*)
    uint8_t lengthOfSwitchingPattern;   ///< length of the antenna switching pattern (2-75)
    const uint8_t *antennaIds;          ///< antenna IDs (may be NULL when length is 0)
} GapLeTransmitterTestV3Param;

/**
 * @brief       Parameters of the LE Set Connectionless CTE Transmit Parameters command (7.8.80).
 */
typedef struct {
    uint8_t advHandle;                  ///< advertising set handle (0x00-0xEF)
    uint8_t cteLength;                  ///< CTE length in 8us units (2-20)
    uint8_t cteType;                    ///< CTE type (see GAP_LE_CTE_TYPE_*)
    uint8_t cteCount;                   ///< number of CTEs between switches (1-16)
    uint8_t lengthOfSwitchingPattern;   ///< length of the antenna switching pattern (2-75)
    const uint8_t *antennaIds;          ///< antenna IDs (may be NULL when length is 0)
} GapLeSetConnectionlessCteTransmitParametersParam;

/**
 * @brief       Parameters of the LE Set Connectionless IQ Sampling Enable command (7.8.82).
 */
typedef struct {
    uint16_t syncHandle;                ///< sync handle (0x0000-0x0EFF, or 0x0FFF for receiver test)
    uint8_t samplingEnable;             ///< enable (0x01) or disable (0x00) IQ sampling
    uint8_t slotDurations;              ///< CTE slot durations (1 or 2 us)
    uint8_t maxSampledCtes;             ///< maximum number of CTEs to sample (1-16)
    uint8_t lengthOfSwitchingPattern;   ///< length of the antenna switching pattern (2-75)
    const uint8_t *antennaIds;          ///< antenna IDs (may be NULL when length is 0)
} GapLeSetConnectionlessIqSamplingEnableParam;

/**
 * @brief       Register LE 5.1 CTE (Constant Tone Extension) callback function.
 * @param[in]   callback            CTE callback structure
 * @param[in]   context             CTE callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_RegisterLeCteCallback(const GapLeCteCallback *callback, void *context);

/**
 * @brief       Deregister LE 5.1 CTE callback function.
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_DeregisterLeCteCallback(void);

/**
 * @brief       Start a receiver test with CTE (Bluetooth 5.1, Vol 2, Part E, 7.8.78).
 * @param[in]   param               test parameters (see GapLeReceiverTestV3Param)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeReceiverTestV3(const GapLeReceiverTestV3Param *param);

/**
 * @brief       Start a transmitter test with CTE (Bluetooth 5.1, Vol 2, Part E, 7.8.79).
 * @param[in]   param               test parameters (see GapLeTransmitterTestV3Param)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeTransmitterTestV3(const GapLeTransmitterTestV3Param *param);

/**
 * @brief       Set the CTE transmit parameters of an advertising set (7.8.80).
 * @param[in]   param               CTE transmit parameters (see GapLeSetConnectionlessCteTransmitParametersParam)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetConnectionlessCteTransmitParameters(
    const GapLeSetConnectionlessCteTransmitParametersParam *param);

/**
 * @brief       Enable or disable CTE transmission on an advertising set (7.8.81).
 * @param[in]   advHandle               advertising set handle (0x00-0xEF)
 * @param[in]   cteEnable               enable (0x01) or disable (0x00) CTE transmission
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetConnectionlessCteTransmitEnable(uint8_t advHandle, uint8_t cteEnable);

/**
 * @brief       Enable or disable IQ sampling on a synchronized periodic advertising train (7.8.82).
 * @param[in]   param               IQ sampling parameters (see GapLeSetConnectionlessIqSamplingEnableParam)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetConnectionlessIqSamplingEnable(const GapLeSetConnectionlessIqSamplingEnableParam *param);

/**
 * @brief       Set the CTE receive parameters of a connection (7.8.83).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   samplingEnable          enable (0x01) or disable (0x00) IQ sampling
 * @param[in]   slotDurations           CTE slot durations (1 or 2 us)
 * @param[in]   lengthOfSwitchingPattern length of the antenna switching pattern (2-75)
 * @param[in]   antennaIds              antenna IDs (may be NULL when length is 0)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetConnectionCteReceiveParameters(uint16_t connectionHandle, uint8_t samplingEnable,
    uint8_t slotDurations, uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds);

/**
 * @brief       Set the CTE transmit parameters of a connection (7.8.84).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   cteTypes                bit mask of CTE types to transmit (see GAP_LE_PAST_CTE_TYPE_*)
 * @param[in]   lengthOfSwitchingPattern length of the antenna switching pattern (2-75)
 * @param[in]   antennaIds              antenna IDs (may be NULL when length is 0)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetConnectionCteTransmitParameters(uint16_t connectionHandle, uint8_t cteTypes,
    uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds);

/**
 * @brief       Enable or disable CTE requests on a connection (7.8.85).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   enable                  enable (0x01) or disable (0x00) CTE requests
 * @param[in]   cteRequestInterval      CTE request interval in events
 * @param[in]   requestedCteLength      requested CTE length in 8us units (0x02-0x14)
 * @param[in]   requestedCteType        requested CTE type (see GAP_LE_CTE_TYPE_*)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeConnectionCteRequestEnable(
    uint16_t connectionHandle, uint8_t enable, uint16_t cteRequestInterval, uint8_t requestedCteLength,
    uint8_t requestedCteType);

/**
 * @brief       Enable or disable CTE responses on a connection (7.8.86).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   enable                  enable (0x01) or disable (0x00) CTE responses
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeConnectionCteResponseEnable(uint16_t connectionHandle, uint8_t enable);

/**
 * @brief       Read the antenna information of the local controller (7.8.87).
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeReadAntennaInformation(void);

/**
 * @brief       Enable or disable reports of a synchronized periodic advertising train (7.8.88).
 * @param[in]   syncHandle              sync handle (0x0000-0x0EFF)
 * @param[in]   enable                  enable (0x01) or disable (0x00) reports
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetPeriodicAdvertisingReceiveEnable(uint16_t syncHandle, uint8_t enable);

/**
 * @brief       Transfer a synchronized periodic advertising train to a connected device (7.8.89).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   serviceData             service data (0x0000-0xFFFF)
 * @param[in]   syncHandle              sync handle of the train to transfer (0x0000-0x0EFF)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LePeriodicAdvertisingSyncTransfer(
    uint16_t connectionHandle, uint16_t serviceData, uint16_t syncHandle);

/**
 * @brief       Transfer the periodic advertising set information to a connected device (7.8.90).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   serviceData             service data (0x0000-0xFFFF)
 * @param[in]   advertisingHandle       advertising set handle (0x00-0xEF)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LePeriodicAdvertisingSetInfoTransfer(
    uint16_t connectionHandle, uint16_t serviceData, uint8_t advertisingHandle);

/**
 * @brief       Set the PA sync transfer parameters of a connection (7.8.91).
 * @param[in]   connectionHandle        connection handle (0x0000-0x0EFF)
 * @param[in]   mode                    sync transfer mode (see GAP_LE_PAST_MODE_*)
 * @param[in]   skip                    number of packets that may be skipped (0x0000-0x01F3)
 * @param[in]   syncTimeout             synchronization timeout (0x000A-0x4000)
 * @param[in]   cteType                 CTE type bit mask (see GAP_LE_PAST_CTE_TYPE_*)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
    uint16_t connectionHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType);

/**
 * @brief       Set the default PA sync transfer parameters (7.8.92).
 * @param[in]   mode                    sync transfer mode (see GAP_LE_PAST_MODE_*)
 * @param[in]   skip                    number of packets that may be skipped (0x0000-0x01F3)
 * @param[in]   syncTimeout             synchronization timeout (0x000A-0x4000)
 * @param[in]   cteType                 CTE type bit mask (see GAP_LE_PAST_CTE_TYPE_*)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(
    uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType);

#ifdef __cplusplus
}
#endif

#endif /* GAP_LE_IF_5_1_H */
