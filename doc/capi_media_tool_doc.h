/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_MEDIA_TOOL_DOC_H__
#define __TIZEN_MEDIA_TOOL_DOC_H__


/**
 * @file capi_media_tool_doc.h
 * @brief This file contains high level documentation of the CAPI MEDIA TOOL API.
 */

/**
 * @defgroup CAPI_MEDIA_TOOL_MODULE Media Tool
 * @brief  The @ref CAPI_MEDIA_TOOL_MODULE  APIs provides functions for AV packet buffer for interworking between mutimedia framework module .
 * @ingroup CAPI_MEDIA_FRAMEWORK
 *
 * @section CAPI_MEDIA_TOOL_MODULE_HEADER Required Header
 *   \#include <media_packet.h>
 *
 * @section CAPI_MEDIA_TOOL_MODULE_OVERVIEW Overview
 *
 * MEDIA TOOL API allows :
 *- Create media_packet handle and Allocate buffer to heap or tbm_surface
 *- Set and Get metadata of media packet
 *
 * The media packet handle is created by caller.
 * After creating media_packet_h handle, the caller can set & get other meta datas.
 *
 */

/**
 * @defgroup CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE Media Format
 * @brief The @ref CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE APIs provides functions for specify video or audio information.
 * @ingroup CAPI_MEDIA_TOOL_MODULE
 *
 * @section CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE_HEADER Required Header
 *   \#include <media_format.h>
 */

#endif // __TIZEN_MEDIA_TOOL_DOC_H__
