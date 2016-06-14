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
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY Out of memory
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
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Must have media_format_h instance by media_format_create().
 * @post Must do media_format_unref().
 */
int media_packet_pool_set_media_format(media_packet_pool_h pool, media_format_h fmt);

/**
 * @brief	Sets the media packet pool size.
 * @details	Sets the number of packets to allocate with given parameters.
 *          The media packet pool will be allocated with #min_buffers when media_pacet_pool_allocate() is called.
 *          When there are no media packets available, it will be increased to #max_buffers of given parameter.
 *
 * @since_tizen 3.0
 *
 * @param[in]  pool The allocated pool handle
 * @param[in]  min_buffers   The minimum number of buffers to allocate
 * @param[in]  max_buffers   The maximum number of buffers to allocate
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY Out of memory
 * @post After the pool has been configured, it can be allocated with media_packet_pool_allocate().
 * @see media_packet_pool_get_size()
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
 * @param[out]	min_buffers		The minimum number of buffers to allocate
 * @param[out]	max_buffers		The maximum number of buffers to allocate
 * @param[out]	curr_buffers	The current number of allocated buffers
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @post After the pool has been configured, it can be allocated with media_packet_pool_allocate().
 * @see media_packet_pool_set_size()
 */
int media_packet_pool_get_size(media_packet_pool_h pool, int *min_buffers, int *max_buffers, int *curr_buffers);

/**
 * @brief	Allocates the media packet pool.
 * @details It will allocate media packets with #min_buffers which given to media_packet_pool_set_size().
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The allocated pool handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION The user set the invalid format with media_packet_pool_set_format() or set the invalid size with media_packet_pool_set_size().\n
 *		The user already allocate the pool.
 * @pre media_packet_pool_set_media_format() and media_pool_set_size() should be called before calling media_packet_pool_allocate()
 * @post The pool will be allocated. When the pool is allocated, media_packet_pool_acquire_packet() can be used to retrieve a packet from the pool.
 * @see media_packet_pool_set_media_format()
 * @see media_packet_pool_set_size()
 *
 */
int media_packet_pool_allocate(media_packet_pool_h pool);

/**
 * @brief	Acquires a media packet from the media packet pool.
 * @details	It will take a media packet from the queue and block until media packet is released into the pool agaian.
 *			When there are no media packets available, it will be increased to #max_buffers of given parameter.
 *
 * @since_tizen 3.0
 *
 * @param[in]	pool	The media packet pool handle
 * @param[out]	pkt		The media packet handle
 * @param[in]   timeout It indicates the millisecond-timeouts to block
 *				while waiting for one of acquired packet to be released.
 *				If the value of timeout is -1, the pool blocks indefinitely until media packet into the pool.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION The user does not allocate the pool.\n
 *			The pool should be allocated with media_packet_pool_allocate() before acquiring media packet from the pool.
 * @retval #MEDIA_PACKET_ERROR_NO_AVAILABLE_PACKET No available packet
 * @see media_pakcet_pool_set_size()
 * @see media_packet_pool_release_packet()
 *
 */
int media_packet_pool_acquire_packet(media_packet_pool_h pool, media_packet_h *pkt, long timeout);

/**
 * @brief	Releases the media packet to pool.
 * @details	Rlease a packet back in the pool. It will put the packet back in the queue.
 *
 * @since_tizen 3.0
 *
 * @param[in]  pool The media packet pool handle
 * @param[in]  pkt The packet to release, the pkt should have previously been acquired from the pool with media_packet_pool_acquire_packet()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION The user releases media packet which is not associated with a given pool.
 * @see media_packet_pool_acquire_packet()
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
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION The user does not release all media packets to the pool.
 *
 * @pre media packet should be released before calling media_packet_pool_deallocate().
 * @post The pool will be deallocated. media_packet_pool_acquire_packet() calls will return an error.
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
 * @retval #MEDIA_PACKET_ERROR_NONE Successful
 * @retval #MEDIA_PACKET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_PACKET_ERROR_INVALID_OPERATION The user does not create media packet pool instance, or not deallocate all media packets with media_packet_pool_deallocate().
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
