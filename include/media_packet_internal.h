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

#ifndef __TIZEN_MEDIA_PACKET_INTERNAL_H__
#define __TIZEN_MEDIA_PACKET_INTERNAL_H__

#include <media_packet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_packet_internal.h
 * @brief This file contains the capi media tool internal API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MODULE
 * @{
 */


/**
 * @internal
 * @brief Sets codec data and the codec data size of media packet.
 * @since_tizen 2.4
 *
 * @param[in] packet   The media packet handle
 * @param[in] codec_data    The codec data to set
 * @param[in] codec_data_size    The codec data size to set
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE              Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_set_codec_data(media_packet_h packet, void* codec_data, unsigned int codec_data_size);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_PACKET_INTERNAL_H__ */
