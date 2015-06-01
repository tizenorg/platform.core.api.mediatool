/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlog.h>
#include <media_format.h>
#include <media_packet.h>
#include <media_packet_internal.h>
#include <media_packet_private.h>
#include <media_format_private.h>

int media_packet_set_codec_data(media_packet_h packet, void* codec_data, unsigned int codec_data_size)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    LOGI("Set: codec data = %p, codec_data_size = %u\n", codec_data, codec_data_size);

    handle->codec_data = (void*)malloc(codec_data_size);
    if (handle->codec_data != NULL)
    {
        memset(handle->codec_data, 0 , codec_data_size);
    }
    else
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)" ,__FUNCTION__,MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
    }

    memcpy(handle->codec_data, codec_data, codec_data_size);
    handle->codec_data_size = codec_data_size;

    return ret;
}

