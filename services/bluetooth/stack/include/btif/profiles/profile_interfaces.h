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

#ifndef BTIF_PROFILES_PROFILE_INTERFACES_H
#define BTIF_PROFILES_PROFILE_INTERFACES_H

class ProfileInterfaces final {
public:
    static const void *GetGatt();
    static const void *GetA2dpSource();
    static const void *GetA2dpSink();
    static const void *GetAvrcpTg();
    static const void *GetAvrcpCt();
    static const void *GetHidHost();
    static const void *GetHidDevice();
    static const void *GetPan();
    static const void *GetSdp();
    static const void *GetSocket();
    static const void *GetHearingAid();
    static const void *GetHfpAg();
    static const void *GetVendor();
};

#endif // BTIF_PROFILES_PROFILE_INTERFACES_H
