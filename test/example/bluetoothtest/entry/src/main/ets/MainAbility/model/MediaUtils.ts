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
import image from '@ohos.multimedia.image'
import fileio from '@ohos.fileio'
import { photoAccessHelper } from '@kit.MediaLibraryKit'
import DateTimeUtil from './DateTimeUtil'
import Logger from './Logger'
import promptAction from '@ohos.promptAction';

const TAG: string = '[MediaUtils]'

class MediaUtils {
  async createAndGetFile(context: any) {
    let mediaTest = photoAccessHelper.getPhotoAccessHelper(context)
    let options: photoAccessHelper.CreateOptions = {
      title: 'IMG_'
    }
    return await mediaTest.createAsset(photoAccessHelper.PhotoType.IMAGE, 'jpg', options)
  }

  async savePicture(data: image.PixelMap, context: any) {
    Logger.info(TAG, `savePicture`)
    //[TODO] 接口变化重新写
    // let packOpts: image.PackingOption = {
    //   format: "image/jpeg", quality: 100
    // }
    // let imagePackerApi = image.createImagePacker()
    // let arrayBuffer = await imagePackerApi.packing(data, packOpts)
    // let fileAsset = await this.createAndGetFile(context)
    // let fd = await fileAsset.open('Rw')
    // imagePackerApi.release()
    // try {
    //   await fileio.write(fd, arrayBuffer)
    // } catch(err) {
    //   Logger.error(`write failed, code is ${err.code}, message is ${err.message}`)
    // }
    // await fileAsset.close(fd)
    // Logger.info(TAG, `write done`)
    // promptAction.showToast({
    //   message: '图片保存成功', duration: 1000
    // })
  }
}

export default new MediaUtils()