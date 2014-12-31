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
#include <media_packet_private.h>
#include <media_format_private.h>

/* NOTE : static internal functions does not check anything on incomming parameters
 * Caller should takecare it
 */
static int _pkt_alloc_buffer(media_packet_s* pkt);
static uint64_t _pkt_calculate_video_buffer_size(media_packet_s* pkt);
static uint64_t _pkt_calculate_audio_buffer_size(media_packet_s* pkt);
static uint32_t _convert_to_tbm_surface_format(media_format_mimetype_e format_type);

int media_packet_create_alloc(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(fmt);
    MEDIA_PACKET_NULL_ARG_CHECK(packet);
    /* TODO : need more validation on fmt */

    handle = (media_packet_s*)malloc(sizeof(media_packet_s));
    if (handle != NULL)
        memset(handle, 0, sizeof(media_packet_s));
    else
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)" ,__FUNCTION__, MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
    }

    /* TODO : need to consider to give application to select buffer type(tbm or heap) */

    /* check if tbm surface is needed.
     * This time, Only raw video format is considered to be allocated in TBM
     */
    if (MEDIA_FORMAT_IS_VIDEO(fmt) && MEDIA_FORMAT_IS_RAW(fmt))
    {
        handle->type = MEDIA_BUFFER_TYPE_TBM_SURFACE;
    }
    else
    {
        handle->type = MEDIA_BUFFER_TYPE_NORMAL;
    }

    /* take fmt */
    handle->format = MEDIA_FORMAT_CAST(fmt);

    /* alloc buffer */
    int err = _pkt_alloc_buffer(handle);
    if (err != MEDIA_PACKET_ERROR_NONE)
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)", __FUNCTION__, MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        ret = err;
        goto fail;
    }

    /* allocated buffer */
    handle->is_allocated = true;

    /* set finalized callback and user data */
    handle->finalizecb_func = fcb;
    handle->userdata = fcb_data;

    /* increase format reference count */
    media_format_ref((media_format_h)handle->format);

    /* take handle */
    *packet = (media_packet_h)handle;
    LOGI("[%s] new handle : %p", __FUNCTION__, *packet);
    return ret;

fail:

    if (handle)
        {
            free(handle);
            handle = NULL;
        }

    *packet = NULL;
    return ret;
}

int media_packet_create(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(fmt);
    MEDIA_PACKET_NULL_ARG_CHECK(packet);
    /* TODO : need more validation on fmt */

    handle = (media_packet_s*)malloc(sizeof(media_packet_s));
    if (handle != NULL)
        memset(handle, 0, sizeof(media_packet_s));
    else
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)" ,__FUNCTION__, MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
    }

    /* TODO : need to consider to give application to select buffer type(tbm or heap) */

    /* check if tbm surface is needed.
     * This time, Only raw video format is considered to be allocated in TBM
     */
    if (MEDIA_FORMAT_IS_VIDEO(fmt) && MEDIA_FORMAT_IS_RAW(fmt))
    {
        handle->type = MEDIA_BUFFER_TYPE_TBM_SURFACE;
    }
    else
    {
        handle->type = MEDIA_BUFFER_TYPE_NORMAL;
    }

    /* take fmt */
    handle->format = MEDIA_FORMAT_CAST(fmt);

    /* NOT allocated buffer */
    handle->is_allocated = false;

    /* set finalized callback and user data */
    handle->finalizecb_func = fcb;
    handle->userdata = fcb_data;

    /* increase format reference count */
    media_format_ref((media_format_h)handle->format);

    /* take handle */
    *packet = (media_packet_h)handle;
    LOGI("[%s] new handle : %p", __FUNCTION__, *packet);
    return ret;

}

int media_packet_copy(media_packet_h org_packet, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h* new_packet)
{
    media_packet_s* handle;
    media_packet_s* org_handle;

    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(org_packet);
    MEDIA_PACKET_NULL_ARG_CHECK(new_packet);

    org_handle = (media_packet_s*)org_packet;

    handle = (media_packet_s*)malloc(sizeof(media_packet_s));
    if (handle != NULL)
        memset(handle, 0, sizeof(media_packet_s));
    else
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)" ,__FUNCTION__, MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
    }

    /* copy handle*/
    memcpy(handle, org_handle, sizeof(media_packet_s));

    /* the caller can not get exist allocated buffer pointer */
    handle->data = NULL;
    handle->surface_data = NULL;
    handle->is_allocated = false;
    handle->size = 0;

    /* set finalized callback and user data */
    handle->finalizecb_func = fcb;
    handle->userdata = fcb_data;

    /* increase format reference count */
    media_format_ref((media_format_h)handle->format);

    /* take handle */
    *new_packet = (media_packet_h)handle;
    LOGI("[%s] new handle : %p", __FUNCTION__, *new_packet);
    return ret;

}

int media_packet_alloc(media_packet_h packet)
{
    LOGI("[%s] start", __FUNCTION__);
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    /* alloc buffer */
    int err = _pkt_alloc_buffer(handle);
    if (err != MEDIA_PACKET_ERROR_NONE)
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)", __FUNCTION__, MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        ret = err;
        goto fail;
    }

    /* allocated buffer */
    handle->is_allocated = true;

    LOGI("[%s] end", __FUNCTION__);
    return ret;

fail:
    if(handle)
    {
        free(handle);
        handle = NULL;
    }
    return ret;

}


int _pkt_alloc_buffer(media_packet_s* pkt)
{
    /* skip validating pkt */
    uint64_t buffersize = 0;

    /* need to use format,width,height to get buffer size for raw video frame */
    if (pkt->type == MEDIA_BUFFER_TYPE_TBM_SURFACE)
    {
        buffersize = _pkt_calculate_video_buffer_size(pkt);
        pkt->size = buffersize;
    /* NOTE : size of buffer may not equal to w*h*bpp. Not sure we need to
        * do some verifying between given size and calculated size.
        */
    }
    else
    {
        buffersize = pkt->size;
    }

    if (pkt->type == MEDIA_BUFFER_TYPE_NORMAL)
    {
        /* need to use format,width,height to get buffer size */
        if (MEDIA_FORMAT_IS_VIDEO(pkt->format))
        {
            buffersize = _pkt_calculate_video_buffer_size(pkt);
            pkt->data = (void*)malloc(buffersize);
            if (!pkt->data)
            {
                return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
            }
        }
        else
        {
            buffersize = _pkt_calculate_audio_buffer_size(pkt);
            pkt->data = (void*)malloc(buffersize);
            if (!pkt->data)
            {
                return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
            }
        }
        pkt->size = buffersize;
    }
    else if (pkt->type == MEDIA_BUFFER_TYPE_TBM_SURFACE)
    {

        /* convert to tbm_format*/
        uint32_t tbm_format;
        tbm_format = _convert_to_tbm_surface_format(pkt->format->mimetype);

#if 0
        /*check whether given tbm_format is supported or not*/
        uint32_t *supported_formats;
        uint32_t supported_format_num;
        bool is_supported_format;
        int i;

        if (tbm_surface_query_formats(&supported_formats, &supported_format_num))
        {
            is_supported_format = false;
            for (i = 0 ; i < supported_format_num ; i++)
            {
                if (supported_formats[i] == tbm_format)
                {
                    is_supported_format = true;
                    break;
                }
            }

            free (supported_formats);
        }
        else
        {
            /* tbm_surface_query_format returns error */
            LOGE("tbm_surface_query_format is failed..");
            return MEDIA_PACKET_ERROR_INVALID_OPERATION;
        }

        /* if not supported tbm_format by backend, don't need to do tbm_surface_create() */
        if (!is_supported_format)
        {
            LOGE("the backend doesn't support 0x%x mimetype", pkt->format->mimetype);
            return MEDIA_PACKET_ERROR_INVALID_OPERATION;
        }
#endif
        /*create tbm_surface*/
        pkt->surface_data = (void*)tbm_surface_create(pkt->format->detail.video.width, pkt->format->detail.video.height, tbm_format);
        if (pkt->surface_data != NULL)
        {

            /* get tbm_surface_info */
            tbm_surface_info_s surface_info;
            int err = tbm_surface_get_info((tbm_surface_h)pkt->surface_data, &surface_info);
            if (err == TBM_SURFACE_ERROR_NONE)
            {
                pkt->data = surface_info.planes[0].ptr;
                pkt->size = (uint64_t)surface_info.size;
            }
            else
            {
                tbm_surface_destroy((tbm_surface_h)pkt->surface_data);
                return MEDIA_PACKET_ERROR_INVALID_OPERATION;
            }
        }
        else
        {
            LOGE("\n tbm_surface_create() is failed!! \n");
            return MEDIA_PACKET_ERROR_INVALID_OPERATION;
        }

    }
    return MEDIA_PACKET_ERROR_NONE;
}

/* TODO : contact Kim Young Hun to make below api as a common */
static uint64_t _pkt_calculate_video_buffer_size(media_packet_s* pkt)
{
/* TODO : rename below macro or make it able to use original from mm_transform */
#define _ROUND_UP_4(num) (((num)+3)&~3)
#define _GEN_MASK(x) ((1<<(x))-1)
#define _ROUND_UP_X(v,x) (((v) + _GEN_MASK(x)) & ~_GEN_MASK(x))
#define _DIV_ROUND_UP_X(v,x) (((v) + _GEN_MASK(x)) >> (x))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

    unsigned char x_chroma_shift = 0;
    unsigned char y_chroma_shift = 0;
    int size, w2, h2, size2;
    int stride, stride2;
    int width = 0;
    int height = 0;
    uint64_t buffersize = 0;

    if (MEDIA_FORMAT_IS_VIDEO(pkt->format)) {
        width = pkt->format->detail.video.width;
        height = pkt->format->detail.video.height;
    }

    /* TODO : need to check type mapping again with mm_transform */
    switch (pkt->format->mimetype)
    {

        case MEDIA_FORMAT_I420:
        //case MM_UTIL_IMG_FMT_YUV420:
            x_chroma_shift = 1;
            y_chroma_shift = 1;
            stride = _ROUND_UP_4 (width);
            h2 = _ROUND_UP_X (height, x_chroma_shift);
            size = stride * h2;
            w2 = _DIV_ROUND_UP_X (width, x_chroma_shift);
            stride2 = _ROUND_UP_4 (w2);
            h2 = _DIV_ROUND_UP_X (height, y_chroma_shift);
            size2 = stride2 * h2;
            buffersize = size + 2 * size2;
            break;

        //case MM_UTIL_IMG_FMT_YUV422:
        case MEDIA_FORMAT_YUYV:
        case MEDIA_FORMAT_UYVY:
        case MEDIA_FORMAT_NV16:
        //case MM_UTIL_IMG_FMT_NV61:
            stride = _ROUND_UP_4 (width * 2);
            size = stride * height;
            buffersize = size;
            break;

        case MEDIA_FORMAT_RGB565:
            stride = _ROUND_UP_4 (width * 2);
            size = stride * height;
            buffersize = size;
            break;

        case MEDIA_FORMAT_RGB888:
            stride = _ROUND_UP_4 (width * 3);
            size = stride * height;
            buffersize = size;
            break;

        case MEDIA_FORMAT_ARGB:
        //case MM_UTIL_IMG_FMT_BGRA8888:
        case MEDIA_FORMAT_RGBA:
        //case MM_UTIL_IMG_FMT_BGRX8888:
            stride = width * 4;
            size = stride * height;
            buffersize = size;
            break;

        case MEDIA_FORMAT_NV12:
        case MEDIA_FORMAT_NV12T:
        case MEDIA_FORMAT_H261:
        case MEDIA_FORMAT_H263:
        case MEDIA_FORMAT_H263P:
        case MEDIA_FORMAT_H264_SP:
        case MEDIA_FORMAT_H264_MP:
        case MEDIA_FORMAT_H264_HP:
        case MEDIA_FORMAT_MJPEG:
        case MEDIA_FORMAT_MPEG1:
        case MEDIA_FORMAT_MPEG2_SP:
        case MEDIA_FORMAT_MPEG2_MP:
        case MEDIA_FORMAT_MPEG2_HP:
        case MEDIA_FORMAT_MPEG4_SP:
        case MEDIA_FORMAT_MPEG4_ASP:
            x_chroma_shift = 1;
            y_chroma_shift = 1;
            stride = _ROUND_UP_4 (width);
            h2 = _ROUND_UP_X (height, y_chroma_shift);
            size = stride * h2;
            w2 = 2 * _DIV_ROUND_UP_X (width, x_chroma_shift);
            stride2 = _ROUND_UP_4 (w2);
            h2 = _DIV_ROUND_UP_X (height, y_chroma_shift);
            size2 = stride2 * h2;
            buffersize = size + size2;
            break;
        default:
            LOGE("Not supported format\n");
            return 0;
    }

    LOGD("format 0x%x, buffersize %llu\n", pkt->format->mimetype, buffersize);

    return buffersize;
}

/* TODO : written by joungkook seo for audio */
static uint64_t _pkt_calculate_audio_buffer_size(media_packet_s* pkt)
{
/* TODO : rename below macro or make it able to use original from mm_transform */
#define PCM_MIN_FRM_SIZE         (1024)
#define AAC_MIN_FRM_SIZE         (1024)
#define AAC_MIN_NCH                    (2)

    int channel = 0;
    int bit = 0;
    uint64_t buffersize = 0;

    if (!MEDIA_FORMAT_IS_VIDEO(pkt->format)) {
        channel = pkt->format->detail.audio.channel;
        bit = pkt->format->detail.audio.bit;
    }

    /* TODO : need to check type mapping again with mm_transform */
    switch (pkt->format->mimetype)
    {
        case MEDIA_FORMAT_PCM:
            buffersize = (PCM_MIN_FRM_SIZE * channel) * (uint64_t)(bit /8);
            break;
        case MEDIA_FORMAT_AAC:
            buffersize = (AAC_MIN_FRM_SIZE * AAC_MIN_NCH) * (uint64_t)(2);      /* 2 = (16bit/8)*/
            break;
        default:
            LOGE("Not supported format\n");
            return 0;
    }

    LOGD("format 0x%x, buffersize %llu\n", pkt->format->mimetype, buffersize);

    return buffersize;
}

int media_packet_create_from_tbm_surface(media_format_h fmt, tbm_surface_h surface, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    if (surface == NULL)
        return MEDIA_PACKET_ERROR_INVALID_PARAMETER;

    MEDIA_PACKET_INSTANCE_CHECK(fmt);
    MEDIA_PACKET_INSTANCE_CHECK(surface);
    MEDIA_PACKET_NULL_ARG_CHECK(packet);
    /* TODO : need more validation on fmt */

    handle = (media_packet_s*)malloc( sizeof(media_packet_s));
    if (handle != NULL)
    {
        memset(handle, 0 , sizeof(media_packet_s));
    }
    else
    {
        LOGE("[%s] MEDIA_PACKET_ERROR_OUT_OF_MEMORY(0x%08x)" ,__FUNCTION__,MEDIA_PACKET_ERROR_OUT_OF_MEMORY);
        return MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
    }

    handle->type = MEDIA_BUFFER_TYPE_EXTERNAL_TBM_SURFACE;
    handle->surface_data = (void*)surface;

    /* alloc handle->format */
    handle->format = MEDIA_FORMAT_CAST(fmt);

    /* get tbm_surface_info */
    tbm_surface_info_s surface_info;
    int err = tbm_surface_get_info((tbm_surface_h)handle->surface_data, &surface_info);
    if (err == TBM_SURFACE_ERROR_NONE)
    {
        handle->data = surface_info.planes[0].ptr;
        handle->size = (uint64_t)surface_info.size;
    }
    else
    {
        ret = MEDIA_PACKET_ERROR_INVALID_OPERATION;
        goto fail;
    }

    /* allocated buffer */
    handle->is_allocated = true;

    /* set finalized callback and user data */
    handle->finalizecb_func = fcb;
    handle->userdata = fcb_data;

    /* increase format reference count */
    ret = media_format_ref((media_format_h)handle->format);

    /* take handle */
    *packet = (media_packet_h)handle;
    LOGI("[%s] new handle : %p", __FUNCTION__, *packet);
    return ret;

fail:

    if (handle)
    {
        free(handle);
        handle = NULL;
    }

    *packet = NULL;
    return ret;
}

int media_packet_get_buffer_data_ptr(media_packet_h packet, void **data)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(data);

    handle = (media_packet_s*) packet;

    *data = handle->data;

    return ret;

}

int media_packet_get_tbm_surface(media_packet_h packet, tbm_surface_h *surface)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(surface);

    handle = (media_packet_s*) packet;

    *surface = (tbm_surface_h)handle->surface_data;

    return ret;
}

int media_packet_get_format(media_packet_h packet, media_format_h *fmt)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(fmt);

    handle = (media_packet_s*) packet;

    /* ref exist format */
    media_format_ref((media_format_h)handle->format);

    *fmt = (media_format_h)handle->format;

    return ret;
}

int media_packet_set_format(media_packet_h packet, media_format_h fmt)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(fmt);

    handle = (media_packet_s*) packet;

    /* if trying to set same format,  return*/
    if (handle->format == MEDIA_FORMAT_CAST(fmt))
    {
        return MEDIA_PACKET_ERROR_INVALID_OPERATION;
    }

    /* unref exist format */
    media_format_unref((media_format_h)handle->format);

    /* set as new format to packet */
    handle->format = MEDIA_FORMAT_CAST(fmt);

    /* ref new format */
    media_format_ref(fmt);

    return ret;
}

int media_packet_set_pts(media_packet_h packet, uint64_t pts)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->pts = pts;

    return ret;
}

int media_packet_set_dts(media_packet_h packet, uint64_t dts)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->dts = dts;

    return ret;
}

int media_packet_set_duration(media_packet_h packet, uint64_t duration)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->duration = duration;

    return ret;
}

int media_packet_set_buffer_size(media_packet_h packet, uint64_t size)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->size = size;

    return ret;
}

int media_packet_get_pts(media_packet_h packet, uint64_t *pts)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(pts);

    handle = (media_packet_s*) packet;

    *pts = handle->pts;

    return ret;
}

int media_packet_get_dts(media_packet_h packet, uint64_t *dts)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(dts);

    handle = (media_packet_s*) packet;

    *dts = handle->dts;

    return ret;
}

int media_packet_get_duration(media_packet_h packet, uint64_t *duration)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(duration);

    handle = (media_packet_s*) packet;

    *duration = handle->duration;

    return ret;
}

int media_packet_get_buffer_size(media_packet_h packet, uint64_t *size)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(size);

    handle = (media_packet_s*) packet;

    *size = handle->size;

    return ret;
}

int media_packet_set_extra(media_packet_h packet, void *extra)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(extra);

    handle = (media_packet_s*) packet;

    handle->extradata = extra;

    return ret;
}

int media_packet_get_extra(media_packet_h packet, void **extra)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(extra);

    handle = (media_packet_s*) packet;

    *extra = handle->extradata;

    return ret;
}

int media_packet_is_video(media_packet_h packet, bool *is_video)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_video);

    handle = (media_packet_s*) packet;

    if (MEDIA_FORMAT_IS_VIDEO(handle->format))
        *is_video = true;
    else
        *is_video = false;

    return ret;
}

int media_packet_is_audio(media_packet_h packet, bool *is_audio)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_audio);

    handle = (media_packet_s*) packet;

    if (MEDIA_FORMAT_IS_AUDIO(handle->format))
        *is_audio = true;
    else
        *is_audio = false;

    return ret;
}

int media_packet_is_encoded(media_packet_h packet, bool *is_encoded)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_encoded);

    handle = (media_packet_s*) packet;

    if (MEDIA_FORMAT_IS_ENCODED(handle->format))
        *is_encoded = true;
    else
        *is_encoded = false;

    return ret;
}

int media_packet_is_raw(media_packet_h packet, bool *is_raw)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_raw);

    handle = (media_packet_s*) packet;

    if (MEDIA_FORMAT_IS_RAW(handle->format))
        *is_raw = true;
    else
        *is_raw = false;

    return ret;
}

int media_packet_set_flags(media_packet_h packet, media_buffer_flags_e flags)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->flags |= flags;

    return ret;
}

int media_packet_unset_flags(media_packet_h packet, media_buffer_flags_e flags)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    handle->flags &= ~flags;

    return ret;
}

int media_packet_is_codec_config(media_packet_h packet, bool *is_codec_config)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_codec_config);

    handle = (media_packet_s*) packet;

    if (MEDIA_PACKET_CODEC_CONFIG_DATA(packet))
        *is_codec_config = true;
    else
        *is_codec_config = false;

    return ret;
}

int media_packet_is_end_of_stream(media_packet_h packet, bool *is_eos)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_eos);

    handle = (media_packet_s*) packet;

    if (MEDIA_PACKET_CODEC_END_OF_STREAM(packet))
        *is_eos = true;
    else
        *is_eos = false;

    return ret;
}

int media_packet_is_sync_frame(media_packet_h packet, bool *is_sync)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(is_sync);

    handle = (media_packet_s*) packet;

    if (MEDIA_PACKET_CODEC_SYNC_FRAME(packet))
        *is_sync = true;
    else
        *is_sync = false;

    return ret;
}

int media_packet_has_tbm_surface_buffer(media_packet_h packet, bool* has_tbm_surface)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);
    MEDIA_PACKET_NULL_ARG_CHECK(has_tbm_surface);

    handle = (media_packet_s*) packet;

    if ((handle->type == MEDIA_BUFFER_TYPE_TBM_SURFACE || handle->type == MEDIA_BUFFER_TYPE_EXTERNAL_TBM_SURFACE) && handle->surface_data)
    {
        *has_tbm_surface = true;
    }
    else
    {
        *has_tbm_surface = false;
    }
    return ret;
}

int media_packet_destroy(media_packet_h packet)
{
    media_packet_s* handle;
    int ret = MEDIA_PACKET_ERROR_NONE;

    MEDIA_PACKET_INSTANCE_CHECK(packet);

    handle = (media_packet_s*) packet;

    /* finailize callback */
    if (handle->finalizecb_func)
    {
        int finalize_cb_ret;
        finalize_cb_ret = handle->finalizecb_func((media_packet_h)handle, MEDIA_PACKET_ERROR_NONE, handle->userdata);

        /* creator do not want to destroy media packet handle*/
        if(finalize_cb_ret == MEDIA_PACKET_REUSE)
        {
            LOGI("the media packet handle will be reused.");
            return MEDIA_PACKET_ERROR_NONE;
        }
    }

    if(handle->type == MEDIA_BUFFER_TYPE_TBM_SURFACE)
    {
        if(handle->surface_data)
        {
            tbm_surface_destroy((tbm_surface_h)handle->surface_data);
        }
    }
    else if (handle->type == MEDIA_BUFFER_TYPE_NORMAL)
    {
        if(handle->data)
        {
            free(handle->data);
            handle->data = NULL;
        }
    }
    else if (handle->type == MEDIA_BUFFER_TYPE_EXTERNAL_TBM_SURFACE)
    {
        // there is nothing to do
    }

    /* unreference media_format */
    media_format_unref(handle->format);

    free(handle);
    handle = NULL;

    return ret;
}

static uint32_t _convert_to_tbm_surface_format(media_format_mimetype_e format_type)
{
    uint32_t tbm_format;

    switch(format_type) {
        case MEDIA_FORMAT_NV12:
        case MEDIA_FORMAT_NV12T:
            tbm_format = TBM_FORMAT_NV12;
            break;
        case MEDIA_FORMAT_NV16:
            tbm_format = TBM_FORMAT_NV16;
            break;
        case MEDIA_FORMAT_NV21:
            tbm_format = TBM_FORMAT_NV21;
            break;
        case MEDIA_FORMAT_YUYV:
            tbm_format = TBM_FORMAT_YUYV;
            break;
        case MEDIA_FORMAT_UYVY:
            tbm_format = TBM_FORMAT_UYVY;
            break;
        case MEDIA_FORMAT_422P:
            tbm_format = TBM_FORMAT_YUV422;
            break;
        case MEDIA_FORMAT_I420:
            tbm_format = TBM_FORMAT_YUV420;
            break;
        case MEDIA_FORMAT_YV12:
            tbm_format = TBM_FORMAT_YVU420;
            break;
        case MEDIA_FORMAT_RGB565:
            tbm_format = TBM_FORMAT_RGB565;
            break;
        case MEDIA_FORMAT_RGB888:
            tbm_format = TBM_FORMAT_RGB888;
            break;
        case MEDIA_FORMAT_RGBA:
            tbm_format = TBM_FORMAT_RGBA8888;
            break;
        case MEDIA_FORMAT_ARGB:
            tbm_format = TBM_FORMAT_ARGB8888;
            break;
        default:
            LOGE("Invalid format mime type!");
            tbm_format = 0;
            break;
        }

    return tbm_format;
}


#if 0
int media_tool_create(mediatool_h *mediatool)
{
    MediaTool_s *handle;
    int ret;
    handle = (MediaTool_s*)malloc(sizeof(MediaTool_s));

    if (handle != NULL)
            memset(handle, 0, sizeof(MediaTool_s));
    else
            return MEDIA_TOOL_ERROR_OUT_OF_MEMORY;

    //TODO : make _media_tool_create function
    //ret = _media_tool_create(&handle->mediatool_handle);
    if(ret != MEDIA_TOOL_ERROR_NONE)
        {
            free(handle);
            handle = NULL;
            return MEDIA_TOOL_ERROR_INVALID_OPERATION;
        }
    else
        {
            *mediatool = (mediatool_h*)handle;
            handle->refcount = 1;
            return MEDIA_TOOL_ERROR_NONE;
        }
}

int media_tool_destroy(mediatool_h mediatool)
{
    MediaTool_s* handle = (MediaTool_s *) mediatool;
    int ret;

    if (handle == NULL)
        return MEDIA_TOOL_ERROR_INVALID_PARAMETER;

    //TODO : make _media_tool_create function
    ret = _media_tool_destroy(handle->mediatool_handle);
    if (ret != MEDIA_TOOL_ERROR_NONE)
        {
            return MEDIA_TOOL_ERROR_INVALID_OPERATION;
        }
    else
        {
            free(handle);
            handle = NULL;
            return MEDIA_TOOL_ERROR_NONE;
        }
}

int media_tool_is_Iframe(mediatool_h mediatool, bool *isIFrame)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    if (handle == NULL)
        return MEDIA_TOOL_ERROR_INVALID_PARAMETER;

    *isIFrame  = handle->videocodecformat->isIFrame;

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_set_buffer(mediatool_h mediatool, MediaToolBufferPrivate_s* privBuffer)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    handle->bufferpriv = (MediaToolBufferPrivate_s *)malloc(sizeof(MediaToolBufferPrivate_s));

    if (handle->bufferpriv != NULL)
        memset(handle->bufferpriv, 0, sizeof(MediaToolBufferPrivate_s));
    else
        return MEDIA_TOOL_ERROR_OUT_OF_MEMORY;

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_get_buffer(mediatool_h mediatool, MediaToolBufferPrivate_s** privBuffer)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    if (handle->bufferpriv == NULL)
        return MEDIA_TOOL_ERROR_INVALID_OPERATION;

    memcpy(*privBuffer, handle->bufferpriv, sizeof(handle->bufferpriv));

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_set_video_codec_format(mediatool_h mediatool, VideoCodecFormat_s* videoCodecFormat)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    handle->videocodecformat = (VideoCodecFormat_s *)malloc(sizeof(VideoCodecFormat_s));

    if (handle->videocodecformat != NULL)
        memset(handle->videocodecformat, 0, sizeof(VideoCodecFormat_s));
    else
        return MEDIA_TOOL_ERROR_OUT_OF_MEMORY;

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_get_video_codec_format(mediatool_h mediatool, VideoCodecFormat_s** videoCodecFormat)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    if (handle->videocodecformat == NULL)
        return MEDIA_TOOL_ERROR_INVALID_OPERATION;

    memcpy(*videoCodecFormat, handle->videocodecformat, sizeof(handle->videocodecformat));

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_set_audio_codec_format(mediatool_h mediatool, AudioCodecFormat_s* audioCodecFormat)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    handle->audiocodecformat = (AudioCodecFormat_s *)malloc(sizeof(AudioCodecFormat_s));

    if (handle->audiocodecformat != NULL)
        memset(handle->audiocodecformat, 0, sizeof(AudioCodecFormat_s));
    else
        return MEDIA_TOOL_ERROR_OUT_OF_MEMORY;

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_get_audio_codec_format(mediatool_h mediatool, AudioCodecFormat_s** audioCodecFormat)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;
    int ret;
    if (handle->audiocodecformat == NULL)
        return MEDIA_TOOL_ERROR_INVALID_OPERATION;

    memcpy(*audioCodecFormat, handle->audiocodecformat, sizeof(handle->audiocodecformat));

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_ref(mediatool_h mediatool)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;

    if (handle == NULL)
        return MEDIA_TOOL_ERROR_INVALID_PARAMETER;

    if (!(handle->refcount> 0))
        return MEDIA_TOOL_ERROR_INVALID_OPERATION;

    g_atomic_int_inc (&handle->refcount);

    return MEDIA_TOOL_ERROR_NONE;
}

int media_tool_unref(mediatool_h mediatool)
{
    MediaTool_s *handle = (MediaTool_s *) mediatool;

    if (handle == NULL)
        return MEDIA_TOOL_ERROR_INVALID_PARAMETER;

    if (!(handle->refcount> 0))
        return MEDIA_TOOL_ERROR_INVALID_OPERATION;

    int zero;

    zero = g_atomic_int_dec_and_test (&handle->refcount);
    if (zero)
        {
            free(handle);
            handle = NULL;
        }
    return MEDIA_TOOL_ERROR_NONE;
}

#endif

