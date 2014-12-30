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


#include <appcore-efl.h>

#define PACKAGE "media_tool_test"
#define MAX_STRING_LEN	2048
#define MAX_HANDLE 20


enum
{
    CURRENT_STATUS_MAINMENU,
    CURRENT_STATUS_SET_FORMAT,
    CURRENT_STATUS_DURATION,
};


char g_uri[MAX_STRING_LEN];
int g_menu_state = CURRENT_STATUS_MAINMENU;
int g_handle_num = 1;
static media_packet_h g_media_packet[MAX_HANDLE] = {0};
static media_format_h format[3] = {0};
bool is_only_created_handle;
bool is_excute_create;

/***********************************************/
/***  Test API part
/***********************************************/
static void _create_format_480_640_es(void)
{

    if (media_format_create(&format[0]) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_create success! \n");

        int ret = MEDIA_FORMAT_ERROR_NONE;
        ret = media_format_set_video_mime(format[0], MEDIA_FORMAT_H264_HP);
        ret = media_format_set_video_width(format[0], 480);
        ret = media_format_set_video_height(format[0], 640);
        ret = media_format_set_video_avg_bps(format[0], 10000000);
        ret = media_format_set_video_max_bps(format[0], 15000000);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
        {
            g_print("media_format_set_video_xxx success! w: 480, h: 640, MEDIA_FORMAT_H264_HP \n");
            g_print("\t\t\t\t ====>> ref_count = %d", MEDIA_FORMAT_GET_REFCOUNT(format[0]));
        }
        else
            g_print("media_format_set_video_xxx failed..\n");
    }
    else
    {
        g_print("media_format_create failed..\n");
    }

}

static void _create_format_320_240_es(void)
{

    if (media_format_create(&format[1]) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_create success! \n");
        int ret = MEDIA_FORMAT_ERROR_NONE;
        ret = media_format_set_video_mime(format[1], MEDIA_FORMAT_H264_HP);
        ret = media_format_set_video_width(format[1], 320);
        ret = media_format_set_video_height(format[1], 240);
        ret = media_format_set_video_avg_bps(format[1], 3000000);
        ret = media_format_set_video_max_bps(format[1], 15000000);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
        {
            g_print("media_format_set_video_xxx success! w: 320, h: 240, MEDIA_FORMAT_H264_HP \n");
            g_print("\t\t\t\t ====>> ref_count = %d", MEDIA_FORMAT_GET_REFCOUNT(format[1]));
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

    if (media_format_create(&format[2]) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_create success! \n");
        int ret = MEDIA_FORMAT_ERROR_NONE;
        ret = media_format_set_video_mime(format[2], MEDIA_FORMAT_RGB888);
        ret = media_format_set_video_width(format[2], 128);
        ret = media_format_set_video_height(format[2], 128);
        ret = media_format_set_video_avg_bps(format[2], 2000000);
        ret = media_format_set_video_max_bps(format[2], 15000000);
        if (ret == MEDIA_FORMAT_ERROR_NONE)
        {
            g_print("media_format_set_video_xxx success! w:128, h:128, MEDIA_FORMAT_RGB888\n");
            g_print("\t\t\t\t ====>> ref_count = %d", MEDIA_FORMAT_GET_REFCOUNT(format[2]));
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

static void _create_and_alloc(void)
{
    g_print("=== create_and_alloc!!\n");
    int i;

    for(i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i] != NULL)
        {
            media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = NULL;
        }

        if(format[0] == NULL)
        {
            g_print("media_format_h is NULL!! create media_format_h handle \n");
            break;
        }
        /* only ES format , if you want to another format, see _create_format_es() */
        if (media_packet_create_alloc(format[0], _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
        {
            g_print("media_packet create is failed\n");
            media_format_unref(format[0]);
        }
        else
        {
            g_print("media_packet_create_alloc is sucess!!\n");
            media_format_unref(format[0]);
            is_excute_create = true;
        }

    }
}

static void _create(void)
{
    g_print("=== create!!\n");
    is_only_created_handle = true;
    int i;

    for(i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i] != NULL)
        {
            media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = NULL;
        }

        if(format[0] == NULL)
        {
            g_print("media_format_h is NULL!! create media_format_h handle \n");
            break;
        }

        /* only ES format , if you want to another format, see _create_format_es() */
        if (media_packet_create(format[0], _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
        {
            g_print("media_packet create is failed\n");
            media_format_unref(format[0]);
        }
        else
        {
            g_print("media_packet_create is sucess!!\n");
            is_excute_create = true;
        }
    }
}

static void _alloc(void)
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

static void _create_from_tbm_surface(void)
{
    g_print("=== create_from_tbm_surface!!\n");

    tbm_surface_h surface;
    surface = tbm_surface_create(128, 128, TBM_FORMAT_RGB888);

    if (surface)
    {
        int i;

        for(i = 0; i < g_handle_num; i++)
        {
            if(g_media_packet[i] != NULL)
            {
                media_packet_destroy(g_media_packet[i]);
                g_media_packet[i] = NULL;
            }

            if(format[2] == NULL)
            {
                g_print("media_format_h is NULL!! create media_format_h handle \n");
                break;
            }

            /* only RAW format , if you want to another format, see _create_format_raw() */
            if (media_packet_create_from_tbm_surface(format[2], surface, _finalize_callback, NULL, &g_media_packet[i]) != MEDIA_PACKET_ERROR_NONE)
            {
                g_print("media_packet_create_from_tbm_surface is failed\n");
                media_format_unref(format[2]);
            }
            else
            {
                g_print("media_packet_create_from_tbm_surface is sucess!!\n");
                is_excute_create = true;
            }
        }

    }
    else
    {
        g_print("tbm_surface_create failed...\n");
    }
}

static void _copy(void)
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

static void _destroy()
{
    g_print("=== destroy!!\n");
    int i;
    int ret;
    for (i = 0; i < g_handle_num ; i++)
    {
        if(g_media_packet[i]!=NULL)
        {
            ret = media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = 0;
            if (ret == MEDIA_PACKET_ERROR_NONE)
                g_print("media_packet_destroy is sucess!!\n");
            else
                g_print("media_packet_destroy is failed...\n");
        }
    }
}

static void _get_size(void)
{
    uint64_t size;
    if (media_packet_get_buffer_size(g_media_packet[0],&size) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_buffer_size is sucess!!");
        g_print("\t\t[media_packet]===> size = %llu", size);
    }
    else
    {
        g_print("media_packet_get_buffer_size is failed...");
    }
}

static void _get_duration(void)
{
    uint64_t duration;

    if (media_packet_get_duration(g_media_packet[0], &duration) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_duration is sucess!!");
        g_print("\t\t[media_packet]===> duration = %llu", duration);
    }
    else
    {
        g_print("media_packet_get_duration is failed...");
    }
}

static void _get_buffer_ptr(void)
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
        g_print("\t\t[media_packet]===> buffer_data_ptr = %p", ptr);
    }
    else
    {
        g_print("media_packet_get_buffer_data_ptr is failed...");
    }

}

static void _get_tbm_surface()
{
    tbm_surface_h tbm_surface;

    if (media_packet_get_tbm_surface(g_media_packet[0], &tbm_surface) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_get_tbm_surface is sucess!!");
        g_print("\t\t[media_packet]===> tbm_surface = %p", (void*)tbm_surface);
    }
    else
    {
        g_print("media_packet_get_duration is failed...");
    }
}

static void _set_duration(uint64_t duration)
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

static void _is_video(void)
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

static void _is_audio(void)
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

static void _is_encoded(void)
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
static void _is_raw(void)
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

static void _get_format(void)
{
    media_format_h fmt;
    if (media_packet_get_format(g_media_packet[0], &fmt) == MEDIA_PACKET_ERROR_NONE)
    {
        int w;
        int h;

        g_print("media_packet_get_format is sucess!\n");
        if (media_format_get_video_info(fmt, NULL, &w, &h, NULL, NULL) == MEDIA_PACKET_ERROR_NONE)
        {
            g_print("\t\t [media_format] width = %d, height =%d", w, h);
        }
        else
        {
            g_print("media_format_get_video is failed...");
        }
    }
    else
    {
        g_print("media_packet_get_format is failed...");
    }

}


static void _set_format(media_format_h fmt)
{
    if (media_packet_set_format(g_media_packet[0], fmt) == MEDIA_PACKET_ERROR_NONE)
    {
        g_print("media_packet_set_format is sucess!\n");
    }
    else
    {
        g_print("media_packet_get_format is failed...");
    }

}

static void _set_extra(void)
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

static void _get_extra(void)
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

static void _has_tbm_surface_buffer(void)
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

static void _format_get_video(void)
{
    media_format_mimetype_e mime;
    int w;
    int h;

    media_packet_s* packet;
    packet = (media_packet_s*)g_media_packet[0];

    if (media_format_get_video_info(packet->format, &mime, &w, &h, NULL, NULL) == MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_get_video is sucess!\n");
        g_print("\t\t[media_format_get_video]mime:0x%x, width :%d, height :%d\n", mime, w, h);
        g_print("packet format ref_count: %d", MEDIA_FORMAT_GET_REFCOUNT(packet->format));
    }
    else
    {
        g_print("media_format_get_video is failed..");
    }
}

static void _format_get_audio(void)
{
    media_format_mimetype_e mime;
    int channel;
    int samplerate;
    int bit;
    int avg_bps;

    media_packet_s* packet;
    packet = (media_packet_s*)g_media_packet[0];

    if (media_format_get_audio_info(packet->format, &mime, &channel, &samplerate, &bit, &avg_bps)== MEDIA_FORMAT_ERROR_NONE)
    {
        g_print("media_format_get_audio_info is sucess!\n");
        g_print("\t\t[media_format_get_audio_info]mime:0x%x, channel :%d, samplerate :%d, bit: %d, avg_bps:%d \n", mime, channel, samplerate, bit, avg_bps);
        g_print("packet format ref_count: %d", MEDIA_FORMAT_GET_REFCOUNT(packet->format));
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

    for (i = 0; i < g_handle_num; i++)
    {
        if(g_media_packet[i]!=NULL)
        {
            media_packet_destroy(g_media_packet[i]);
            g_media_packet[i] = NULL;
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
                _create_and_alloc();
            }
            else if (strncmp(cmd, "d", 1) == 0)
            {
                _destroy();
            }
            else if (strncmp(cmd, "b", 1) == 0)
            {
                _create_from_tbm_surface();
            }
            else if (strncmp(cmd, "c", 1) == 0)
            {
                _copy();
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
                    _create();
                }
            else if (strncmp(cmd, "ab", 2) == 0)
                {
                    _alloc();
                }
            else if (strncmp(cmd, "iv", 2) == 0)
                {
                    _is_video();
                }
            else if (strncmp(cmd, "ia", 2) == 0)
                {
                    _is_audio();
                }
            else if (strncmp(cmd, "ie", 2) == 0)
                {
                    _is_encoded();
                }
            else if (strncmp(cmd, "ir", 2) == 0)
                {
                    _is_raw();
                }
            else if (strncmp(cmd, "gd", 2) == 0)
                {
                    _get_duration();
                }
            else if (strncmp(cmd, "sd", 2) == 0)
                {
                    g_menu_state = CURRENT_STATUS_DURATION;
                }
            else if (strncmp(cmd, "gf", 2) == 0)
                {
                    _get_format();
                }
            else if (strncmp(cmd, "gs", 2) == 0)
                {
                    _get_size();
                }
            else if (strncmp(cmd, "sf", 2) == 0)
                {
                    g_menu_state = CURRENT_STATUS_SET_FORMAT;
                }
            else if (strncmp(cmd, "se", 2) == 0)
                {
                    _set_extra();
                }
            else if (strncmp(cmd, "ge", 2) == 0)
                {
                    _get_extra();
                }
            else if (strncmp(cmd, "ht", 2) == 0)
                {
                    _has_tbm_surface_buffer();
                }

        }
    else if (len == 3)
        {
            if (strncmp(cmd, "gbp", 3) == 0)
                {
                    _get_buffer_ptr();
                }
            else if (strncmp(cmd, "gts", 3) == 0)
                {
                    _get_tbm_surface();
                }
            else if (strncmp(cmd, "fes", 3) == 0)
                {
                    _create_format_480_640_es();
                }
            else if (strncmp(cmd, "fgv", 3) == 0)
                {
                    _format_get_video();
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
        }


}

static void displaymenu(void)
{
    if (g_menu_state == CURRENT_STATUS_MAINMENU)
        {
            display_sub_basic();
        }
    else if (g_menu_state == CURRENT_STATUS_SET_FORMAT)
        {
            g_print("*** choose format.\n");
            g_print("0. 640x480 , MEDIA_FORMAT_H264_HP\n");
            g_print("1. 320x240 , MEDIA_FORMAT_H264_HP\n");
            g_print("2. 128x128 , MEDIA_FORMAT_RGB888\n");
        }
    else if (g_menu_state == CURRENT_STATUS_DURATION)
        {
            g_print("*** input duration \n");
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
        case CURRENT_STATUS_SET_FORMAT:
            {
                int num = atoi(cmd);
                switch (num)
                 {
                    case 0:
                        if (format[0])
                        {
                            _set_format(format[0]);
                        }
                        else
                        {
                            _create_format_480_640_es();
                            _set_format(format[0]);
                        }
                        break;
                    case 1:
                        if(format[1])
                        {
                            _set_format(format[1]);
                        }
                        else
                        {
                            _create_format_320_240_es();
                            _set_format(format[1]);
                        }
                        break;
                    case 2:
                        if(format[2])
                        {
                            _set_format(format[2]);
                        }
                        else
                        {
                            _create_format_raw();
                            _set_format(format[2]);
                        }
                        break;
                    default:
                        g_print("invalid number..");


                 }
                reset_menu_state();
            }
            break;
        case CURRENT_STATUS_DURATION:
            {
                uint64_t duration = atoi(cmd);
                _set_duration(duration);
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
    g_print("fes. create format for es 480x640\t\t");
    g_print("fes2. create format for es 320x240\t\t");
    g_print("fraw. create format for raw 128x128 \t\t");
    g_print("\n");
    g_print("a. Create_and _Alloc \t");
    g_print("aa. Create \t");
    g_print("ab. Alloc \t");
    g_print("c. Copy \t");
    g_print("b. Create from tbm_surface \n");
    g_print("iv. is_video \t\t");
    g_print("ia. is_audio \t\t");
    g_print("ie. is_encoded \t\t");
    g_print("ir. is_raw \t\t");
    g_print("ht. has_tbm_surface \t\t");
    g_print("\n");
    g_print("sd. set duration \t\t");
    g_print("sf. set format \t\t");
    g_print("se. set extra \t\t");
    g_print("\n");
    g_print("gd. get duration \t\t");
    g_print("gbp. get buffer data ptr \t\t");
    g_print("gts. get tbm surface \t\t");
    g_print("gf. get format \t\t");
    g_print("ge. get extra \t\t");
    g_print("\n");
    g_print("fgv. media_format_get_video_info \t\t");
    g_print("fga. media_format_get_audio_info \t\t");
    g_print("\n");
    g_print("gs. get size\t\t");
    g_print("\n");
    g_print("d. Destroy \n");
    g_print("q. quit test suite");
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

