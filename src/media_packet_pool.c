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
#include <media_format.h>
#include <media_packet.h>
#include <media_packet_pool.h>
#include <media_packet_pool_private.h>

static gboolean _is_packet_in_queue(media_packet_pool_s *pool_handle, media_packet_h packet);
static int _packet_finalize_cb(media_packet_h packet, int error_code, void *user_data);

int media_packet_pool_create(media_packet_pool_h *pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);

	pool_handle = (media_packet_pool_s *)malloc(sizeof(media_packet_pool_s));
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
	media_format_type_e type;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	MEDIA_PACKET_POOL_NULL_ARG_CHECK(fmt);

	ret = media_format_get_type(fmt, &type);
	if (ret == MEDIA_FORMAT_ERROR_INVALID_OPERATION || type == MEDIA_FORMAT_CONTAINER || type == MEDIA_FORMAT_TEXT) {
		LOGE("Invaild media format");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	pool_handle = (media_packet_pool_s *)pool;

	/* increase format reference count */
	if (media_format_ref(fmt) != MEDIA_FORMAT_ERROR_NONE) {
		LOGE("failed to increase ref count");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}
	pool_handle->fmt_h = fmt;

	return ret;
}

int media_packet_pool_set_size(media_packet_pool_h pool, int min_buffers, int max_buffers)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

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

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	MEDIA_PACKET_POOL_NULL_ARG_CHECK(min_size);
	MEDIA_PACKET_POOL_NULL_ARG_CHECK(max_size);
	MEDIA_PACKET_POOL_NULL_ARG_CHECK(curr_size);

	pool_handle = (media_packet_pool_s *)pool;

	*curr_size = pool_handle->curr_pool_size;
	*min_size = pool_handle->min_pool_size;
	*max_size = pool_handle->max_pool_size;

	return ret;
}

int _packet_finalize_cb(media_packet_h packet, int error_code, void *user_data)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_h pool = NULL;
	media_packet_pool_s *pool_handle = NULL;

	pool = (media_packet_pool_h)user_data;
	pool_handle = (media_packet_pool_s *)pool;

	if (g_atomic_int_get(&pool_handle->pool_allocated)) {
		if (media_packet_pool_release_packet(pool, packet) != MEDIA_PACKET_ERROR_NONE) {
			LOGW("media packet couldn't be released. packet(%p) might be in queue", packet);
		}
		return MEDIA_PACKET_REUSE;

	} else {
		return MEDIA_PACKET_FINALIZE;
	}
}

int media_packet_pool_allocate(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int i, num_pkts = 0;

	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

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

	if (g_atomic_int_get(&pool_handle->pool_allocated)) {
		LOGE("The media packet is already allocated. set media format size...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	for (i = 0; i < pool_handle->min_pool_size; i++) {
		if (media_packet_create_alloc(pool_handle->fmt_h, _packet_finalize_cb, pool_handle, &pool_handle->packet[i]) != MEDIA_PACKET_ERROR_NONE) {
			LOGE("The media packet pool is full or out of memory...");
			return MEDIA_PACKET_ERROR_INVALID_OPERATION;
		}
		g_queue_push_tail(pool_handle->queue, pool_handle->packet[i]);
		LOGD("[%d]%p queued", i, pool_handle->packet[i]);
		pool_handle->curr_pool_size++;
	}
	g_atomic_int_set(&pool_handle->pool_allocated, 1);

	return ret;
}

int media_packet_pool_acquire_packet(media_packet_pool_h pool, media_packet_h *pkt, long timeout)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	media_packet_pool_s *pool_handle = NULL;
	media_packet_h packet = NULL;
	gint64 wait_until = -1;
	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

	if (timeout != -1) {
		gint64 add = timeout;
		wait_until = g_get_monotonic_time() + add;
	}

	if (!g_atomic_int_get(&pool_handle->pool_allocated)) {
		LOGE("The media packet pool is not allocated...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	g_mutex_lock(&pool_handle->mutex);
	if (!(pool_handle->curr_pool_size)) {
		g_mutex_unlock(&pool_handle->mutex);
		LOGE("The media packet pool size is not set. set pool size...");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	while (TRUE) {
		packet = g_queue_pop_head(pool_handle->queue);

		if (packet)
			break;

		if (pool_handle->curr_pool_size < pool_handle->max_pool_size) {
			LOGD("no buffer, trying to allocate");

			if (media_packet_create_alloc(pool_handle->fmt_h, _packet_finalize_cb, pool_handle, &pool_handle->packet[pool_handle->curr_pool_size]) != MEDIA_PACKET_ERROR_NONE) {
				LOGE("The media packet pool is full or out of memory...");
				return MEDIA_PACKET_ERROR_INVALID_OPERATION;
			}
			packet = pool_handle->packet[pool_handle->curr_pool_size];
			pool_handle->curr_pool_size++;

			break;
		} else if (timeout == -1) {
			LOGD("Queue is empty, waiting for till release is done");
			g_cond_wait(&pool_handle->queue_cond, &pool_handle->mutex);
			packet = g_queue_pop_head(pool_handle->queue);
			break;

		} else {
			g_cond_wait_until(&pool_handle->queue_cond, &pool_handle->mutex, wait_until);
			LOGD("Queue is Empty, waiting for timeout %" G_GSIZE_FORMAT "", wait_until);
			packet = g_queue_pop_head(pool_handle->queue);
			if (!packet) {
				LOGE("pkt is null");
				ret = MEDIA_PACKET_ERROR_NO_AVAILABLE_PACKET;
			}
			break;
		}
	}

	*pkt = (media_packet_h)packet;
	g_mutex_unlock(&pool_handle->mutex);
	LOGD("The packet handle aquired from pool is  %p..", packet);
	return ret;
}

gboolean _is_packet_in_queue(media_packet_pool_s *pool_handle, media_packet_h packet)
{
	int i;
	for (i = 0; i < pool_handle->curr_pool_size; i++) {
		if (pool_handle->packet[i] == packet)
			return true;
	}
	return false;
}

int media_packet_pool_release_packet(media_packet_pool_h pool, media_packet_h pkt)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int num_pkts = 0;
	media_packet_pool_s *pool_handle = NULL;
	GList *find;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

	g_mutex_lock(&pool_handle->mutex);

	if ((find = g_queue_find (pool_handle->queue, pkt))) {
		LOGE("unable to release '%p' to the pool. Already released", pkt);
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	num_pkts = g_queue_get_length(pool_handle->queue);

	if (num_pkts == pool_handle->curr_pool_size) {
		LOGE("Queue is already full");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	if (_is_packet_in_queue(pool_handle, pkt)) {
		g_queue_push_tail(pool_handle->queue, pkt);
		g_cond_signal(&pool_handle->queue_cond);
		LOGD("The packet released to pool is  %p..\n", pkt);
	} else {
		LOGE("packet is not aquired from pool %p", pkt);
		g_mutex_unlock(&pool_handle->mutex);
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	/* reset flags of media packet */
	media_packet_unset_flags(pkt, MEDIA_PACKET_CODEC_CONFIG);
	media_packet_unset_flags(pkt, MEDIA_PACKET_END_OF_STREAM);
	media_packet_unset_flags(pkt, MEDIA_PACKET_SYNC_FRAME);

	g_mutex_unlock(&pool_handle->mutex);

	return ret;
}

int media_packet_pool_deallocate(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;
	int i, num_pkts = 0;
	media_packet_pool_s *pool_handle = NULL;
	media_packet_h packet = NULL;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

	if (!(pool_handle->pool_created))
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;

	g_mutex_lock(&pool_handle->mutex);
	num_pkts = g_queue_get_length(pool_handle->queue);

	if (num_pkts < pool_handle->curr_pool_size) {
		LOGE("packet is being used, release the packet before deallocate");
		g_mutex_unlock(&pool_handle->mutex);
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	g_atomic_int_set(&pool_handle->pool_allocated, 0);

	while (!g_queue_is_empty(pool_handle->queue)) {

		packet = g_queue_pop_head(pool_handle->queue);
		if (packet == NULL) {
			LOGE("Failed to get packet handle from Queue ");
			return MEDIA_PACKET_ERROR_INVALID_OPERATION;
		}
		ret = media_packet_destroy(packet);
		LOGI("The packet handle(%p) will be deallocated..", packet);
		if (ret != MEDIA_PACKET_ERROR_NONE) {
			g_mutex_unlock(&pool_handle->mutex);
			return ret;
		}
	}
	g_mutex_unlock(&pool_handle->mutex);

	return ret;
}

int media_packet_pool_destroy(media_packet_pool_h pool)
{
	int ret = MEDIA_PACKET_ERROR_NONE;;
	int num_pkts = 0;

	media_packet_pool_s *pool_handle = NULL;

	MEDIA_PACKET_POOL_INSTANCE_CHECK(pool);
	pool_handle = (media_packet_pool_s *)pool;

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
	if (media_format_unref(pool_handle->fmt_h) != MEDIA_FORMAT_ERROR_NONE) {
		LOGE("failed to decrease ref count");
		return MEDIA_PACKET_ERROR_INVALID_OPERATION;
	}

	g_queue_free(pool_handle->queue);
	LOGI("The packet pool handle(%p) will be destroyed..", pool_handle);
	free(pool_handle);
	pool_handle = NULL;

	return ret;
}

