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

import LogUtil from '../../Utils/LogUtil';

const TAG = 'bttestTag';
const PLACEHOLDER = '%{public}s';

function formatMessage(format: string, ...args: any[]): string {
  let msg = format;
  for(const a of args){
    const i = msg.indexOf(PLACEHOLDER);
    if (i < 0){
      break;
    }
    msg = msg.substring(0, i) + String(a) + msg.substring(i + PLACEHOLDER.length);
  }
  return msg;
}

export default class dlog {
  static info(format: string, ...args: any[]){
    LogUtil.info(`[${TAG}] ${formatMessage(format, ...args)}`);
  }

  static debug(format: string, ...args: any[]){
    LogUtil.debug(`[${TAG}] ${formatMessage(format, ...args)}`);
  }

  static warn(format: string, ...args: any[]){
    LogUtil.warn(`[${TAG}] ${formatMessage(format, ...args)}`);
  }

  static err(format: string, ...args: any[]){
    LogUtil.error(`[${TAG}] ${formatMessage(format, ...args)}`);
  }
}
