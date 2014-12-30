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

#ifndef __TIZEN_MEDIA_PACKET_H__
#define __TIZEN_MEDIA_PACKET_H__

#include <tizen.h>
#include <stdint.h>

#include <media_format.h>

#include <tbm_type.h>
#include <tbm_surface.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_packet.h
 * @brief This file contains the capi media tool API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MODULE
 * @{
 */

/**
 * @brief  The Media Packet handle.
 * @since_tizen 2.3
 */
typedef struct media_packet_s *media_packet_h;

/**
 * @brief Enumeration for media packet error.
 * @since_tizen 2.3
 */
typedef enum
{
    MEDIA_PACKET_ERROR_NONE     = TIZEN_ERROR_NONE,                                     /**< Successful */
    MEDIA_PACKET_ERROR_OUT_OF_MEMORY    = TIZEN_ERROR_OUT_OF_MEMORY,                    /**< Out of memory */
    MEDIA_PACKET_ERROR_INVALID_PARAMETER  = TIZEN_ERROR_INVALID_PARAMETER,              /**< Invalid parameter */
    MEDIA_PACKET_ERROR_INVALID_OPERATION    = TIZEN_ERROR_INVALID_OPERATION,            /**< Invalid operation */
    MEDIA_PACKET_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,   /**< No space left on device */
} media_packet_error_e;

/**
 * @brief Enumeration for media buffer flag.
 * @since_tizen 2.3
 */
typedef enum
{
    MEDIA_PACKET_CODEC_CONFIG = 0x1,   /**< The buffer marked as such contains codec initialization/codec specific data instead of media data */
    MEDIA_PACKET_END_OF_STREAM = 0x2,  /**< The end of stream */
    MEDIA_PACKET_SYNC_FRAME = 0x4,     /**< The buffer marked as such contains the data for a sync frame */
} media_buffer_flags_e;

/**
 * @brief Enumeration for the return values of media packet finalize call back functions.
 * @since_tizen 2.3
 * @see media_packet_finalize_cb()
 */
typedef enum _finalize_cb_ret
{
    MEDIA_PACKET_REUSE = 0,  /**< Packet handle is not destroyed */
    MEDIA_PACKET_FINALIZE,   /**< Destroy packet handle */
} media_packet_finalize_cb_ret_t;

/**
 * @brief   Called when the media packet is destroyed.
 * @details It will be invoked when media_packet_destroy() is called.
 *
 * @since_tizen 2.3
 *
 * @param[in] packet        The media packet handle
 * @param[in] error_code    The error code of #media_packet_error_e
 * @param[in] user_data  The user data passed from the callback registration function
 *
 * @pre It will be invoked when media packet is destroyed.
 *
 * @retval #MEDIA_PACKET_REUSE     Packet handle is not destroyed, the handle will be reused.
 * @retval #MEDIA_PACKET_FINALIZE  Destroy media packet handle, the handle will not be reused.
 *
 * @see media_packet_destroy()
 * @see media_packet_create_alloc()
 * @see media_packet_create()
 * @see media_packet_copy()
 * @see media_packet_create_from_tbm_surface()
 */
typedef int (*media_packet_finalize_cb)(media_packet_h packet, int error_code, void *user_data);

/**
 * @brief    Creates a media packet handle and allocates buffer.
 * @details  The buffer will be allocated to heap or tbm_surface.
 *
 * @since_tizen 2.3
 *
 * @param[in]  fmt       The allocated #media_format_h by caller
 * @param[in]  fcb       The media_packet_finalize_cb() to register
 * @param[in]  fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out] packet    A new handle for media packet
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre must have media_format_h instance by media_format_create()
 * @post must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
  * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_info(fmt, MEDIA_FORMAT_H264_HP, 480, 640, 10000000, 15000000);

   media_packet_create_alloc (fmt, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create_alloc(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet);

/**
 * @brief    Creates a media packet handle.
 * @details  It creates only media packet handle without allocated buffer.
 *
 * @since_tizen 2.3
 *
 * @param[in] fmt       The allocated #media_format_h by caller
 * @param[in] fcb       The media_packet_finalize_cb() to register
 * @param[in] fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out] packet   A new handle for media packet
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre must have media_format_h instance by media_format_create()
 * @post must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
  * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_info(fmt, MEDIA_FORMAT_H264_HP, 480, 640, 10000000, 15000000);

   media_packet_create (fmt, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create(media_format_h fmt, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet);

/**
 * @brief    Copies a media packet handle.
 * @details  It re-creates only media packet handle with exist media packet handle.
 *
 * @since_tizen 2.3
 *
 * @param[in]   org_packet   The existing media packet handle
 * @param[in]   fcb          The media_packet_finalize_cb() to register
 * @param[in]   fcb_data     The user data to be passed to the media_packet_finalize_cb() function
 * @param[out]  new_packet   A new handle for media packet
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 */
int media_packet_copy(media_packet_h org_packet, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h* new_packet);

/**
 * @brief    Allocates buffer with media packet handle.
 * @details  Before using media_packet_alloc(), media packet handle must be exist.
 *
 * @since_tizen 2.3
 *
 * @param[in] packet   The existing media packet handle
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_create()
 * @see media_packet_destroy()
 */
int media_packet_alloc(media_packet_h packet);

/**
 * @brief    Creates media packet handle and allocates buffer with #tbm_surface_h.
 *
 * @since_tizen 2.3
 *
 * @param[in]   fmt       The allocated #media_format_h by caller
 * @param[in]   surface   The #tbm_surface_h by caller
 * @param[in]   fcb       The media_packet_finalize_cb() to register
 * @param[in]   fcb_data  The user data to be passed to the media_packet_finalize_cb() function
 * @param[out]  packet  A new handle for media packet
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre must have media_format_h instance by media_format_create()
 * @post must do media_format_unref()
 *
 * @see media_packet_destroy()
 * @see media_packet_finalize_cb()
 * @see media_format_unref()
  * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;
   media_packet_h packet;

   media_format_create(&fmt);
   media_format_set_video_info(fmt, MEDIA_FORMAT_RGBA, 128, 128, 2000000, 15000000);

   media_packet_create_from_tbm_surface (fmt, surface, _finalize_callback, fcb_data, &packet);
   media_format_unref(fmt);

   ...
   }

   int _finalize_callback(media_packet_h packet, int err, void* userdata)
   {
       ...
       return MEDIA_PACKET_FINALIZE;
   }

   @endcode
 */
int media_packet_create_from_tbm_surface(media_format_h fmt, tbm_surface_h surface, media_packet_finalize_cb fcb, void *fcb_data, media_packet_h *packet);

/**
 * @brief Gets #media_format_h of media packet
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] fmt      The media format of media packet
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_format_unref()
  * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h* fmt;

   media_packet_get_format (pakcet, &fmt);
   ..

   media_format_unref(fmt);
   ...
   }

   @endcode
 */
int media_packet_get_format(media_packet_h packet, media_format_h *fmt);

/**
 * @brief Sets #media_format_h of media packet
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] fmt      The #media_format_h to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_format_unref()
  * @par Example
   @code
   #include <media_packet.h>

   {
   media_format_h fmt;

   media_packet_set_format (pakcet, fmt);
   ..

   media_format_unref(fmt);
   ...
   }

   @endcode
 */
int media_packet_set_format(media_packet_h packet, media_format_h fmt);

/**
 * @brief Sets PTS of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] pts      The PTS value to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_pts(media_packet_h packet, uint64_t pts);

/**
 * @brief Sets DTS of media packet handle.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[in]  dts      The DTS value to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_dts(media_packet_h packet, uint64_t dts);

/**
 * @brief Sets PTS of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet    The media packet handle
 * @param[in] duration  The duration value to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_duration(media_packet_h packet, uint64_t duration);

/**
 * @brief Sets buffer size of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] size     The buffer size value to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_buffer_size(media_packet_h packet, uint64_t size);

/**
 * @brief Gets PTS of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] pts      The PTS value to get
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_packet_get_pts(media_packet_h packet, uint64_t *pts);

/**
 * @brief Gets DTS of media packet
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] dts      The DTS value to get
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_dts(media_packet_h packet, uint64_t *dts);

/**
 * @brief Gets duration of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet    The media packet handle
 * @param[out] duration  The duration value to get
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_duration(media_packet_h packet, uint64_t *duration);

/**
 * @brief Gets buffer size of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] size     The buffer size value to get
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_buffer_size(media_packet_h packet, uint64_t *size);

/**
 * @brief Gets buffer data pointer of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] data     The allocated buffer data pointer
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_buffer_data_ptr(media_packet_h packet, void **data);

/**
 * @brief Gets TBM surface data of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] surface  The tbm_surface data pointer
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_tbm_surface(media_packet_h packet, tbm_surface_h *surface);

/**
 * @brief Sets extra data of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] extra    The extra data to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_extra(media_packet_h packet, void *extra);

/**
 * @brief Gets extra data of media packet.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] extra    The extra data to get
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_get_extra(media_packet_h packet, void **extra);

/**
 * @brief Checks whether the given media packet is for video.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_video @c true if the given media packet is for video,
 *                      otherwise @c false if the given media packet is not for video
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_type_e
 */
int media_packet_is_video(media_packet_h packet, bool *is_video);

/**
 * @brief Checks whether the given media packet is for audio.
 * @since_tizen 2.3
 *
 * @param[in]  packet    The media packet handle
 * @param[out] is_audio  @c true if the given media packet is for audio,
 *                       otherwise @c false if the given media packet is not for audio
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_mimetype_e
 * @see media_format_type_e
 */
int media_packet_is_audio(media_packet_h packet, bool *is_audio);

/**
 * @brief Checks whether the given media packet is encoded type.
 * @since_tizen 2.3
 *
 * @param[in]  packet      The media packet handle
 * @param[out] is_encoded  @c true if the given media packet is encoded,
 *                         otherwise @c false if the given media packet is not encoded
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_encoded(media_packet_h packet, bool *is_encoded);

/**
 * @brief Checks whether the given media packet is raw type.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_raw   @c true if the given media packet is for raw video,
 *                      otherwise @c false if the given media packet is not for raw video
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_raw(media_packet_h packet, bool *is_raw);

/**
 * @brief Sets #media_buffer_flags_e of media packet.
 * @since_tizen 2.3
 *
 * @param[in] packet    The media packet handle
 * @param[in] flags    The media_buffer_flags_e of media packet to set
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_buffer_flags_e
 * @see media_packet_unset_flags()
 */
int media_packet_set_flags(media_packet_h packet, media_buffer_flags_e flags);

/**
 * @brief Unsets media_buffer_flags_e of media packet
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[in] flags    The media_buffer_flags_e of media packet to unset
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_buffer_flags_e
 * @see media_packet_set_flags()
 * */
int media_packet_unset_flags(media_packet_h packet, media_buffer_flags_e flags);

/**
 * @brief Checks whether the given media packet is codec data.
 * @since_tizen 2.3
 *
 * @param[in] packet            The media packet handle
 * @param[out] is_codec_config  @c true if the given media packet is for codec data,
 *                              otherwise @c false if the given media packet is not for codec data
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_codec_config(media_packet_h packet, bool *is_codec_config);

/**
 * @brief Checks whether the given media packet is eos.
 * @since_tizen 2.3
 *
 * @param[in] packet   The media packet handle
 * @param[out] is_eos  @c true if the given media packet is for eos,
 *                     otherwise @c false if the given media packet is not for eos
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_end_of_stream(media_packet_h packet, bool *is_eos);

/**
 * @brief Checks whether the given media packet is sync frame.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] is_sync  @c true if the given media packet is for sync frame,
 *                      otherwise @c false if the given media packet is not for sync frame
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_format_mimetype_e
 * @see media_format_data_type_e
 */
int media_packet_is_sync_frame(media_packet_h packet, bool *is_sync);

/**
 * @brief Checks whether the allocated buffer is tbm surface or not.
 * @since_tizen 2.3
 *
 * @param[in]  packet   The media packet handle
 * @param[out] has_tbm_surface  @c true if the given media packet's allocated buffer is tbm surface,
 *                      otherwise @c false if the given media packet's allocated buffer is not tbm surface
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_has_tbm_surface_buffer(media_packet_h packet, bool* has_tbm_surface);

/**
 * @brief Destroys the media packet handle.
 * @details  The registered finalize_callback() function will be invoked to destroy the media packet handle.
 * @since_tizen 2.3
 *
 * @param[in] packet  The handle to media packet to be destroyed
 *
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_create_alloc()
 * @see media_packet_create()
 * @see media_packet_copy()
 * @see media_packet_create_from_tbm_surface()
 */
int media_packet_destroy(media_packet_h packet);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_AV_PACKET_BUFFER_H__ */
