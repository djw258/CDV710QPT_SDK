
#include "layout_define.h"
enum
{

	power_setting_obj_id_power_setting_label,
	power_setting_obj_id_welcom_label,
	power_setting_obj_id_languge_btn,
	power_setting_obj_id_languge_label,
	power_setting_obj_id_languge_icon,
	power_setting_obj_id_right_btn,
	power_setting_obj_id_run_btn,
	power_setting_obj_id_skip_btn,
	power_setting_obj_id_msgbox_bg,
};

static void power_setting_language_btn_client(lv_event_t *ev)
{
	sat_layout_goto(setting_language, LV_SCR_LOAD_ANIM_FADE_OUT, SAT_VOID);
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-13 15:23:33
** 说明: 运行按钮
***********************************************/
static void power_setting_run_btn_client(lv_event_t *ev)
{
	if (0 /*(user_data_get()->system_mode & 0xF0) == 0x00*/)
	{
		sat_layout_goto(single_operation_network, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
	}
	else
	{
		sat_layout_goto(server_operation_network, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
	}
}

static int convert_a_string_to_an_integer_number(const char *str, int len)
{
	int val = 0;
	for (int i = 0; i < len; i++)
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
		{
			val = val * 16 + (str[i] - '0');
		}
		else if ((str[i] >= 'A') && (str[i] <= 'F'))
		{
			val = val * 16 + ((str[i] - 'A') + 10);
		}
		else if ((str[i] >= 'a') && (str[i] <= 'f'))
		{
			val = val * 16 + ((str[i] - 'a') + 10);
		}
	}
	return val;
}

static bool mac_mapping_ip_get(char *ip)
{
	char mac[32] = {0};

	sat_ip_mac_addres_get("eth0", NULL, mac, NULL);
	/*获取mac的xx:xx:xx:xx:xx:xx 低字节*/
	char *str = mac;
	for (int i = 0; i < 3; i++)
	{
		str = strchr(str, ':');
		if (str == NULL)
		{
			SAT_DEBUG("char *str = strchar(mac, ':');%d", i);
			return false;
		}
		str++;
	}
	if (strlen(str) != 8)
	{
		SAT_DEBUG("char *str = %s", str);
		return false;
	}

	int ip_part[3] = {0};
	ip_part[0] = convert_a_string_to_an_integer_number(str, 2);
	ip_part[1] = convert_a_string_to_an_integer_number(str + 3, 2);
	ip_part[2] = convert_a_string_to_an_integer_number(str + 6, 2);

	sprintf(ip, "%d.%d.%d.%d", 10, ip_part[0], ip_part[1], ip_part[2]);
	return true;
}

static void layout_power_setting_confirm(lv_event_t *ev)
{
	user_data_get()->is_device_init = true;
	user_data_save(false, false);
	system("reboot");
}
static void layout_power_setting_concel(lv_event_t *ev)
{
	setting_msgdialog_msg_del(power_setting_obj_id_msgbox_bg);
}

static bool layout_power_setting_default_data_changge_check(void)
{
	// 获取MAC映射的ip
	char ip[32] = {0};
	mac_mapping_ip_get(ip);

	user_network_info default_network_data = {0};
	memcpy(&default_network_data, network_defauld_data_get(), sizeof(user_network_info));
	strcpy(default_network_data.network.ipaddr, ip);
	user_data_info default_user_data = {0};
	memcpy(&default_user_data, user_default_data_get(), sizeof(user_data_info));
	memcpy(default_user_data.mastar_wallpad_ip, user_default_data_get()->mastar_wallpad_ip, sizeof(user_default_data_get()->mastar_wallpad_ip));
	default_user_data.sync_timestamp = user_data_get()->sync_timestamp;
	memcpy(default_user_data.compile_time, user_data_get()->compile_time, sizeof(user_data_get()->compile_time));
	memcpy(default_user_data.alarm.alarm_gpio_value_group, user_data_get()->alarm.alarm_gpio_value_group, sizeof(user_data_get()->alarm.alarm_gpio_value_group));

	if ((memcmp(network_data_get(), &default_network_data, sizeof(user_network_info))) || (memcmp(user_data_get(), &default_user_data, sizeof(user_data_info))))
	{
		return true;
	}
	return false;
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-13 15:23:55
** 说明: 跳过按钮
***********************************************/
static void power_setting_skip_btn_client(lv_event_t *ev)
{

	// system("rm -rf /app/data/user_data.cfg");
	// system("rm -rf /app/data/network_data.cfg");
	// user_data_init();
	// network_data_init();

	if (layout_power_setting_default_data_changge_check())
	{
		lv_obj_t *masgbox = setting_msgdialog_msg_bg_create(power_setting_obj_id_msgbox_bg, 0, 282, 143, 460, 283);
		setting_msgdialog_msg_create(masgbox, 0, lang_str_get(POWER_SETTING_XLS_LANG_ID_WILL_REBOOT), 20, 80, 420, 120, false);
		setting_msgdialog_msg_confirm_and_cancel_btn_create(masgbox, 1, 2, layout_power_setting_confirm, layout_power_setting_concel);
	}
	else
	{
		user_data_get()->is_device_init = true;
		user_data_save(false, false);
		sat_layout_goto(home, LV_SCR_LOAD_ANIM_FADE_IN, SAT_VOID);
	}
}

static void sat_layout_enter(power_setting)
{
	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-2 13:46:56
	** 说明: 标题显示
	***********************************************/
	{
		lv_common_text_create(sat_cur_layout_screen_get(), power_setting_obj_id_power_setting_label, 0, 20, 1024, 40,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													lang_str_get(POWER_SETTING_XLS_LANG_ID_POWER_SET), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-2 13:51:30
	** 说明: 欢迎开始使用
	***********************************************/
	{
		lv_common_text_create(sat_cur_layout_screen_get(), power_setting_obj_id_welcom_label, 53, 164, 918, 100,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													lang_str_get(POWER_SETTING_XLS_LANG_ID_WELCOM), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);
	}

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-2 14:16:18
	** 说明: 语言设置
	***********************************************/
	{
		lv_common_img_text_btn_create(sat_cur_layout_screen_get(), power_setting_obj_id_languge_btn, 318, 302, 388, 72,
																	power_setting_language_btn_client, LV_OPA_TRANSP, 0x00, LV_OPA_TRANSP, 0x101010,
																	0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																	0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																	0, 19, 207, 33, power_setting_obj_id_languge_label,
																	lang_str_get(XLS_LANG_ID_LANGUAGE_TYPE), 0xffffff, 0x00a8ff, LV_TEXT_ALIGN_RIGHT, lv_font_normal,
																	208, 12, 48, 48, power_setting_obj_id_languge_icon,
																	(const char *)resource_ui_src_get("btn_list_right.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-2 14:42:13
	** 说明: 运行和跳过
	***********************************************/
	{
		lv_obj_t *obj = lv_common_text_create(sat_cur_layout_screen_get(), power_setting_obj_id_run_btn, 165, 450, 311, 72,
																					power_setting_run_btn_client, LV_OPA_COVER, 0x00a8ff, LV_OPA_COVER, 0x808080,
																					60, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																					60, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																					lang_str_get(POWER_SETTING_XLS_LANG_ID_RUN), 0XFFFFFFFF, 0xFFFFff, LV_TEXT_ALIGN_CENTER, lv_font_normal);

		lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN);

		obj = lv_common_text_create(sat_cur_layout_screen_get(), power_setting_obj_id_skip_btn, 548, 450, 311, 72,
																power_setting_skip_btn_client, LV_OPA_COVER, 0x47494a, LV_OPA_COVER, 0x808080,
																60, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																60, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
																lang_str_get(POWER_SETTING_XLS_LANG_ID_SKIP), 0XFFFFFFFF, 0XFFFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_normal);

		lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN);
	}
}

static void sat_layout_quit(power_setting)
{
}

sat_layout_create(power_setting);
