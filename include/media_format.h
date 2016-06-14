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

#ifndef __TIZEN_MEDIA_FORMAT_H__
#define __TIZEN_MEDIA_FORMAT_H__

#include <stdbool.h>
#include <stdint.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_format.h
 * @brief This file contains the capi media tool API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE
 * @{
 */

/**
 * @brief Media Format handle type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_format_s *media_format_h;

/**
 * @brief Enumerations of  media format error.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	MEDIA_FORMAT_ERROR_NONE = TIZEN_ERROR_NONE,					/**< Successful */
	MEDIA_FORMAT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,				/**< Out of memory */
	MEDIA_FORMAT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,		/**< Invalid parameter */
	MEDIA_FORMAT_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,			/**< Invalid operation */
	MEDIA_FORMAT_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,	/**< No space left on device */
} media_format_error_e;

/**
 * @brief Enumeration for media format type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	MEDIA_FORMAT_NONE = 0x00000000,					 /**< media format type is NONE */
	MEDIA_FORMAT_AUDIO = 0x00100000,				 /**< media format type is AUDIO */
	MEDIA_FORMAT_VIDEO = 0x00200000,				 /**< media format type is VIDEO */
	MEDIA_FORMAT_CONTAINER = 0x00400000,			 /**< media format type is CONTAINER */
	MEDIA_FORMAT_TEXT = 0x00800000,					 /**< media format type is TEXT */
	MEDIA_FORMAT_UNKNOWN = 0x08000000,				 /**< media format type is UNKNOWN */
} media_format_type_e;

/**
 * @brief Enumeration for media format data type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	MEDIA_FORMAT_ENCODED = 0x10000000,			/**< media format data type is encoded type */
	MEDIA_FORMAT_RAW = 0x20000000,				/**< media format data type is raw type */
} media_format_data_type_e;

/**
 * @brief Enumeration for media format MIME type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	/* Audio */
	MEDIA_FORMAT_L16 = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1010),		   /**< media format mime type is L16, AUDIO*/
	MEDIA_FORMAT_ALAW = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1020),		   /**< media format mime type is ALAW, AUDIO*/
	MEDIA_FORMAT_ULAW = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1030),		   /**< media format mime type is ULAW,  AUDIO */
	MEDIA_FORMAT_AMR = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1040),		   /**< media format mime type is AMR,  AUDIO,  indicates MEDIA_FORMAT_AMR_NB (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_AMR_NB = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1040),		   /**< media format mime type is AMR_NB,  AUDIO , (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_AMR_WB = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1041),		   /**< media format mime type is AMR_WB,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_G729 = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1050),		   /**< media format mime type is G729,  AUDIO*/
	MEDIA_FORMAT_AAC = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1060),		   /**< media format mime type is AAC,  AUDIO, indicates MEDIA_FORMAT_AAC_LC (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_AAC_LC = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1060),		   /**< media format mime type is AAC_LC,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_AAC_HE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1061),		   /**< media format mime type is AAC_HE,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_AAC_HE_PS = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1062),	   /**< media format mime type is AAC_HE_PS,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_MP3 = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1070),		   /**< media format mime type is MP3,  AUDIO*/
	MEDIA_FORMAT_VORBIS = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1080),		   /**< media format mime type is VORBIS,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_FLAC = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x1090),		   /**< media format mime type is FLAC,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_WMAV1 = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x10A0),		   /**< media format mime type is WMAV1,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_WMAV2 = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x10A1),		   /**< media format mime type is WMAV2,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_WMAPRO = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x10A2),		   /**< media format mime type is WMAVPRO,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_WMALSL = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_ENCODED | 0x10A3),		   /**< media format mime type is WMAVLSL,  AUDIO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */

	MEDIA_FORMAT_PCM = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1510),			   /**< media format mime type is PCM, AUDIO, indicates MEDIA_FORMAT_PCM_S16LE (Since 3.0) */
	MEDIA_FORMAT_PCM_S16LE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1510),		   /**< media format mime type is PCM signed 16-bit little-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_S24LE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1511),		   /**< media format mime type is PCM signed 24-bit little-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_S32LE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1512),		   /**< media format mime type is PCM signed 32-bit little-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_S16BE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1513),		   /**< media format mime type is PCM signed 16-bit big-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_S24BE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1514),		   /**< media format mime type is PCM signed 24-bit big-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_S32BE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1515),		   /**< media format mime type is PCM signed 32-bit big-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_F32LE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1516),		   /**< media format mime type is PCM 32-bit floating point little-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCM_F32BE = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1517),		   /**< media format mime type is PCM 32-bit floating point big-endian, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_PCMA = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1520),			   /**< media format mime type is PCM A-law, AUDIO */
	MEDIA_FORMAT_PCMU = (MEDIA_FORMAT_AUDIO | MEDIA_FORMAT_RAW | 0x1530),			   /**< media format mime type is PCM U-law, AUDIO */

	/* Video */
	MEDIA_FORMAT_H261 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2010),		   /**< media format mime type is H261, VIDEO */
	MEDIA_FORMAT_H263 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2020),		   /**< media format mime type is H263, VIDEO */
	MEDIA_FORMAT_H263P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2021),		   /**< media format mime type is H263P, VIDEO */
	MEDIA_FORMAT_H263_BLP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2022),		  /**< media format mime type is H263 Baseline Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_H320_CEP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2023),		   /**< media format mime type is H263 H.320 Coding Efficiency Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_BCP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2024),		  /**< media format mime type is H263 Backward-Compatibility Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_ISW2P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2025),			/**< media format mime type is H263 Interactive and Streaming Wireless Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_ISW3P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2026),			/**< media format mime type is H263 Interactive and Streaming Wireless Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_CHCP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2027),		   /**< media format mime type is H263 Conversation High Compression Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_C_INTERNETP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2028),		  /**< media format mime type is H263 Conversational Internet Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_C_INTERLACEP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2029),		   /**< media format mime type is H263 Conversational Interlace Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H263_HLP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x202A),		  /**< media format mime type is H263 High Latency Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_H264_SP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2030),	   /**< media format mime type is H264_SP, VIDEO */
	MEDIA_FORMAT_H264_MP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2031),	   /**< media format mime type is H264_MP, VIDEO */
	MEDIA_FORMAT_H264_HP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2032),	   /**< media format mime type is H264_HP, VIDEO */
	MEDIA_FORMAT_H264_EXTP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2033),		 /**< media format mime type is H264 Extended Profile, VIDEO (Since 3.0) */
	MEDIA_FORMAT_H264_H10P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2034),		 /**< media format mime type is H264 High10 Profile, VIDEO (Since 3.0) */
	MEDIA_FORMAT_H264_H422P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2035),		  /**< media format mime type is H264 High422 Profile, VIDEO (Since 3.0) */
	MEDIA_FORMAT_H264_H444P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2036),		  /**< media format mime type is H264 High444 Profile, VIDEO (Since 3.0) */
	MEDIA_FORMAT_H264_C444P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2037),		  /**< media format mime type is H264 CAVLC444 Profile, VIDEO (Since 3.0) */
	MEDIA_FORMAT_MJPEG = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2040),		   /**< media format mime type is MJPEG, VIDEO */
	MEDIA_FORMAT_MPEG1 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2050),		   /**< media format mime type is MPEG1, VIDEO */
	MEDIA_FORMAT_MPEG2_SP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2060),	   /**< media format mime type is MPEG2_SP, VIDEO */
	MEDIA_FORMAT_MPEG2_MP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2061),	   /**< media format mime type is MPEG2_MP, VIDEO */
	MEDIA_FORMAT_MPEG2_HP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2062),	   /**< media format mime type is MPEG2_HP, VIDEO */
	MEDIA_FORMAT_MPEG4_SP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2070),	   /**< media format mime type is MPEG4_SP, VIDEO */
	MEDIA_FORMAT_MPEG4_ASP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2071),	   /**< media format mime type is MPEG4_ASP, VIDEO */
	MEDIA_FORMAT_HEVC = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2080),		   /**< media format mime type is HEVC, VIDEO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_HEVC_MP = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2081),		  /**< media format mime type is HEVC Main Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_HEVC_M10P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2082),			/**< media format mime type is HEVC Main10 Profile, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_VP8 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x2090),		   /**< media format mime type is VP8, VIDEO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_VP9 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x20A0),		   /**< media format mime type is VP9, VIDEO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	MEDIA_FORMAT_VC1 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_ENCODED | 0x20B0),		   /**< media format mime type is VC1, VIDEO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */

	MEDIA_FORMAT_I420 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2510),			   /**< media format mime type is I420, VIDEO */
	MEDIA_FORMAT_NV12 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2520),			   /**< media format mime type is NV12, VIDEO */
	MEDIA_FORMAT_NV12T = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2530),			   /**< media format mime type is NV12T, VIDEO */
	MEDIA_FORMAT_YV12 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2540),			   /**< media format mime type is YV12, VIDEO */
	MEDIA_FORMAT_NV21 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2550),			   /**< media format mime type is NV21, VIDEO */
	MEDIA_FORMAT_NV16 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2560),			   /**< media format mime type is NV16, VIDEO */
	MEDIA_FORMAT_YUYV = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2570),			   /**< media format mime type is YUYV, VIDEO */
	MEDIA_FORMAT_UYVY = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2580),			   /**< media format mime type is UYVY, VIDEO */
	MEDIA_FORMAT_422P = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x2590),			   /**< media format mime type is 422P, VIDEO */
	MEDIA_FORMAT_RGB565 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x25a0),			   /**< media format mime type is RGB565, VIDEO */
	MEDIA_FORMAT_RGB888 = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x25b0),			   /**< media format mime type is RGB888, VIDEO */
	MEDIA_FORMAT_RGBA = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x25c0),			   /**< media format mime type is RGBA, VIDEO */
	MEDIA_FORMAT_ARGB = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x25d0),			   /**< media format mime type is ARGB, VIDEO */
	MEDIA_FORMAT_BGRA = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x25e0),			   /**< media format mime type is BGRA, VIDEO, (Since 3.0) */

	/* CONTAINER VIDEO */
	MEDIA_FORMAT_CONTAINER_MP4 = (MEDIA_FORMAT_CONTAINER | 0x3010),					   /**< media format mime type is MP4 container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_AVI = (MEDIA_FORMAT_CONTAINER | 0x3020),					   /**< media format mime type is AVI container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_MPEG2TS = (MEDIA_FORMAT_CONTAINER | 0x3030),				   /**< media format mime type is MPEG2TS container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_MPEG2PS = (MEDIA_FORMAT_CONTAINER | 0x3040),				   /**< media format mime type is MPEG2PS container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_MATROSKA = (MEDIA_FORMAT_CONTAINER | 0x3050),			   /**< media format mime type is MATROSKA container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_WEBM = (MEDIA_FORMAT_CONTAINER | 0x3060),				   /**< media format mime type is WEBM container, VIDEO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_3GP = (MEDIA_FORMAT_CONTAINER | 0x3070),					   /**< media format mime type is 3GP container, VIDEO, (Since 3.0) */

	/*CONTAINER AUDIO */
	MEDIA_FORMAT_CONTAINER_WAV = (MEDIA_FORMAT_CONTAINER | 0x4010),					   /**< media format mime type is WAV container, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_OGG = (MEDIA_FORMAT_CONTAINER | 0x4020),					   /**< media format mime type is OGG container, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_AAC_ADTS = (MEDIA_FORMAT_CONTAINER | 0x4030),			   /**< media format mime type is AAC_ADTS container, AUDIO, (Since 3.0) */
	MEDIA_FORMAT_CONTAINER_AAC_ADIF = (MEDIA_FORMAT_CONTAINER | 0x4031),			   /**< media format mime type is AAC_ADIF container, AUDIO, (Since 3.0) */

	MEDIA_FORMAT_NATIVE_VIDEO = (MEDIA_FORMAT_VIDEO | MEDIA_FORMAT_RAW | 0x7000),	   /**< media format mime type is HW dependent, VIDEO, (Since @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif) */
	/* Text */
	MEDIA_FORMAT_TEXT_MP4 = (MEDIA_FORMAT_TEXT | MEDIA_FORMAT_ENCODED | 0x8010),		/**< media format mime type is MP4, TEXT, (Since 3.0) */
	MEDIA_FORMAT_TEXT_3GP = (MEDIA_FORMAT_TEXT | MEDIA_FORMAT_ENCODED | 0x8020),		/**< media format mime type is 3GP, TEXT, (Since 3.0) */

	MEDIA_FORMAT_MAX																   /**< media format mime type is MEDIA_FORMAT_MAX, Do not use */
} media_format_mimetype_e;

/**
 * @brief Enumeration for media color model.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	MEDIA_COLOR_MODEL_NONE,		   /**< media format color model is NONE */
	MEDIA_COLOR_MODEL_RGB,		   /**< media format color model is RGB */
	MEDIA_COLOR_MODEL_YUV		   /**< media format color model is YUV */
} media_format_color_model_e;

/**
 * @brief Enumeration for media format aac header type.
 * @since_tizen 3.0
 */
typedef enum {
	MEDIA_FORMAT_AAC_HEADER_NONE,		   /**< media format aac header type is RAW, no header */
	MEDIA_FORMAT_AAC_HEADER_ADTS,		   /**< media format aac header type is ADTS header */
	MEDIA_FORMAT_AAC_HEADER_ADIF,		   /**< media format aac header type is ADIF header */
	MEDIA_FORMAT_AAC_HEADER_UNKNOWN,		  /**< media format aac header type is UNKNOWN */
} media_format_aac_header_type_e;

/**
 * @brief Enumeration for media format text type.
 * @since_tizen 3.0
 */
typedef enum {
	MEDIA_FORMAT_TEXT_TYPE_NONE,	/**< media format text type is none */
	MEDIA_FORMAT_TEXT_TYPE_TIMED_TEXT_MP4,	  /**< media format text type is MPEG-4 Timed Text */
	MEDIA_FORMAT_TEXT_TYPE_TIMED_TEXT_3GPP,	   /**< media format text type is 3GPP Timed Text */
	MEDIA_FORMAT_TEXT_TYPE_UNKNOWN,	   /**< media format text type is unknown */
} media_format_text_type_e;

/**
 * @brief Creates a media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details	It creates media format. and reference count will be set to 1
			For destroying media_format_h handle, use media_format_unref().
 * @param[out] fmt allocated media_format_h
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_ref()
 * @see media_format_unref()
 */
int media_format_create(media_format_h * fmt);

/**
 * @brief Gets format type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get format type
 * @param[out] formattype The media_format_type_e, ex) MEDIA_FORMAT_AUDIO
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_get_type(media_format_h fmt, media_format_type_e * formattype);

/**
 * @brief Gets container MIME type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get container mime type
 * @param[out] mimetype The media_format_mimetype_e, ex) MEDIA_FORMAT_CONTAINER_MP4
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_container_mime()
 */
int media_format_get_container_mime(media_format_h fmt, media_format_mimetype_e * mimetype);

/**
 * @brief Gets text track information of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get text information
 * @param[out] mimetype The media_format_mimetype_e, ex) MEDIA_FORMAT_TEXT_MP4
 * @param[out] type The media_format_text_type_e
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_text_mime()
 */
int media_format_get_text_info(media_format_h fmt, media_format_mimetype_e * mimetype, media_format_text_type_e * type);

/**
 * @brief Gets video information of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details   returns mimetype, width, height, average bps, max bps of this media format.
 * @param[in] fmt media_format_h to get video information
 * @param[out] mimetype media_format_mimetype_e , ex) MEDIA_FORMAT_H264_HP
 * @param[out] width width of the video
 * @param[out] height height of the video
 * @param[out] avg_bps average bps of the video
 * @param[out] max_bps max bps of the video
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_video_mime()
 * @see media_format_set_video_width()
 * @see media_format_set_video_height()
 * @see media_format_set_video_avg_bps()
 * @see media_format_set_video_max_bps()
 */
int media_format_get_video_info(media_format_h fmt, media_format_mimetype_e * mimetype, int *width, int *height, int *avg_bps, int *max_bps);

/**
 * @brief Gets audio information of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details   returns mimetype, channels, height, samplerate bps, max bps of this media format.
 * @param[in] fmt media_format_h to get audio information
 * @param[out] mimetype media_format_mimetype_e , ex) MEDIA_FORMAT_PCM
 * @param[out] channel channel of the audio
 * @param[out] samplerate sampling rate of the audio
 * @param[out] bit bit resolution of the audio
 * @param[out] avg_bps average bps of the audio
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_audio_mime()
 * @see media_format_set_audio_channel()
 * @see media_format_set_audio_samplerate()
 * @see media_format_set_audio_bit()
 * @see media_format_set_audio_avg_bps()
 */
int media_format_get_audio_info(media_format_h fmt, media_format_mimetype_e * mimetype, int *channel, int *samplerate, int *bit, int *avg_bps);

/**
 * @brief Gets audio aac type of media format.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @param[in] fmt media_format_h to get audio information
 * @param[out] is_adts aac adts flag of the audio
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_audio_aac_type()
 */
int media_format_get_audio_aac_type(media_format_h fmt, bool * is_adts);

/**
 * @brief Gets audio aac header type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get audio information
 * @param[out] aac_header_type The media_format_aac_header_type_e, ex) MEDIA_FORMAT_AAC_HEADER_ADTS
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_set_audio_aac_header_type()
 */
int media_format_get_audio_aac_header_type(media_format_h fmt, media_format_aac_header_type_e * aac_header_type);

/**
 * @brief Gets video frame rate of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get
 * @param[out] frame_rate The video frame rate
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_get_video_frame_rate(media_format_h fmt, int *frame_rate);

/**
 * @brief Sets container MIME type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to set
 * @param[in] mimetype The media_format_mimetype_e, ex) MEDIA_FORMAT_MP4
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_container_mime()
 */
int media_format_set_container_mime(media_format_h fmt, media_format_mimetype_e mimetype);

/**
 * @brief Sets text MIME type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to set
 * @param[in] mimetype The media_format_mimetype_e, ex) MEDIA_FORMAT_TEXT*
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_text_info()
 */
int media_format_set_text_mime(media_format_h fmt, media_format_mimetype_e mimetype);

/**
 * @brief Sets text type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to set
 * @param[in] type The text type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_text_info()
 */
int media_format_set_text_type(media_format_h fmt, media_format_text_type_e type);

/**
 * @brief Sets video MIME type of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] mimetype media_format_mimetype_e , ex) MEDIA_FORMAT_H264_HP
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_video()
 */
int media_format_set_video_mime(media_format_h fmt, media_format_mimetype_e mimetype);

/**
 * @brief Sets video width of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] width width of the video
 * @pre must set video MIME type by media_format_set_video_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_video()
 */
int media_format_set_video_width(media_format_h fmt, int width);

/**
 * @brief Sets video height of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] height height of the video
 * @pre must set video MIME type by media_format_set_video_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_video()
 */
int media_format_set_video_height(media_format_h fmt, int height);

/**
 * @brief Sets video avg_bps of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] avg_bps average bps of the video
 * @pre must set video MIME type by media_format_set_video_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_video()
 */
int media_format_set_video_avg_bps(media_format_h fmt, int avg_bps);

/**
 * @brief Sets video max_bps of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] max_bps max bps of the video
 * @pre must set video MIME type by media_format_set_video_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_video()
 */
int media_format_set_video_max_bps(media_format_h fmt, int max_bps);

/**
 * @brief Sets video frame rate of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to set
 * @param[in] frame_rate The video frame rate
 * @pre must set video MIME type by media_format_set_video_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_set_video_frame_rate(media_format_h fmt, int frame_rate);

/**
 * @brief Sets audio MIME type of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set audio information
 * @param[in] mimetype media_format_mimetype_e , ex) MEDIA_FORMAT_PCM
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio()
 */
int media_format_set_audio_mime(media_format_h fmt, media_format_mimetype_e mimetype);

/**
 * @brief Sets audio channel of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set audio information
 * @param[in] channel channel of the audio
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio()
 */
int media_format_set_audio_channel(media_format_h fmt, int channel);

/**
 * @brief Sets audio samplerate of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] samplerate sampling rate of the audio
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio()
 */
int media_format_set_audio_samplerate(media_format_h fmt, int samplerate);

 /**
 * @brief Sets audio bit of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details   Sets audio bit resolution of this media format
 * @param[in] fmt media_format_h to set
 * @param[in] bit bit of the audio
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio()
 */
int media_format_set_audio_bit(media_format_h fmt, int bit);

 /**
 * @brief Sets audio avg_bps of media format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt media_format_h to set
 * @param[in] avg_bps avg_bps of the audio
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio()
 */
int media_format_set_audio_avg_bps(media_format_h fmt, int avg_bps);

 /**
 * @brief Sets audio aac type of media format.
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 * @details   Sets audio aac data type of this media format
 * @param[in] fmt media_format_h to set
 * @param[in] is_adts aac adts flag of the audio
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio_aac_type()
 */
int media_format_set_audio_aac_type(media_format_h fmt, bool is_adts);

/**
 * @brief Sets audio aac header type of media format.
 * @since_tizen 3.0
 * @param[in] fmt The media_format_h to get audio information
 * @param[in] aac_header_type The media_format_aac_header_type_e, ex) MEDIA_FORMAT_AAC_HEADER_ADTS
 * @pre must set audio MIME type by media_format_set_audio_mime()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_get_audio_aac_header_type()
 */
int media_format_set_audio_aac_header_type(media_format_h fmt, media_format_aac_header_type_e aac_header_type);

/**
 * @brief Increase reference count of media_format_h object.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details    increase ref_count of media_format_h
 * @param[in] fmt exist media_format_h
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_unref()
 */
int media_format_ref(media_format_h fmt);

/**
 * @brief Decrease reference count of media_format_h object.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt exist media_format_h
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 * @see media_format_ref()
 */
int media_format_unref(media_format_h fmt);

/**
 * @brief Check whether the media_format_h is writable or not.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] fmt exist media_format_h
 * @param[out] is_writable if ref_count is 1, return true. if not, return false
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_is_writable(media_format_h fmt, bool * is_writable);

 /**
 * @brief Returns a writable copy of media_format_h.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details If there is only one reference count on fmt, the caller must be the owner,
 *              and so this function will return the object unchanged.
 *              while, if there is more than one reference count on the object, a new object will be returned.
 *              The caller will own a reference to the returned object.
 *              In a nutshell, this function unrefs the fmt and refs the out_fmt returned.
 *              Don't access fmt after calling this function.
 * @param[in] fmt exist media_format_h , don't access it after calling this function
 * @param[out] out_fmt new copied media_format_h, the caller is owner of this object
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_FORMAT_ERROR_NONE Successful
 * @retval #MEDIA_FORMAT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_FORMAT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_FORMAT_ERROR_INVALID_OPERATION Invalid operation
 */
int media_format_make_writable(media_format_h fmt, media_format_h * out_fmt);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif							/* __TIZEN_MEDIA_FORMAT_H__ */
