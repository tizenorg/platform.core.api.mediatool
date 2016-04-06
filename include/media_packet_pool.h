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
 * @brief This file contains the Media Packet Pool API.
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
 * @brief Enumeration for media packet pool errors.
 * @since_tizen 3.0
 */
typedef enum {
	MEDIA_PACKET_POOL_ERROR_NONE = TIZEN_ERROR_NONE,                              /**< Successful */
	MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,            /**< Out of memory */
	MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,    /**< Invalid operation */
} media_packet_pool_error_e;

/**
 * @brief	Creates a media packet pool to handle the media packets.
 * @details	It creates a media packet pool instance
 *
 * @since_tizen 3.0
 *
 * @remarks	The @a pool must be released by using packet_pool_destroy().
 * @param[out] pool The media packet pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 */
int media_packet_pool_create(media_packet_pool_h *pool);

/**
 * @brief	Sets the media format for the media packet pool.
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The media packet pool handle
 * @param[in]	fmt	The #media_format_h allocated by the caller
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Must have media_format_h instance by media_format_create().
 * @post Must do media_format_unref().
 */
int media_packet_pool_set_media_format(media_packet_pool_h pool, media_format_h fmt);

/**
 * @brief	Sets the media packet pool size.
 * @details	Sets the amount of packets to allocate with given parameters.
 *
 * @since_tizen 3.0
 *
 * @param[in]  pool The allocated pool handle
 * @param[in]  min_buffers   The minimum amount of buffers to allocate
 * @param[in]  max_buffers   The maximum amount of buffers to allocate
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 */
int media_packet_pool_set_size(media_packet_pool_h pool, int min_buffers, int max_buffers);

/**
 * @brief	Gets the media packet pool size.
 * @details	Gets the configuration values from the pool.
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The allocated pool handle
 * @param[out]	min_buffers		The minimum amount of buffers to allocate
 * @param[out]	max_buffers		The maximum amount of buffers to allocate
 * @param[out]	curr_buffers	The current amount of buffers
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 */
int media_packet_pool_get_size(media_packet_pool_h pool, int *min_buffers, int *max_buffers, int *curr_buffers);

/**
 * @brief	Allocates the media packet pool.
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The allocated pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre media_packet_pool_set_media_format() and media_pool_set_size() should be called before calling media_packet_pool_allocate()
 *
 */
int media_packet_pool_allocate(media_packet_pool_h pool);

/**
 * @brief	Acquires a media packet from the media packet pool.
 * @details	Gets the media packet if the pool is already created.
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The media packet pool handle
 * @param[out]	pkt		The media packet handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 */
int media_packet_pool_acquire_packet(media_packet_pool_h pool, media_packet_h *pkt);

/**
 * @brief	Releases the media packet to pool.
 * @details	Reuses the media packet by adding it to the media packet pool.
 *
 * @since_tizen 3.0
 *
 * @param[in]  pool The media packet pool handle
 * @param[in]  pkt The packet to release, the pkt should have previously been acquired from the pool with media_packet_pool_acquire_packet()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 */
int media_packet_pool_release_packet(media_packet_pool_h pool, media_packet_h pkt);


/**
 * @brief	Deallocates all the media packets.
 * @details	Deallocates the packets allocated with media_packet_pool_allocate().
 *
 * @since_tizen 3.0
 *
 * @param[in]  pool The media packet pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see media_packet_pool_allocate()
 */

int media_packet_pool_deallocate(media_packet_pool_h pool);

/**
 * @brief	Destroys the media packet pool.
 * @details	Destroys the media packet pool handle and releases all its resources.
 *
 * @since_tizen 3.0
 *
 * @param[in]	 pool	The handle to media packet pool to be destroyed
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_POOL_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_POOL_ERROR_INVALID_OPERATION Invalid operation
 *
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
