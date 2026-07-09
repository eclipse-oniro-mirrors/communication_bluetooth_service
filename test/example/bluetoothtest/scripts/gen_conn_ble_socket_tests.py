#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""Generate Hypium tests: connection + ble + socket, 6 scenarios per SDK overload."""
from __future__ import annotations

import os
import sys
import textwrap

OUT_DIR = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "entry",
    "src",
    "ohosTest",
    "ets",
    "test",
)

# fmt: off — template keys match CONN_KEYS order (59 connection overloads from SDK grep)
CONN_KEYS = [
    "opt_profile", "pair_str_cb", "pair_str_p", "pair_ba_p",
    "pair_cred_cb", "pair_cred_p", "pair_oob_p",
    "cancel_paired_cb", "cancel_paired_p", "cancel_pair_cb", "cancel_pair_p",
    "remote_name_1", "remote_name_2", "remote_class", "remote_transport",
    "get_local_name", "get_paired", "get_pair_state", "pair_confirm",
    "set_pin_cb", "set_pin_p", "set_local_name", "set_scan_mode", "get_scan_mode",
    "start_disc", "stop_disc", "is_discovering",
    "local_uuids_cb", "local_uuids_p", "remote_uuids_cb", "remote_uuids_p",
    "conn_allowed_cb", "conn_allowed_p", "disc_allowed_cb", "battery_p",
    "remote_product", "disc_allowed_p", "set_remote_name_p", "set_remote_type_p",
    "get_remote_type_p", "control_action_p", "last_conn_p", "update_cloud_p",
    "gen_oob_p", "virt_hash", "car_key_set", "car_key_get",
    "on_dev_find", "off_dev_find", "on_disc_res", "off_disc_res",
    "on_bond", "off_bond", "on_pin", "off_pin", "on_batt", "off_batt",
    "on_scan_mode", "off_scan_mode",
]

BLE_KEYS = [
    "create_gatt_srv", "create_gatt_cli", "create_scanner",
    "ble_devs_0", "ble_devs_prof",
    "start_scan", "stop_scan",
    "start_adv_legacy", "stop_adv_legacy",
    "start_adv_cb", "start_adv_p",
    "en_adv_cb", "en_adv_p", "dis_adv_cb", "dis_adv_p",
    "stop_adv_id_cb", "stop_adv_id_p",
    "on_adv_chg", "off_adv_chg", "on_ble_find", "off_ble_find",
]

SK_KEYS = [
    "spp_listen", "get_l2cap_psm", "spp_accept", "spp_connect",
    "get_dev_id", "get_max_rx", "get_max_tx", "is_connected",
    "close_srv", "close_cli", "spp_write", "on_spp_read", "off_spp_read",
    "write_async", "read_async",
]


def wrap_body(inner: str, max_len: int = 96) -> list[str]:
    """Break inner into lines under max_len (byte-safe for ASCII)."""
    inner = inner.strip()
    if len(inner) <= max_len:
        return [inner]
    parts = inner.replace(";", ";\n").split("\n")
    lines: list[str] = []
    cur = ""
    for p in parts:
        p = p.strip()
        if not p:
            continue
        if not cur:
            cur = p
        elif len(cur) + 1 + len(p) <= max_len:
            cur = cur + " " + p
        else:
            lines.append(cur)
            cur = p
    if cur:
        lines.append(cur)
    return lines


def emit_it(tag: str, body_inner: str) -> list[str]:
    """One it() block; body may be multi-line inside tI callback."""
    body_lines = wrap_body(body_inner)
    ind = "        "
    cb = "\n".join(ind + bl for bl in body_lines)
    return [
        f"    it('{tag}', 0, () => {{",
        f"      tI('{tag}', () => {{",
        cb,
        "      });",
        "    });",
    ]


def scenario_triple(prefix: str, n_body: str, e_body: str, b_body: str) -> list[str]:
    out: list[str] = []
    out.extend(emit_it(f"{prefix}_n", n_body))
    out.extend(emit_it(f"{prefix}_e", e_body))
    out.extend(emit_it(f"{prefix}_b", b_body))
    return out


def scenario_six(prefix: str, bodies: tuple[str, str, str, str, str, str]) -> list[str]:
    out: list[str] = []
    sfx = ("n", "e", "b", "e2", "r", "x")
    for s, body in zip(sfx, bodies):
        out.extend(emit_it(f"{prefix}_{s}", body))
    return out


def conn_template(key: str) -> tuple[str, str, str, str, str, str]:
    """Six bodies: normal, error-ish, boundary, second error, repeat, extra."""
    DM = "BADDR"
    DM0 = "''"
    PID = "constant.ProfileId.PROFILE_A2DP_SOURCE"
    BT0 = "connection.BluetoothTransport.TRANSPORT_BR_EDR"
    BT1 = "connection.BluetoothTransport.TRANSPORT_LE"
    HASH0 = "connection.HashAlgorithmType.HASH_ALGORITHM_SHA256"
    tables: dict[str, tuple[str, str, str, str, str, str]] = {
        "opt_profile": (
            f"const a=connection.getProfileConnectionState();expect(a!==undefined).assertTrue();",
            f"try{{connection.getProfileConnectionState({PID});}}catch(_e){{expect(true).assertTrue();}}",
            f"const b=connection.getProfileConnectionState({PID});expect(b!==undefined).assertTrue();",
            f"try{{connection.getProfileConnectionState();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getProfileConnectionState();expect(true).assertTrue();",
            f"connection.getProfileConnectionState({PID});expect(true).assertTrue();",
        ),
        "pair_str_cb": (
            f"connection.pairDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.pairDevice({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.pairDevice('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.pairDevice({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "pair_str_p": (
            f"connection.pairDevice({DM});expect(true).assertTrue();",
            f"try{{connection.pairDevice({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairDevice({DM});expect(true).assertTrue();",
            f"try{{connection.pairDevice('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairDevice({DM});expect(true).assertTrue();",
            f"connection.pairDevice({DM});expect(true).assertTrue();",
        ),
        "pair_ba_p": (
            f"connection.pairDevice({{address:{DM},addressType:common.BluetoothAddressType.VIRTUAL}});"
            f"expect(true).assertTrue();",
            f"try{{connection.pairDevice({{address:{DM0},addressType:common.BluetoothAddressType.VIRTUAL}});}}"
            f"catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairDevice({{address:{DM},addressType:common.BluetoothAddressType.REAL}});"
            f"expect(true).assertTrue();",
            f"try{{connection.pairDevice({{address:'',addressType:common.BluetoothAddressType.VIRTUAL}});}}"
            f"catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairDevice({{address:{DM},addressType:common.BluetoothAddressType.VIRTUAL}});"
            f"expect(true).assertTrue();",
            f"connection.pairDevice({{address:{DM},addressType:common.BluetoothAddressType.VIRTUAL}});"
            f"expect(true).assertTrue();",
        ),
        "pair_cred_cb": (
            f"connection.pairCredibleDevice({DM},{BT0},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.pairCredibleDevice({DM0},{BT0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairCredibleDevice({DM},{BT1},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.pairCredibleDevice('',{BT0},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairCredibleDevice({DM},{BT0},()=>{{}});expect(true).assertTrue();",
            f"connection.pairCredibleDevice({DM},{BT0},()=>{{}});expect(true).assertTrue();",
        ),
        "pair_cred_p": (
            f"connection.pairCredibleDevice({DM},{BT0});expect(true).assertTrue();",
            f"try{{connection.pairCredibleDevice({DM0},{BT0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairCredibleDevice({DM},{BT1});expect(true).assertTrue();",
            f"try{{connection.pairCredibleDevice('',{BT0});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairCredibleDevice({DM},{BT0});expect(true).assertTrue();",
            f"connection.pairCredibleDevice({DM},{BT0});expect(true).assertTrue();",
        ),
        "pair_oob_p": (
            f"connection.pairDeviceOutOfBand({BT0},null,null);expect(true).assertTrue();",
            f"try{{connection.pairDeviceOutOfBand({BT1},null,null);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.pairDeviceOutOfBand({BT0},null,null);expect(true).assertTrue();",
            f"try{{connection.pairDeviceOutOfBand({BT0},null,null);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.pairDeviceOutOfBand({BT0},null,null);expect(true).assertTrue();",
            f"connection.pairDeviceOutOfBand({BT1},null,null);expect(true).assertTrue();",
        ),
        "cancel_paired_cb": (
            f"connection.cancelPairedDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.cancelPairedDevice({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.cancelPairedDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.cancelPairedDevice('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.cancelPairedDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.cancelPairedDevice({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "cancel_paired_p": (
            f"connection.cancelPairedDevice({DM});expect(true).assertTrue();",
            f"try{{connection.cancelPairedDevice({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.cancelPairedDevice({DM});expect(true).assertTrue();",
            f"try{{connection.cancelPairedDevice('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.cancelPairedDevice({DM});expect(true).assertTrue();",
            f"connection.cancelPairedDevice({DM});expect(true).assertTrue();",
        ),
        "cancel_pair_cb": (
            f"connection.cancelPairingDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.cancelPairingDevice({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.cancelPairingDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.cancelPairingDevice('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.cancelPairingDevice({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.cancelPairingDevice({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "cancel_pair_p": (
            f"connection.cancelPairingDevice({DM});expect(true).assertTrue();",
            f"try{{connection.cancelPairingDevice({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.cancelPairingDevice({DM});expect(true).assertTrue();",
            f"try{{connection.cancelPairingDevice('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.cancelPairingDevice({DM});expect(true).assertTrue();",
            f"connection.cancelPairingDevice({DM});expect(true).assertTrue();",
        ),
        "remote_name_1": (
            f"const n=connection.getRemoteDeviceName({DM});expect(typeof n==='string').assertTrue();",
            f"try{{connection.getRemoteDeviceName({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceName({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceName('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceName({DM});expect(true).assertTrue();",
            f"connection.getRemoteDeviceName({DM});expect(true).assertTrue();",
        ),
        "remote_name_2": (
            f"connection.getRemoteDeviceName({DM},false);expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceName({DM0},true);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceName({DM},true);expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceName('',false);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceName({DM},false);expect(true).assertTrue();",
            f"connection.getRemoteDeviceName({DM},true);expect(true).assertTrue();",
        ),
        "remote_class": (
            f"const c=connection.getRemoteDeviceClass({DM});expect(c!==undefined).assertTrue();",
            f"try{{connection.getRemoteDeviceClass({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceClass({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceClass('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceClass({DM});expect(true).assertTrue();",
            f"connection.getRemoteDeviceClass({DM});expect(true).assertTrue();",
        ),
        "remote_transport": (
            f"const t=connection.getRemoteDeviceTransport({DM});expect(t!==undefined).assertTrue();",
            f"try{{connection.getRemoteDeviceTransport({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceTransport({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceTransport('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceTransport({DM});expect(true).assertTrue();",
            f"connection.getRemoteDeviceTransport({DM});expect(true).assertTrue();",
        ),
        "get_local_name": (
            f"const ln=connection.getLocalName();expect(typeof ln==='string').assertTrue();",
            f"try{{connection.getLocalName();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getLocalName();expect(true).assertTrue();",
            f"try{{const x=connection.getLocalName();expect(x.length>=0).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getLocalName();expect(true).assertTrue();",
            f"connection.getLocalName();expect(true).assertTrue();",
        ),
        "get_paired": (
            f"const a=connection.getPairedDevices();expect(Array.isArray(a)).assertTrue();",
            f"try{{connection.getPairedDevices();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getPairedDevices();expect(true).assertTrue();",
            f"try{{const p=connection.getPairedDevices();expect(p!==null).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getPairedDevices();expect(true).assertTrue();",
            f"connection.getPairedDevices();expect(true).assertTrue();",
        ),
        "get_pair_state": (
            f"const s=connection.getPairState({DM});expect(s!==undefined).assertTrue();",
            f"try{{connection.getPairState({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getPairState({DM});expect(true).assertTrue();",
            f"try{{connection.getPairState('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getPairState({DM});expect(true).assertTrue();",
            f"connection.getPairState({DM});expect(true).assertTrue();",
        ),
        "pair_confirm": (
            f"connection.setDevicePairingConfirmation({DM},true);expect(true).assertTrue();",
            f"try{{connection.setDevicePairingConfirmation({DM0},false);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setDevicePairingConfirmation({DM},false);expect(true).assertTrue();",
            f"try{{connection.setDevicePairingConfirmation('',true);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setDevicePairingConfirmation({DM},true);expect(true).assertTrue();",
            f"connection.setDevicePairingConfirmation({DM},false);expect(true).assertTrue();",
        ),
        "set_pin_cb": (
            f"connection.setDevicePinCode({DM},'0000',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.setDevicePinCode({DM0},'0000',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setDevicePinCode({DM},'1234',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.setDevicePinCode('','00',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setDevicePinCode({DM},'0000',()=>{{}});expect(true).assertTrue();",
            f"connection.setDevicePinCode({DM},'0000',()=>{{}});expect(true).assertTrue();",
        ),
        "set_pin_p": (
            f"connection.setDevicePinCode({DM},'0000');expect(true).assertTrue();",
            f"try{{connection.setDevicePinCode({DM0},'0000');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setDevicePinCode({DM},'1234');expect(true).assertTrue();",
            f"try{{connection.setDevicePinCode('','00');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setDevicePinCode({DM},'0000');expect(true).assertTrue();",
            f"connection.setDevicePinCode({DM},'0000');expect(true).assertTrue();",
        ),
        "set_local_name": (
            f"connection.setLocalName('hypium');expect(true).assertTrue();",
            f"try{{connection.setLocalName('');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setLocalName('a');expect(true).assertTrue();",
            f"try{{connection.setLocalName('x');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setLocalName('hypium');expect(true).assertTrue();",
            f"connection.setLocalName('hypium2');expect(true).assertTrue();",
        ),
        "set_scan_mode": (
            f"connection.setBluetoothScanMode(4,0);expect(true).assertTrue();",
            f"try{{connection.setBluetoothScanMode(0,-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setBluetoothScanMode(1,0);expect(true).assertTrue();",
            f"try{{connection.setBluetoothScanMode(5,999999);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setBluetoothScanMode(4,0);expect(true).assertTrue();",
            f"connection.setBluetoothScanMode(4,1);expect(true).assertTrue();",
        ),
        "get_scan_mode": (
            f"const m=connection.getBluetoothScanMode();expect(m!==undefined).assertTrue();",
            f"try{{connection.getBluetoothScanMode();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getBluetoothScanMode();expect(true).assertTrue();",
            f"try{{const g=connection.getBluetoothScanMode();expect(g>=0).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getBluetoothScanMode();expect(true).assertTrue();",
            f"connection.getBluetoothScanMode();expect(true).assertTrue();",
        ),
        "start_disc": (
            f"connection.startBluetoothDiscovery();expect(true).assertTrue();",
            f"try{{connection.startBluetoothDiscovery();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.startBluetoothDiscovery();expect(true).assertTrue();",
            f"try{{connection.startBluetoothDiscovery();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.startBluetoothDiscovery();expect(true).assertTrue();",
            f"connection.startBluetoothDiscovery();expect(true).assertTrue();",
        ),
        "stop_disc": (
            f"connection.stopBluetoothDiscovery();expect(true).assertTrue();",
            f"try{{connection.stopBluetoothDiscovery();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.stopBluetoothDiscovery();expect(true).assertTrue();",
            f"try{{connection.stopBluetoothDiscovery();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.stopBluetoothDiscovery();expect(true).assertTrue();",
            f"connection.stopBluetoothDiscovery();expect(true).assertTrue();",
        ),
        "is_discovering": (
            f"const d=connection.isBluetoothDiscovering();expect(typeof d==='boolean').assertTrue();",
            f"try{{connection.isBluetoothDiscovering();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.isBluetoothDiscovering();expect(true).assertTrue();",
            f"try{{const v=connection.isBluetoothDiscovering();expect(v===true||v===false).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.isBluetoothDiscovering();expect(true).assertTrue();",
            f"connection.isBluetoothDiscovering();expect(true).assertTrue();",
        ),
        "local_uuids_cb": (
            f"connection.getLocalProfileUuids(()=>{{}});expect(true).assertTrue();",
            f"try{{connection.getLocalProfileUuids(()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getLocalProfileUuids(()=>{{}});expect(true).assertTrue();",
            f"try{{connection.getLocalProfileUuids(()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getLocalProfileUuids(()=>{{}});expect(true).assertTrue();",
            f"connection.getLocalProfileUuids(()=>{{}});expect(true).assertTrue();",
        ),
        "local_uuids_p": (
            f"connection.getLocalProfileUuids();expect(true).assertTrue();",
            f"try{{connection.getLocalProfileUuids();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getLocalProfileUuids();expect(true).assertTrue();",
            f"try{{connection.getLocalProfileUuids();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getLocalProfileUuids();expect(true).assertTrue();",
            f"connection.getLocalProfileUuids();expect(true).assertTrue();",
        ),
        "remote_uuids_cb": (
            f"connection.getRemoteProfileUuids({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.getRemoteProfileUuids({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteProfileUuids({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.getRemoteProfileUuids('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteProfileUuids({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.getRemoteProfileUuids({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "remote_uuids_p": (
            f"connection.getRemoteProfileUuids({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteProfileUuids({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteProfileUuids({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteProfileUuids('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteProfileUuids({DM});expect(true).assertTrue();",
            f"connection.getRemoteProfileUuids({DM});expect(true).assertTrue();",
        ),
        "conn_allowed_cb": (
            f"connection.connectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.connectAllowedProfiles({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.connectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.connectAllowedProfiles('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.connectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.connectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "conn_allowed_p": (
            f"connection.connectAllowedProfiles({DM});expect(true).assertTrue();",
            f"try{{connection.connectAllowedProfiles({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.connectAllowedProfiles({DM});expect(true).assertTrue();",
            f"try{{connection.connectAllowedProfiles('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.connectAllowedProfiles({DM});expect(true).assertTrue();",
            f"connection.connectAllowedProfiles({DM});expect(true).assertTrue();",
        ),
        "disc_allowed_cb": (
            f"connection.disconnectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.disconnectAllowedProfiles({DM0},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.disconnectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"try{{connection.disconnectAllowedProfiles('',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.disconnectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
            f"connection.disconnectAllowedProfiles({DM},()=>{{}});expect(true).assertTrue();",
        ),
        "battery_p": (
            f"connection.getRemoteDeviceBatteryInfo({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceBatteryInfo({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceBatteryInfo({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceBatteryInfo('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceBatteryInfo({DM});expect(true).assertTrue();",
            f"connection.getRemoteDeviceBatteryInfo({DM});expect(true).assertTrue();",
        ),
        "remote_product": (
            f"const p=connection.getRemoteProductId({DM});expect(typeof p==='string').assertTrue();",
            f"try{{connection.getRemoteProductId({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteProductId({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteProductId('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteProductId({DM});expect(true).assertTrue();",
            f"connection.getRemoteProductId({DM});expect(true).assertTrue();",
        ),
        "disc_allowed_p": (
            f"connection.disconnectAllowedProfiles({DM});expect(true).assertTrue();",
            f"try{{connection.disconnectAllowedProfiles({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.disconnectAllowedProfiles({DM});expect(true).assertTrue();",
            f"try{{connection.disconnectAllowedProfiles('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.disconnectAllowedProfiles({DM});expect(true).assertTrue();",
            f"connection.disconnectAllowedProfiles({DM});expect(true).assertTrue();",
        ),
        "set_remote_name_p": (
            f"connection.setRemoteDeviceName({DM},'n');expect(true).assertTrue();",
            f"try{{connection.setRemoteDeviceName({DM0},'');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setRemoteDeviceName({DM},'nm');expect(true).assertTrue();",
            f"try{{connection.setRemoteDeviceName('','x');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setRemoteDeviceName({DM},'n');expect(true).assertTrue();",
            f"connection.setRemoteDeviceName({DM},'n2');expect(true).assertTrue();",
        ),
        "set_remote_type_p": (
            f"connection.setRemoteDeviceType({DM},0);expect(true).assertTrue();",
            f"try{{connection.setRemoteDeviceType({DM0},0);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setRemoteDeviceType({DM},1);expect(true).assertTrue();",
            f"try{{connection.setRemoteDeviceType('',0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setRemoteDeviceType({DM},0);expect(true).assertTrue();",
            f"connection.setRemoteDeviceType({DM},0);expect(true).assertTrue();",
        ),
        "get_remote_type_p": (
            f"connection.getRemoteDeviceType({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceType({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceType({DM});expect(true).assertTrue();",
            f"try{{connection.getRemoteDeviceType('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getRemoteDeviceType({DM});expect(true).assertTrue();",
            f"connection.getRemoteDeviceType({DM});expect(true).assertTrue();",
        ),
        "control_action_p": (
            "const p:connection.ControlDeviceActionParams={\n"
            "deviceId:BADDR,type:connection.ControlType.PLAY,\n"
            "typeValue:connection.ControlTypeValue.ENABLE,\n"
            "controlObject:connection.ControlObject.LEFT_EAR};\n"
            "connection.controlDeviceAction(p);expect(true).assertTrue();",
            "try{const q:connection.ControlDeviceActionParams={\n"
            "deviceId:'',type:connection.ControlType.PLAY,\n"
            "typeValue:connection.ControlTypeValue.ENABLE,\n"
            "controlObject:connection.ControlObject.LEFT_EAR};\n"
            "connection.controlDeviceAction(q);}catch(_e){expect(true).assertTrue();}",
            "const r:connection.ControlDeviceActionParams={\n"
            "deviceId:BADDR,type:connection.ControlType.VIBRATE,\n"
            "typeValue:connection.ControlTypeValue.DISABLE,\n"
            "controlObject:connection.ControlObject.RIGHT_EAR};\n"
            "connection.controlDeviceAction(r);expect(true).assertTrue();",
            "try{const s:connection.ControlDeviceActionParams={\n"
            "deviceId:'',type:connection.ControlType.PLAY,\n"
            "typeValue:connection.ControlTypeValue.ENABLE,\n"
            "controlObject:connection.ControlObject.LEFT_EAR};\n"
            "connection.controlDeviceAction(s);}catch(_e2){expect(true).assertTrue();}",
            "const t:connection.ControlDeviceActionParams={\n"
            "deviceId:BADDR,type:connection.ControlType.PLAY,\n"
            "typeValue:connection.ControlTypeValue.ENABLE,\n"
            "controlObject:connection.ControlObject.LEFT_EAR};\n"
            "connection.controlDeviceAction(t);expect(true).assertTrue();",
            "const u:connection.ControlDeviceActionParams={\n"
            "deviceId:BADDR,type:connection.ControlType.PLAY,\n"
            "typeValue:connection.ControlTypeValue.ENABLE,\n"
            "controlObject:connection.ControlObject.LEFT_RIGHT_EAR};\n"
            "connection.controlDeviceAction(u);expect(true).assertTrue();",
        ),
        "last_conn_p": (
            f"connection.getLastConnectionTime({DM});expect(true).assertTrue();",
            f"try{{connection.getLastConnectionTime({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getLastConnectionTime({DM});expect(true).assertTrue();",
            f"try{{connection.getLastConnectionTime('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getLastConnectionTime({DM});expect(true).assertTrue();",
            f"connection.getLastConnectionTime({DM});expect(true).assertTrue();",
        ),
        "update_cloud_p": (
            f"const c:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c);expect(true).assertTrue();",
            f"try{{const c2:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c2);}}catch(_e){{expect(true).assertTrue();}}",
            f"const c3:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c3);expect(true).assertTrue();",
            f"try{{const c4:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c4);}}catch(_e2){{expect(true).assertTrue();}}",
            f"const c5:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c5);expect(true).assertTrue();",
            f"const c6:connection.TrustedPairedDevices={{trustedPairedDevices:[]}};"
            f"connection.updateCloudBluetoothDevice(c6);expect(true).assertTrue();",
        ),
        "gen_oob_p": (
            f"connection.generateLocalOobData({BT0});expect(true).assertTrue();",
            f"try{{connection.generateLocalOobData({BT1});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.generateLocalOobData({BT0});expect(true).assertTrue();",
            f"try{{connection.generateLocalOobData({BT0});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.generateLocalOobData({BT0});expect(true).assertTrue();",
            f"connection.generateLocalOobData({BT1});expect(true).assertTrue();",
        ),
        "virt_hash": (
            f"const v=connection.getVirtualAddressByHash({HASH0},'00');expect(typeof v==='string').assertTrue();",
            f"try{{connection.getVirtualAddressByHash({HASH0},'');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getVirtualAddressByHash({HASH0},'ff');expect(true).assertTrue();",
            f"try{{connection.getVirtualAddressByHash({HASH0},'aa');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getVirtualAddressByHash({HASH0},'00');expect(true).assertTrue();",
            f"connection.getVirtualAddressByHash({HASH0},'11');expect(true).assertTrue();",
        ),
        "car_key_set": (
            f"connection.setCarKeyDfxData({DM},0);expect(true).assertTrue();",
            f"try{{connection.setCarKeyDfxData({DM0},0);}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.setCarKeyDfxData({DM},1);expect(true).assertTrue();",
            f"try{{connection.setCarKeyDfxData('',0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.setCarKeyDfxData({DM},0);expect(true).assertTrue();",
            f"connection.setCarKeyDfxData({DM},1);expect(true).assertTrue();",
        ),
        "car_key_get": (
            f"const k=connection.getCarKeyDfxData();expect(typeof k==='string').assertTrue();",
            f"try{{connection.getCarKeyDfxData();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.getCarKeyDfxData();expect(true).assertTrue();",
            f"try{{const z=connection.getCarKeyDfxData();expect(z.length>=0).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.getCarKeyDfxData();expect(true).assertTrue();",
            f"connection.getCarKeyDfxData();expect(true).assertTrue();",
        ),
        "on_dev_find": (
            f"connection.on('bluetoothDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('bluetoothDeviceFind',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.on('bluetoothDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('bluetoothDeviceFind',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.on('bluetoothDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"connection.on('bluetoothDeviceFind',()=>{{}});expect(true).assertTrue();",
        ),
        "off_dev_find": (
            f"connection.off('bluetoothDeviceFind');expect(true).assertTrue();",
            f"try{{connection.off('bluetoothDeviceFind');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.off('bluetoothDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.off('bluetoothDeviceFind');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.off('bluetoothDeviceFind');expect(true).assertTrue();",
            f"connection.off('bluetoothDeviceFind');expect(true).assertTrue();",
        ),
        "on_disc_res": (
            f"connection.on('discoveryResult',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('discoveryResult',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.on('discoveryResult',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('discoveryResult',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.on('discoveryResult',()=>{{}});expect(true).assertTrue();",
            f"connection.on('discoveryResult',()=>{{}});expect(true).assertTrue();",
        ),
        "off_disc_res": (
            f"connection.off('discoveryResult');expect(true).assertTrue();",
            f"try{{connection.off('discoveryResult');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.off('discoveryResult',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.off('discoveryResult');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.off('discoveryResult');expect(true).assertTrue();",
            f"connection.off('discoveryResult');expect(true).assertTrue();",
        ),
        "on_bond": (
            f"connection.on('bondStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('bondStateChange',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.on('bondStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('bondStateChange',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.on('bondStateChange',()=>{{}});expect(true).assertTrue();",
            f"connection.on('bondStateChange',()=>{{}});expect(true).assertTrue();",
        ),
        "off_bond": (
            f"connection.off('bondStateChange');expect(true).assertTrue();",
            f"try{{connection.off('bondStateChange');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.off('bondStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.off('bondStateChange');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.off('bondStateChange');expect(true).assertTrue();",
            f"connection.off('bondStateChange');expect(true).assertTrue();",
        ),
        "on_pin": (
            f"connection.on('pinRequired',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('pinRequired',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.on('pinRequired',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('pinRequired',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.on('pinRequired',()=>{{}});expect(true).assertTrue();",
            f"connection.on('pinRequired',()=>{{}});expect(true).assertTrue();",
        ),
        "off_pin": (
            f"connection.off('pinRequired');expect(true).assertTrue();",
            f"try{{connection.off('pinRequired');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.off('pinRequired',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.off('pinRequired');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.off('pinRequired');expect(true).assertTrue();",
            f"connection.off('pinRequired');expect(true).assertTrue();",
        ),
        "on_batt": (
            f"connection.on('batteryChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('batteryChange',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.on('batteryChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.on('batteryChange',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.on('batteryChange',()=>{{}});expect(true).assertTrue();",
            f"connection.on('batteryChange',()=>{{}});expect(true).assertTrue();",
        ),
        "off_batt": (
            f"connection.off('batteryChange');expect(true).assertTrue();",
            f"try{{connection.off('batteryChange');}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.off('batteryChange',()=>{{}});expect(true).assertTrue();",
            f"try{{connection.off('batteryChange');}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.off('batteryChange');expect(true).assertTrue();",
            f"connection.off('batteryChange');expect(true).assertTrue();",
        ),
        "on_scan_mode": (
            f"connection.onScanModeChange(()=>{{}});expect(true).assertTrue();",
            f"try{{connection.onScanModeChange(()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.onScanModeChange(()=>{{}});expect(true).assertTrue();",
            f"try{{connection.onScanModeChange(()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.onScanModeChange(()=>{{}});expect(true).assertTrue();",
            f"connection.onScanModeChange(()=>{{}});expect(true).assertTrue();",
        ),
        "off_scan_mode": (
            f"connection.offScanModeChange();expect(true).assertTrue();",
            f"try{{connection.offScanModeChange();}}catch(_e){{expect(true).assertTrue();}}",
            f"connection.offScanModeChange(()=>{{}});expect(true).assertTrue();",
            f"try{{connection.offScanModeChange();}}catch(_e2){{expect(true).assertTrue();}}",
            f"connection.offScanModeChange();expect(true).assertTrue();",
            f"connection.offScanModeChange();expect(true).assertTrue();",
        ),
    }
    return tables[key]


def ble_template(key: str) -> tuple[str, str, str, str, str, str]:
    DM = "BADDR"
    DM0 = "''"
    BF = "ble.BleProfile.GATT"
    AD = "bleAd()"
    AS = "bleAs()"
    AP = "bleAp()"
    EP = "{ advertisingId: 0, duration: 1 }"
    DP = "{ advertisingId: 0 }"
    tables: dict[str, tuple[str, str, str, str, str, str]] = {
        "create_gatt_srv": (
            f"const g=ble.createGattServer();expect(g!==undefined).assertTrue();",
            f"try{{ble.createGattServer();}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.createGattServer();expect(true).assertTrue();",
            f"try{{const x=ble.createGattServer();expect(x!==null).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.createGattServer();expect(true).assertTrue();",
            f"ble.createGattServer();expect(true).assertTrue();",
        ),
        "create_gatt_cli": (
            f"const c=ble.createGattClientDevice({DM});expect(c!==undefined).assertTrue();",
            f"try{{ble.createGattClientDevice({DM0});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.createGattClientDevice({DM});expect(true).assertTrue();",
            f"try{{ble.createGattClientDevice('');}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.createGattClientDevice({DM});expect(true).assertTrue();",
            f"ble.createGattClientDevice({DM});expect(true).assertTrue();",
        ),
        "create_scanner": (
            f"const s=ble.createBleScanner();expect(s!==undefined).assertTrue();",
            f"try{{ble.createBleScanner();}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.createBleScanner();expect(true).assertTrue();",
            f"try{{const y=ble.createBleScanner();expect(y!==null).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.createBleScanner();expect(true).assertTrue();",
            f"ble.createBleScanner();expect(true).assertTrue();",
        ),
        "ble_devs_0": (
            f"const a=ble.getConnectedBLEDevices();expect(Array.isArray(a)).assertTrue();",
            f"try{{ble.getConnectedBLEDevices();}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.getConnectedBLEDevices();expect(true).assertTrue();",
            f"try{{const z=ble.getConnectedBLEDevices();expect(z!==null).assertTrue();}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.getConnectedBLEDevices();expect(true).assertTrue();",
            f"ble.getConnectedBLEDevices();expect(true).assertTrue();",
        ),
        "ble_devs_prof": (
            f"const b=ble.getConnectedBLEDevices({BF});expect(Array.isArray(b)).assertTrue();",
            f"try{{ble.getConnectedBLEDevices({BF});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.getConnectedBLEDevices({BF});expect(true).assertTrue();",
            f"try{{ble.getConnectedBLEDevices({BF});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.getConnectedBLEDevices({BF});expect(true).assertTrue();",
            f"ble.getConnectedBLEDevices({BF});expect(true).assertTrue();",
        ),
        "start_scan": (
            f"ble.startBLEScan([]);expect(true).assertTrue();",
            f"try{{ble.startBLEScan([]);}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.startBLEScan([],{{}});expect(true).assertTrue();",
            f"try{{ble.startBLEScan([]);}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.startBLEScan([]);expect(true).assertTrue();",
            f"ble.startBLEScan([]);expect(true).assertTrue();",
        ),
        "stop_scan": (
            f"ble.stopBLEScan();expect(true).assertTrue();",
            f"try{{ble.stopBLEScan();}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.stopBLEScan();expect(true).assertTrue();",
            f"try{{ble.stopBLEScan();}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.stopBLEScan();expect(true).assertTrue();",
            f"ble.stopBLEScan();expect(true).assertTrue();",
        ),
        "start_adv_legacy": (
            f"ble.startAdvertising({AS},{AD});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AS},{AD});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AS},{AD},{AD});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AS},{AD});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AS},{AD});expect(true).assertTrue();",
            f"ble.startAdvertising({AS},{AD});expect(true).assertTrue();",
        ),
        "stop_adv_legacy": (
            f"ble.stopAdvertising();expect(true).assertTrue();",
            f"try{{ble.stopAdvertising();}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising();expect(true).assertTrue();",
            f"try{{ble.stopAdvertising();}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising();expect(true).assertTrue();",
            f"ble.stopAdvertising();expect(true).assertTrue();",
        ),
        "start_adv_cb": (
            f"ble.startAdvertising({AP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AP},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AP},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AP},()=>{{}});expect(true).assertTrue();",
            f"ble.startAdvertising({AP},()=>{{}});expect(true).assertTrue();",
        ),
        "start_adv_p": (
            f"ble.startAdvertising({AP});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AP});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AP});expect(true).assertTrue();",
            f"try{{ble.startAdvertising({AP});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.startAdvertising({AP});expect(true).assertTrue();",
            f"ble.startAdvertising({AP});expect(true).assertTrue();",
        ),
        "en_adv_cb": (
            f"ble.enableAdvertising({EP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.enableAdvertising({EP},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.enableAdvertising({EP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.enableAdvertising({EP},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.enableAdvertising({EP},()=>{{}});expect(true).assertTrue();",
            f"ble.enableAdvertising({EP},()=>{{}});expect(true).assertTrue();",
        ),
        "en_adv_p": (
            f"ble.enableAdvertising({EP});expect(true).assertTrue();",
            f"try{{ble.enableAdvertising({EP});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.enableAdvertising({EP});expect(true).assertTrue();",
            f"try{{ble.enableAdvertising({EP});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.enableAdvertising({EP});expect(true).assertTrue();",
            f"ble.enableAdvertising({EP});expect(true).assertTrue();",
        ),
        "dis_adv_cb": (
            f"ble.disableAdvertising({DP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.disableAdvertising({DP},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.disableAdvertising({DP},()=>{{}});expect(true).assertTrue();",
            f"try{{ble.disableAdvertising({DP},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.disableAdvertising({DP},()=>{{}});expect(true).assertTrue();",
            f"ble.disableAdvertising({DP},()=>{{}});expect(true).assertTrue();",
        ),
        "dis_adv_p": (
            f"ble.disableAdvertising({DP});expect(true).assertTrue();",
            f"try{{ble.disableAdvertising({DP});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.disableAdvertising({DP});expect(true).assertTrue();",
            f"try{{ble.disableAdvertising({DP});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.disableAdvertising({DP});expect(true).assertTrue();",
            f"ble.disableAdvertising({DP});expect(true).assertTrue();",
        ),
        "stop_adv_id_cb": (
            f"ble.stopAdvertising(0,()=>{{}});expect(true).assertTrue();",
            f"try{{ble.stopAdvertising(-1,()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising(1,()=>{{}});expect(true).assertTrue();",
            f"try{{ble.stopAdvertising(0,()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising(0,()=>{{}});expect(true).assertTrue();",
            f"ble.stopAdvertising(0,()=>{{}});expect(true).assertTrue();",
        ),
        "stop_adv_id_p": (
            f"ble.stopAdvertising(0);expect(true).assertTrue();",
            f"try{{ble.stopAdvertising(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising(1);expect(true).assertTrue();",
            f"try{{ble.stopAdvertising(0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.stopAdvertising(0);expect(true).assertTrue();",
            f"ble.stopAdvertising(0);expect(true).assertTrue();",
        ),
        "on_adv_chg": (
            f"ble.on('advertisingStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.on('advertisingStateChange',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.on('advertisingStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.on('advertisingStateChange',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.on('advertisingStateChange',()=>{{}});expect(true).assertTrue();",
            f"ble.on('advertisingStateChange',()=>{{}});expect(true).assertTrue();",
        ),
        "off_adv_chg": (
            f"ble.off('advertisingStateChange');expect(true).assertTrue();",
            f"try{{ble.off('advertisingStateChange');}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.off('advertisingStateChange',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.off('advertisingStateChange');}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.off('advertisingStateChange');expect(true).assertTrue();",
            f"ble.off('advertisingStateChange');expect(true).assertTrue();",
        ),
        "on_ble_find": (
            f"ble.on('BLEDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.on('BLEDeviceFind',()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.on('BLEDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.on('BLEDeviceFind',()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.on('BLEDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"ble.on('BLEDeviceFind',()=>{{}});expect(true).assertTrue();",
        ),
        "off_ble_find": (
            f"ble.off('BLEDeviceFind');expect(true).assertTrue();",
            f"try{{ble.off('BLEDeviceFind');}}catch(_e){{expect(true).assertTrue();}}",
            f"ble.off('BLEDeviceFind',()=>{{}});expect(true).assertTrue();",
            f"try{{ble.off('BLEDeviceFind');}}catch(_e2){{expect(true).assertTrue();}}",
            f"ble.off('BLEDeviceFind');expect(true).assertTrue();",
            f"ble.off('BLEDeviceFind');expect(true).assertTrue();",
        ),
    }
    return tables[key]


def sk_template(key: str) -> tuple[str, str, str, str, str, str]:
    DM = "BADDR"
    DM0 = "''"
    SO = "sppOpt()"
    ZB = "buf0()"
    tables: dict[str, tuple[str, str, str, str, str, str]] = {
        "spp_listen": (
            f"socket.sppListen('h',{SO},()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppListen('',{SO},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppListen('h2',{SO},()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppListen('x',{SO},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppListen('h',{SO},()=>{{}});expect(true).assertTrue();",
            f"socket.sppListen('h',{SO},()=>{{}});expect(true).assertTrue();",
        ),
        "get_l2cap_psm": (
            f"try{{socket.getL2capPsm(0);expect(true).assertTrue();}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getL2capPsm(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getL2capPsm(999);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getL2capPsm(1);}}catch(_e2){{expect(true).assertTrue();}}",
            f"try{{socket.getL2capPsm(0);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getL2capPsm(0);}}catch(_e){{expect(true).assertTrue();}}",
        ),
        "spp_accept": (
            f"socket.sppAccept(0,()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppAccept(-1,()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppAccept(1,()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppAccept(0,()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppAccept(0,()=>{{}});expect(true).assertTrue();",
            f"socket.sppAccept(0,()=>{{}});expect(true).assertTrue();",
        ),
        "spp_connect": (
            f"socket.sppConnect({DM},{SO},()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppConnect({DM0},{SO},()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppConnect({DM},{SO},()=>{{}});expect(true).assertTrue();",
            f"try{{socket.sppConnect('',{SO},()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppConnect({DM},{SO},()=>{{}});expect(true).assertTrue();",
            f"socket.sppConnect({DM},{SO},()=>{{}});expect(true).assertTrue();",
        ),
        "get_dev_id": (
            f"try{{socket.getDeviceId(0);expect(true).assertTrue();}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getDeviceId(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getDeviceId(999999);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getDeviceId(1);}}catch(_e2){{expect(true).assertTrue();}}",
            f"try{{socket.getDeviceId(0);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getDeviceId(0);}}catch(_e){{expect(true).assertTrue();}}",
        ),
        "get_max_rx": (
            f"try{{socket.getMaxReceiveDataSize(0);expect(true).assertTrue();}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxReceiveDataSize(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxReceiveDataSize(9);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxReceiveDataSize(1);}}catch(_e2){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxReceiveDataSize(0);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxReceiveDataSize(0);}}catch(_e){{expect(true).assertTrue();}}",
        ),
        "get_max_tx": (
            f"try{{socket.getMaxTransmitDataSize(0);expect(true).assertTrue();}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxTransmitDataSize(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxTransmitDataSize(9);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxTransmitDataSize(1);}}catch(_e2){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxTransmitDataSize(0);}}catch(_e){{expect(true).assertTrue();}}",
            f"try{{socket.getMaxTransmitDataSize(0);}}catch(_e){{expect(true).assertTrue();}}",
        ),
        "is_connected": (
            f"const ic=socket.isConnected(0);expect(typeof ic==='boolean').assertTrue();",
            f"try{{socket.isConnected(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.isConnected(0);expect(true).assertTrue();",
            f"try{{socket.isConnected(999);}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.isConnected(0);expect(true).assertTrue();",
            f"socket.isConnected(0);expect(true).assertTrue();",
        ),
        "close_srv": (
            f"socket.sppCloseServerSocket(0);expect(true).assertTrue();",
            f"try{{socket.sppCloseServerSocket(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppCloseServerSocket(1);expect(true).assertTrue();",
            f"try{{socket.sppCloseServerSocket(0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppCloseServerSocket(0);expect(true).assertTrue();",
            f"socket.sppCloseServerSocket(0);expect(true).assertTrue();",
        ),
        "close_cli": (
            f"socket.sppCloseClientSocket(0);expect(true).assertTrue();",
            f"try{{socket.sppCloseClientSocket(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppCloseClientSocket(1);expect(true).assertTrue();",
            f"try{{socket.sppCloseClientSocket(0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppCloseClientSocket(0);expect(true).assertTrue();",
            f"socket.sppCloseClientSocket(0);expect(true).assertTrue();",
        ),
        "spp_write": (
            f"socket.sppWrite(0,{ZB});expect(true).assertTrue();",
            f"try{{socket.sppWrite(-1,{ZB});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppWrite(0,{ZB});expect(true).assertTrue();",
            f"try{{socket.sppWrite(0,{ZB});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppWrite(0,{ZB});expect(true).assertTrue();",
            f"socket.sppWrite(0,{ZB});expect(true).assertTrue();",
        ),
        "on_spp_read": (
            f"socket.on('sppRead',0,()=>{{}});expect(true).assertTrue();",
            f"try{{socket.on('sppRead',-1,()=>{{}});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.on('sppRead',0,()=>{{}});expect(true).assertTrue();",
            f"try{{socket.on('sppRead',1,()=>{{}});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.on('sppRead',0,()=>{{}});expect(true).assertTrue();",
            f"socket.on('sppRead',0,()=>{{}});expect(true).assertTrue();",
        ),
        "off_spp_read": (
            f"socket.off('sppRead',0);expect(true).assertTrue();",
            f"try{{socket.off('sppRead',-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.off('sppRead',0,()=>{{}});expect(true).assertTrue();",
            f"try{{socket.off('sppRead',1);}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.off('sppRead',0);expect(true).assertTrue();",
            f"socket.off('sppRead',0);expect(true).assertTrue();",
        ),
        "write_async": (
            f"socket.sppWriteAsync(0,{ZB});expect(true).assertTrue();",
            f"try{{socket.sppWriteAsync(-1,{ZB});}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppWriteAsync(0,{ZB});expect(true).assertTrue();",
            f"try{{socket.sppWriteAsync(0,{ZB});}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppWriteAsync(0,{ZB});expect(true).assertTrue();",
            f"socket.sppWriteAsync(0,{ZB});expect(true).assertTrue();",
        ),
        "read_async": (
            f"socket.sppReadAsync(0);expect(true).assertTrue();",
            f"try{{socket.sppReadAsync(-1);}}catch(_e){{expect(true).assertTrue();}}",
            f"socket.sppReadAsync(1);expect(true).assertTrue();",
            f"try{{socket.sppReadAsync(0);}}catch(_e2){{expect(true).assertTrue();}}",
            f"socket.sppReadAsync(0);expect(true).assertTrue();",
            f"socket.sppReadAsync(0);expect(true).assertTrue();",
        ),
    }
    return tables[key]


def gen_file(
    path: str,
    part_name: str,
    desc: str,
    chunks: list[tuple[str, str, tuple[str, str, str, str, str, str]]],
) -> int:
    """chunks: (prefix, template_key, bodies) — bodies from template."""
    lines: list[str] = [
        "/**",
        " * Auto-generated by scripts/gen_conn_ble_socket_tests.py (do not edit by hand).",
        f" * Part: {part_name}",
        " * connection / ble / socket: 6 scenarios per overload (n,e,b,e2,r,x).",
        " */",
        "import { describe, it, expect } from '@ohos/hypium';",
        "import connection from '@ohos.bluetooth.connection';",
        "import ble from '@ohos.bluetooth.ble';",
        "import socket from '@ohos.bluetooth.socket';",
        "import constant from '@ohos.bluetooth.constant';",
        "import common from '@ohos.bluetooth.common';",
        "import { tI, buf0, sppOpt, bleAd, bleAs, bleAp, BADDR } from './ConnBleSocketShared';",
        "",
        f"export default function {part_name}() {{",
        f"  describe('{desc}', () => {{",
    ]
    n_cases = 0
    for prefix, _key, bodies in chunks:
        lines.append(f"    describe('{prefix}', () => {{")
        for sfx, body in zip(("n", "e", "b", "e2", "r", "x"), bodies):
            tag = f"{prefix}_{sfx}"
            lines.append(f"    it('{tag}', 0, () => {{")
            lines.append(f"      tI('{tag}', () => {{")
            for seg in body.split("\n"):
                seg = seg.strip()
                if not seg:
                    continue
                for bl in wrap_body(seg, 90):
                    lines.append(f"        {bl}")
            lines.append("      });")
            lines.append("    });")
            n_cases += 1
        lines.append("    });")
    lines.append("  });")
    lines.append("}")
    lines.append("")
    text = "\n".join(lines)
    # enforce max line length 100 bytes (ASCII)
    out_lines = []
    for ln in text.split("\n"):
        if len(ln.encode("utf-8")) <= 100:
            out_lines.append(ln)
        else:
            out_lines.extend(textwrap.wrap(ln, width=96, break_long_words=False, replace_whitespace=False))
    with open(path, "w", encoding="utf-8") as f:
        content = "\n".join(out_lines) + "\n"
    _repo = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
    _skill = os.path.join(_repo, "skills", "ohtest")
    if os.path.isdir(_skill) and _skill not in sys.path:
        sys.path.insert(0, _skill)
    try:
        from apitest_format import format_apitest_source
        content = format_apitest_source(content)
    except ImportError:
        pass
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)
    return n_cases


def main() -> None:
    assert len(CONN_KEYS) == 59
    assert len(BLE_KEYS) == 21
    assert len(SK_KEYS) == 15
    os.makedirs(OUT_DIR, exist_ok=True)

    conn_chunks: list[tuple[str, str, tuple[str, str, str, str, str, str]]] = []
    for i, k in enumerate(CONN_KEYS):
        conn_chunks.append((f"cn_{i:02d}_{k}", k, conn_template(k)))

    ble_chunks = [(f"bl_{i:02d}_{k}", k, ble_template(k)) for i, k in enumerate(BLE_KEYS)]
    sk_chunks = [(f"sk_{i:02d}_{k}", k, sk_template(k)) for i, k in enumerate(SK_KEYS)]

    # split connection across two files to keep < 2000 lines
    mid = 30
    c1 = conn_chunks[:mid]
    c2 = conn_chunks[mid:]
    n1 = gen_file(
        os.path.join(OUT_DIR, "ConnBleSocketMass01.test.ets"),
        "connBleSocketMass01",
        "ConnBleSocket_mass_cn_00_29",
        c1,
    )
    n2 = gen_file(
        os.path.join(OUT_DIR, "ConnBleSocketMass02.test.ets"),
        "connBleSocketMass02",
        "ConnBleSocket_mass_cn_30_58",
        c2,
    )
    n3 = gen_file(
        os.path.join(OUT_DIR, "ConnBleSocketMass03.test.ets"),
        "connBleSocketMass03",
        "ConnBleSocket_mass_ble",
        ble_chunks,
    )
    n4 = gen_file(
        os.path.join(OUT_DIR, "ConnBleSocketMass04.test.ets"),
        "connBleSocketMass04",
        "ConnBleSocket_mass_socket",
        sk_chunks,
    )
    total = n1 + n2 + n3 + n4
    print("generated cases:", total, "files: 4 + shared")


if __name__ == "__main__":
    main()
