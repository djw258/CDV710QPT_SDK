#include "layout_define.h"

typedef enum
{
	frame_show_frame1_id,
	frame_show_frame2_id,
	frame_show_scr_act_obj_id_cancel_obj,
	frame_show_scr_act_obj_id_setting_obj,
	frame_show_scr_act_obj_id_timer_label_obj,
	frame_show_scr_act_obj_id_date_label_obj,
	frame_show_scr_act_obj_id_calendar_year_label,
	frame_show_scr_act_obj_id_calendar_week_label,
	frame_show_scr_act_obj_id_calendar_mon_label,
	frame_show_scr_act_obj_id_calendar_day_label,
	frame_show_scr_act_obj_id_playback_name_label,
	frame_show_scr_act_obj_id_playback_chann_label

} frame_show_id;
#define THUMB_WIDTH (1024) // 1024
#define THUMB_HIGHT (608)	 // 600

#define FRAME_1_FILE_PATH "/app/resource/wallpaper/bg_openframe_background01.jpg"
#define FRAME_2_FILE_PATH "/app/resource/wallpaper/bg_openframe_background02.jpg"
#define FRAME_3_FILE_PATH "/app/resource/wallpaper/bg_openframe_background03.jpg"
#define FRAME_4_FILE_PATH "/app/resource/wallpaper/bg_openframe_background04.jpg"
#define FRAME_5_FILE_PATH "/app/resource/wallpaper/bg_openframe_background05.jpg"
#define FRAME_6_FILE_PATH "/app/resource/wallpaper/bg_openframe_background06.jpg"
#define FRAME_7_FILE_PATH "/app/resource/wallpaper/bg_openframe_background07.jpg"
#define FRAME_8_FILE_PATH "/app/resource/wallpaper/bg_openframe_background08.jpg"
#define FRAME_9_FILE_PATH "/app/resource/wallpaper/bg_openframe_background09.jpg"
#define FRAME_10_FILE_PATH "/app/resource/wallpaper/bg_openframe_background10.jpg"

char *frame_path[3] = {

		FRAME_3_FILE_PATH,
		FRAME_4_FILE_PATH,
		FRAME_5_FILE_PATH,

};
static int frame_path_index = 0;
static lv_img_dsc_t *frame_buffer_cur_a = NULL;
static lv_img_dsc_t *frame_buffer_cur_b = NULL;

static void frame_show_restart(void);
/***
**   日期:2022-06-10 09:40:37
**   作者: leo.liu
**   函数作用：当前显示位置
**   参数说明:
***/
static int frame_show_frame_index = 0x00;
/***
**   日期:2022-06-10 11:33:05
**   作者: leo.liu
**   函数作用：显示偏移
**   参数说明:
***/
static int frame_show_effeset_x = 0;

/***
**   日期:2022-06-10 11:27:00
**   作者: leo.liu
**   函数作用：当前显示的是a/b
**   参数说明: 2-b 1-a
***/
static lv_img_dsc_t *frame_show_pbuffer = NULL;

/***
**   日期:2022-06-22 09:55:40
**   作者: leo.liu
**   函数作用：上次亮度值
**   参数说明:
***/
static int pre_frame_diplay_light_value = -1;

/***
**   日期:2022-06-10 11:33:05
**   作者: leo.liu
**   函数作用：解码文件路径
**   参数说明:
***/
static char *frame_show_frame_path = NULL;

/***
**   日期:2022-06-10 11:17:29
**   作者: leo.liu
**   函数作用：等待渲染次数
**   参数说明:
***/
static int frame_show_refresh_cont = 0;

/************************************************************
** 函数说明: 获取通道
** 作者: xiaoxiao
** 日期: 2023-05-26 08:00:46
** 参数说明:
** 注意事项:
************************************************************/
static int frame_show_channel_get(void)
{

	if (monitor_door_first_valid_get(true) < 0 && monitor_door_first_valid_get(false) < 0)
	{
		return MON_CH_NONE;
	}
	int ch = monitor_channel_get();
	if (ch == MON_CH_NONE)
	{
		ch = MON_CH_DOOR1;
		if (monitor_valid_channel_check(ch) == true && user_data_get()->display.frame_list & 0x08)
		{
			return ch;
		}
	}
	int find = 0;
find_start:
	for (int i = 0; i < 7; i++)
	{
		if ((ch == MON_CH_DOOR1 + i) && (ch != MON_CH_DOOR8))
		{
			ch = MON_CH_DOOR2 + i;
			if (monitor_valid_channel_check(ch) == true && user_data_get()->display.frame_list & 0x08)
			{
				return ch;
			}
		}
	}
	if (ch == MON_CH_DOOR8)
	{
		ch = MON_CH_CCTV1;
		if (monitor_valid_channel_check(ch) == true && user_data_get()->display.frame_list & 0x10)
		{
			return ch;
		}
	}
	for (int i = 0; i < 7; i++)
	{
		if ((ch == MON_CH_CCTV1 + i) && (ch != MON_CH_CCTV8))
		{
			ch = MON_CH_CCTV2 + i;
			if (monitor_valid_channel_check(ch) == true && user_data_get()->display.frame_list & 0x10)
			{
				return ch;
			}
		}
	}
	if (ch == MON_CH_CCTV8)
	{
		ch = MON_CH_DOOR1;
		if (monitor_valid_channel_check(ch) == true && user_data_get()->display.frame_list & 0x08)
		{
			return ch;
		}
	}
	find++;
	if (find == 2)
	{
		return MON_CH_NONE;
	}
	goto find_start;
}

/***
**   日期:2022-06-10 09:51:32
**   作者: leo.liu
**   函数作用：判断是否可以进入frame show
**   参数说明:
***/
bool frame_display_timeout_check(void)
{
	if (user_data_get()->display.frame_time_en == false)
	{
		return false;
	}
	struct tm tm;
	user_time_read(&tm);
	unsigned int cur = tm.tm_hour * 60 + tm.tm_min;
	unsigned int start = user_data_get()->display.frame_time_start;
	unsigned int end = user_data_get()->display.frame_time_end;
	if (start > end)
	{
		end += 24 * 60;
	}
	if ((cur > start && (cur < end)))
	{
		return false;
	}
	return true;
}

/***
**   日期:2022-06-11 11:00:52
**   作者: leo.liu
**   函数作用：判断夜间模式
**   参数说明:
***/
bool frame_display_lightmode_check(void)
{
	if (user_data_get()->display.night_mode == false)
	{
		return false;
	}
	struct tm tm;
	user_time_read(&tm);
	unsigned int cur = tm.tm_hour * 60 + tm.tm_min;
	unsigned int start = user_data_get()->display.night_time_start;
	unsigned int end = user_data_get()->display.night_time_end;
	if (start > end)
	{
		end += 24 * 60;
	}
	if ((cur < end) && (cur >= start))
	{
		return true;
	}
	return false;
}
static bool frame2_show_thumb_media_display_callback(const char *data, int x, int y, int w, int h)
{
	if ((frame_buffer_cur_b == frame_show_pbuffer) || (frame_show_pbuffer == NULL))
	{
		if ((frame_buffer_cur_a != NULL) && (frame_buffer_cur_a->data != NULL))
		{
			if (frame_buffer_cur_a->header.w != w)
			{
				SAT_DEBUG("frame_buffer_cur_a->header.w != w (%d != %d)", frame_buffer_cur_a->header.w, w);
				return false;
			}
			if (frame_buffer_cur_a->header.h != h)
			{
				SAT_DEBUG("frame_buffer_cur_a->header.h != h (%d != %d)", frame_buffer_cur_a->header.h, h);
				return false;
			}
			lv_memcpy_small((uint8_t *)frame_buffer_cur_a->data, data, w * h * 3);
		}
	}
	else
	{
		if ((frame_buffer_cur_b != NULL) && (frame_buffer_cur_b->data != NULL))
		{
			if (frame_buffer_cur_b->header.w != w)
			{
				SAT_DEBUG("frame_buffer_cur_a->header.w != w (%d != %d)", frame_buffer_cur_b->header.w, w);
				return false;
			}
			if (frame_buffer_cur_b->header.h != h)
			{
				SAT_DEBUG("frame_buffer_cur_a->header.h != h (%d != %d)", frame_buffer_cur_b->header.h, h);
				return false;
			}
			lv_memcpy_small((uint8_t *)frame_buffer_cur_b->data, data, w * h * 3);
		}
	}

	return true;
}

static void frame_show_media_thumb_display(const char *path)
{
	char arry[1][128] = {0}; //{"/tmp/tf/media/300116-191938.JPG 1 42 216 174"};

	memset(arry[0], 0, sizeof(arry[0]));
	sprintf(arry[0], "%s 0 0 %d %d", path, THUMB_WIDTH, THUMB_HIGHT);
	sat_linphone_media_thumb_display(arry, 1, frame2_show_thumb_media_display_callback);
}

static void frame_show_delay_close_monitor_timer(lv_timer_t *ptimer)
{
	monitor_close(0x02);
	lv_timer_del(ptimer);
	frame_show_restart();
}

/***
**   日期:2022-06-10 11:34:27
**   作者: leo.liu
**   函数作用：动画左移效果
**   参数说明:
***/
static void frame_show_animation_effect_timer(lv_timer_t *ptimer)
{

	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_clear_flag(frame1, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(frame2, LV_OBJ_FLAG_HIDDEN);
#define FRAME_STEP 16
	frame_show_effeset_x += FRAME_STEP;
	if (frame_show_effeset_x <= 1024)
	{
		if (frame_show_pbuffer == frame_buffer_cur_a)
		{
			lv_obj_set_style_x(frame1, -frame_show_effeset_x, LV_PART_MAIN);
			lv_obj_set_style_x(frame2, 1024 - frame_show_effeset_x, LV_PART_MAIN);
		}
		else
		{
			lv_obj_set_style_x(frame2, -frame_show_effeset_x, LV_PART_MAIN);
			lv_obj_set_style_x(frame1, 1024 - frame_show_effeset_x, LV_PART_MAIN);
		}
	}
	else
	{
		frame_show_pbuffer = frame_show_pbuffer == frame_buffer_cur_a ? frame_buffer_cur_b : frame_buffer_cur_a;

		if ((frame_show_frame_index == 0x08) || (frame_show_frame_index == 0x09) || (frame_show_frame_index == 0x10) || (frame_show_frame_index == 0x11))
		{
			monitor_open(true, true);
		}
		lv_sat_timer_create(frame_show_delay_close_monitor_timer, 10000, NULL);
		lv_timer_del(ptimer);
	}
}

static void frame_show_thumb_refresh_display_callback(void)
{
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = frame_show_pbuffer == frame_buffer_cur_a ? frame2 : frame1;
	if (frame_show_frame_index == 0x01 || frame_show_frame_index == 0x02 || frame_show_frame_index == 0x04)
	{

		lv_img_cache_invalidate_src(obj->bg_img_src);
		lv_obj_set_style_bg_img_src(obj, frame_show_pbuffer == frame_buffer_cur_a ? frame_buffer_cur_b : frame_buffer_cur_a, LV_PART_MAIN);
	}
	else
	{
		lv_obj_set_style_bg_img_src(obj, NULL, LV_PART_MAIN);
	}
}

/***
**   日期:2022-06-10 11:00:37
**   作者: leo.liu
**   函数作用：等待渲染完成
**   参数说明:
***/
static void frame_show_refresh_wait_task(lv_timer_t *ptimer)
{
	if (frame_show_pbuffer == NULL)
	{
		frame_show_pbuffer = frame_buffer_cur_a;
	}

	frame_show_refresh_cont++;
	if (frame_show_refresh_cont == 2)
	{
		frame_show_refresh_cont = 1;

		frame_show_effeset_x = 0;
		lv_sat_timer_create(frame_show_animation_effect_timer, 40, NULL);
	}
	else
	{
		if ((frame_show_frame_index == 0x08) || (frame_show_frame_index == 0x09) || (frame_show_frame_index == 0x10) || (frame_show_frame_index == 0x11))
		{
			monitor_open(true, true);
			// sat_linphone_calls_cmd_send();
		}
		lv_sat_timer_create(frame_show_delay_close_monitor_timer, 10000, NULL);
	}
	lv_timer_del(ptimer);
}

/************************************************************
** 函数说明: 退出待机壁纸界面
** 作者: xiaoxiao
** 日期: 2023-05-23 13:38:54
** 参数说明:
** 注意事项:
************************************************************/
static void layout_frame_show_cancel_btn_liick(lv_event_t *e)
{
	sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
/***
**   日期:2022-06-10 10:36:16
**   作者: leo.liu
**   函数作用：返回按钮显示
**   参数说明:
***/
static void frame_show_cancel_btn_display(void)
{
	// lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	// lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	// lv_obj_t *obj = lv_obj_get_child_form_id(frame1, frame_show_scr_act_obj_id_cancel_obj);
	// if (obj == NULL)
	// {
	// 	lv_common_img_btn_create(frame1, frame_show_scr_act_obj_id_cancel_obj, 40, 15, 80, 80,
	// 							 layout_frame_show_cancel_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	// }
	// obj = lv_obj_get_child_form_id(frame2, frame_show_scr_act_obj_id_cancel_obj);
	// if (obj == NULL)
	// {
	// 	lv_common_img_btn_create(frame2, frame_show_scr_act_obj_id_cancel_obj, 40, 15, 80, 80,
	// 							 layout_frame_show_cancel_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	// }
	// lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	// lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_cancel_obj);
	if (obj == NULL)
	{
		lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_cancel_obj, 40, 15, 80, 80,
														 layout_frame_show_cancel_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}
	obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_cancel_obj);
	if (obj == NULL)
	{
		lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_cancel_obj, 40, 15, 80, 80,
														 layout_frame_show_cancel_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}
}

/************************************************************
** 函数说明: 进入待机设置
** 作者: xiaoxiao
** 日期: 2023-05-23 13:43:29
** 参数说明:
** 注意事项:
************************************************************/
static void layout_frame_show_setting_btn_liick(lv_event_t *e)
{
	sat_layout_goto(setting_standby_screen, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
}
/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期: 2023-05-23 13:41:31
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_setting_btn_display(void)
{
	// lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	// lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	// lv_obj_t *obj = lv_obj_get_child_form_id(frame1, frame_show_scr_act_obj_id_setting_obj);
	// if (obj == NULL)
	// {
	// 	lv_common_img_btn_create(frame1, frame_show_scr_act_obj_id_setting_obj, 904, 15, 80, 80,
	// 							 layout_frame_show_setting_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 resource_ui_src_get("btn_title_setting.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	// }
	// obj = lv_obj_get_child_form_id(frame2, frame_show_scr_act_obj_id_setting_obj);
	// if (obj == NULL)
	// {
	// 	lv_common_img_btn_create(frame2, frame_show_scr_act_obj_id_setting_obj, 904, 15, 80, 80,
	// 							 layout_frame_show_setting_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
	// 							 resource_ui_src_get("btn_title_setting.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	// }
	// lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	// lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_setting_obj);
	if (obj == NULL)
	{
		lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_setting_obj, 904, 15, 80, 80,
														 layout_frame_show_setting_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 resource_ui_src_get("btn_title_setting.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}
	obj = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_setting_obj);
	if (obj == NULL)
	{
		lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_scr_act_obj_id_setting_obj, 904, 15, 80, 80,
														 layout_frame_show_setting_btn_liick, true, LV_OPA_TRANSP, 0X101010, LV_OPA_TRANSP, 0x808080,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 resource_ui_src_get("btn_title_setting.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}
}

/************************************************************
** 函数说明: 时间显示
** 作者: xiaoxiao
** 日期: 2023-05-23 17:09:02
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_time_obj_display(lv_obj_t *parent)
{
	if (parent == NULL)
	{
		return;
	}
	lv_obj_t *obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_timer_label_obj);
	if (obj == NULL)
	{
		return;
	}
	struct tm tm;
	user_time_read(&tm);
	lv_label_set_text_fmt(obj, "%02d:%02d", tm.tm_hour, tm.tm_min);
}

/************************************************************
** 函数说明: 日期显示
** 作者: xiaoxiao
** 日期: 2023-05-23 17:11:03
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_date_obj_display(lv_obj_t *parent)
{
	if (parent == NULL)
	{
		return;
	}
	lv_obj_t *label_date = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_date_label_obj);
	if (label_date == NULL)
	{
		return;
	}
	struct tm tm;
	user_time_read(&tm);
	char week_str[64] = {0};
	strcpy(week_str, lang_str_get(tm.tm_wday - 1 + HOME_XLS_LANG_ID_MON));
	// layout_home_week_language_get(tm.tm_wday - 1);
	char mon_str[64] = {0};
	strcpy(mon_str, lang_str_get(tm.tm_mon - 1 + HOME_XLS_LANG_ID_JAN));
	// layout_home_month_language_get(tm.tm_mon - 1);

	LANGUAGE_ID lang = language_id_get();
	if (lang == LANGUAGE_ID_ENGLISH)
	{
		lv_label_set_text_fmt(label_date, "%s %s %d. %04d", week_str, mon_str, tm.tm_mday, tm.tm_year);
	}
	else if (lang == LANGUAGE_ID_HANYU)
	{
		lv_label_set_text_fmt(label_date, "%04d년 %d월 %d일 %s", tm.tm_year, tm.tm_mon, tm.tm_mday, week_str);
	}
	else if (lang == LANGUAGE_ID_ELUOSI)
	{
		lv_label_set_text_fmt(label_date, "%s%d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
	}
	else if (lang == LANGUAGE_ID_XIBANYA)
	{
		lv_label_set_text_fmt(label_date, "%s %d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
	}
	else if (lang == LANGUAGE_ID_CHINESE)
	{
		lv_label_set_text_fmt(label_date, "%04d年 %d月 %d日 %s", tm.tm_year, tm.tm_mon, tm.tm_mday, week_str);
	}
	else if (lang == LANGUAGE_ID_YUENAN)
	{
		lv_label_set_text_fmt(label_date, "%s %d %s %04d", week_str, tm.tm_mday, mon_str, tm.tm_year);
	}
	else if (lang == LANGUAGE_ID_ALABOYU)
	{
		lv_label_set_text_fmt(label_date, "%d %s %04d %s", tm.tm_mday, mon_str, tm.tm_year, week_str);
	}
}

static void frame_show_setting_time_label_display(void)
{
	if (frame_show_frame_index != 0x01)
	{
		return;
	}
	lv_obj_t *parent = NULL;
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	if (frame_show_pbuffer != frame_buffer_cur_a)
	{
		parent = frame1;
	}
	else
	{
		parent = frame2;
	}
	if (user_data_get()->display.frame_background == false)
	{
		lv_obj_set_style_bg_img_src(parent, NULL, LV_PART_MAIN);
	}
	lv_obj_t *time_obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_timer_label_obj);
	if (time_obj == NULL)
	{
		time_obj = lv_common_text_create(parent, frame_show_scr_act_obj_id_timer_label_obj, 360, 139, 304, 147,
																		 NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
																		 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																		 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																		 NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_plus);
	}

	lv_obj_t *date_obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_date_label_obj);
	if (date_obj == NULL)
	{
		date_obj = lv_common_text_create(parent, frame_show_scr_act_obj_id_date_label_obj, 360, 288, 304, 30,
																		 NULL, LV_OPA_TRANSP, 0X303030, LV_OPA_TRANSP, 0,
																		 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																		 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																		 NULL, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}
	frame_show_time_obj_display(parent);
	frame_show_date_obj_display(parent);
}
/************************************************************
** 函数说明: 时间加载
** 作者: xiaoxiao
** 日期: 2023-05-23 11:53:02
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_time_display(void)
{
	// frame_show_cancel_btn_display();
	// frame_show_setting_btn_display();
	frame_show_setting_time_label_display();
}
/************************************************************
** 函数说明: 开始时间显示
** 作者: xiaoxiao
** 日期: 2023-05-23 11:23:11
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_time_start(void)
{
	if (user_data_get()->display.frame_background == true)
	{
		if ((frame_show_frame_path == NULL) || (frame_path_index == 3))
		{
			frame_path_index = 0;
			frame_show_frame_path = frame_path[frame_path_index++];
		}
		else
		{
			frame_show_frame_path = frame_path[frame_path_index++];
		}
		frame_show_media_thumb_display(frame_show_frame_path);
	}
	frame_show_time_display();
	lv_sat_timer_create(frame_show_refresh_wait_task, 1000, NULL);
}

/************************************************************
** 函数说明: 日历控件显示
** 作者: xiaoxiao
** 日期: 2023-05-24 08:25:49
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_calendar_label_display(void)
{
	if (frame_show_frame_index != 0x02)
	{
		return;
	}
	struct tm tm;
	user_time_read(&tm);
	lv_obj_t *parent = NULL;
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = NULL;
	if (frame_show_pbuffer != frame_buffer_cur_a)
	{
		parent = frame1;
	}
	else
	{
		parent = frame2;
	}

	if (user_data_get()->display.frame_background == false)
	{
		lv_obj_set_style_bg_img_src(parent, NULL, LV_PART_MAIN);
	}

	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_calendar_year_label);
	if (obj == NULL)
	{

		obj = lv_common_text_create(parent, frame_show_scr_act_obj_id_calendar_year_label, 373, 165, 280, 42,
																NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																"", 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}
	lv_label_set_text_fmt(obj, "%04d", tm.tm_year);

	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_calendar_week_label);

	if (obj == NULL)
	{
		lv_common_text_create(parent, frame_show_scr_act_obj_id_calendar_week_label, 373, 349, 280, 42,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													lang_str_get(tm.tm_wday - 1 + HOME_XLS_LANG_ID_MON), 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}

	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_calendar_mon_label);
	if (obj == NULL)
	{

		lv_common_text_create(parent, frame_show_scr_act_obj_id_calendar_mon_label, 373, 399, 280, 42,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													lang_str_get(tm.tm_mon - 1 + HOME_XLS_LANG_ID_JAN), 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_normal);
	}

	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_calendar_day_label);
	if (obj == NULL)
	{
		obj = lv_common_text_create(parent, frame_show_scr_act_obj_id_calendar_day_label, 293, 220, 440, 119,
																NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																"", 0xffffff, 0xffffff, LV_TEXT_ALIGN_CENTER, lv_font_plus);
	}
	lv_label_set_text_fmt(obj, "%02d", tm.tm_mday);
}
/************************************************************
** 函数说明: 日历显示
** 作者: xiaoxiao
** 日期: 2023-05-24 08:24:00
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_clendar_display(void)
{
	// frame_show_cancel_btn_display();
	// frame_show_setting_btn_display();
	frame_show_calendar_label_display();
}
static void frame_show_calendar_start(void)
{
	if (user_data_get()->display.frame_background == true)
	{
		if ((frame_show_frame_path == NULL) || (frame_path_index == 3))
		{
			frame_path_index = 0;
			frame_show_frame_path = frame_path[frame_path_index++];
		}
		else
		{
			frame_show_frame_path = frame_path[frame_path_index++];
		}
		frame_show_media_thumb_display(frame_show_frame_path);
	}
	frame_show_clendar_display();
	lv_sat_timer_create(frame_show_refresh_wait_task, 1000, NULL);
}

/************************************************************
** 函数说明: 回放媒体文件名字显示
** 作者: xiaoxiao
** 日期: 2023-06-01 21:29:19
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_playback_name_display(const char *name)
{
	if (frame_show_frame_index != 0x04)
	{
		return;
	}
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = NULL;
	lv_obj_t *parent = NULL;
	if (frame_show_pbuffer != frame_buffer_cur_a)
	{
		parent = frame1;
	}
	else
	{
		parent = frame2;
	}
	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_playback_name_label);
	if (obj == NULL)
	{
		lv_common_text_create(parent, frame_show_scr_act_obj_id_playback_name_label, 0, 35, 1024, 80,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													name, 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}
}
/***********************************************
 ** 作者: leo.liu
 ** 日期: 2023-2-2 13:42:25
 ** 说明: 顶部时间通道显示
 ***********************************************/
static void frame_show_monitor_channle_display(void)
{
	if (frame_show_frame_index <= 0x07)
	{
		return;
	}
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	lv_obj_t *obj = NULL;
	lv_obj_t *parent = NULL;
	if (frame_show_pbuffer != frame_buffer_cur_a)
	{
		parent = frame1;
	}
	else
	{
		parent = frame2;
	}
	obj = lv_obj_get_child_form_id(parent, frame_show_scr_act_obj_id_playback_chann_label);
	// struct tm tm;
	// user_time_read(&tm);
	int channel = monitor_channel_get();
	if (obj == NULL)
	{
		obj = lv_common_text_create(parent, frame_show_scr_act_obj_id_playback_chann_label, 0, 35, 1024, 80,
																NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																"", 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
		if (obj != NULL)
		{
			lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
		}
	}

	if (is_channel_ipc_camera(channel) == true)
	{
		channel -= 8;
		lv_label_set_text_fmt(obj, "%s/%s", lang_str_get(HOME_XLS_LANG_ID_MONITORING), network_data_get()->cctv_device[channel].door_name);
	}
	else
	{
		lv_label_set_text_fmt(obj, "%s/%s", lang_str_get(HOME_XLS_LANG_ID_MONITORING), network_data_get()->door_device[channel].door_name);
	}
}

/************************************************************
** 函数说明: 开始回放显示
** 作者: xiaoxiao
** 日期: 2023-05-23 11:23:21
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_playback_start(void)
{

	// frame_show_cancel_btn_display();
	// frame_show_setting_btn_display();
	frame_show_setting_time_label_display();

	file_type type = FILE_TYPE_FLASH_PHOTO;
	int total = 0, new_total = 0;
	if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
	{
		type = FILE_TYPE_VIDEO;
	}
	media_file_total_get(type, &total, &new_total);
	if (total > 0)
	{
		const file_info *info = media_file_info_get(type, total - 1);
		char frame_show_frame_path[128] = {0};
		memset(frame_show_frame_path, 0, sizeof(frame_show_frame_path));
		sprintf(frame_show_frame_path, "%s%s", type == FILE_TYPE_FLASH_PHOTO ? FLASH_PHOTO_PATH : SD_MEDIA_PATH, info->file_name);
		frame_show_media_thumb_display(frame_show_frame_path);
		char file_path[128];
		sprintf(file_path, "%s/%s", info->file_name, info->ch);
		frame_show_playback_name_display(file_path);
	}
	lv_sat_timer_create(frame_show_refresh_wait_task, 1000, NULL);
}

/************************************************************
** 函数说明: 通道信息显示
** 作者: xiaoxiao
** 日期: 2023-05-23 14:28:55
** 参数说明:
** 注意事项:
************************************************************/
static void frame_show_door_display(void)
{
	// frame_show_monitor_head_display();
	frame_show_monitor_channle_display();
	// frame_show_monitor_auto_display();
	// frame_show_monitor_sd_display();
	// frame_show_cancel_btn_display();
	// frame_show_setting_btn_display();
	frame_show_setting_time_label_display();
}

static void frame_show_door1_start(void)
{
	monitor_enter_flag_set(MON_ENTER_MANUAL_DOOR_FLAG);
	int ch = frame_show_channel_get();

	monitor_channel_set(ch);
	frame_show_door_display();
	frame_show_thumb_refresh_display_callback();
	lv_sat_timer_create(frame_show_refresh_wait_task, 1000, NULL);
}

static void frame_show_cctv_start(void)
{
	monitor_enter_flag_set(MON_ENTER_MANUAL_CCTV_FLAG);
	monitor_channel_set(frame_show_channel_get());
	frame_show_door_display();
	frame_show_thumb_refresh_display_callback();
	lv_sat_timer_create(frame_show_refresh_wait_task, 1000, NULL);
}

/***
**   日期:2022-06-10 09:41:46
**   作者: leo.liu
**   函数作用：开始显示
**   参数说明:
***/
static void frame_show_restart(void)
{
	sat_linphone_media_thumb_destroy();
	if (frame_display_timeout_check() == true)
	{
		SAT_DEBUG("HHHHHHHHHHHHHHHHHHHH");
		sat_layout_goto(close, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
		return;
	}

	if (frame_display_lightmode_check() == true)
	{
		if (pre_frame_diplay_light_value != 0)
		{
			pre_frame_diplay_light_value = 0;
			backlight_brightness_set(5);
		}
	}
	else
	{
		pre_frame_diplay_light_value = user_data_get()->display.lcd_brigtness;
		backlight_brightness_set(user_data_get()->display.lcd_brigtness <= 5 ? 5 : user_data_get()->display.lcd_brigtness);
	}
	lv_obj_t *frame1 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame1_id);
	lv_obj_t *frame2 = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), frame_show_frame2_id);
	if (frame_show_pbuffer == frame_buffer_cur_a)
	{
		lv_obj_clean(frame2);
	}
	else
	{
		lv_obj_clean(frame1);
	}
	file_type type = FILE_TYPE_FLASH_PHOTO;
	int total = 0;
	if ((media_sdcard_insert_check() == SD_STATE_INSERT) || (media_sdcard_insert_check() == SD_STATE_FULL))
	{
		type = FILE_TYPE_VIDEO;
	}
	media_file_total_get(type, &total, NULL);

	if ((user_data_get()->display.frame_list & 0x01) && (frame_show_frame_index < 0x01))
	{
		/*****  开始时间的显示 *****/
		frame_show_frame_index = 0x01;
		frame_show_time_start();
	}
	else if ((user_data_get()->display.frame_list & 0x02) && (frame_show_frame_index < 0x02))
	{
		frame_show_frame_index = 0x02;
		frame_show_calendar_start();
	}
	else if ((user_data_get()->display.frame_list & 0x04) && (frame_show_frame_index < 0x04) && total)
	{
		frame_show_frame_index = 0x04;
		frame_show_playback_start();
	}
	else if ((user_data_get()->display.frame_list & 0x08) && (frame_show_frame_index <= 0x08))
	{
		// sat_linphone_media_thumb_destroy();
		frame_show_frame_index = 0x08;
		if ((monitor_door_first_valid_get(true) < 0) || (monitor_channel_get() == monitor_door_last_valid_get(true))) // 没有注册或者是已经显示完最后一个Door camera通道
		{
			frame_show_frame_index = 0x09;

			return frame_show_restart();
		}
		frame_show_door1_start();
	}
	else if ((user_data_get()->display.frame_list & 0x10) && (frame_show_frame_index <= 0x10))
	{
		// sat_linphone_media_thumb_destroy();
		frame_show_frame_index = 0x10;
		if ((monitor_door_first_valid_get(false) < 0) || (monitor_channel_get() == monitor_door_last_valid_get(false))) // 没有注册或者是已经显示完最后一个CCTV通道
		{
			frame_show_frame_index = 0x11;
			return frame_show_restart();
		}
		frame_show_cctv_start();
	}
	else
	{
		if ((user_data_get()->display.frame_list & 0X1F) == 00)
		{
			SAT_DEBUG("HHHHHHHHHHHHHHHHHHHH");
			sat_layout_goto(close, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
		}
		monitor_channel_set(MON_CH_NONE);
		frame_show_frame_index = 0x00;
		frame_show_restart();
	}
}

/***
** 日期: 2022-05-10 16:04
** 作者: leo.liu
** 函数作用：初始化帧页面
** 返回参数说明：
***/
static void frame_show_layer_init(void)
{
	frame_buffer_cur_a = lv_img_buf_alloc(THUMB_WIDTH, THUMB_HIGHT, LV_IMG_CF_TRUE_COLOR);
	if (frame_buffer_cur_a == NULL)
	{
		printf("[%s:%d] frame show frame buffer a is nullpter\n", __func__, __LINE__);
		return;
	}
	frame_buffer_cur_b = lv_img_buf_alloc(THUMB_WIDTH, THUMB_HIGHT, LV_IMG_CF_TRUE_COLOR);
	if (frame_buffer_cur_b == NULL)
	{
		printf("[%s:%d]frame show buffer b is nullper\n", __func__, __LINE__);
		return;
	}
	frame_buffer_cur_a->header.reserved = 0x03;
	frame_buffer_cur_b->header.reserved = 0x03;
	frame_show_frame_index = 0x00;
	frame_show_refresh_cont = 0;
	frame_show_pbuffer = NULL;
	pre_frame_diplay_light_value = -1;
	thumb_display_refresh_register(frame_show_thumb_refresh_display_callback);
}

extern bool layout_frame_show_ch_vaile_check(void);

static void frame_show_param_checktimer(lv_timer_t *ptimer)
{
	if (layout_frame_show_ch_vaile_check() == false)
	{
		SAT_DEBUG("HHHHHHHHHHHHHHHHHHHH");
		sat_layout_goto(close, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
	}
}
static void sat_layout_enter(frame_show)
{
	monitor_channel_set(MON_CH_NONE);
	{

		lv_obj_t *frame1 = lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_frame1_id, 0, 0, 1024, 600,
																								NULL, false, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x00,
																								0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																								0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																								NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
		lv_obj_clear_flag(frame1, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(frame1, LV_OBJ_FLAG_SCROLLABLE);
	}
	{

		lv_obj_t *frame2 = lv_common_img_btn_create(sat_cur_layout_screen_get(), frame_show_frame2_id, 1024, 0, 1024, 600,
																								NULL, false, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x00,
																								0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																								0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																								NULL, LV_OPA_TRANSP, 0x00a8ff, LV_ALIGN_CENTER);
		lv_obj_clear_flag(frame2, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(frame2, LV_OBJ_FLAG_SCROLLABLE);
	}

	frame_show_cancel_btn_display();
	frame_show_setting_btn_display();
	/***** frame 页面初始化 *****/
	frame_show_layer_init();

	standby_timer_close();

	frame_show_restart();
	if ((user_data_get()->system_mode & 0x0f) != 0x01)
	{
		lv_timer_reset(lv_sat_timer_create(frame_show_param_checktimer, 1000, NULL));
	}
}
static void sat_layout_quit(frame_show)
{
	standby_timer_restart(true, true);
	sat_linphone_media_thumb_destroy();
	thumb_display_refresh_register(NULL);
	sd_state_channge_callback_register(sd_state_change_default_callback);
	if (frame_buffer_cur_a)
	{
		lv_img_buf_free(frame_buffer_cur_a);
		frame_buffer_cur_a = NULL;
	}
	if (frame_buffer_cur_b)
	{
		lv_img_buf_free(frame_buffer_cur_b);
		frame_buffer_cur_b = NULL;
	}
	monitor_close(0x02);
	backlight_brightness_set(user_data_get()->display.lcd_brigtness <= 5 ? 5 : user_data_get()->display.lcd_brigtness);
	lv_img_cache_invalidate_all();
}

sat_layout_create(frame_show);
