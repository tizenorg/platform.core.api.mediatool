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

#ifndef __TIZEN_MEDIA_FORMAT_PRIVATE_H__
#define __TIZEN_MEDIA_FORMAT_PRIVATE_H__

#include <media_format.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_MEDIA_TOOL"

#define MEDIA_FORMAT_CHECK_CONDITION(condition,error,msg)     \
                if(condition) {} else \
                { LOGE("[%s] %s(0x%08x)", __FUNCTION__, msg,error); return error;}; \

#define MEDIA_FORMAT_INSTANCE_CHECK(media_format)   \
        MEDIA_FORMAT_CHECK_CONDITION(media_format != NULL, MEDIA_FORMAT_ERROR_INVALID_PARAMETER, "MEDIA_FORMAT_ERROR_INVALID_PARAMETER")

#define MEDIA_FORMAT_NULL_ARG_CHECK(arg)      \
        MEDIA_FORMAT_CHECK_CONDITION(arg != NULL, MEDIA_FORMAT_ERROR_INVALID_PARAMETER, "MEDIA_FORMAT_ERROR_INVALID_PARAMETER")

#if 0
#define atomic_int_inc(atomic) \
  (__extension__({                                                          \
    STATIC_ASSERT (sizeof *(atomic) == sizeof (int));                     \
    (void) (0 ? *(atomic) ^ *(atomic) : 0);                                  \
    (void) __sync_fetch_and_add ((atomic), 1);                               \
  }))
#endif


#define MEDIA_FORMAT_CAST(obj) ((media_format_s*)(obj))

/**
 * @brief Check whether given media format is for audio or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_AUDIO(x_fmt) (MEDIA_FORMAT_CAST(x_fmt)->mimetype & MEDIA_FORMAT_AUDIO)

/**
 * @brief Check whether given media format is for video or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_VIDEO(x_fmt) (MEDIA_FORMAT_CAST(x_fmt)->mimetype & MEDIA_FORMAT_VIDEO)

/**
 * @brief Check whether given media format is for text or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_TEXT(x_fmt) (false)

/**
 * @brief Check whether given media format is for raw type or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_RAW(x_fmt) (MEDIA_FORMAT_CAST(x_fmt)->mimetype & MEDIA_FORMAT_RAW)

/**
 * @brief Check whether given media format is for encoded type or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_ENCODED(x_fmt) (MEDIA_FORMAT_CAST(x_fmt)->mimetype & MEDIA_FORMAT_ENCODED)

/**
 * @brief Gets the number of media format ref_count
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_GET_REFCOUNT(x_fmt) (g_atomic_int_get(&(MEDIA_FORMAT_CAST(x_fmt))->ref_count))

/**
 * @brief Check whether given media format is writable or not
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_IS_WRITABLE(x_fmt) (MEDIA_FORMAT_GET_REFCOUNT(x_fmt) == 1)

/**
 * @brief increase the media format's ref_count
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_INC_REFCOUNT(x_fmt) (g_atomic_int_inc(&(MEDIA_FORMAT_CAST(x_fmt))->ref_count))

/**
 * @brief decrease the media format's ref_count and return true if ref_count become zero.
 * @since_tizen 2.3
 */
#define MEDIA_FORMAT_DEC_REFCOUNT_TEST(x_fmt) (g_atomic_int_dec_and_test(&(MEDIA_FORMAT_CAST(x_fmt))->ref_count))

/**
 * @brief Media format for configuring video codec.
 * @since_tizen 2.3
 */
typedef struct _media_format_video_spec_s {
    int width;          /**< media format video width */
    int height;          /**< media format video height */
    int avg_bps;          /**< media format video average bps */
    int max_bps;          /**< media format video max bps */
} media_format_video_spec_s;

/**
 * @brief Media format for configuring audio codec.
 * @since_tizen 2.3
 */
typedef struct _media_fomat_audio_spec_s {
    int channel;          /**< media format audio's the number of channels */
    int samplerate;          /**< media format audio sampling rate */
    int bit;                /**< media format audio's the bit resolution */
    int avg_bps;          /**< media format audio max bps */
} media_format_audio_spec_s;

/**
 * @brief Structure of media format.
 * @since_tizen 2.3
 */
typedef struct _media_format_s
{
    int ref_count;          /**< media format struct reference count */
    media_format_mimetype_e mimetype;          /**< media format struct mimetype of media_format_mimetype_e */

    union {
        media_format_audio_spec_s audio;          /**< media format struct audio of media_format_audio_spec_s */
        media_format_video_spec_s video;          /**< media format struct video of media_format_video_spec_s */
    }detail;

} media_format_s;

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_FORMAT_PRIVATE_H__ */

