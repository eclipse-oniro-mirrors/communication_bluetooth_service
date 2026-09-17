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
 * @brief channel sounding (CS) session management interface
 *
 */

/**
 * @file cs_le_if.h
 *
 * @brief bluetooth channel sounding interface
 *
 */

#ifndef CS_LE_IF_H
#define CS_LE_IF_H

#include "btstack.h"
#include "cs_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/// BT 6.0 Channel Sounding session management.
///
/// Layer: public API (CSIF_) -> internal CS module (Cs*) -> HCI command (HCI_LeCs*).
/// A CS session is opened on an existing LE ACL and owns everything the Host does on
/// that link afterwards: the capability exchange, the per-connection configurations
/// with their procedure schedules, and the measurement reports the Controller streams
/// back. A CS test is not a session - 0x0095 carries no Connection_Handle and its
/// results arrive under the reserved handle 0x0FFF - so the two test entries stand on
/// their own and no session has to be open for them.
///
/// The CS commands are Vol 4, Part E 7.8.130-7.8.143 (pp2798-2855) and the LE Meta CS
/// subevents 7.7.65,39-7.7.65,46 (pp2426-2463); each entry below names its own.
///
/// Threading: a CSIF_* entry may be called from any thread, but its work runs on the
/// Stack thread and the caller blocks until it completes. So an entry never runs
/// concurrently with a callback - the caller's own thread needs no synchronization
/// against them - and a call made from inside a callback runs inline there instead of
/// blocking a thread that would have to drain the very queue it waits on. Because that
/// inline call bypasses the queue, it may run ahead of tasks that other threads enqueued
/// earlier: issue the dependent commands of one session from a single thread, or a later
/// command can observe state an earlier, still-queued command was about to set (for
/// example CSIF_LeCsProcedureEnable refuses with a synthesized 0x0C while an earlier
/// CSIF_LeCsSetProcedureParameters from another thread is still waiting in the queue).
/// All result callbacks run on the Stack thread, exactly like the HCI events that
/// produce them; callers must not block inside a callback, and anything they keep must
/// be copied out before returning.
///
/// Return codes common to every entry: BT_NO_MEMORY when this entry's own dispatch block,
/// or the block the CS processing queue builds for it, cannot be allocated; BT_BAD_STATUS
/// when the CS processing queue does not exist, which is before CsInitialize created it or
/// after CsFinalize removed it. The per-entry @return lists that entry's own codes on top
/// of these, so a BT_BAD_STATUS from an entry that documents a handle check does not by
/// itself mean the handle was the problem.

#define CS_LE_CONFIG_ID_MAX 0x03                // Config_ID 0x00-0x03 (0x0090/0x0091/0x2F)
#define CS_LE_CHANNEL_MAP_LEN 10                // 80 one-bit fields (0x0090/0x0092/0x2F)
#define CS_LE_REMOTE_FAE_TABLE_LEN 72           // 1/32 ppm signed, one octet per allowed channel
#define CS_LE_ANTENNA_PATH_MAX 0x04             // Max_Antenna_Paths_Supported 0x01-0x04 (0x0089)
#define CS_LE_SUB_MODE_UNUSED 0xFF              // Sub_Mode_Type "unused" (0x0090/0x0095)
#define CS_LE_TEST_PREFIX_LEN 30                // fixed prefix of the 0x0095 parameter list
#define CS_LE_24BIT_FIELD_LEN 3              // Subevent_Len and other 3-octet LE values
#define CS_LE_TEST_OVERRIDE_MIN 2               // a one-channel bit-0-set override tail
#define CS_LE_TEST_OVERRIDE_MAX 225             // Parameter_Total_Length (1 octet) less the prefix
#define CS_LE_TEST_OVERRIDE_CONFIG_MASK 0x05FD  // defined Override_Config bits (p2848)

/**
 * @brief       CS capability body, mirrors the 28-octet container of 7.8.130 pp2798-2804
 *
 * Field names and order are the wire order of that section, which is shared by the
 * 0x0089 return, the 0x008B parameter and the 0x2C subevent. Returned by
 * CSIF_LeCsReadLocalCaps and carried by CsLeRemoteCapabilities.
 *
 * The struct is not the wire image: 28 octets is the spec's size for this field set,
 * while the struct is laid out by the compiler (the iso_le_if.h convention, where a
 * 24-bit wire value is stored as uint32_t). A copy in or out is therefore field by
 * field, and the two headers are what holds the two in step.
 */
typedef struct {
    uint8_t numConfigSupported;                 ///< Number_of_Configurations_Supported
    uint16_t maxConsecutiveProceduresSupported;
    uint8_t numAntennasSupported;
    uint8_t maxAntennaPathsSupported;
    uint8_t rolesSupported;                     ///< bits 0/1: Initiator / Reflector
    uint8_t modesSupported;                     ///< bit 0: Mode-3, other bits reserved (7.7.65,39 p2429)
    uint8_t rttCapability;
    uint8_t rttAaOnlyN;
    uint8_t rttSoundingN;
    uint8_t rttRandomPayloadN;
    uint16_t nadmSoundingCapability;
    uint16_t nadmRandomCapability;
    uint8_t csSyncPhysSupported;
    uint16_t subfeaturesSupported;
    uint16_t tIp1TimesSupported;
    uint16_t tIp2TimesSupported;
    uint16_t tFcsTimesSupported;
    uint16_t tPmTimesSupported;
    uint8_t tSwTimeSupported;
    uint8_t txSnrCapability;
} CsLeCapabilities;

/** @brief 0x008D parameter, mirrors HciLeCsSetDefaultSettingsParam fields (7.8.134 pp2816-2818) */
typedef struct {
    uint16_t connectionHandle;
    uint8_t roleEnable;              ///< bits 0/1; disabling a role with a valid config -> 0x12
    uint8_t csSyncAntennaSelection;  ///< 0x01-0x04 antenna id, 0xFE repeat in order, 0xFF no recommendation
    int8_t maxTxPower;               ///< dBm EIRP, -127..20; the Controller clamps to its own maximum
} CsLeSetDefaultSettingsParam;

/** @brief 0x0090 parameter, mirrors HciLeCsCreateConfigParam fields (7.8.137 pp2822-2827) */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t createContext;          ///< 0x00 local Controller only, 0x01 local and peer
    uint8_t mainModeType;           ///< 0x01-0x03
    uint8_t subModeType;            ///< 0x01-0x03, or CS_LE_SUB_MODE_UNUSED
    uint8_t minMainModeSteps;       ///< reserved when subModeType is CS_LE_SUB_MODE_UNUSED
    uint8_t maxMainModeSteps;       ///< reserved when subModeType is CS_LE_SUB_MODE_UNUSED
    uint8_t mainModeRepetition;     ///< 0x00-0x03
    uint8_t mode0Steps;             ///< 0x01-0x03
    uint8_t role;                   ///< 0x00 Initiator, 0x01 Reflector
    uint8_t rttType;                ///< 0x00-0x06
    uint8_t csSyncPhy;              ///< 0x01 LE 1M, 0x02 LE 2M, 0x03 LE 2M 2BT
    uint8_t channelMap[CS_LE_CHANNEL_MAP_LEN];
    uint8_t channelMapRepetition;   ///< 0x01-0xFF
    uint8_t channelSelectionType;   ///< 0x00 algorithm #3b, 0x01 algorithm #3c
    uint8_t ch3cShape;              ///< 0x00 hat, 0x01 X (only when channelSelectionType is #3c)
    uint8_t ch3cJump;               ///< 0x02-0x08 (same condition as ch3cShape)
    uint8_t reserved;               ///< shall be 0x00
} CsLeCreateConfigParam;

/** @brief 0x0091 parameter, mirrors HciLeCsRemoveConfigParam fields (7.8.138 p2828) */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
} CsLeRemoveConfigParam;

/** @brief 0x0092 parameter, mirrors HciLeCsSetChannelClassificationParam fields (7.8.139 pp2830-2831) */
typedef struct {
    uint8_t channelClassification[CS_LE_CHANNEL_MAP_LEN];
} CsLeSetChannelClassificationParam;

/** @brief 0x0093 parameter, mirrors HciLeCsSetProcedureParametersParam fields (7.8.140 pp2832-2837) */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
    uint16_t maxProcedureLen;       ///< N x 0.625 ms
    uint16_t minProcedureInterval;  ///< connection events; ignored when maxProcedureCount is 1
    uint16_t maxProcedureInterval;  ///< connection events; ignored when maxProcedureCount is 1
    uint16_t maxProcedureCount;     ///< 0x0000: run until disabled
    uint8_t minSubeventLen[CS_LE_24BIT_FIELD_LEN];      ///< microseconds, 1250 us to 4 s (raw wire octets)
    uint8_t maxSubeventLen[CS_LE_24BIT_FIELD_LEN];      ///< microseconds, 1250 us to 4 s (raw wire octets)
    uint8_t toneAntennaConfigSelection;  ///< 0x00-0x07
    uint8_t phy;                    ///< 0x01 LE 1M, 0x02 LE 2M, 0x03/0x04 LE Coded S=8/S=2
    int8_t txPowerDelta;            ///< signed dB; 0x80 = no recommendation
    uint8_t preferredPeerAntenna;   ///< bits 0-3, the ordered antenna elements
    uint8_t snrControlInitiator;    ///< 0x00-0x04, or 0xFF not to be applied
    uint8_t snrControlReflector;    ///< 0x00-0x04, or 0xFF not to be applied
} CsLeSetProcedureParametersParam;

/** @brief 0x0094 parameter, mirrors HciLeCsProcedureEnableParam fields (7.8.141 pp2838-2839) */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t enable;                 ///< 0x00 disable, 0x01 enable
} CsLeProcedureEnableParam;

/** @brief 0x0095 parameter prefix, mirrors HciLeCsTestPrefix fields (7.8.142 pp2840-2848, 30 octets) */
typedef struct {
    uint8_t mainModeType;               ///< 0x01 Mode-1, 0x02 Mode-2, 0x03 Mode-3
    uint8_t subModeType;                ///< 0x01-0x03, or CS_LE_SUB_MODE_UNUSED
    uint8_t mainModeRepetition;         ///< 0x00-0x03
    uint8_t mode0Steps;                 ///< 0x01-0x03
    uint8_t role;                       ///< 0x00 Initiator, 0x01 Reflector
    uint8_t rttType;                    ///< 0x00-0x06
    uint8_t csSyncPhy;                  ///< 0x01 LE 1M, 0x02 LE 2M, 0x03 LE 2M 2BT
    uint8_t csSyncAntennaSelection;     ///< 0x01-0x04 antenna id
    uint8_t subeventLen[CS_LE_24BIT_FIELD_LEN];             ///< microseconds, 1250 us to 4 s (raw wire octets)
    uint16_t subeventInterval;          ///< 0x0000 single subevent, else units of 0.625 ms
    uint8_t maxNumSubevents;            ///< 0x00 ignored, 0x01-0x20
    int8_t transmitPowerLevel;          ///< dBm -127..20; 0x7E minimum, 0x7F maximum
    uint8_t tIp1Time;                   ///< enumerated, see the T_IP1/T_IP2 sets of p2846
    uint8_t tIp2Time;                   ///< enumerated, same set as tIp1Time
    uint8_t tFcsTime;                   ///< enumerated, see the T_FCS set of p2846
    uint8_t tPmTime;                    ///< enumerated, see the T_PM set of p2846
    uint8_t tSwTime;                    ///< enumerated, see the T_SW set of p2846
    uint8_t toneAntennaConfigSelection; ///< 0x00-0x07
    uint8_t reserved;                   ///< shall be 0x00
    uint8_t snrControlInitiator;        ///< 0x00-0x04, or 0xFF not to be applied
    uint8_t snrControlReflector;        ///< 0x00-0x04, or 0xFF not to be applied
    uint16_t drbgNonce;                 ///< octets 14 and 15 of the initial DRBG nonce
    uint8_t channelMapRepetition;       ///< 0x01-0xFF; 0x00 is reserved
    uint16_t overrideConfig;            ///< the defined bits are CS_LE_TEST_OVERRIDE_CONFIG_MASK
    uint8_t overrideParametersLength;   ///< octets of Override_Parameters_Data on the wire
} CsLeTestPrefix;

/**
 * @brief       0x0095 parameter, mirrors HciLeCsTestParam fields (7.8.142 pp2840-2854)
 *
 * The 30-octet prefix followed by the caller-owned override tail. The tail is a
 * sequence of parameter groups selected by the prefix's overrideConfig bits, so only
 * its owner can lay it out: it is appended verbatim behind the prefix, never
 * interpreted, and overrideParametersLength says how many octets of it go on the wire
 * (CS_LE_TEST_OVERRIDE_MIN to CS_LE_TEST_OVERRIDE_MAX). It must stay valid for the
 * duration of the call.
 */
typedef struct {
    CsLeTestPrefix prefix;
    const uint8_t *overrideParametersData;
} CsLeTestParam;

/**
 * @brief       0x2C result body: the CS Capability Exchange answered by the peer
 *
 * Delivered as the caps argument of remoteCapabilitiesComplete, whose status carries
 * the 7.7.65,39 p2428 Status. Only Status 0x00 holds a capability body.
 */
typedef struct {
    uint16_t connectionHandle;
    CsLeCapabilities capabilities;
} CsLeRemoteCapabilities;

/**
 * @brief       0x2D result body: the peer's mode-0 FAE table (7.7.65,40 pp2433-2434)
 *
 * Delivered as the table argument of remoteFaeTableComplete. Only Status 0x00 holds a
 * table: Status 0x11 is the Controller's "the peer reports a zero FAE" answer given in
 * place of one (7.8.135 p2819), and every other status is a failure; on all of them the
 * 72 octets carry no value. Each octet is an 8-bit signed 1/32 ppm value
 * (-128 = -4 ppm, 127 = +3.96875 ppm), so a reader casts to int8_t rather than
 * treating the octets as raw magnitudes.
 */
typedef struct {
    uint16_t connectionHandle;
    int8_t remoteFaeTable[CS_LE_REMOTE_FAE_TABLE_LEN];
} CsLeRemoteFaeTable;

/**
 * @brief       0x2F result body: the CS configuration the local Controller holds
 *
 * Delivered as the config argument of configComplete. action tells a 0x0090 create
 * (0x01) from a 0x0091 remove (0x00); when action is 0x00 the spec ignores every
 * parameter after it (7.7.65,42 p2435), so only action 0x01 carries a usable mirror.
 * The four T_*_Time values are the durations actually in use, chosen by the Controller
 * rather than asking for by the Host.
 */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t action;                 ///< 0x00 removed, 0x01 created
    uint8_t mainModeType;           ///< config mirror starts here, valid when action is 0x01
    uint8_t subModeType;
    uint8_t minMainModeSteps;
    uint8_t maxMainModeSteps;
    uint8_t mainModeRepetition;
    uint8_t mode0Steps;
    uint8_t role;
    uint8_t rttType;
    uint8_t csSyncPhy;
    uint8_t channelMap[CS_LE_CHANNEL_MAP_LEN];
    uint8_t channelMapRepetition;
    uint8_t channelSelectionType;
    uint8_t ch3cShape;
    uint8_t ch3cJump;
    uint8_t reserved;
    uint8_t tIp1Time;
    uint8_t tIp2Time;
    uint8_t tFcsTime;
    uint8_t tPmTime;
} CsLeConfig;

/**
 * @brief       0x30 result body: the schedule of a CS procedure (7.7.65,43 pp2442-2445)
 *
 * Delivered as the schedule argument of procedureEnableComplete. state reports the
 * state that stands (0x00 disabled, 0x01 enabled), not the enable that was asked for;
 * when it is 0x00 the spec ignores every parameter after it (p2442), so only state 0x01
 * carries a usable schedule mirror. selectedTxPower is signed dBm, where 0 is a real
 * level and only 0x7F means unavailable.
 */
typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t state;                      ///< 0x00 disabled, 0x01 enabled
    uint8_t toneAntennaConfigSelection; ///< schedule mirror starts here, valid when state is 0x01
    int8_t selectedTxPower;
    uint8_t subeventLen[CS_LE_24BIT_FIELD_LEN];             ///< microseconds, 3-octet LE value
    uint8_t subeventsPerEvent;
    uint16_t subeventInterval;          ///< connection events
    uint16_t eventInterval;             ///< connection events
    uint16_t procedureInterval;         ///< connection events
    uint16_t procedureCount;
    uint16_t maxProcedureLen;           ///< N x 0.625 ms
} CsLeProcedureSchedule;

/**
 * @brief       CS session result callback structure
 *
 * One member per completion the CS engine produces, plus the measurement and test
 * members. status is always the first argument and is the completion status of the
 * event (0x00 success; on the 0x2D it is 0x11 for a No_FAE peer), so a callback that
 * only cares whether the step succeeded reads no structure at all.
 *
 * Every member runs on the Stack thread with the values the event just produced: they
 * arrive in the callback arguments, and there is no reader that hands this state back
 * afterwards, so a consumer that needs it later keeps its own copy. Any member may be NULL.
 *
 * The measurement and test members are outside the completion set: subeventResult
 * carries the reassembled measurement set of one CS subevent, and the two test members
 * the CS test's own outcomes, neither of which closes a session command.
 */
typedef struct {
    ///< 0x2C, raised by 0x008A. Status 0x00 is what caches the peer's capability body
    void (*remoteCapabilitiesComplete)(uint8_t status, const CsLeRemoteCapabilities *caps, void *context);
    ///< 0x2D, raised by 0x008E. Status 0x11 is a No_FAE peer, not a failure
    void (*remoteFaeTableComplete)(uint8_t status, const CsLeRemoteFaeTable *table, void *context);
    ///< the 0x008B Command_Complete. Status 0x00 means the Controller took the cached
    ///< body; 0x0C is the Controller refusing because a CS configuration already
    ///< exists on the link (Host does not predict that locally). Without this report a
    ///< refused write-back would reach no one
    void (*writeCachedRemoteCapabilitiesComplete)(uint8_t status, uint16_t connectionHandle, void *context);
    ///< the 0x008F Command_Complete. FAE counterpart of writeCachedRemoteCapabilitiesComplete
    void (*writeCachedRemoteFaeTableComplete)(uint8_t status, uint16_t connectionHandle, void *context);
    ///< 0x2E, raised by the CS Security Start procedure of 0x008C
    void (*securityEnableComplete)(uint8_t status, uint16_t connectionHandle, void *context);
    ///< 0x2F, raised by 0x0090 and 0x0091, and by the local Controller answering a
    ///< configuration the peer requested, so it can arrive with no command outstanding
    void (*configComplete)(uint8_t status, const CsLeConfig *config, void *context);
    ///< 0x30, raised by 0x0094, and when a CS procedure the Controller started on its
    ///< own ends, so it too can arrive with no command outstanding
    void (*procedureEnableComplete)(uint8_t status, const CsLeProcedureSchedule *schedule, void *context);
    ///< the 0x0093 Command_Complete. The return names only the connection handle -
    ///< Config_ID is not echoed - so the caller correlates it with its own request. A
    ///< completion with no request outstanding is reported as well: it is still the
    ///< result of some 0x0093, and dropping it would leave its waiter hanging
    void (*procedureParametersComplete)(uint8_t status, uint16_t connectionHandle, void *context);
    ///< the 0x008D Command_Complete. Status 0x00 means the Controller took the
    ///< defaults for that connection; 0x11 is a value it will not take and 0x12 a role
    ///< it will not accept in the connection's current state, and neither is something
    ///< the caller can predict, since the defaults live in the Controller and the Host
    ///< keeps no copy. Without this report a refused 0x008D would reach no one. A
    ///< command that timed out arrives here too, with status 0xE1; when the Host could
    ///< not build the command's parameter block it has no handle to report, and
    ///< connectionHandle is 0xFFFF - so waiting on this member terminates either way
    void (*setDefaultSettingsComplete)(uint8_t status, uint16_t connectionHandle, void *context);
    ///< the 0x0092 Command_Complete. The classification is Controller-wide, so there is
    ///< no connection handle to carry and status is the whole result: 0x00 taken, 0x0C
    ///< the command repeated inside the Controller's 1-second window, 0x12 an invalid
    ///< map, 0xE1 a command that timed out. The caller must wait for this one before
    ///< re-issuing: the window is the Controller's and the Host does not track it
    void (*setChannelClassificationComplete)(uint8_t status, void *context);
    ///< the reassembled measurement set of one CS subevent, from its 0x31 and every
    ///< 0x32 continuing it. A CS test's results arrive here too, under handle 0x0FFF.
    ///< The report is the engine's own storage, reused for that connection's next
    ///< subevent, so a callback that keeps anything must copy it out before returning.
    ///< Calling CSIF_LeCsDisableSession on that connection, or re-registering this
    ///< callback, from inside this one is allowed: the engine defers releasing the
    ///< report until the callback has returned rather than freeing it under the reader
    void (*subeventResult)(const CsSubeventReport *report, void *context);
    ///< the CS session ended because its ACL did. Carries no reason, because the ACL
    ///< layer reports that itself. Not raised when the Host closes the session itself
    ///< (CSIF_LeCsDisableSession) - that is the caller's own doing, not an event
    void (*sessionClosed)(uint16_t connectionHandle, void *context);
    ///< the 0x0095 Command_Complete: status 0x00 means the test started. A test already
    ///< enabled draws 0x0C here rather than a local refusal
    void (*testComplete)(uint8_t status, void *context);
    ///< the 0x33 LE CS Test End Complete (7.7.65,46 p2463): status 0x00 means the test
    ///< ended and every one of its results has been reported. A nonzero status leaves a
    ///< test running that is still running. The caller waits for this before starting
    ///< the next test, and does not have to call CSIF_LeCsTestEnd again: a BT_ALREADY
    ///< from that call means one is already on the wire
    void (*testEndComplete)(uint8_t status, void *context);
} CsLeCallback;

/**
 * @brief       Register the CS session result callback
 * @details     Single slot: a second registration replaces the first. callback may be
 *              NULL, which clears the slot and releases every buffered report while the
 *              HCI event layer is up, so a later registration starts from an empty
 *              buffer rather than inheriting a half-absorbed subevent; with that layer
 *              closed nothing is cleared and the call reports BT_NOT_SUPPORT. callback
 *              and context must both stay valid until deregistered; the structure is
 *              copied, so the caller's own storage need not outlive the call. The HCI
 *              event layer must be up (HciInitEvent) before this is called, which is
 *              after BTM_Initialize's module loop - CSIF_LeCsEnableSession may already
 *              have run by then. The three engine registrations this forwards to return
 *              void, so the event-layer check is made here, ahead of them:
 *              HciGetEventCallbackListLock is a plain getter of the very lock those
 *              three take, so a NULL one means none would apply.
 * @return      BT_SUCCESS on a registration applied, BT_NOT_SUPPORT when the HCI event
 *              layer is not up yet
 */
BTSTACK_API int CSIF_RegisterLeCsCallback(const CsLeCallback *callback, void *context);

/**
 * @brief       Clear the registered callback
 * @details     Releases every buffered report, as registering with a NULL callback
 *              does. A no-op when none is registered. With the HCI event layer already
 *              closed nothing is released and the slot stays as it is: the caller must
 *              deregister again once the event layer is back, and must not release its
 *              context on the failure.
 * @return      BT_SUCCESS on a deregistration applied, BT_NOT_SUPPORT when the HCI
 *              event layer is already closed
 */
BTSTACK_API int CSIF_DeregisterLeCsCallback(void);

/**
 * @brief       Open a CS session on an existing LE ACL
 * @details     Claims the connection handle for CS work and, once per stack lifetime,
 *              reads the local capabilities with 0x0089 if they are not cached yet.
 *              That read is the engine's own housekeeping, not the caller's request:
 *              its failure does not fail the open, and a later open retries it. No CS
 *              command cancels a session, so closing one only drops the Host's record.
 * @param       connectionHandle  an LE ACL handle, 0x0000-0x0EFF
 * @return      BT_SUCCESS once the handle is claimed; the ACL layer's result for a
 *              handle with no live ACL; BT_NO_MEMORY when the session table is full. A
 *              second open of the same handle is idempotent
 */
BTSTACK_API int CSIF_LeCsEnableSession(uint16_t connectionHandle);

/**
 * @brief       Release the CS session held for connectionHandle
 * @details     The Controller's view does not change and the peer's cached capabilities
 *              and FAE table stay valid for the handle, because closing a session does
 *              not take them away from the Controller. Closes only what
 *              CSIF_LeCsEnableSession opened: an unsolicited 0x2C/0x2F/0x30/0x0093 on the
 *              handle leaves the engine holding a row for it, and that row is not a
 *              session.
 * @return      BT_SUCCESS on a session released, BT_NOT_SUPPORT when none was held
 */
BTSTACK_API int CSIF_LeCsDisableSession(uint16_t connectionHandle);

/**
 * @brief       Read the local CS capabilities cached from the 0x0089 result
 * @details     A pure cache read: the 0x0089 that fills the cache is sent by
 *              CSIF_LeCsEnableSession. 0x0089 itself is only usable after the Host has
 *              declared CS Host Support (7.8.130 p2798).
 * @param       caps  filled with the capability body on BT_SUCCESS
 * @return      BT_SUCCESS + the body's 20 fields, BT_BAD_PARAM when caps is NULL,
 *              BT_NOT_SUPPORT when no successful 0x0089 has been cached
 */
BTSTACK_API int CSIF_LeCsReadLocalCaps(CsLeCapabilities *caps);

/**
 * @brief       Start the CS Capability Exchange with the peer (0x008A)
 * @details     The result is the 0x2C callback, which caches the peer's capability
 *     body; this call only sends. Re-issuing it after an exchange or a write-back is
 *     cheap: the Controller may answer from its own copy (p2805).
 * @return      the HCI send result, or the ACL layer's result for a handle with no
 *              live ACL
 */
BTSTACK_API int CSIF_LeCsReadRemoteCaps(uint16_t connectionHandle);

/**
 * @brief       Run the CS Mode-0 FAE Table Request procedure (0x008E)
 * @details     The result is the 0x2D callback, which answers Status 0x11 for a peer
 *              that reports no FAE instead of a table. Only Status 0x00 caches a table.
 * @return      the HCI send result, or the ACL layer's result for a handle with no
 *              live ACL
 */
BTSTACK_API int CSIF_LeCsReadRemoteFae(uint16_t connectionHandle);

/**
 * @brief       Write the peer's cached capabilities back to the Controller (0x008B)
 * @details     What lets a reconnection skip the Capability Exchange the peer's cached
 *              body already answers (7.8.132 p2807). The capabilities written are the
 *              ones the engine cached from the peer's own 0x2C, keyed by the peer's
 *              address and kept across a disconnect. Whether a CS configuration already
 *              exists is left to the Controller (0x0C on the Command_Complete); that
 *              outcome is reported through writeCachedRemoteCapabilitiesComplete.
 * @return      the HCI send result; BT_ALREADY once this connection has exchanged
 *              capabilities or written them back (learnedOnHandle already names it);
 *              BT_NOT_SUPPORT when the peer has no cached body; the ACL layer's
 *              result for a handle with no live ACL
 */
BTSTACK_API int CSIF_LeCsWriteBackRemoteCaps(uint16_t connectionHandle);

/**
 * @brief       Write the peer's cached mode-0 FAE table back to the Controller (0x008F)
 * @details     The 0x008F counterpart of CSIF_LeCsWriteBackRemoteCaps: it lets a
 *              reconnection skip the FAE Table Request procedure (7.8.136 pp2820-2821).
 *              As with 0x008B, "CS configuration already created" is Controller-owned
 *              and arrives on writeCachedRemoteFaeTableComplete.
 * @return      the HCI send result; BT_ALREADY once this connection has written that
 *              table back (faeLearnedOnHandle already names it); BT_NOT_SUPPORT when
 *              the peer has no cached table, which is how a No_FAE peer reads here too
 *              since a 0x11 answer caches nothing; the ACL layer's result for a handle
 *              with no live ACL
 */
BTSTACK_API int CSIF_LeCsWriteBackRemoteFae(uint16_t connectionHandle);

/**
 * @brief       Start or restart the CS Security Start procedure (0x008C)
 * @details     The caller must be the Central on an encrypted link; this call does not
 *              check either, so a Peripheral or an unencrypted link is answered by the
 *              Controller with a Command_Status rejection, funneled into the same 0x2E
 *              callback shape as a started procedure that fails (nothing is sent when
 *              the handle has no active ACL, so that case is refused locally instead).
 *              The result is the 0x2E callback either way.
 * @return      the HCI send result, or BT_BAD_STATUS for a handle with no active ACL
 */
BTSTACK_API int CSIF_LeCsSecurityEnable(uint16_t connectionHandle);

/**
 * @brief       Set the per-connection default settings (0x008D)
 * @details     The Role_Enable / CS_SYNC antenna selection / Max_TX_Power a later
 *              0x0090 is judged against (7.8.134 p2816). The Controller owns them and
 *              they do not outlive the connection, so the Host keeps no copy. The
 *              result is the Command_Complete, reported through
 *              setDefaultSettingsComplete: this call returns as soon as the command is
 *              on the wire, and a Controller that refuses the defaults (0x11 an
 *              unsupported value, 0x12 a role not acceptable in the connection's state)
 *              reports it through that same callback rather than here.
 * @return      the HCI send result, BT_BAD_PARAM when param is NULL, or BT_BAD_STATUS for a
 *              handle with no active ACL
 */
BTSTACK_API int CSIF_LeCsSetDefaultSettings(const CsLeSetDefaultSettingsParam *param);

/**
 * @brief       Create a CS configuration on a Config_ID (0x0090)
 * @details     The result is the 0x2F callback with action 0x01, not this call, which
 *              returns as soon as the command is on the wire. The Controller decides
 *              every relationship guard (0x11 unsupported value, 0x12 invalid
 *              configuration or a role not enabled by 0x008D, 0x0C already enabled).
 * @return      the HCI send result, BT_BAD_PARAM when param is NULL, or BT_BAD_STATUS for a
 *              handle with no active ACL
 */
BTSTACK_API int CSIF_LeCsCreateConfig(const CsLeCreateConfigParam *param);

/**
 * @brief       Remove a CS configuration (0x0091)
 * @details     The result is the 0x2F callback with action 0x00, which drops that
 *              slot's state including the procedure parameters a later 0x0093 would
 *              have set. Whether the Config_ID exists (0x12) or a procedure is still
 *              enabled on it (0x0C) is the Controller's answer.
 * @return      the HCI send result, BT_BAD_PARAM when param is NULL, or BT_BAD_STATUS for a
 *              handle with no active ACL
 */
BTSTACK_API int CSIF_LeCsRemoveConfig(const CsLeRemoveConfigParam *param);

/**
 * @brief       Replace the Controller-wide CS channel classification (0x0092)
 * @details     The only CS command with no Connection_Handle: the classification
 *              outlives any link and the Controller may combine it with its own
 *              information (7.8.139 p2830). The reserved channels must be zero and at
 *              least 15 channels enabled, which is the same rule behind the
 *              Controller's 0x12, so a malformed map is refused before the wire. The
 *              result is the Command_Complete, reported through
 *              setChannelClassificationComplete; a repeat inside the Controller's
 *              1-second window then arrives there as 0x0C, and that callback is the
 *              only place the caller can see it.
 * @return      the HCI send result, BT_BAD_PARAM for a NULL or malformed map
 */
BTSTACK_API int CSIF_LeCsSetChannelClassification(const CsLeSetChannelClassificationParam *param);

/**
 * @brief       Set the scheduling parameters a later 0x0094 enables (0x0093)
 * @details     The result is the Command_Complete, reported through
 *              procedureParametersComplete. It names only the connection handle -
 *              Config_ID is not echoed - so a caller correlates it with its own
 *              request. Whether the Config_ID exists (0x12), already has a procedure
 *              enabled (0x0C) or conflicts with the Controller's scheduling (0x0D) is
 *              the Controller's answer.
 * @return      the HCI send result, BT_BAD_PARAM when param is NULL; BT_ALREADY when a
 *              previous 0x0093 is still outstanding on the same handle, because the two
 *              identical Command_Completes could not be told apart; BT_BAD_STATUS for a
 *              handle with no active ACL; BT_NO_MEMORY when the session table is full
 */
BTSTACK_API int CSIF_LeCsSetProcedureParameters(const CsLeSetProcedureParametersParam *param);

/**
 * @brief       Start or stop the CS procedure on a Config_ID (0x0094)
 * @details     The result is the 0x30 callback, not this call. Enabling a Config_ID
 *              the session holds no slot for, or one whose 0x0093 has not been
 *              accepted, is refused before the wire and reported through that same
 *              callback with the shape a Controller rejection has, so the caller still
 *              gets BT_SUCCESS here and learns the outcome there. Every other guard
 *              (0x0D, 0x48, 0x0C already enabled) is the Controller's and passes
 *              through; the disable path is not guarded at all.
 * @return      the HCI send result, BT_BAD_STATUS for a handle with no active ACL,
 *              BT_BAD_PARAM for a NULL parameter or a Config_ID outside 0x00-0x03
 */
BTSTACK_API int CSIF_LeCsProcedureEnable(const CsLeProcedureEnableParam *param);

/**
 * @brief       Start a CS test against a tester (0x0095)
 * @details     A test is a CS procedure the Controller runs with no peer: no session is
 *              involved, the command carries no Connection_Handle, and the results
 *              arrive as ordinary subeventResult reports under the reserved handle
 *              0x0FFF (7.7.65,44 p2448). The outcome is testComplete, whose return is
 *              Status alone - the parameters are not echoed - so the engine records the
 *              Role and RTT_Type when it sends, and they become the running test's only
 *              when status 0x00 comes back. A test already in progress is not refused
 *              here: the Controller answers the 0x0C of p2843 on its own, and a refused
 *              start leaves the test in progress untouched.
 * @return      the HCI send result; BT_ALREADY when a previous 0x0095 is still
 *              outstanding (two status-only Command_Completes could not be told apart)
 *              or when a 0x0096 is outstanding - the 0x33 that closes it can land
 *              after this Command_Complete and would wipe the Role and RTT_Type it just
 *              recorded, so a caller refused that way retries after testEndComplete;
 *              BT_BAD_PARAM for a NULL parameter
 */
BTSTACK_API int CSIF_LeCsTestStart(const CsLeTestParam *param);

/**
 * @brief       End the CS test in progress (0x0096)
 * @details     The command carries no parameter and the engine keeps nothing from it:
 *              its outcome is the 0x33 LE CS Test End Complete, which the Controller
 *              sends only once every result of that test has been reported (7.8.143
 *              p2855) and which arrives as testEndComplete. Calling it with no test
 *              running is left to the Controller, whose 0x0C (p2855) reaches the
 *              caller through that same callback.
 * @return      the HCI send result, or BT_ALREADY while a previous 0x0096 has neither
 *              been answered by a 0x33 nor failed locally
 */
BTSTACK_API int CSIF_LeCsTestEnd(void);

#ifdef __cplusplus
}
#endif

#endif
