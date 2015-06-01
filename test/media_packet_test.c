/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <glib.h>

#include <media_packet_private.h>
#include <media_format.h>
#include <media_format_private.h>
#include <media_packet_internal.h>

#include <appcore-efl.h>

#define PACKAGE "media_tool_test"
#define MAX_STRING_LEN	2048
#define MAX_HANDLE 10
#define MEDIA_FORMAT_MAX_HANDLE 3


enum
{
    CURRENT_STATUS_MAINMENU,
    CURRENT_STATUS_PACKET_SET_FORMAT,
    CURRENT_STATUS_DURATION,
    CURRENT_STATUS_FORMAT_SET_VIDEO_MIME,
    CURRENT_STATUS_FORMAT_SET_VIDEO_WIDTH,
    CURRENT_STATUS_FORMAT_SET_VIDEO_HEIGHT,
    CURRENT_STATUS_FORMAT_SET_VIDEO_AVG_BPS,
    CURRENT_STATUS_FORMAT_SET_VIDEO_MAX_BPS,
    CURRENT_STATUS_FORMAT_SET_AUDIO_MIME,
    CURRENT_STATUS_FORMAT_SET_AUDIO_CHANNEL,
    CURRENT_STATUS_FORMAT_SET_AUDIO_SAMPLERATE,
    CURRENT_STATUS_FORMAT_SET_AUDIO_BIT,
    CURRENT_STATUS_FORMAT_SET_AUDIO_AVG_BPS,
    CURRENT_STATUS_FORMAT_SET_AUDIO_AAC_TYPE,
};


char g_uri[MAX_STRING_LEN];
int g_menu_state = CURRENT_STATUS_MAINMENU;
int g_handle_num = 1;
static media_packet_h g_media_packet[MAX_HANDLE] = {0};
 media_format_h g_media_format[MEDIA_FORMAT_MAX_HANDLE] = {0};
int media_format_idx = -1;
bool is_only_created_handle;
bool is_excute_create;

unsigned char codec_data[0x04] = {0x00, 0x01, 0x02, 0x03};
unsigned int codec_data_size = 4;
unsigned char* codec_data_ptr = codec_data;


/***********************************************/
/***  Test API part
/***********************************************/
static void _media_format_create(void)
{
    int ret;

    if (g_media_format[0] == NULL)
    {
        int ret = media_format_create(&g_media_format[0]);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
       {
            g_print("media_format_h[0] is created successfully! \n");
            media_format_idx = 0;
            g_print("ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
       }
        else
        {
            g_print("media_format_create failed...\n");
            g_print("==> ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
        }
    }
    else
    {
        ret = MEDIA_FORMAT_ERROR_INVALID_OPERATION;
        g_print("\n can not create media_format_h handle anymore...already exist.\n");
    }


}

static void _media_format_set_video_mime(int num)
{
    int ret;
    if (num == 0)
    {
        ret = media_format_set_video_mime(g_media_format[media_format_idx], MEDIA_FORMAT_H264_HP);
    }
    else if (num == 1)
    {
        ret = media_format_set_video_mime(g_media_format[media_format_idx], MEDIA_FORMAT_RGB888);
    }
    else if (num == 2)
    {
        ret = media_format_set_video_mime(g_media_format[media_format_idx], MEDIA_FORMAT_I420);
    }
    else
    {
        g_print("Invalid number...\n");
    }

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_video_mime is succeeded!!\n");
    else
        g_print("media_format_set_video_mime is failed...!!\n");

}

static void _media_format_set_video_width(int w)
{
    int ret;
    ret = media_format_set_video_width(g_media_format[media_format_idx], w);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_video_width is succeeded!!\n");
    else
        g_print("media_format_set_video_width is failed...!!\n");
}

static void _media_format_set_video_height(int h)
{
    int ret;
    ret = media_format_set_video_height(g_media_format[media_format_idx], h);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_video_height is succeeded!!\n");
    else
        g_print("media_format_set_video_height is failed...!!\n");


}

static void _media_format_set_video_avg_bps(int avg_bps)
{
    int ret;
    ret = media_format_set_video_avg_bps(g_media_format[media_format_idx], avg_bps);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_video_avg_bps is succeeded!!\n");
    else
        g_print("media_format_set_video_avg_bps is failed...!!\n");
}

static void _media_format_set_video_max_bps(int max_bps)
{
    int ret;
    ret = media_format_set_video_max_bps(g_media_format[media_format_idx], max_bps);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_video_max_bps is succeeded!!\n");
    else
        g_print("media_format_set_video_max_bps is failed...!!\n");
}

static void _media_format_set_audio_mime(int num)
{
    int ret;
    if (num == 0)
    {
        ret = media_format_set_audio_mime(g_media_format[media_format_idx], MEDIA_FORMAT_AMR);
    }
    else if (num == 1)
    {
        ret = media_format_set_audio_mime(g_media_format[media_format_idx], MEDIA_FORMAT_PCM);
    }
    else if (num == 2)
    {
        ret = media_format_set_audio_mime(g_media_format[media_format_idx], MEDIA_FORMAT_AAC);
    }
    else
    {
        g_print("Invalid number...\n");
    }

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_mime is succeeded!!\n");
    else
        g_print("media_format_set_audio_mime is failed...!!\n");

}

static void _media_format_set_audio_channel(int channel)
{
    int ret;
    ret = media_format_set_audio_channel(g_media_format[media_format_idx], channel);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_channel is succeeded!!\n");
    else
        g_print("media_format_set_audio_channel is failed...!!\n");
}

static void _media_format_set_audio_samplerate(int samplerate)
{
    int ret;
    ret = media_format_set_audio_samplerate(g_media_format[media_format_idx], samplerate);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_samplerate is succeeded!!\n");
    else
        g_print("media_format_set_audio_samplerate is failed...!!\n");
}

static void _media_format_set_audio_bit(int bit)
{
    int ret;
    ret = media_format_set_audio_bit(g_media_format[media_format_idx], bit);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_bit is succeeded!!\n");
    else
        g_print("media_format_set_audio_bit is failed...!!\n");
}

static void _media_format_set_audio_avg_bps(int avg_bps)
{
    int ret;
    ret = media_format_set_audio_avg_bps(g_media_format[media_format_idx], avg_bps);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_avg_bps is succeeded!!\n");
    else
        g_print("media_format_set_audio_avg_bps is failed...!!\n");
}

static void _media_format_set_audio_aac_type(bool is_adts)
{
    int ret;
    ret = media_format_set_audio_aac_type(g_media_format[media_format_idx], is_adts);

    if (ret == MEDIA_FORMAT_ERROR_NONE)
        g_print("media_format_set_audio_aac_type is succeeded!!\n");
    else
        g_print("media_format_set_audio_aac_type is failed...!!\n");
}

static void _create_format_320_240_es(void)
{

    if (media_format_create(&g_media_format[1]) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_create is succeeded! \n");
        g_print("the media_format_h[1] is created..\n");
        int ret = MEDIA_FORMAT_ERROR_NONE;
        media_format_idx = 1;

        media_format_mimetype_e mime = MEDIA_FORMAT_H264_HP;
        int w = 320;
        int h = 240;
        int avg_bps = 3000000;
        int max_bps = 15000000;
        ret = media_format_set_video_mime(g_media_format[1], MEDIA_FORMAT_H264_HP);
        ret = media_format_set_video_width(g_media_format[1], 320);
        ret = media_format_set_video_height(g_media_format[1], 240);
        ret = media_format_set_video_avg_bps(g_media_format[1], 3000000);
        ret = media_format_set_video_max_bps(g_media_format[1], 15000000);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
        {
            g_print("media_format_set_video_xxx succeed! w:%d, h:%d, 0x%x, avg_bps: %d, max_bps: %d\n", w, h, mime, avg_bps, max_bps);
            g_print("\t\t\t\t ====>> ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[1]));
        }
        else
            g_print("media_format_set_video_xxx failed..\n");
    }
    else
    {
        g_print("media_format_create failed..\n");
    }

}


static void _create_format_raw(void)
{

    if (media_format_create(&g_media_format[2]) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_create is succeeded! \n");
        int ret = MEDIA_FORMAT_ERROR_NONE;

        media_format_mimetype_e mime = MEDIA_FORMAT_I420;
        int w = 128;
        int h = 128;
        int avg_bps = 2000000;
        int max_bps = 15000000;
        ret = media_format_set_video_mime(g_media_format[2], mime);
        ret = media_format_set_video_width(g_media_format[2], w);
        ret = media_format_set_video_height(g_media_format[2], h);
        ret = media_format_set_video_avg_bps(g_media_format[2], avg_bps);
        ret = media_format_set_video_max_bps(g_media_format[2], max_bps);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
        {
            g_print("media_format_set_video_xxx success! w:%d, h:%d, 0x%x, avg_bps: %d, max_bps: %d\n", w, h, mime, avg_bps, max_bps);
            g_print("\t\t\t\t ====>> ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[2]));
        }
        else
            g_print("media_format_set_video_xxx failed..\n");
    }
    else
    {
        g_print("media_format_create failed..\n");
    }


}

static int _finalize_callback(media_packet_h packet, int err, void* userdata)
{
    g_print("==> finalize callback func is called\n");
    return MEDIA_PACKET_FINALIZE;
}

static void _media_packet_create_alloc(void)
{
    g_print("=== create_and_alloc!!\n");
    int i;

    for(i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i] != NULL)
        {
            int ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]);
            media_packet_destroy(g_media_packet[i]);
            if (ref_count == 1)
            {
                g_media_format[media_format_idx] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", media_format_idx);
            }
            g_media_packet[i] = NULL;
        }

        if(g_media_format[0] == NULL)
        {
            g_print("media_format_h is NULL!! create media_format_h handle \n");
            break;
        }

        if (media_packet_create_alloc(g_media_format[0], _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
        {
            g_print("media_packet create is failed\n");
        }
        else
        {
            media_format_idx = 0;
            g_print("succeeded media_packet_create_alloc !!\n");
            g_print(" ==> media_format_h[0] ref_count =%d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
            media_format_unref(g_media_format[0]);
            g_print(" ====> media_format_unref, media_format_h[0] ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
            is_excute_create = true;
        }

    }
}

static void _media_packet_create(void)
{
    g_print("=== create!!\n");
    is_only_created_handle = true;
    int i;

    for(i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i] != NULL)
        {
            int ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]);
            media_packet_destroy(g_media_packet[i]);
            if (ref_count == 1)
            {
                g_media_format[media_format_idx] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", media_format_idx);
            }
            g_media_packet[i] = NULL;
        }

        if(g_media_format[0] == NULL)
        {
            g_print("media_format_h is NULL!! create media_format_h handle \n");
            break;
        }

        /* only ES format , if you want to another format, see _create_format_es() */
        if (media_packet_create(g_media_format[0], _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
        {
            g_print("media_packet create is failed\n");
        }
        else
        {
            media_format_idx = 0;
            g_print("media_packet_create is sucess!!\n");
            g_print(" ==> media_format_h[0] ref_count =%d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
            media_format_unref(g_media_format[0]);
            g_print(" ====> media_format_unref, media_format_h[0] ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
            is_excute_create = true;
        }
    }
}

static void _media_packet_alloc(void)
{
    g_print("=== alloc!!\n");
    if (is_only_created_handle)
    {
        if (media_packet_alloc(g_media_packet[0]) == MEDIA_PACKET_ERROR_NONE)
            g_print("media_packet_alloc is sucess\n");
        else
            g_print("media_packet_alloc is failed\n");
    }
    else
    {
        g_print("*** can not excute!! fisrt excute media_packet_create() before this alloc. \n");
    }
}

static void _media_packet_create_from_tbm_surface(void)
{
    g_print("=== create_from_tbm_surface!!\n");

    tbm_surface_h surface;
    surface = tbm_surface_create(128, 128, TBM_FORMAT_YUV420);

    if (surface)
    {
        int i;

        for(i = 0; i < g_handle_num; i++)
        {
            if(g_media_packet[i] != NULL)
            {
                int ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]);
                media_packet_destroy(g_media_packet[i]);

                if (ref_count == 1 )
                {
                    g_media_format[media_format_idx] = NULL;
                    g_print("media_format_h[%d] is destroyed...\n", media_format_idx);
                }

                g_media_packet[i] = NULL;
            }

            if(g_media_format[2] == NULL)
            {
                g_print("create raw video type media_format_h[2]... \n");
                _create_format_raw();
            }

            /* only RAW format , if you want to another format, see _create_format_raw() */
            if (media_packet_create_from_tbm_surface(g_media_format[2], surface, _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
            {
                g_print("media_packet_create_from_tbm_surface is failed\n");
            }
            else
            {
                g_print("media_packet_create_from_tbm_surface is sucess!!\n");
                g_print(" ==> media_format_h[2] ref_count =%d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[2]));
                media_format_unref(g_media_format[2]);
                g_print(" ====> media_format_unref, media_format_h[2] ref_count = %d\n", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[2]));
                media_format_idx = 2;
                is_excute_create = true;
            }
        }

    }
    else
    {
        g_print("tbm_surface_create failed...\n");
    }
}

static void _media_packet_copy(void)
{
    g_print("=== copy!!\n");

    if (is_excute_create)
    {
        if (media_packet_copy(g_media_packet[0], _finalize_callback, NULL, &g_media_packet[1]) == MEDIA_PACKET_ERROR_NONE)
        {
            g_print("media_packet_copy sucess !!");
            is_only_created_handle = true;
            memset (g_media_packet[0], 0, sizeof(media_packet_s));
            memcpy (g_media_packet[0], g_media_packet[1], sizeof(media_packet_s));
            if(g_media_packet[1])
            {
                free(g_media_packet[1]);
                g_media_packet[1] = NULL;
            }

        }
        else
        {
            g_print("media_packet_copy failed");
        }
    }
    else
    {
        g_print("denied... excute create or create_alloc or create_from_surface");
    }

}

static void _media_packet_destroy()
{
    g_print("=== destroy!!\n");
    int i;
    int ret;

    int ref_count;

    for (i = 0; i < g_handle_num ; i++)
    {
        if(g_media_packet[i]!=NULL)
        {
            ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]);
            ret = media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = NULL;

            if (ref_count == 1)
            {
                g_media_format[media_format_idx] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", media_format_idx);
            }

            if (ret == MEDIA_PACKET_ERROR_NONE)
            {
                g_print("media_packet_destroy is succeeded!!\n");
                if (g_media_format[media_format_idx] != NULL)
                    g_print(" ==> media_format_h[%d] ref_count = %d\n", media_format_idx, MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]));
            }
            else
                g_print("media_packet_destroy is failed...\n");
        }
        else
            g_print("There is nothing to destroy media_packet_h[%d] handle...\n", i);
    }
}

static void _media_packet_get_buffer_size(void)
{
    uint64_t size;
    if (media_packet_get_buffer_size(g_media_packet[0],&size) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_buffer_size is sucess!!");
        g_print("\t\t[media_packet]===> size = %llu\n", size);
    }
    else
    {
        g_print("media_packet_get_buffer_size is failed...");
    }
}

static void _media_packet_get_duration(void)
{
    uint64_t duration;

    if (media_packet_get_duration(g_media_packet[0], &duration) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_duration is sucess!!");
        g_print("\t\t[media_packet]===> duration = %llu\n", duration);
    }
    else
    {
        g_print("media_packet_get_duration is failed...");
    }
}

static void _media_packet_get_buffer_data_ptr(void)
{
    void* ptr;
    media_packet_s* packet = g_media_packet[0];
    ptr = (void*)malloc(sizeof(packet->size));

    if (ptr)
        memset(packet, 0, sizeof(packet->size));
    else
        g_print("_get_buffer_ptr , malloc failed...");

    if (media_packet_get_buffer_data_ptr(g_media_packet[0], &ptr) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_buffer_data_ptr is sucess!!");
        g_print("\t\t[media_packet]===> buffer_data_ptr = %p\n", ptr);
    }
    else
    {
        g_print("media_packet_get_buffer_data_ptr is failed...");
    }

}

static void _media_packet_get_tbm_surface()
{
    tbm_surface_h tbm_surface;

    if (media_packet_get_tbm_surface(g_media_packet[0], &tbm_surface) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_tbm_surface is sucess!!");
        g_print("\t\t[media_packet]===> tbm_surface = %p\n", (void*)tbm_surface);
    }
    else
    {
        g_print("media_packet_get_duration is failed...");
    }
}

static void _media_packet_get_number_of_video_planes()
{
    uint32_t num = 0;
    if (media_packet_get_number_of_video_planes(g_media_packet[0], &num)== MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_number_of_video_planes is sucess!!\n");
        g_print("\t\t[media_packet]===> number of planes = %ld\n", num);

    }
    else
    {
        g_print("media_packet_get_number_of_video_planes is failed...");
    }
}

static void _media_packet_get_video_stride_width()
{
    int stride_w;
    if (media_packet_get_video_stride_width(g_media_packet[0], 0, &stride_w)== MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_video_stride_width is sucess!!\n");
        g_print("\t\t[media_packet]===> stride width = %d\n", stride_w);

    }
    else
    {
        g_print("media_packet_get_video_stride_width is failed...");
    }
}

static void _media_packet_get_video_stride_height()
{
    int stride_h;
    if (media_packet_get_video_stride_height(g_media_packet[0], 0, &stride_h)== MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_video_stride_height is sucess!!\n");
        g_print("\t\t[media_packet]===> stride height = %d\n", stride_h);

    }
    else
    {
        g_print("media_packet_get_video_stride_height is failed...");
    }
}

static void _media_packet_get_video_plane_data_ptr()
{
    void* ptr;
    if (media_packet_get_video_plane_data_ptr(g_media_packet[0], 0, &ptr)== MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_video_plane_data_ptr is sucess!!\n");
        g_print("\t\t[media_packet]===> ptr = %p\n", ptr);

    }
    else
    {
        g_print("_media_packet_get_plane_data_ptr is failed...");
    }
}

static void _media_packet_set_codec_data()
{
    if (media_packet_set_codec_data(g_media_packet[0], (void*)codec_data_ptr, codec_data_size) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_set_codec_data is sucess..!\n");
    }
    else
    {
        g_print("media_packet_set_codec_data is failed...\n");
    }
}

static void _media_packet_get_codec_data()
{
    unsigned char* get_codec_data;
    unsigned int get_codec_data_size;

    if (media_packet_get_codec_data(g_media_packet[0], &get_codec_data, &get_codec_data_size) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_codec_data is sucess ... !\n");
        g_print("codec_data_size = %u\n", get_codec_data_size);

        if (get_codec_data_size == 0)
            return;

        int i;
        for (i=0 ; i < get_codec_data_size; i++)
        {
            g_print("codec_data[%d] ", i);
            g_print(" = 0x%x\n", get_codec_data[i]);
        }
    }
    else
    {
        g_print("media_packet_get_codec_data is failed...\n");
    }
}

static void _media_packet_set_duration(uint64_t duration)
{
    if (media_packet_set_duration(g_media_packet[0], duration) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_set_duration is sucess!!\n");
    }
    else
    {
        g_print("media_packet_set_duration is failed..\n");
    }
}

static void _media_packet_is_video(void)
{
    bool is_video;
    if (media_packet_is_video(g_media_packet[0], &is_video) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_is_video is sucess!!\n");
        g_print("\t\t[media_packet]===> is_video = %d", is_video);
    }
    else
    {
        g_print("media_packet_is_video is failed...");
    }
}

static void _media_packet_is_audio(void)
{
    bool is_audio;
    if (media_packet_is_audio(g_media_packet[0], &is_audio) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_is_audio is sucess!!\n");
        g_print("\t\t[media_packet]===> is_audio = %d", is_audio);
    }
    else
    {
        g_print("media_packet_is_audio is failed...");
    }
}

static void _media_packet_is_encoded(void)
{
    bool is_encoded;
    if (media_packet_is_encoded(g_media_packet[0], &is_encoded) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_is_encoded is sucess!!\n");
        g_print("\t\t[media_packet]===> is_encoded = %d", is_encoded);
    }
    else
    {
        g_print("media_packet_is_encoded is failed...");
    }

}
static void _media_packet_is_raw(void)
{
    bool is_raw;
    if (media_packet_is_raw(g_media_packet[0], &is_raw) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_is_raw is sucess!!\n");
        g_print("\t\t[media_packet]===> is_raw = %d", is_raw);
    }
    else
    {
        g_print("media_packet_is_raw is failed...");
    }

}

static void _media_packet_get_format(void)
{
    media_format_h fmt;
    if (media_packet_get_format(g_media_packet[0], &fmt) == MEDIA_PACKET_ERROR_NONE)
    {
        int w;
        int h;

        int channel;
        int samplerate;
        int bit;
        int audio_avg_bps;

        g_print("media_packet_get_format is sucess! %p \n", fmt);

        if (MEDIA_FORMAT_IS_VIDEO(fmt))
        {
            if (media_format_get_video_info(fmt, NULL, &w, &h, NULL, NULL) == MEDIA_PACKET_ERROR_NONE)
            {
                g_print("\t\t [media_format] width = %d, height =%d", w, h);
            }
            else
            {
                g_print("media_format_get_video is failed...");
            }
        }
        else if (MEDIA_FORMAT_IS_AUDIO(fmt))
        {
             if (media_format_get_audio_info(fmt, NULL, &channel, &samplerate, &bit, &audio_avg_bps) == MEDIA_PACKET_ERROR_NONE)
            {
                g_print("\t\t [media_format] channel = %d, samplerate = %d, bit = %d, avg_bps = %d", channel, samplerate, bit, audio_avg_bps);
            }
            else
            {
                g_print("media_format_get_video is failed...");
            }
        }

    }
    else
    {
        g_print("media_packet_get_format is failed...");
    }

}


static void _media_packet_set_format(media_format_h fmt)
{
    if (media_packet_set_format(g_media_packet[0], fmt) == MEDIA_PACKET_ERROR_NONE)
    {
        media_format_unref(fmt);
        g_print("media_packet_set_format is sucess!\n");
    }
    else
    {
        g_print("media_packet_get_format is failed...");
    }

}

static void _media_packet_set_extra(void)
{
    char* extra = "extra";

    if (media_packet_set_extra(g_media_packet[0], (void*)extra) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_set_extra is sucess!\n");
    }
    else
    {
        g_print("media_packet_set_extra is failed...");
    }

}

static void _media_packet_get_extra(void)
{
    void* extra;

    if (media_packet_get_extra(g_media_packet[0], &extra) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_extra is sucess!\n");
        g_print("\t\t extra = %s\n", (char*)extra);
    }
    else
    {
        g_print("media_packet_get_extra is failed...");
    }

}

static void _media_packet_has_tbm_surface_buffer(void)
{
    bool has_tbm;

    if (media_packet_has_tbm_surface_buffer(g_media_packet[0], &has_tbm) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_has_tbm_surface_buffer is sucess! \n");
        g_print("\t\t has_tbm_surface_buffer = %d \n", has_tbm);
    }
    else
    {
        g_print("media_packet_has_tbm_surface_buffer is failed! \n");
    }
}

static void _media_format_get_video_info(void)
{
    media_format_mimetype_e mime;
    int w = 0;
    int h = 0;
    int avg_bps = 0;
    int max_bps = 0;

    if (media_format_get_video_info(g_media_format[0], &mime, &w, &h, &avg_bps, &max_bps) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_get_video is sucess!\n");
        g_print("\t\t[media_format_get_video]mime:0x%x, width :%d, height :%d\n", mime, w, h);
        g_print("packet format ref_count: %d", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
    }
    else
    {
        g_print("media_format_get_video is failed..");
    }
}

static void _media_format_get_audio_info(void)
{
    media_format_mimetype_e mime;
    int channel = 0;
    int samplerate = 0;
    int bit = 0;
    int avg_bps = 0;

    if (media_format_get_audio_info(g_media_format[0], &mime, &channel, &samplerate, &bit, &avg_bps)== MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_get_audio_info is sucess!\n");
        g_print("\t\t[media_format_get_audio_info]mime:0x%x, channel :%d, samplerate :%d, bit: %d, avg_bps:%d, is_adts:%d \n", mime, channel, samplerate, bit, avg_bps);
        g_print("packet format ref_count: %d", MEDIA_FORMAT_GET_REFCOUNT(g_media_format[0]));
    }
    else
    {
        g_print("media_format_get_audio_info is failed..");
    }
}


/***********************************************/
/***  Test suit frame API part
/***********************************************/
static int _create_app(void *data)
{
    printf("media_packet_test is started!\n");
    return 0;
}

static int _terminate_app(void *data)
{
    printf("media_packet_test is terminated!\n");
    return 0;
}


struct appcore_ops ops = {
    .create = _create_app,
    .terminate = _terminate_app,
};

void reset_menu_state()
{
    g_menu_state = CURRENT_STATUS_MAINMENU;
}

void quit_program(void)
{
    int i = 0;
    int ref_count;

    for (i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i] != NULL)
        {
            ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[media_format_idx]);
            media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = NULL;
            if (ref_count == 1)
            {
                g_media_format[media_format_idx] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", media_format_idx);
            }
        }
    }


    for (i = 0; i < MEDIA_FORMAT_MAX_HANDLE; i++)
    {
        if (g_media_format[i] != NULL)
        {
            int ref_count = MEDIA_FORMAT_GET_REFCOUNT(g_media_format[i]);
            g_print("media_format_h[%d] ref_count:%d. excute media_format_unref().\n", i, ref_count);

            if (ref_count == 1)
            {
                media_format_unref(g_media_format[i]);
                g_media_format[i] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", i);
            }
            else
            {

                int j = 1;
                for (j = 1; j <= ref_count; j++)
                {
                    g_print("(%d) media_format_unref\n", j);
                    media_format_unref(g_media_format[i]);

                }
                g_media_format[i] = NULL;
                g_print("media_format_h[%d] is destroyed...\n", i);
            }

        }
    }

    elm_exit();
}

void _interpret_main_menu(char *cmd)
{
    int len =  strlen(cmd);
    if (len == 1)
    {
        if (strncmp(cmd, "a", 1) == 0)
        {
            _media_packet_create_alloc();
        }
        else if (strncmp(cmd, "d", 1) == 0)
        {
            _media_packet_destroy();
        }
        else if (strncmp(cmd, "b", 1) == 0)
        {
            _media_packet_create_from_tbm_surface();
        }
        else if (strncmp(cmd, "c", 1) == 0)
        {
            _media_packet_copy();
        }
        else if (strncmp(cmd, "q", 1) == 0)
        {
            quit_program();
        }
    }
    else if (len == 2)
    {
        if (strncmp(cmd, "aa", 2) == 0)
        {
            _media_packet_create();
        }
        else if (strncmp(cmd, "ab", 2) == 0)
        {
            _media_packet_alloc();
        }
        else if (strncmp(cmd, "iv", 2) == 0)
        {
            _media_packet_is_video();
        }
        else if (strncmp(cmd, "ia", 2) == 0)
        {
            _media_packet_is_audio();
        }
        else if (strncmp(cmd, "ie", 2) == 0)
        {
            _media_packet_is_encoded();
        }
        else if (strncmp(cmd, "ir", 2) == 0)
        {
            _media_packet_is_raw();
        }
        else if (strncmp(cmd, "gd", 2) == 0)
        {
            _media_packet_get_duration();
        }
        else if (strncmp(cmd, "sd", 2) == 0)
        {
            g_menu_state = CURRENT_STATUS_DURATION;
        }
        else if (strncmp(cmd, "gf", 2) == 0)
        {
            _media_packet_get_format();
        }
        else if (strncmp(cmd, "gs", 2) == 0)
        {
            _media_packet_get_buffer_size();
        }
        else if (strncmp(cmd, "sf", 2) == 0)
        {
            g_menu_state = CURRENT_STATUS_PACKET_SET_FORMAT;
        }
        else if (strncmp(cmd, "se", 2) == 0)
        {
            _media_packet_set_extra();
        }
        else if (strncmp(cmd, "ge", 2) == 0)
        {
            _media_packet_get_extra();
        }
        else if (strncmp(cmd, "ht", 2) == 0)
        {
            _media_packet_has_tbm_surface_buffer();
        }
        else if (strncmp(cmd, "cf", 2) == 0)
        {
            _media_format_create();
        }

    }
    else if (len == 3)
    {
        if (strncmp(cmd, "gbp", 3) == 0)
        {
            _media_packet_get_buffer_data_ptr();
        }
        else if (strncmp(cmd, "gts", 3) == 0)
        {
            _media_packet_get_tbm_surface();
        }
        else if (strncmp(cmd, "fgv", 3) == 0)
        {
            _media_format_get_video_info();
        }
        else if (strncmp(cmd, "fga", 3) == 0)
        {
            _media_format_get_audio_info();
        }
        else if (strncmp(cmd, "svm", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_VIDEO_MIME;
        }
        else if (strncmp(cmd, "svw", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_VIDEO_WIDTH;
        }
        else if (strncmp(cmd, "svh", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_VIDEO_HEIGHT;
        }
        else if (strncmp(cmd, "sam", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_MIME;
        }
        else if (strncmp(cmd, "sac", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_CHANNEL;
        }
        else if (strncmp(cmd, "sas", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_SAMPLERATE;
        }
        else if (strncmp(cmd, "sab", 3) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_BIT;
        }
        else if (strncmp(cmd, "gnp", 3) == 0)
        {
            _media_packet_get_number_of_video_planes();
        }
        else if (strncmp(cmd, "gsw", 3) == 0)
        {
            _media_packet_get_video_stride_width();
        }
        else if (strncmp(cmd, "gsh", 3) == 0)
        {
            _media_packet_get_video_stride_height();
        }
        else if (strncmp(cmd, "gpp", 3) == 0)
        {
            _media_packet_get_video_plane_data_ptr();
        }
        else if (strncmp(cmd, "scd", 3) == 0)
        {
            _media_packet_set_codec_data();
        }
        else if (strncmp(cmd, "gcd", 3) == 0)
        {
            _media_packet_get_codec_data();
        }

    }
    else if (len == 4)
    {
        if (strncmp(cmd, "fraw", 4) == 0)
        {
            _create_format_raw();
        }
        else if (strncmp(cmd, "fes2", 4) == 0)
        {
            _create_format_320_240_es();
        }
        else if (strncmp(cmd, "svab", 4) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_VIDEO_AVG_BPS;
        }
        else if (strncmp(cmd, "svmb", 4) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_VIDEO_MAX_BPS;
        }
        else if (strncmp(cmd, "saab", 4) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_AVG_BPS;
        }
        else if (strncmp(cmd, "saat", 4) == 0)
        {
            g_menu_state = CURRENT_STATUS_FORMAT_SET_AUDIO_AAC_TYPE;
        }
    }


}

static void displaymenu(void)
{
    if (g_menu_state == CURRENT_STATUS_MAINMENU)
    {
        display_sub_basic();
    }
    else if (g_menu_state == CURRENT_STATUS_PACKET_SET_FORMAT)
    {
        g_print("*** choose format.\n");
        g_print("1. 320x240 , MEDIA_FORMAT_H264_HP\n");
        g_print("2. 128x128 , MEDIA_FORMAT_I420\n");
    }
    else if (g_menu_state == CURRENT_STATUS_DURATION)
    {
        g_print("*** input duration: \n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_VIDEO_MIME)
    {
        g_print("choose video media format mime type\n");
        g_print("0. MEDIA_FORMAT_H264_HP\n");
        g_print("1. MEDIA_FORMAT_RGB888\n");
        g_print("2. MEDIA_FORMAT_I420\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_VIDEO_WIDTH)
    {
        g_print("input video width:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_VIDEO_HEIGHT)
    {
        g_print("input video height:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_VIDEO_AVG_BPS)
    {
        g_print("input video avg_bps:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_VIDEO_MAX_BPS)
    {
        g_print("input video max_bps:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_MIME)
    {
        g_print("choose audio media format mime type\n");
        g_print("0. MEDIA_FORMAT_AMR\n");
        g_print("1. MEDIA_FORMAT_PCM\n");
        g_print("2. MEDIA_FORMAT_AAC\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_CHANNEL)
    {
        g_print("input audio channel:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_SAMPLERATE)
    {
       g_print("input audio sample rate:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_BIT)
    {
        g_print("input audio bit:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_AVG_BPS)
    {
        g_print("input audio average bps:\n");
    }
    else if (g_menu_state == CURRENT_STATUS_FORMAT_SET_AUDIO_AAC_TYPE)
    {
        g_print("input audio aac type (0 or 1):\n");
    }
    else
    {
        g_print("*** unknown status.\n");
        quit_program();
    }
    g_print(" >>> ");
}


gboolean timeout_menu_display(void* data)
{
    displaymenu();
    return FALSE;
}

static void interpret (char *cmd)
{
    switch (g_menu_state)
    {
        case CURRENT_STATUS_MAINMENU:
            {
                _interpret_main_menu(cmd);
            }
            break;
        case CURRENT_STATUS_PACKET_SET_FORMAT:
            {
                int num = atoi(cmd);
                switch (num)
                 {
                    case 1:
                        if(g_media_format[1])
                        {
                            _media_packet_set_format(g_media_format[1]);
                        }
                        else
                        {
                            _create_format_320_240_es();
                            _media_packet_set_format(g_media_format[1]);
                        }
                        break;
                    case 2:
                        if(g_media_format[2])
                        {
                            _media_packet_set_format(g_media_format[2]);
                        }
                        else
                        {
                            _create_format_raw();
                            _media_packet_set_format(g_media_format[2]);
                        }
                        break;
                    default:
                        g_print("invalid number..");


                 }
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_VIDEO_MIME:
            {
                int video_mime_idx = atoi(cmd);
                _media_format_set_video_mime(video_mime_idx);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_VIDEO_WIDTH:
            {
                int width = atoi(cmd);
                _media_format_set_video_width(width);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_VIDEO_HEIGHT:
            {
                int height = atoi(cmd);
                _media_format_set_video_height(height);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_VIDEO_AVG_BPS:
            {
                int video_avg_bps = atoi(cmd);
                _media_format_set_video_avg_bps(video_avg_bps);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_VIDEO_MAX_BPS:
            {
                int max_bps = atoi(cmd);
                _media_format_set_video_max_bps(max_bps);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_MIME:
            {
                int audio_mime_idx = atoi(cmd);
                _media_format_set_audio_mime(audio_mime_idx);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_CHANNEL:
            {
                int channel = atoi(cmd);
                _media_format_set_audio_channel(channel);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_SAMPLERATE:
            {
                int samplerate = atoi(cmd);
                _media_format_set_audio_samplerate(samplerate);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_BIT:
            {
                int bit = atoi(cmd);
                _media_format_set_audio_bit(bit);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_AVG_BPS:
            {
                int audio_avg_bps = atoi(cmd);
                _media_format_set_audio_avg_bps(audio_avg_bps);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_FORMAT_SET_AUDIO_AAC_TYPE:
            {
                bool is_ats = atoi(cmd);
                _media_format_set_audio_aac_type(is_ats);
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_DURATION:
            {
                uint64_t duration = atoi(cmd);
                _media_packet_set_duration(duration);
                reset_menu_state();
            }
    }

    g_timeout_add(100, timeout_menu_display, 0);
}

void display_sub_basic()
{
    g_print("\n");
    g_print("=========================================================================================\n");
    g_print("                                    media tool test\n");
    g_print("-----------------------------------------------------------------------------------------\n");
    g_print(" *** How to use ***\n");
    g_print(" : You can create 1 media_format_h handle by 'cf' and 1 media_packet_handle \n");
    g_print(" : ex) cf > svm > svw > svh > svab > svmb >fgv > > a > iv > ir ...> d\n");
    g_print(" : media_format_create is set to 'media_format_h[0]' \n");
    g_print(" : media_packet_create_alloc & media_packet_create use 'media_format_h[0]' \n");
    g_print(" : media_packet_create_from_tbm_surface use 'media_format_h[2]' \n");
    g_print("\n");
    g_print("cf. media_format_create\t\t");
    g_print("\n");
    g_print("svm. media_format_set_video_mime\t");
    g_print("svw. media_format_set_video_width\t");
    g_print("svh. media_format_set_video_height\t");
    g_print("svab. media_format_set_video_avg_bps\t");
    g_print("svmb. media_format_set_video_max_bps\t");
    g_print("\n");
    g_print("sam. media_format_set_audio_mime\t");
    g_print("sac. media_format_set_audio_channel\t");
    g_print("sas. media_format_set_audio_samplerate\t");
    g_print("sab. media_format_set_audio_bit \t");
    g_print("saab. media_format_set_audio_avg_bps\t");
    g_print("saat. media_format_set_audio_aac_type\t");
    g_print("\n");
    g_print("fgv. media_format_get_video_info \t");
    g_print("fga. media_format_get_audio_info \t\t");
    g_print("\n");
    g_print("\n");
    g_print("a. media_packet_create_alloc(+media_format_unref)  \t");
    g_print("aa. media_packet_create(+media_format_unref) \t");
    g_print("b. media_packet_create_from_tbm_surface(+media_format_unref) \n");
    g_print("ab. media_packet_alloc \t\t");
    g_print("c. media_packet_copy  \t");
    g_print("\n");
    g_print("\n");
    g_print("iv. media_packet_is_video \t");
    g_print("ia. media_packet_is_audio \t");
    g_print("ie. media_packet_is_encoded \t");
    g_print("ir. media_packet_is_raw \t");
    g_print("ht. media_packet_has_tbm_surface_buffer \t");
    g_print("\n");
    g_print("sd. media_packet_set_duration \t");
    g_print("sf. media_packet_set_format(+media_format_unref) \t");
    g_print("se. media_packet_set_extra \t");
    g_print("\n");
    g_print("gd. media_packet_get_duration \t");
    g_print("gbp. media_packet_get_buffer_data_ptr \t");
    g_print("gts. media_packet_get_tbm_surface \t");
    g_print("gf. media_packet_get_format \t");
    g_print("ge. media_packet_get_extra \t");
    g_print("\n");
    g_print("gs. media_packet_get_buffer_size\t");
    g_print("\n");
    g_print("gnp. media_packet_get_number_of_video_planes\t");
    g_print("gsw. media_packet_get_video_stride_width\t");
    g_print("gsh. media_packet_get_video_stride_height\t");
    g_print("gpp. media_packet_get_video_plane_data_ptr\t");
    g_print("\n");
    g_print("scd. media_packet_set_codec_data\t");
    g_print("gcd. media_packet_get_codec_data\t");
    g_print("\n");
    g_print("d. media_packet_destroy \n");
    g_print("q. quit test suite(if exist alive media_format, do media_format_unref)");
    g_print("\n");
    g_print("=========================================================================================\n");
}


gboolean input (GIOChannel *channel)
{
    gchar buf[MAX_STRING_LEN];
    gsize read;
    GError *error = NULL;

    g_io_channel_read_chars(channel, buf, MAX_STRING_LEN, &read, &error);
    buf[read] = '\0';
    g_strstrip(buf);
    interpret (buf);

    return TRUE;
}

int main(int argc, char *argv[])
{
    GIOChannel *stdin_channel;
    stdin_channel = g_io_channel_unix_new(0);
    g_io_channel_set_flags (stdin_channel, G_IO_FLAG_NONBLOCK, NULL);
    g_io_add_watch (stdin_channel, G_IO_IN, (GIOFunc)input, NULL);

    displaymenu();

    ops.data = NULL;

    return appcore_efl_main(PACKAGE, &argc, &argv, &ops);

}

