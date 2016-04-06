/*
* Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#ifndef __TIZEN_MEDIA_PACKET_POOL_H__
#define __TIZEN_MEDIA_PACKET_POOL_H__

#include <media_format.h>
#include <media_packet.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_packet_pool.h
 * @brief This file contains the media packet pool API.
 */

/**
 * @addtogroup CAPI_MEDIA_TOOL_MODULE
 * @{
 */

/**
 * @brief  The Media Packet Pool Handle.
 * @since_tizen 3.0
 */
typedef struct media_packet_pool_s *media_packet_pool_h;

/**
 * @brief Enumeration for media packet pool error.
 * @since_tizen 3.0
 */
typedef enum {
	MEDIA_PACKET_POOL_ERROR_NONE = TIZEN_ERROR_NONE,                              /**< Successful */
	MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,            /**< Out of memory */
	MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,    /**< Invalid operation */
} media_packet_pool_error_e;

/**
 * @brief Creates a media packet pool to handle the media packets
 * @since_tizen 3.0
 *
 * @details    It creates media packet pool instance
 *
 * @param[out] pool Media packet pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @post media_packet_pool_allocate()
 * @post media_packet_pool_acquire_packet()
 * @post media_packet_pool_release_packet()
 * @post media_packet_pool_destroy()
 */
int media_packet_pool_create(media_packet_pool_h *pool);


/**
 * @brief Set the media format for media packet pool
 * @since_tizen 3.0
 *
 * @details    Set the media format
 * @param[in]  pool Allocated pool handle
 * @param[in]  fmt Media format handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @post media_packet_pool_allocate()
 * @post media_packet_pool_acquire_packet()
 * @post media_packet_pool_release_packet()
 * @post media_packet_pool_destroy()
 */
int media_packet_pool_set_media_format(media_packet_pool_h pool, media_format_h fmt);

/**
 * @brief Allocates media packet pool
 * @since_tizen 3.0
 *
 * @details    It allocates the media packet pool
 * @param[in]  pool Allocated pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_set_media_format()
 * @post media_packet_pool_acquire_packet()
 * @post media_packet_pool_release_packet()
 * @post media_packet_pool_destroy()
 */
int media_packet_pool_allocate(media_packet_pool_h pool);

/**
 * @brief Sets the media packet pool size
 * @since_tizen 3.0
 * @details    Sets the media packet pool size
 * @param[in]  pool Allocated pool handle
 * @param[in]  min_buffers   the minimum amount of buffers to allocate
 * @param[in]  max_buffers   the maximum amount of buffers to allocate
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_allocate()
 * @see media_packet_pool_release_packet()
 * @see media_packet_pool_get_size()
 */
int media_packet_pool_set_size(media_packet_pool_h pool, int min_buffers, int max_buffers);

/**
 * @brief Gets the media packet pool size
 * @since_tizen 3.0
 * @details    Gets the media packet pool size
 * @param[in]  pool Allocated pool handle
 * @param[out]  min_buffers   the minimum amount of buffers to allocate
 * @param[out]  max_buffers   the maximum amount of buffers to allocate
 * @param[out]  curr_buffers  the current amount of buffers
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_allocate()
 * @pre media_packet_pool_get_size()()
 */

int media_packet_pool_get_size(media_packet_pool_h pool, int *min_buffers, int *max_buffers, int *curr_buffers);

/**
 * @brief Aquires the media packet from media packet pool
 * @since_tizen 3.0
 * @details    Get the media packet if the pool is already created.
 * @param[in]  pool Media packet pool handle
 * @param[out] pkt  Media packet handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_allocate()
 * @post media_packet_pool_release_packet()
 */

int media_packet_pool_acquire_packet(media_packet_pool_h pool, media_packet_h *pkt);

/**
 * @brief Release the media packet to pool
 * @since_tizen 3.0
 * @details    Reuse the media packet by adding into media packet buffer pool
 * @param[in]  pool Mediapacket pool handle
 * @param[in]  pkt  Media packet handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_acquire_packet()
 */
int media_packet_pool_release_packet(media_packet_pool_h pool, media_packet_h pkt);


/**
 * @brief Deallocate the allocated packets in media_packet_pool_allocate()
 * @since_tizen 3.0
 * @details    De allocate all the media packets
 * @param[in]  pool Mediapacket pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
  * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_create()
 * @pre media_packet_pool_allocate()
 * @post media_packet_pool_destroy()
 */

int media_packet_pool_deallocate(media_packet_pool_h pool);

/**
 * @brief Destroy the media packat pool
 * @since_tizen 3.0
 * @details    Destroy the media pool
  * @param[in]  pool Mediapacket pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_deallocate()
 * @see media_packet_pool_allocate()
 * @see media_packet_pool_create()
 */

int media_packet_pool_destroy(media_packet_pool_h pool);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif   /*__TIZEN_MEDIA_PACKET_POOL_H__*/
