/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

/**
 * @addtogroup Bluetooth
 * @{
 *
 * @brief Generic Access Profile
 *
 */

/**
 * @file gap_le_if_5_4.h
 *
 * @brief bluetooth gap interface for the Bluetooth 5.4 additions
 *
 */

#ifndef GAP_LE_IF_5_4_H
#define GAP_LE_IF_5_4_H

// The [v1] types this header embeds (GapLeExAdvParam inside GapExAdvParamV2)
// are defined in gap_le_if.h, so it is included here to make this header
// self-contained. gap_le_if.h includes this file at its tail (after every
// type this file depends on); the include guards turn the mutual include
// into a no-op in either include order.
#include "gap_comm.h"
#include "gap_le_if.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Extended advertising set parameters of HCI_LE_Set_Extended_Advertising_
/// Parameters [v2] (Core Spec 5.4 Vol 4 Part E 7.8.53): the [v1] entry
/// parameter set plus the two Advertising Coding Selection (ACS) options.
typedef struct {
    uint8_t advHandle;   /// Used to identify an advertising set (0x00-0xEF)
    uint8_t properties;  /// Extended advertising event properties
    int8_t txPower;      /// Advertising TX power
    GapLeExAdvParam advExParam;  /// [v1] extended advertising parameters
    /// Primary_Advertising_PHY_Options (7.8.53): 0x00 no preference/requirement,
    /// 0x01 prefer S=2, 0x02 prefer S=8, 0x03 require S=2, 0x04 require S=8.
    /// Meaningful only when advExParam.primaryAdvPhy is LE Coded.
    uint8_t primaryPhyOptions;
    /// Secondary_Advertising_PHY_Options, same values, meaningful only when
    /// advExParam.secondaryAdvPhy is LE Coded (also applies to periodic
    /// advertising started from this set).
    uint8_t secondaryPhyOptions;
} GapExAdvParamV2;

/**
 * @brief Set extended advertising parameters with Advertising Coding Selection
 *        (ACS) coding options (HCI_LE_Set_Extended_Advertising_Parameters [v2],
 *        Core Spec 5.4 Vol 4 Part E 7.8.53).
 *
 * The [v1] flow (GAPIF_LeExAdvSetParam) keeps the controller-chosen coding;
 * this entry additionally lets the Host express a preference or requirement
 * for the S=2/S=8 coding of the LE Coded PHY advertisements.
 *
 * Command selection inside the GAP layer keys only on the Advertising Coding
 * Selection Controller support (LL feature bit 40):
 *  - bit 40 set  - the [v2] HCI command (OCF 0x007F) is always used, even when
 *                  both options are 0x00;
 *  - bit 40 clear - the [v1] command is used and the options are ignored
 *                  (identical to pre-5.4 behavior, so the entry also works on
 *                  pre-5.4 Controllers).
 * Bit 41 (ACS Host Support) is intentionally NOT a command-selection condition:
 * it only governs the receive path (the Controller then reports the exact
 * coding used on Coded-PHY advertisements, 7.7.65.13 - see the report params
 * of the extended scan callback).
 *
 * Options naming a non-LE-Coded PHY are ignored by the Controller (7.8.53) -
 * not an error. A "require" option (0x03/0x04) that the Controller cannot
 * satisfy returns Command Disallowed (0x0C) in the set-param result callback.
 * A [v2] command that still reaches a Controller without the feature is
 * answered Unsupported Feature or Parameter Value (0x11) - the GAP gate above
 * prevents that combination; the HCI layer keeps the pass-through semantics
 * for direct callers and PTS coverage.
 *
 * @param[in] param  Extended advertising set parameters incl. the coding
 *                   options; option values 0x05-0xFF are reserved and rejected.
 * @return @c BT_SUCCESS          : The command was sent to the Controller; the
 *                                outcome (incl. a Controller "Command
 *                                Disallowed" when a required coding cannot be
 *                                met) is reported asynchronously through the
 *                                exAdvSetParamResult callback.
 *         @c BT_BAD_PARAM        : @c param is NULL.
 *         @c BT_NO_MEMORY        : No memory for the internal request context.
 *         @c GAP_ERR_NOT_ENABLE  : LE is not enabled.
 *         @c GAP_ERR_INVAL_STATE : The Broadcaster role is not enabled.
 *         @c GAP_ERR_INVAL_PARAM : Parameter is invalid (incl. an option value
 *                                above 0x04, which is reserved).
 *         @c otherwise           : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LeExAdvSetParamV2(const GapExAdvParamV2 *param);

/// Data of one subevent of a PAwR train (HCI_LE_Set_Periodic_Advertising_
/// Subevent_Data, Core Spec 5.4 Vol 4 Part E 7.8.125): the payload to
/// transmit once in the subevent plus the response-slot window it opens.
/// The spec wire length limit is 251 bytes (252-0xFF reserved), but the
/// payload of one subevent is transmitted in a single HCI command whose
/// one-octet parameter length field (hci_cmd.c) leaves at most 249 octets
/// for it, so 250/251 are rejected by GAPIF_LePawrSetSubeventData.
typedef struct {
    /// First response slot the subevent's receivers may use (0x00-0xFF).
    uint8_t responseSlotStart;
    /// Number of response slots usable in this subevent (0x00-0xFF).
    uint8_t responseSlotCount;
    /// Length of @c data (0-249; see the frame-limit note above; higher
    /// values are rejected).
    uint16_t dataLength;
    /// Payload to transmit once in the subevent. Ignored when @c dataLength
    /// is 0; must not be NULL when @c dataLength is non-zero. The contents
    /// are copied by the GAP layer - the array is read during the call only.
    const uint8_t *data;
} GapPawrSubeventData;

/// Maximum number of responses a single PAwR response report can carry
/// (7.7.65,37, Num_Responses: 0x00-0x19).
#define GAP_PAWR_RESPONSE_REPORT_NUM_MAX 0x19
/// Tx_Status of a PAwR response report (7.7.65,37).
#define GAP_PAWR_TX_STATUS_TRANSMITTED 0x00
#define GAP_PAWR_TX_STATUS_NOT_TRANSMITTED 0x01
/// Data_Status of one response record (7.7.65,37).
#define GAP_PAWR_RESPONSE_DATA_STATUS_COMPLETE 0x00
#define GAP_PAWR_RESPONSE_DATA_STATUS_INCOMPLETE 0x01
#define GAP_PAWR_RESPONSE_DATA_STATUS_FAILED 0xFF

/// One response received on a response slot of a PAwR train (one record of
/// the LE Periodic Advertising Response Report event, 7.7.65,37).
typedef struct {
    /// TX power of the received PDU in dBm: -127 to +20; 0x7F = not available.
    int8_t txPower;
    /// RSSI of the received PDU in dBm: -127 to +20; 0x7F = not available.
    int8_t rssi;
    /// CTE type of the received PDU: 0x00 AoA, 0x01 AoD 1 us, 0x02 AoD 2 us,
    /// 0xFF no CTE.
    uint8_t cteType;
    /// Response slot the data was received in (0x00-0xFF).
    uint8_t responseSlot;
    /// @c GAP_PAWR_RESPONSE_DATA_STATUS_COMPLETE / @c _INCOMPLETE / @c _FAILED.
    uint8_t dataStatus;
    /// Length of @c data (0x00-0xFF).
    uint8_t dataLength;
    /// Received data; valid for the duration of the callback only.
    const uint8_t *data;
} GapPawrResponseReportRecord;

/// LE Periodic Advertising Response Report event (7.7.65,37) delivered
/// through the PAwR callback: the responses received on the response slots of
/// a PAwR advertising train. @c numResponses (0x00-0x19) records are filled
/// in @c response in received order.
typedef struct {
    /// Advertising handle of the PAwR train (0x00-0xEF).
    uint8_t advertisingHandle;
    /// Subevent the responses were received in (0x00-0x7F).
    uint8_t subevent;
    /// @c GAP_PAWR_TX_STATUS_TRANSMITTED when the Controller transmitted the
    /// AUX_SYNC_SUBEVENT_IND PDU, @c GAP_PAWR_TX_STATUS_NOT_TRANSMITTED when
    /// it did not.
    uint8_t txStatus;
    /// Number of valid entries in @c response.
    uint8_t numResponses;
    /// Response records; only entries below @c numResponses are valid.
    GapPawrResponseReportRecord response[GAP_PAWR_RESPONSE_REPORT_NUM_MAX];
} GapPawrResponseReport;

/**
 * @brief Callback group of the advertiser-side PAwR (periodic advertising
 *        with responses) feature, Core Spec 5.4 Vol 4 Part E 7.8.125 /
 *        7.7.65.36-37.
 *
 * Register through GAPIF_RegisterPawrAdvCallback. All members are optional;
 * initialize the struct with zeroes or {0} and set only the members needed.
 * The @c context pointer passed to GAPIF_RegisterPawrAdvCallback is handed
 * back as the last argument of every callback invocation.
 *
 * All callbacks run on the Stack thread (the same task the HCI events are
 * processed on) - the data pointers of @c GapPawrResponseReport and of the
 * @c GapPawrSubeventData entries are valid during the callback only. Do not
 * call blocking GAP interfaces from within a callback.
 */
typedef struct {
    /// The Controller requests data for the upcoming subevents of a PAwR
    /// train (LE Periodic Advertising Subevent Data Request, 7.7.65,36).
    /// The GAP layer replies for the subevents that were prefilled through
    /// GAPIF_LePawrSetSubeventData and then hands the full requested window
    /// to this member; subevents of an earlier request that are requested
    /// again may therefore have been transmitted already.
    /// A request window that wraps around the 0x00-0x7F subevent ring
    /// (subeventStart + subeventDataCount > 0x80, which the spec allows the
    /// Controller to issue) cannot be answered from the linear per-handle
    /// buffer and is dropped: this member is not invoked for such a window.
    /// @param advertisingHandle  PAwR train (0x00-0xEF)
    /// @param subeventStart      First requested subevent (0x00-0x7F)
    /// @param subeventDataCount  Number of requested subevents (0x01-0x80)
    /// @param context            Registration context
    void (*subeventDataRequest)(uint8_t advertisingHandle, uint8_t subeventStart, uint8_t subeventDataCount,
        void *context);
    /// Responses received on the response slots of a PAwR train (LE Periodic
    /// Advertising Response Report, 7.7.65,37).
    /// @param report   Report contents; valid during the callback only
    /// @param context  Registration context
    void (*responseReport)(const GapPawrResponseReport *report, void *context);
    /// Command completion of HCI_LE_Set_Periodic_Advertising_Subevent_Data
    /// (7.8.125), i.e. of a GAPIF_LePawrSetSubeventData call or of a reply
    /// the GAP layer made to a 7.7.65,36 request.
    /// @param status             Command status: 0x00 success, 0x42 advertising
    ///                           set not found, 0x0C subevent outside the
    ///                           requested range, 0x45 combined data too long
    ///                           (all data discarded), 0x46/0x47 subevent
    ///                           passed or too early (data discarded)
    /// @param advertisingHandle  PAwR train (0x00-0xEF)
    /// @param context            Registration context
    void (*subeventDataResult)(uint8_t status, uint8_t advertisingHandle, void *context);
    /// Command completion of HCI_LE_Set_Periodic_Advertising_Parameters [v2]
    /// (7.8.61 [v2]) / of the [v1] command used when @c numSubevents is 0.
    /// For the [v1] path this member is not called - the result arrives at
    /// the legacy extended advertising callback
    /// (GapExAdvCallback.periodicAdvSetParamResult, registered via
    /// GAPIF_RegisterExAdvCallback).
    /// @param status             Command status; 0x0C = periodic advertising
    ///                           already enabled (the controller-side check)
    /// @param advertisingHandle  PAwR train (0x00-0xEF)
    /// @param context            Registration context
    void (*setSubeventParamsResult)(uint8_t status, uint8_t advertisingHandle, void *context);
} GapPawrAdvCallback;

/// Parameters of the subevent structure of a PAwR train, passed to
/// GAPIF_LePawrSetSubeventParams. The fields map 1:1 onto the parameters of
/// HCI_LE_Set_Periodic_Advertising_Parameters [v2] (Core Spec 5.4 Vol 4
/// Part E 7.8.61 [v2]); they are copied by the GAP layer - the struct is read
/// during the call only.
typedef struct {
    /// Periodic advertising interval min, N x 1.25 ms (0x0006-0xFFFF).
    uint16_t intervalMin;
    /// Periodic advertising interval max, N x 1.25 ms (0x0006-0xFFFF), no
    /// smaller than @c intervalMin.
    uint16_t intervalMax;
    /// Number of subevents of the train: 0x00 for a plain periodic
    /// advertising train (the [v1] command is used), else 0x01-0x80.
    uint8_t numSubevents;
    /// Subevent interval, N x 1.25 ms (0x06-0xFF, 7.5-318.75 ms); ignored when
    /// @c numSubevents is 0x00.
    uint16_t subeventInterval;
    /// Response slot delay: 0x00 none, else N x 1.25 ms (0x01-0xFE); ignored
    /// when @c numSubevents is 0x00 or @c numResponseSlots is 0x00.
    uint16_t responseSlotDelay;
    /// Response slot spacing: 0x00 none, else N x 0.125 ms (0x02-0xFF);
    /// ignored when @c numSubevents is 0x00 or @c numResponseSlots is 0x00 or
    /// 0x01.
    uint16_t responseSlotSpacing;
    /// Number of response slots per subevent: 0x00 none, else 0x01-0xFF.
    uint8_t numResponseSlots;
} GapPawrSubeventParams;

/**
 * @brief Set the parameters of a PAwR train (HCI_LE_Set_Periodic_Advertising_
 *        Parameters [v2], Core Spec 5.4 Vol 4 Part E 7.8.61 [v2]).
 *
 * This is the 5.4 PAwR counterpart of GAPIF_LePeriodicAdvSetParam: it is used
 * together with that flow, in the order of the PAwR message sequence chart
 * (periodic advertising parameters, then periodic advertising data, then
 * periodic advertising enable before the extended advertising enable), and
 * the extended advertising set must be set up as a connectable periodic
 * advertising set first.
 *
 * @c params.numSubevents of 0x00 configures a plain periodic advertising
 * train without response slots: the parameters are forwarded through the
 * [v1] command (OCF 0x003E, identical parameter set) and the remaining PAwR
 * parameters of @c params are ignored, exactly as the Controller ignores
 * them (7.8.61 [v2]).
 * @c params.numSubevents above 0x00 requires Controller support for the
 * advertiser-side PAwR (LE feature bit 43,
 * BTM_IsControllerSupportPawrAdvertiser) and uses the [v2] command; the
 * remaining parameters are the timing relation inputs the Controller
 * checks:
 *  - Subevent_Interval x 1.25 ms with numSubevents x Subevent_Interval
 *    no greater than the periodic advertising interval min (Subevent_Interval
 *    x Num_Subevents <= Interval_Min holds on the wire);
 *  - Response_Slot_Delay (0x00 none, else x 1.25 ms) smaller than
 *    Subevent_Interval;
 *  - Response_Slot_Spacing (0x00 none, else x 0.125 ms) x Num_Response_Slots
 *    no greater than 10 x (Subevent_Interval - Response_Slot_Delay) once
 *    more than one response slot is configured (the factor of 10 converts
 *    the 1.25 ms units into 0.125 ms units).
 * 0x00 of Response_Slot_Delay / Response_Slot_Spacing / Num_Response_Slots
 * means the train has no response slots; the values of the remaining timing
 * parameters are then ignored by the Controller and not validated here.
 *
 * The result of the configuration arrives at the registered PAwR callback
 * @c GapPawrAdvCallback.setSubeventParamsResult.
 *
 * @param[in] advHandle  Periodic advertising train (0x00-0xEF)
 * @param[in] params     Parameters of the subevent structure of the train
 *                       (see @c GapPawrSubeventParams); must not be NULL
 * @return @c BT_SUCCESS         : The function is executed successfully.
 *         @c BT_BAD_PARAM       : Parameter is invalid (@c params NULL, or a
 *                               field out of the ranges or timing relations
 *                               above).
 *         @c otherwise          : The function is not executed successfully
 *                               (incl. GAP_ERR_NOT_ENABLE / GAP_ERR_INVAL_
 *                               STATE / GAP_ERR_NOT_SUPPORT, see the
 *                               pre-5.4 entries).
 */
BTSTACK_API int GAPIF_LePawrSetSubeventParams(uint8_t advHandle, const GapPawrSubeventParams *params);

/**
 * @brief Set (prefill) the data of consecutive subevents of a PAwR train
 *        (HCI_LE_Set_Periodic_Advertising_Subevent_Data, Core Spec 5.4
 *        Vol 4 Part E 7.8.125).
 *
 * The entries are buffered on the advertising handle and are transmitted by
 * the GAP layer when the Controller requests the subevents (LE Periodic
 * Advertising Subevent Data Request, 7.7.65.36); the data of a subevent is
 * transmitted once and then discarded. A later call replaces the buffered
 * data of the subevents it covers. The completion status of each reply is
 * delivered through @c GapPawrAdvCallback.subeventDataResult.
 *
 * The Controller only accepts data for the subevents it requested; data set
 * for subevents that are never requested is never transmitted and is
 * discarded when the buffer is replaced or the feature is torn down.
 *
 * @param[in] advHandle         Periodic advertising train (0x00-0xEF)
 * @param[in] subeventStart     First subevent of the window (0x00-0x7F); the
 *                              @c i-th entry of @c data belongs to subevent
 *                              @c subeventStart + i (no wraparound)
 * @param[in] subeventDataCount Number of entries of @c data (0x01-0x80);
 *                              @c subeventStart + @c subeventDataCount must
 *                              stay below 0x80
 * @param[in] data              Entry array of @c subeventDataCount entries,
 *                              one per subevent
 * @return @c BT_SUCCESS        : The function is executed successfully (the
 *                               command result arrives at the callback).
 *         @c BT_BAD_PARAM      : Parameter is invalid (handle, window, or an
 *                               entry length above 249, which cannot be
 *                               framed in one 0x82 command).
 *         @c otherwise         : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LePawrSetSubeventData(uint8_t advHandle, uint16_t subeventStart, uint16_t subeventDataCount,
    const GapPawrSubeventData data[]);

/**
 * @brief Register the PAwR advertiser callback group. A previous registration
 *        is replaced. Deregister through GAPIF_DeregisterPawrAdvCallback.
 * @param[in] callback  Callback group; all members are optional (see
 *                      @c GapPawrAdvCallback)
 * @param[in] context   Context handed back to each callback invocation
 * @return @c BT_SUCCESS        : The function is executed successfully.
 *         @c otherwise         : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_RegisterPawrAdvCallback(const GapPawrAdvCallback *callback, void *context);

/**
 * @brief Deregister the PAwR advertiser callback group registered through
 *        GAPIF_RegisterPawrAdvCallback.
 * @return @c BT_SUCCESS        : The function is executed successfully.
 */
BTSTACK_API int GAPIF_DeregisterPawrAdvCallback(void);

/// Bit 6 of @c properties of GAPIF_LePawrSetSyncSubevent (7.8.127): include
/// TxPower in the AUX_SYNC_SUBEVENT_RSP PDUs the Controller transmits while
/// it is synchronized to the subevents. All other bits are reserved.
#define GAP_PAWR_SYNC_SUBEVENT_PROPERTIES_TX_POWER 0x0040

/// Values of a synchronization established with a periodic advertising
/// train, reported through @c GapPawrSyncCallback.syncEstablished
/// (LE Periodic Advertising Sync Established [v2], Core Spec 5.4 Vol 4
/// Part E 7.7.65,14).
///
/// The four trailing members describe the subevent/response-slot structure
/// of the train; a plain train without subevents is reported with
/// @c numSubevents 0x00 (the Controller then also sets the other three to
/// 0x00, 7.7.65,14). On 5.4 Controllers the member also reports the outcome
/// of a GAPIF_LePeriodicAdvCreateSync: a failed establishment arrives with
/// a non-zero @c status and the clearly-invalid identity values below.
/// @c advAddr points into the event buffer - it is valid during the callback
/// only.
typedef struct {
    /// 0x00 success, else the HCI status of the establishment.
    uint8_t status;
    /// Sync handle (0x0000-0x0EFF); 0xFFFF when @c status is non-zero.
    uint16_t syncHandle;
    /// Advertising SID (0x00-0x0F); 0xFF when @c status is non-zero.
    uint8_t advSid;
    /// Advertiser address; NULL when @c status is non-zero.
    const BtAddr *advAddr;
    /// Advertiser PHY: 0x01 LE 1M, 0x02 LE 2M, 0x03 LE Coded; 0x00 when
    /// @c status is non-zero.
    uint8_t advPhy;
    /// Periodic advertising interval, N x 1.25 ms; 0xFFFF when @c status is
    /// non-zero.
    uint16_t periodicAdvInterval;
    /// Number of subevents of the train: 0x00 plain train, 0x01-0x80
    /// subevents; 0x00 when @c status is non-zero.
    uint8_t numSubevents;
    /// Subevent interval, N x 1.25 ms (0x06-0xFF); 0x00 when @c status is
    /// non-zero.
    uint8_t subeventInterval;
    /// Response slot delay, N x 1.25 ms (0x00 none, else 0x01-0xFE); 0x00
    /// when @c status is non-zero.
    uint8_t responseSlotDelay;
    /// Response slot spacing, N x 0.125 ms (0x00 none, else 0x02-0xFF); 0x00
    /// when @c status is non-zero.
    uint8_t responseSlotSpacing;
} GapPawrSyncEstablishedReport;

/// Data received on a synchronized periodic advertising train, reported
/// through @c GapPawrSyncCallback.syncReport (LE Periodic Advertising Report
/// [v2], Core Spec 5.4 Vol 4 Part E 7.7.65,15): the [v1] report parameters
/// with Periodic_Event_Counter and Subevent of the [v2] layout. A plain
/// train without subevents is reported with @c subevent 0xFF. @c data points
/// into a copy owned by the GAP layer - it is valid during the callback only.
typedef struct {
    /// Sync handle (0x0000-0x0EFF).
    uint16_t syncHandle;
    /// TX power in dBm (-127 to +20, 0x7F unknown).
    int8_t txPower;
    /// RSSI in dBm (-127 to +20, 0x7F unknown).
    int8_t rssi;
    /// CTE type: 0x00 AoA, 0x01 AoD 1 us, 0x02 AoD 2 us, 0xFF no CTE.
    uint8_t cteType;
    /// paEventCounter of the reporting event.
    uint16_t periodicEventCounter;
    /// Subevent the data was received in (0x00-0x7F), 0xFF = train without
    /// subevents.
    uint8_t subevent;
    /// Data_Status of the [v1] report.
    uint8_t dataStatus;
    /// Length of @c data.
    uint8_t dataLength;
    /// Report data; valid during the callback only, must not be modified or
    /// retained (may be NULL when @c dataLength is 0).
    const uint8_t *data;
} GapPawrSyncReport;

/// Values of a synchronization transferred over a connection and established
/// on this device, reported through @c GapPawrSyncCallback.syncTransferReceived
/// (LE Periodic Advertising Sync Transfer Received [v2], Core Spec 5.4
/// Vol 4 Part E 7.7.65,24). Same semantics and zero-value rules as
/// @c GapPawrSyncEstablishedReport, except for the zero-value rule of the
/// subevent tail: when the train has no subevents the Controller sets
/// @c numSubevents 0x00 and the values of @c subeventInterval,
/// @c responseSlotDelay and @c responseSlotSpacing are unspecified by the
/// spec - they are passed through verbatim and must be ignored.
/// @c advAddr points into the event buffer - it is valid during the callback
/// only.
typedef struct {
    /// 0x00 success, else the HCI status. When non-zero the synchronization
    /// failed, but the advertiser identity and the train parameters still
    /// describe the transfer and are reported verbatim (7.7.65,24); only
    /// @c syncHandle is not meaningful then and is reported as 0xFFFF.
    uint8_t status;
    /// Connection the sync was transferred over; reported verbatim.
    uint16_t connectionHandle;
    /// Service data of the PAST (7.7.65,24); reported verbatim.
    uint16_t serviceData;
    /// Sync handle (0x0000-0x0EFF); 0xFFFF when @c status is non-zero (the
    /// Host shall ignore it in that case).
    uint16_t syncHandle;
    /// Advertising SID (0x00-0x0F).
    uint8_t advSid;
    /// Advertiser address.
    const BtAddr *advAddr;
    /// Advertiser PHY: 0x01 LE 1M, 0x02 LE 2M, 0x03 LE Coded.
    uint8_t advPhy;
    /// Periodic advertising interval, N x 1.25 ms.
    uint16_t periodicAdvInterval;
    /// Advertiser clock accuracy: 0x00 500 ppm, 0x01 250 ppm, 0x02 150 ppm,
    /// 0x03 100 ppm, 0x04 75 ppm, 0x05 50 ppm, 0x06 30 ppm, 0x07 20 ppm.
    uint8_t clockAccuracy;
    /// Number of subevents of the train: 0x00 plain train, 0x01-0x80
    /// subevents.
    uint8_t numSubevents;
    /// Subevent interval, N x 1.25 ms (0x06-0xFF).
    uint8_t subeventInterval;
    /// Response slot delay, N x 1.25 ms.
    uint8_t responseSlotDelay;
    /// Response slot spacing, N x 0.125 ms.
    uint8_t responseSlotSpacing;
} GapPawrSyncTransferReceivedReport;

/**
 * @brief Callback group of the sync-side PAwR (periodic advertising with
 *        responses) feature, Core Spec 5.4 Vol 4 Part E 7.8.126-127 /
 *        7.7.65.14-15 / 7.7.65.24: the scanner or observer of a PAwR train.
 *
 * Register through GAPIF_RegisterPawrSyncCallback. All members are optional;
 * initialize the struct with zeroes or {0} and set only the members needed.
 * The @c context pointer passed to GAPIF_RegisterPawrSyncCallback is handed
 * back as the last argument of every callback invocation.
 *
 * All callbacks run on the Stack thread (the same task the HCI events are
 * processed on) - data pointers are valid during the callback only. Do not
 * call blocking GAP interfaces from within a callback.
 *
 * This group carries the [v2] content of the periodic advertising events
 * (subevent codes 0x24/0x25/0x26, which a 5.4 Controller emits once the [v2]
 * periodic event bits of the event mask are enabled). The pre-5.4 callbacks
 * keep reporting the same events in their [v1] shape:
 * GapPeriodicAdvSyncCallback.syncEstablished / syncReport and, for PAST,
 * GapLeCteCallback.pastSyncTransferReceived. Events without a [v2] variant
 * (sync lost, create-sync cancel, terminate result, periodic advertiser list
 * results) have no counterpart in this group and keep flowing through the
 * pre-5.4 callbacks. A consumer therefore registers this group for the PAwR
 * parameters and registers the pre-5.4 group for the sync lifecycle it needs.
 */
typedef struct {
    /// A synchronization was established with a periodic advertising train
    /// (LE Periodic Advertising Sync Established [v2], 7.7.65,14). On 5.4
    /// Controllers this member also reports the outcome of a
    /// GAPIF_LePeriodicAdvCreateSync (a failed establishment arrives with a
    /// non-zero @c status and the clearly-invalid identity values).
    /// @param report   Report values (see @c GapPawrSyncEstablishedReport)
    /// @param context  Registration context
    void (*syncEstablished)(const GapPawrSyncEstablishedReport *report, void *context);
    /// Data received on a synchronized periodic advertising train
    /// (LE Periodic Advertising Report [v2], 7.7.65,15).
    /// @param report   Report contents (see @c GapPawrSyncReport); @c data of
    ///                 the report is valid during the callback only
    /// @param context  Registration context
    void (*syncReport)(const GapPawrSyncReport *report, void *context);
    /// A synchronization transferred over a connection was established on
    /// this device (LE Periodic Advertising Sync Transfer Received [v2],
    /// 7.7.65,24).
    /// @param report   Report values (see
    ///                 @c GapPawrSyncTransferReceivedReport)
    /// @param context  Registration context
    void (*syncTransferReceived)(const GapPawrSyncTransferReceivedReport *report, void *context);
    /// Command completion of HCI_LE_Set_Periodic_Sync_Subevent (7.8.127),
    /// i.e. of a GAPIF_LePawrSetSyncSubevent call. Subevents of the sync that
    /// are not part of the subset stop being synchronized.
    /// @param status      Command status: 0x00 success, else an HCI status
    ///                    (the Controller rejects a sync that was not
    ///                    established with subevents)
    /// @param syncHandle  Sync handle the command addressed (0x0000-0x0EFF)
    /// @param context     Registration context
    void (*setSyncSubeventResult)(uint8_t status, uint16_t syncHandle, void *context);
    /// Command completion of HCI_LE_Set_Periodic_Advertising_Response_Data
    /// (7.8.126), i.e. of a GAPIF_LePawrSetResponseData call. A successful
    /// completion means the Controller scheduled the data for transmission in
    /// the named response slot; the data is transmitted once and no further
    /// notification of the transmission arrives.
    /// @param status      Command status: 0x00 success, 0x45 data too long for
    ///                    the response slot (discarded), 0x46 response slot
    ///                    already passed (discarded), else an HCI status
    /// @param syncHandle  Sync handle the command addressed (0x0000-0x0EFF)
    /// @param context     Registration context
    void (*setResponseDataResult)(uint8_t status, uint16_t syncHandle, void *context);
} GapPawrSyncCallback;

/**
 * @brief Restrict the synchronization with a PAwR train to a subset of its
 *        subevents (HCI_LE_Set_Periodic_Sync_Subevent, Core Spec 5.4 Vol 4
 *        Part E 7.8.127).
 *
 * The Controller keeps listening only to the listed subevents and stops
 * synchronizing any subevent of the train that is not part of the subset.
 * The sync must have been established with the subevent structure of the
 * train reported (see @c GapPawrSyncCallback.syncEstablished); the result of
 * the command arrives at @c GapPawrSyncCallback.setSyncSubeventResult.
 * Bit 6 of @c properties (GAP_PAWR_SYNC_SUBEVENT_PROPERTIES_TX_POWER) asks
 * the Controller to include TxPower in the AUX_SYNC_SUBEVENT_RSP PDUs it
 * transmits on the synchronized subevents; all other bits are reserved and
 * rejected.
 *
 * @param[in] syncHandle   Sync handle of the PAwR train (0x0000-0x0EFF)
 * @param[in] properties   Periodic advertising properties (only bit 6 is
 *                         defined, see above; reserved bits are rejected)
 * @param[in] subevents    Array of the subevents to synchronize to, each
 *                         0x00-0x7F; must not be NULL
 * @param[in] numSubevents Number of entries of @c subevents (0x01-0x80)
 * @return @c BT_SUCCESS        : The function is executed successfully (the
 *                               command result arrives at the callback).
 *         @c BT_BAD_PARAM      : Parameter is invalid (sync handle,
 *                               properties or an entry of @c subevents out of
 *                               range, empty or NULL @c subevents).
 *         @c otherwise         : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LePawrSetSyncSubevent(uint16_t syncHandle, uint16_t properties,
    const uint8_t subevents[], uint8_t numSubevents);

/// Response data of one response slot of a PAwR train, passed to
/// GAPIF_LePawrSetResponseData (HCI_LE_Set_Periodic_Advertising_Response_
/// Data, Core Spec 5.4 Vol 4 Part E 7.8.126).
///
/// @c requestEvent (the paEventCounter of the packet) and @c requestSubevent
/// point back to the periodic advertising packet the response answers;
/// @c responseSubevent and @c responseSlot name the slot the data is actually
/// transmitted in and may differ from the subevent the request was received
/// in. The data is transmitted once in that slot. The fields are copied by
/// the GAP layer - the struct and @c data are read during the call only.
typedef struct {
    /// paEventCounter of the packet being responded to.
    uint16_t requestEvent;
    /// Subevent the responded-to packet was received in (passed through;
    /// the Controller rejects values that cannot be answered).
    uint8_t requestSubevent;
    /// Subevent the response is sent in (0x00-0x7F).
    uint8_t responseSubevent;
    /// Response slot of @c responseSubevent (0x00-0xFF).
    uint8_t responseSlot;
    /// Data to transmit once (0-247 bytes); may be NULL when @c dataLength
    /// is 0.
    const uint8_t *data;
    /// Length of @c data (0-247: the data is sent in a single 0x83 command
    /// whose one-octet HCI parameter length field frames at most 247 payload
    /// octets, so the spec-valid 248-251 are rejected here; 252-0xFF are
    /// reserved).
    uint8_t dataLength;
} GapPawrResponseData;

/**
 * @brief Schedule the response data of one response slot of a PAwR train
 *        (HCI_LE_Set_Periodic_Advertising_Response_Data, Core Spec 5.4
 *        Vol 4 Part E 7.8.126).
 *
 * The result of the command (incl. a data discarded due to a slot timing
 * error) arrives at @c GapPawrSyncCallback.setResponseDataResult.
 *
 * @param[in] syncHandle  Sync handle of the PAwR train (0x0000-0x0EFF)
 * @param[in] response    Response data of one response slot (see
 *                        @c GapPawrResponseData); must not be NULL
 * @return @c BT_SUCCESS        : The function is executed successfully (the
 *                               command result arrives at the callback).
 *         @c BT_BAD_PARAM      : Parameter is invalid (@c response NULL, sync
 *                               handle, response subevent or data length out
 *                               of range).
 *         @c otherwise         : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_LePawrSetResponseData(uint16_t syncHandle, const GapPawrResponseData *response);

/**
 * @brief Register the PAwR sync callback group. A previous registration is
 *        replaced. Deregister through GAPIF_DeregisterPawrSyncCallback.
 * @param[in] callback  Callback group; all members are optional (see
 *                      @c GapPawrSyncCallback)
 * @param[in] context   Context handed back to each callback invocation
 * @return @c BT_SUCCESS        : The function is executed successfully.
 *         @c otherwise         : The function is not executed successfully.
 */
BTSTACK_API int GAPIF_RegisterPawrSyncCallback(const GapPawrSyncCallback *callback, void *context);

/**
 * @brief Deregister the PAwR sync callback group registered through
 *        GAPIF_RegisterPawrSyncCallback.
 * @return @c BT_SUCCESS        : The function is executed successfully.
 */
BTSTACK_API int GAPIF_DeregisterPawrSyncCallback(void);

/**
 * @brief Create an ACL connection to a synchronized device from a subevent
 *        of a local PAwR train (Bluetooth 5.4 connect-from-PAwR, Vol 4,
 *        Part E 7.8.66 LE Extended Create Connection [v2], Vol 6, Part B
 *        4.4.2.12.2).
 *
 * The local device is the periodic advertiser of the train identified by
 * @p advHandle (the handle configured through GAPIF_LePawrSetSubeventParams
 * and the periodic advertising enable path); the Controller transmits an
 * AUX_CONNECT_REQ PDU instead of the AUX_SYNC_SUBEVENT_IND PDU of
 * subevent @p subevent, targeting the synchronized device @p addr. The
 * local device becomes the Central of the new connection and the
 * synchronized device the Peripheral. Connection parameters default to the
 * values of the legacy connect path.
 *
 * When both @p advHandle and @p subevent are 0xFF the PAwR pair is not used
 * and the call behaves exactly like the legacy address-based connect (the
 * Filter Accept List machinery); a 0xFF on exactly one of the two
 * parameters is invalid.
 *
 * The outcome of the attempt is reported through the connection-complete
 * path already used by address-based connects - no new callback group is
 * registered. This includes the Enhanced Connection Complete [v2] event
 * (0x29) on a 5.4 controller: failure with Status 0x3E (Connection Failed
 * to be Established) when the synchronized device misses the
 * AUX_CONNECT_REQ or the periodic advertiser misses the AUX_CONNECT_RSP
 * follows the ordinary connect-failure flow, and a peer that already
 * established the connection is disconnected by the Controller with
 * Reason 0x3E after six connection events through the ordinary
 * disconnect flow. Once the connection is established its association with
 * the train (Advertising_Handle on the Central side, Sync_Handle on the
 * Peripheral side of a connection created from PAwR, 7.7.65.10) can be
 * queried through BTM_GetLeConnectionPawrAssociation.
 *
 * @param advHandle  Advertising handle of the local PAwR train the
 *                   connection request is initiated from: 0x00-0xEF, or
 *                   0xFF when the pair is not used (fallback to the legacy
 *                   address-based connect).
 * @param subevent   Subevent of the train the connection request is
 *                   initiated from: 0x00-0x7F, or 0xFF when not used.
 * @param addr       Address of the synchronized LE device to connect to
 *                   (the AdvA of the AUX_CONNECT_REQ).
 * @return @c BT_SUCCESS        : The command was sent to the Controller.
 *         @c GAP_ERR_NOT_ENABLE: LE is not enabled (all paths, incl. the
 *                               0xFF fallback).
 *         @c GAP_ERR_INVAL_PARAM: Parameter is invalid (address, handle or
 *                               subevent out of range, 0xFF on one of the
 *                               pair only).
 *         @c GAP_ERR_INVAL_STATE: A PAwR connect was requested but the
 *                               train/connect roles are not enabled (the
 *                               0xFF fallback is exempt - the legacy path
 *                               applies no role gate here).
 *         @c GAP_ERR_NOT_SUPPORT: The Controller does not support the
 *                               Periodic Advertising Advertiser role, or
 *                               this entry point is used without PAwR
 *                               support (the 0xFF fallback needs no PAwR
 *                               capability).
 *         @c otherwise         : BTM-level failure passed through, e.g.
 *                               @c BT_BAD_STATUS when another LE connect
 *                               attempt is already in progress (one
 *                               connection request at a time), @c BT_NO_MEMORY
 *                               or @c BT_CONNECT_NUM_MAX when the connection
 *                               table is full.
 */
BTSTACK_API int GAPIF_LeExtCreateConnFromPawr(uint8_t advHandle, uint8_t subevent, const BtAddr *addr);

#ifdef __cplusplus
}
#endif

#endif // GAP_LE_IF_5_4_H
