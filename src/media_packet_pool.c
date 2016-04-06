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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlog.h>
#include <media_packet_pool.h>
#include <media_format.h>
#include <media_packet.h>
#include <media_packet_private.h>
#include <media_packet_pool_private.h>
#include <media_format_private.h>

int media_packet_pool_create(media_packet_pool_h * pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool)

	pool_handle = (media_packet_pool_s *) malloc(sizeof(media_packet_pool_s));
	if (pool_handle != NULL)
		memset(pool_handle, 0, sizeof(media_packet_pool_s));
	else {
		LOGE("out of memory");
		ret = MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
		return ret;
	}
	pool_handle->pool_created = TRUE;

	/* create Gqueue */
	pool_handle->queue = g_queue_new();
	pool_handle->curr_pool_size = 0;
	pool_handle->min_pool_size = 0;
	pool_handle->max_pool_size = 0;
	pool_handle->pool_allocated = false;
	/* take memory packet pool handle */
	*pool = (media_packet_pool_h) pool_handle;

	LOGI("packet pool handle : %p", *pool);
	return ret;

}

int media_packet_pool_set_media_format(media_packet_pool_h pool, media_format_h fmt)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	MEDIA_PACKET_NULL_ARG_CHECK(fmt);

	if (MEDIA_FORMAT_GET_REFCOUNT(fmt) < 1) {
		LOGE("The media format handle is corrupted or Not set media info");
		return MEDIA_PACKET_ERROR_INVALID_PARAMETER;
	}

	if (!MEDIA_FORMAT_IS_VIDEO(fmt) && !MEDIA_FORMAT_IS_AUDIO(fmt)) {
		LOGE("The media format handle is not specified. set video info or audio info...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	pool_handle = (media_packet_pool_s *) pool;

	/* increase format reference count */
	media_format_ref(fmt);

	pool_handle->fmt_h = MEDIA_FORMAT_CAST(fmt);
	return ret;
}

int media_packet_pool_set_size(media_packet_pool_h pool, int min_buffers, int max_buffers)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;

	if (max_buffers < 0 || min_buffers < 0 || min_buffers > max_buffers)
		return MEDIA_PACKET_ERROR_INVALID_PARAMETER;

	pool_handle->min_pool_size = min_buffers;
	pool_handle->max_pool_size = max_buffers;

	return ret;
}

int media_packet_pool_get_size(media_packet_pool_h pool, int *min_size, int *max_size, int *curr_size)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;

	*curr_size = pool_handle->curr_pool_size;
	*min_size = pool_handle->min_pool_size;
	*max_size = pool_handle->max_pool_size;
	return ret;

}

int _allocate_pkt(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int i, num_pkts = 0;

	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;
	media_format_s *fmt_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;
	fmt_handle = (media_format_s *) pool_handle->fmt_h;
	MEDIA_PACKET_NULL_ARG_CHECK(fmt_handle);

	if (pool_handle->curr_pool_size >= pool_handle->max_pool_size) {
		LOGE("The media packet pool is full..");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}

	pkt_handle = (media_packet_s *) malloc(sizeof(media_packet_s));

	if (pkt_handle != NULL)
		memset(pkt_handle, 0, sizeof(media_packet_s));
	else {
		LOGE("out of memory");
		ret = MEDIA_PACKET_ERROR_OUT_OF_MEMORY;
		goto fail;
	}

	if (MEDIA_FORMAT_IS_VIDEO(fmt_handle) && MEDIA_FORMAT_IS_RAW(fmt_handle))
		pkt_handle->type = MEDIA_BUFFER_TYPE_TBM_SURFACE;
	else
		pkt_handle->type = MEDIA_BUFFER_TYPE_NORMAL;

	LOGI("The created packet handle(%p) is..", pkt_handle);

	/* take fmt */
	pkt_handle->format = MEDIA_FORMAT_CAST(fmt_handle);

	/* alloc buffer */
	int err = _pkt_alloc_buffer(pkt_handle);
	if (err != MEDIA_PACKET_ERROR_NONE) {
		LOGE("failed _pkt_alloc_buffer(), err = (0x%08x)", err);
		ret = err;
		goto fail;
	}

	/* allocated buffer */
	pkt_handle->is_allocated = true;

	/* set finalized callback and user data */
	pkt_handle->finalizecb_func = NULL;
	pkt_handle->userdata = NULL;

	/* increase format reference count */
	media_format_ref((media_format_h) pkt_handle->format);

	pkt_handle->using_pool = true;
	/* push the packet handle into queue */
	g_queue_push_tail(pool_handle->queue, pkt_handle);
	pool_handle->curr_pool_size++;

	return ret;

 fail:
	if (pkt_handle) {
		free(pkt_handle);
		pkt_handle = NULL;
	}

	return ret;
}

int media_packet_pool_allocate(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int i, num_pkts = 0;

	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;
	media_format_s *fmt_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;
	fmt_handle = (media_format_s *) pool_handle->fmt_h;
	MEDIA_PACKET_NULL_ARG_CHECK(fmt_handle);

	if (!(pool_handle->pool_created)) {
		LOGE("The media packet pool is not created..");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}

	if (!(pool_handle->min_pool_size)) {
		LOGE("The media packet pool size is not set. set pool size...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}
	if (pool_handle->curr_pool_size >= pool_handle->max_pool_size) {
		LOGE("The media packet pool is full..");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}

	if ((pool_handle->pool_allocated)) {
		LOGE("The media packet is already allocated. set media format size...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}
	for (i = pool_handle->curr_pool_size; i < pool_handle->min_pool_size; i++) {
		if (_allocate_pkt(pool_handle) != MEDIA_PACKET_ERROR_NONE) {
			LOGE("The media packet pool is full or out of memory...");
			return MEDIA_PACKET_ERROR_INVALID_OPERATION;
		}

	}
	pool_handle->pool_allocated = true;
	return ret;

 fail:
	if (pkt_handle) {
		free(pkt_handle);
		pkt_handle = NULL;
	}

	while (!g_queue_is_empty(pool_handle->queue)) {

		pkt_handle = g_queue_pop_head(pool_handle->queue);
		if (pkt_handle) {
			free(pkt_handle);
			pkt_handle = NULL;
		}
	}
	return ret;
}

int media_packet_pool_acquire_packet(media_packet_pool_h pool, media_packet_h * pkt, long timeout)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;
	gint64 wait_until = -1;
	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;

	if (timeout != -1) {
		gint64 add = timeout;
		wait_until = g_get_monotonic_time() + add;
	}

	if (!(pool_handle->pool_allocated)) {
		LOGE("The media packet pool is not allocated...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}

	if (!(pool_handle->curr_pool_size)) {
		LOGE("The media packet pool size is not set. set pool size...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	g_mutex_lock(&pool_handle->mutex);
	while (TRUE) {
		pkt_handle = g_queue_pop_head(pool_handle->queue);

		if (pkt_handle)
			break;

		if (pool_handle->curr_pool_size < pool_handle->max_pool_size) {
			LOGI("no buffer, trying to allocate");

			if (_allocate_pkt(pool_handle) != MEDIA_PACKET_ERROR_NONE) {
				LOGE("failed in _allocate_pkt ");
				g_mutex_unlock(&pool_handle->mutex);
				return MEDIA_PACKET_ERROR_INVALID_OPERATION;
			}
			pkt_handle = g_queue_pop_head(pool_handle->queue);
			break;
		} else if (timeout == -1) {
			LOGE("Queue is empty, waiting for till release is done");
			g_cond_wait(&pool_handle->queue_cond, &pool_handle->mutex);
			pkt_handle = g_queue_pop_head(pool_handle->queue);
			break;

		} else {
			g_cond_wait_until(&pool_handle->queue_cond, &pool_handle->mutex, wait_until);
			LOGI("Queue is Empty, waiting for timeout %" G_GSIZE_FORMAT "", wait_until);
			pkt_handle = g_queue_pop_head(pool_handle->queue);
			if (!pkt_handle) {
				LOGE("pkt is null");
				ret = MEDIA_PACKET_ERROR_NO_AVAILABLE_PACKET;
			}
			break;
		}
	}

	*pkt = (media_packet_h) pkt_handle;
	g_mutex_unlock(&pool_handle->mutex);
	LOGI("The packet handle aquired from pool is  %p..", pkt_handle);
	return ret;

}


int media_packet_pool_release_packet(media_packet_pool_h pool, media_packet_h pkt)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int num_pkts = 0;
	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;
	pkt_handle = (media_packet_s*) pkt;

	ret = _pkt_reset_buffer(pkt);
	if (ret != MEDIA_PACKET_ERROR_NONE) {
		LOGE("[%s] failed _pkt_alloc_buffer(), err = (0x%08x)", __FUNCTION__, ret);
		return ret;
	}

	g_mutex_lock(&pool_handle->mutex);
	num_pkts = g_queue_get_length(pool_handle->queue);
	g_mutex_unlock(&pool_handle->mutex);

	if (num_pkts == pool_handle->curr_pool_size) {
		LOGE("Queue is already full");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	if (pkt_handle->using_pool) {
		LOGI("The packet released to pool is  %p..\n", pkt_handle);
		g_mutex_lock(&pool_handle->mutex);
		g_queue_push_tail(pool_handle->queue, pkt_handle);
		g_mutex_unlock(&pool_handle->mutex);
		g_cond_signal(&pool_handle->queue_cond);
	} else {
		LOGE("packet is not aquired from pool");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	return ret;
}

int media_packet_pool_deallocate(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int i, num_pkts = 0;
	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;

	if (!(pool_handle->pool_created))
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	g_mutex_lock(&pool_handle->mutex);
	num_pkts = g_queue_get_length(pool_handle->queue);
	g_mutex_unlock(&pool_handle->mutex);

	if (num_pkts < pool_handle->curr_pool_size) {
		LOGE("packet is being used, release the packet before deallocate");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	g_mutex_lock(&pool_handle->mutex);
	while (!g_queue_is_empty(pool_handle->queue)) {

		pkt_handle = g_queue_pop_head(pool_handle->queue);
		g_mutex_unlock(&pool_handle->mutex);
		if (pkt_handle == NULL) {
			LOGE("Failed to get packet handle from Queue ");
			return MEDIA_PACKET_ERROR_INVALID_OPERATION;
		}
		pkt_handle = (media_packet_h) pkt_handle;
		pkt_handle->using_pool = false;
		LOGI("The packet handle(%p) will be deallocated..", pkt_handle);
		ret = media_packet_destroy(pkt_handle);
		g_mutex_lock(&pool_handle->mutex);
	}
	g_mutex_unlock(&pool_handle->mutex);
	return ret;
}

int media_packet_pool_destroy(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;;
	int num_pkts = 0;

	media_packet_pool_s *pool_handle = NULL;
	media_packet_s *pkt_handle = NULL;

	MEDIA_PACKET_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *) pool;

	if (!(pool_handle->pool_created))
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	g_mutex_lock(&pool_handle->mutex);
	num_pkts = g_queue_get_length(pool_handle->queue);
	g_mutex_unlock(&pool_handle->mutex);

	if (num_pkts > 0) {
		LOGE("The packet pool needs to deallocate first ");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	}
	/* unreference media_format */
	media_format_unref(pool_handle->fmt_h);
	g_queue_free(pool_handle->queue);
	LOGI("The packet pool handle(%p) will be destroyed..", pool_handle);
	free(pool_handle);
	pool_handle = NULL;

	return ret;

}

