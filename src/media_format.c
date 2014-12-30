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
#include <media_format_private.h>

static void _media_format_destroy(media_format_s* fmt);

int media_format_create(media_format_h* fmt)
{
    MEDIA_FORMAT_NULL_ARG_CHECK(fmt);
    int ret = MEDIA_FORMAT_ERROR_NONE;

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)malloc(sizeof(media_format_s));
    if (fmt_handle)
    {
        memset(fmt_handle, 0, sizeof(media_format_s));
    }
    else
    {
        return MEDIA_FORMAT_ERROR_OUT_OF_MEMORY;
    }

    fmt_handle->ref_count = 1;

    *fmt = (media_format_h)fmt_handle;

    return ret;
}

static void _media_format_destroy(media_format_s* fmt)
{
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    free(fmt);
    fmt = NULL;
}

int media_format_get_video_info(media_format_h fmt, media_format_mimetype_e* mimetype, int* width, int* height, int* avg_bps, int* max_bps)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    if (mimetype)
        *mimetype = fmt_handle->mimetype;
    if (width)
        *width = fmt_handle->detail.video.width;
    if (height)
        *height = fmt_handle->detail.video.height;
    if (avg_bps)
        *avg_bps = fmt_handle->detail.video.avg_bps;
    if (max_bps)
        *max_bps = fmt_handle->detail.video.max_bps;

    return ret;
}

int media_format_get_audio_info(media_format_h fmt, media_format_mimetype_e* mimetype, int* channel, int* samplerate, int* bit, int* avg_bps)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    *mimetype = fmt_handle->mimetype;
    *channel = fmt_handle->detail.audio.channel;
    *samplerate = fmt_handle->detail.audio.samplerate;
    *bit = fmt_handle->detail.audio.bit;
    *avg_bps = fmt_handle->detail.audio.avg_bps;

    return ret;
}

int media_format_set_video_mime(media_format_h fmt, media_format_mimetype_e mimetype)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->mimetype = mimetype;

    return ret;
}

int media_format_set_video_width(media_format_h fmt, int width)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.video.width = width;

    return ret;
}

int media_format_set_video_height(media_format_h fmt, int height)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.video.height = height;

    return ret;
}

int media_format_set_video_avg_bps(media_format_h fmt, int avg_bps)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.video.avg_bps = avg_bps;

    return ret;
}

int media_format_set_video_max_bps(media_format_h fmt, int max_bps)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_VIDEO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.video.max_bps = max_bps;

    return ret;
}


int media_format_set_audio_mime(media_format_h fmt, media_format_mimetype_e mimetype)
{

    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->mimetype = mimetype;

    return ret;
}

int media_format_set_audio_channel(media_format_h fmt, int channel)
{

    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.audio.channel = channel;

    return ret;
}

int media_format_set_audio_samplerate(media_format_h fmt, int samplerate)
{

    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.audio.samplerate = samplerate;

    return ret;
}

int media_format_set_audio_bit(media_format_h fmt, int bit)
{

    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.audio.bit = bit;

    return ret;
}

int media_format_set_audio_avg_bps(media_format_h fmt, int avg_bps)
{

    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    if (!MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        LOGE("the format can not be changed..", __FUNCTION__);
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (!(fmt_handle->mimetype & MEDIA_FORMAT_AUDIO))
    {
        return MEDIA_FORMAT_ERROR_INVALID_PARAMETER;
    }

    fmt_handle->detail.audio.avg_bps = avg_bps;

    return ret;
}

int media_format_ref(media_format_h fmt)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (MEDIA_FORMAT_GET_REFCOUNT(fmt) <= 0)
    {
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    g_atomic_int_inc(&fmt_handle->ref_count);

    return ret;
}

int media_format_unref(media_format_h fmt)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);
    bool is_zero;

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (MEDIA_FORMAT_GET_REFCOUNT(fmt) <= 0)
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;

    is_zero = g_atomic_int_dec_and_test(&fmt_handle->ref_count);
    if (is_zero)
    {
        /* if reference count become 0 , free fmt. */
        _media_format_destroy(fmt_handle);
    }

    return ret;
}

int media_format_is_writable(media_format_h fmt, bool* is_writable)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (g_atomic_int_get(&fmt_handle->ref_count) <= 0)
    {
        return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
    }

    if (g_atomic_int_get(&fmt_handle->ref_count) == 1)
    {
        /* if reference count is 1, the caller must be owner. */
        *is_writable = true;
    }
    else
    {
        *is_writable = false;
    }

    return ret;
}

int media_format_make_writable(media_format_h fmt, media_format_h* out_fmt)
{
    int ret = MEDIA_FORMAT_ERROR_NONE;
    MEDIA_FORMAT_INSTANCE_CHECK(fmt);

    media_format_s* fmt_handle;
    fmt_handle = (media_format_s*)fmt;

    if (MEDIA_FORMAT_IS_WRITABLE(fmt))
    {
        /* If there is only one reference count on @fmt, the caller must be the owner */
        *out_fmt = fmt;
    }
    else
    {
        /* If there is more than one reference on the object, a new media_format_h object will be returned.
              The caller's reference on @fmt will be removed, and instead the caller will own a reference to the returned object.*/
        media_format_s* copy;
        copy = (media_format_s*)malloc(sizeof(media_format_s));
        if (copy)
        {
            memset(copy, 0, sizeof(media_format_s));
        }
        else
        {
            return MEDIA_FORMAT_ERROR_OUT_OF_MEMORY;
        }

        memcpy(copy, fmt_handle, sizeof(media_format_s));
        copy->ref_count = 1;

        if (media_format_unref(fmt) != MEDIA_FORMAT_ERROR_NONE)
        {
            free(copy);
            return MEDIA_FORMAT_ERROR_INVALID_OPERATION;
        }

        *out_fmt = (media_format_h)copy;

    }

    return ret;
}


