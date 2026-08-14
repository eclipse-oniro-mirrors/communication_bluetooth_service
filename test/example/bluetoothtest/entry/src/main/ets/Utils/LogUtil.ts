/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
import hilog from '@ohos.hilog';
import BaseModel from './BaseModel';

let LogLevel = {
  /**
   * debug
   */
  DEBUG: 3,

  /**
   * info
   */
  INFO: 4,

  /**
   * warn
   */
  WARN: 5,

  /**
   * error
   */
  ERROR: 6,

  /**
   * fatal
   */
  FATAL: 7,
};

const LOG_LEVEL = LogLevel.INFO;
const HILOG_DOMAIN = 0x0000;
const HILOG_TAG = 'BluetoothTest';

/**
 * Log package tool class (wraps hilog).
 */
export class LogUtil extends BaseModel {
  debug(msg: string): void {
    if (LogLevel.DEBUG >= LOG_LEVEL) {
      hilog.debug(HILOG_DOMAIN, HILOG_TAG, '%{public}s', msg);
    }
  }

  log(msg: string): void {
    this.info(msg);
  }

  info(msg: string): void {
    if (LogLevel.INFO >= LOG_LEVEL) {
      hilog.info(HILOG_DOMAIN, HILOG_TAG, '%{public}s', msg);
    }
  }

  warn(msg: string): void {
    if (LogLevel.WARN >= LOG_LEVEL) {
      hilog.warn(HILOG_DOMAIN, HILOG_TAG, '%{public}s', msg);
    }
  }

  error(msg: string): void {
    if (LogLevel.ERROR >= LOG_LEVEL) {
      hilog.error(HILOG_DOMAIN, HILOG_TAG, '%{public}s', msg);
    }
  }
}

let mLogUtil = new LogUtil();

export default mLogUtil as LogUtil;
