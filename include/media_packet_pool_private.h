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
#ifndef __TIZEN_MEDIA_PACKET_POOL_PRIVATE_H__
#define __TIZEN_MEDIA_PACKET_POOL_PRIVATE_H__

#include <media_format.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEDIA_PACKET_POOL_CHECK_CONDITION(condition, error, msg)     \
		if (condition) {} else \
		{ LOGE("[%s] %s(0x%08x)", __FUNCTION__, msg, error); return error; }; \

#define  MEDIA_PACKET_POOL_INSTANCE_CHECK(media_packet_pool)   \
		MEDIA_PACKET_POOL_CHECK_CONDITION(media_packet_pool != NULL,  MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER, " MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER")

#define  MEDIA_PACKET_POOL_NULL_ARG_CHECK(arg)      \
		MEDIA_PACKET_POOL_CHECK_CONDITION(arg != NULL,  MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER, " MEDIA_PACKET_POOL_ERROR_INVALID_PARAMETER")

typedef guint64 ClockTime;

#define CLOCK_TIME_NONE             ((ClockTime) -1)
#define SECOND  (G_USEC_PER_SEC * G_GINT64_CONSTANT(1000))

#define MEDIA_PACKET_POOL_AQUIRE_TIMEOUT CLOCK_TIME_NONE
/**
 * @brief  The Media Packet Pool structure.
 * @since_tizen 3.0
 */
typedef struct _media_packet_pool_s {
	GMutex mutex;
	GQueue *queue;
	GCond queue_cond;
	unsigned int curr_pool_size;
	unsigned int min_pool_size;
	unsigned int max_pool_size;
	bool pool_created;
	bool pool_allocated;
	media_format_h fmt_h;
} media_packet_pool_s;

#ifdef __cplusplus
}
#endif
#endif							/* __TIZEN_MEDIA_PACKET_POOL_PRIVATE_H__ */
