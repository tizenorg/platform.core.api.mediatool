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

#ifndef __TIZEN_MEDIA_PACKET_PRIVATE_H__
#define __TIZEN_MEDIA_PACKET_PRIVATE_H__

#include <media_packet.h>
#include <media_format_private.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_MEDIA_TOOL"

#define MEDIA_PACKET_CHECK_CONDITION(condition,error,msg)     \
                if(condition) {} else \
                { LOGE("[%s] %s(0x%08x)", __FUNCTION__, msg,error); return error;}; \

#define MEDIA_PACKET_INSTANCE_CHECK(media_packet)   \
        MEDIA_PACKET_CHECK_CONDITION(media_packet != NULL, MEDIA_PACKET_ERROR_INVALID_PARAMETER, "MEDIA_PACKET_ERROR_INVALID_PARAMETER")

#define MEDIA_PACKET_NULL_ARG_CHECK(arg)      \
        MEDIA_PACKET_CHECK_CONDITION(arg != NULL, MEDIA_PACKET_ERROR_INVALID_PARAMETER, "MEDIA_PACKET_ERROR_INVALID_PARAMETER")

/**
 * @brief Gets pts of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_PTS(arg) (MEDIA_PACKET_CAST(arg)->pts)

/**
 * @brief Gets dts of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_DTS(arg) (MEDIA_PACKET_CAST(arg)->dts)

/**
 * @brief Gets duration of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_DURATION(arg) (MEDIA_PACKET_CAST(arg)->duration)

/**
 * @brief Gets size of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_SIZE(arg) (MEDIA_PACKET_CAST(arg)->size)

/**
 * @brief Gets data of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_DATA(arg) (MEDIA_PACKET_CAST(arg)->data)

/**
 * @brief Gets the buffer flags of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_FLAGS(arg) (MEDIA_PACKET_CAST(arg)->flags)

/**
 * @brief Gets the buffer config flag of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_CODEC_CONFIG_DATA(arg) (((MEDIA_PACKET_CAST(arg)->flags) >> 0) & 0x001)

/**
 * @brief Gets the buffer eos flag of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_CODEC_END_OF_STREAM(arg) (((MEDIA_PACKET_CAST(arg)->flags) >> 1) & 0x001)

/**
 * @brief Gets the buffer sync flag of media_packet_h
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_CODEC_SYNC_FRAME(arg) (((MEDIA_PACKET_CAST(arg)->flags) >> 2) & 0x001)

/**
 * @brief Casting to media_packet_s
 * @since_tizen 2.3
 */
#define MEDIA_PACKET_CAST(obj) ((media_packet_s*)(obj))


typedef enum {
    MEDIA_BUFFER_TYPE_TBM_SURFACE,
    MEDIA_BUFFER_TYPE_EXTERNAL_TBM_SURFACE,
    MEDIA_BUFFER_TYPE_NORMAL,
    MEDIA_BUFFER_TYPE_AUTO,
    MEDIA_BUFFER_TYPE_MAX
} media_buffer_type_e;

typedef struct _media_packet_s{

    uint64_t pts;
    uint64_t dts;
    uint64_t duration;
    uint64_t size;
    void* data;
    void* surface_data;
    media_packet_finalize_cb finalizecb_func;
    void* userdata;
    bool is_allocated;
    void *extradata;

    media_format_s* format;
    media_buffer_type_e type;
    media_buffer_flags_e flags;

} media_packet_s;

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_PACKET_PRIVATE_H__ */

