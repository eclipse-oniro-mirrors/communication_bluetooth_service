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

class Logger {
  private prefix: string

  constructor(prefix: string){
    this.prefix = prefix
  }

  private line(...args: any[]): string {
    return `${this.prefix} ${args.map((x) => String(x)).join(' ')}`
  }

  debug(...args: any[]){
    LogUtil.debug(this.line(...args))
  }

  info(...args: any[]){
    LogUtil.info(this.line(...args))
  }

  warn(...args: any[]){
    LogUtil.warn(this.line(...args))
  }

  error(...args: any[]){
    LogUtil.error(this.line(...args))
  }
}

export default new Logger('[Screenshot]')