
#include "layout_define.h"

enum
{
	setting_language_obj_id_title_label,

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-1-14 16:42:16
	** 说明: 返回按钮
	***********************************************/
	setting_language_obj_id_cancel_btn,

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-1-14 16:42:23
	** 说明: list控件
	***********************************************/
	setting_language_obj_id_language_list,

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-1-14 16:42:39
	** 说明:	列表子控件
	***********************************************/
	setting_language_obj_id_language_list_hanguo,
	setting_language_obj_id_language_list_hanguo_label,
	setting_language_obj_id_language_list_hanguo_img,
	setting_language_obj_id_language_list_zhongwen,
	setting_language_obj_id_language_list_zhongwen_label,
	setting_language_obj_id_language_list_zhongwen_img,
	setting_language_obj_id_language_list_yingyu,
	setting_language_obj_id_language_list_yingyu_label,
	setting_language_obj_id_language_list_yingyu_img,
	setting_language_obj_id_language_list_eluosi,
	setting_language_obj_id_language_list_eluosi_label,
	setting_language_obj_id_language_list_eluosi_img,
	setting_language_obj_id_language_list_xibanya,
	setting_language_obj_id_language_list_xibanya_label,
	setting_language_obj_id_language_list_xibanya_img,
	setting_language_obj_id_language_list_yuenan,
	setting_language_obj_id_language_list_yuenan_label,
	setting_language_obj_id_language_list_yuenan_img,
	setting_language_obj_id_language_list_alabo,
	setting_language_obj_id_language_list_alabo_label,
	setting_language_obj_id_language_list_alabo_img,
	setting_language_obj_id_language_list_bolan,
	setting_language_obj_id_language_list_bolan_label,
	setting_language_obj_id_language_list_bolan_img,
};

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-14 13:52:45
** 说明: 语言列表创建
***********************************************/
static int setting_language_list_id_get(LANGUAGE_ID lang, int type)
{
	int obj_id[LANGUAGE_ID_TOTAL][3] = {

			{setting_language_obj_id_language_list_hanguo, setting_language_obj_id_language_list_hanguo_img, setting_language_obj_id_language_list_hanguo_label},
			{
					setting_language_obj_id_language_list_zhongwen,
					setting_language_obj_id_language_list_zhongwen_img,
					setting_language_obj_id_language_list_zhongwen_label,
			},
			{
					setting_language_obj_id_language_list_yingyu,
					setting_language_obj_id_language_list_yingyu_img,
					setting_language_obj_id_language_list_yingyu_label,
			},
			{
					setting_language_obj_id_language_list_eluosi,
					setting_language_obj_id_language_list_eluosi_img,
					setting_language_obj_id_language_list_eluosi_label,
			},
			{
					setting_language_obj_id_language_list_xibanya,
					setting_language_obj_id_language_list_xibanya_img,
					setting_language_obj_id_language_list_xibanya_label,
			},
			{
					setting_language_obj_id_language_list_yuenan,
					setting_language_obj_id_language_list_yuenan_img,
					setting_language_obj_id_language_list_yuenan_label,
			},
			{
					setting_language_obj_id_language_list_alabo,
					setting_language_obj_id_language_list_alabo_label,
					setting_language_obj_id_language_list_alabo_img,
			},
			{
					setting_language_obj_id_language_list_bolan,
					setting_language_obj_id_language_list_bolan_label,
					setting_language_obj_id_language_list_bolan_img,
			},

	};

	return obj_id[lang][type];
}

static void setting_language_list_client(lv_event_t *ev)
{
	lv_obj_t *obj = ev->current_target;

	if (obj == NULL)
	{

		return;
	}

	int obj_id = obj->id;
	LANGUAGE_ID old_lang = language_id_get();

	for (int id = LANGUAGE_ID_ENGLISH; id < LANGUAGE_ID_TOTAL; id++)
	{

		if (obj_id == setting_language_list_id_get(id, 0))
		{

			if (old_lang != id)
			{

				lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_language_obj_id_language_list);

				lv_obj_t *item = lv_obj_get_child_form_id(list, setting_language_list_id_get(old_lang, 0));

				lv_obj_t *item_btn = lv_obj_get_child_form_id(item, setting_language_list_id_get(old_lang, 1));

				if (item_btn != NULL)
				{

					lv_obj_set_style_bg_img_src(item_btn, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
				}

				item_btn = lv_obj_get_child_form_id(obj, setting_language_list_id_get(id, 1));

				if (item_btn != NULL)
				{
					lv_obj_set_style_bg_img_src(item_btn, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
				}

				user_data_get()->etc.language = id;
				language_id_set(id);
				user_data_save(false, false);
			}

			return;
		}
	}
}

static lv_obj_t *setting_language_list_create(void)
{

	lv_obj_t *list = lv_obj_get_child_form_id(sat_cur_layout_screen_get(), setting_language_obj_id_language_list);

	if (list != NULL)
	{

		return list;
	}

	list = lv_list_create(sat_cur_layout_screen_get());
	lv_common_style_set_common(list, setting_language_obj_id_language_list, 48, 80, 928, 462, LV_ALIGN_TOP_LEFT, LV_PART_MAIN);

	// lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ELASTIC);

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-1-31 10:35:38
	** 说明: 设置列表属性
	***********************************************/
	{
		lv_obj_t *item = NULL;

		lv_obj_t *item_btn = NULL;
		LANGUAGE_ID old_lang = language_id_get();
		int item_id, img_id, label_id;

		for (LANGUAGE_ID lang = LANGUAGE_ID_ENGLISH; lang < LANGUAGE_ID_TOTAL; lang++)
		{

			language_id_set(lang);

			item_id = setting_language_list_id_get(lang, 0);
			img_id = setting_language_list_id_get(lang, 1);
			label_id = setting_language_list_id_get(lang, 2);
			item = lv_common_setting_btn_title_sub_info_img_create(list, item_id, 0, 0, 920, 66,
																														 setting_language_list_client, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
																														 0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x323237,
																														 0, 1, LV_BORDER_SIDE_BOTTOM, LV_OPA_COVER, 0x00a8ff,
																														 52, 0, 160, 30, label_id,
																														 lang_str_get(XLS_LANG_ID_LANGUAGE_TYPE), 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
																														 0, 0, 0, 0, -1,
																														 NULL, 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
																														 0, 0, 0, 0, -1,
																														 NULL, 0xFFFFFF, 0x00a8ff, LV_TEXT_ALIGN_LEFT, lv_font_normal,
																														 0, 0, 32, 32, img_id,
																														 resource_ui_src_get("btn_radio_n.png"), LV_OPA_TRANSP, 0, LV_ALIGN_CENTER);

			lv_obj_set_style_pad_top(item, 15, LV_PART_MAIN);
			lv_obj_set_style_pad_column(item, 10, LV_PART_MAIN);

			item_btn = lv_obj_get_child_form_id(item, img_id);

			if (lang == old_lang)
			{
				lv_obj_set_style_bg_img_src(item_btn, resource_ui_src_get("btn_radio_s.png"), LV_PART_MAIN);
			}
			else
			{
				lv_obj_set_style_bg_img_src(item_btn, resource_ui_src_get("btn_radio_n.png"), LV_PART_MAIN);
			}
		}

		language_id_set(old_lang);
	}

	return list;
}

static void setting_language_cancel_btn_client(lv_event_t *ev)
{

	if (user_data_get()->is_device_init == false)
	{

		sat_layout_goto(power_setting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
	}
	else
	{

		sat_layout_goto(setting_general, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SAT_VOID);
	}
}

static void sat_layout_enter(setting_language)
{
	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-2 13:46:56
	** 说明: 标题显示
	***********************************************/
	{
		lv_common_text_create(sat_cur_layout_screen_get(), setting_language_obj_id_title_label, 0, 20, 1024, 40,
													NULL, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
													lang_str_get(SETTING_GENERAL_XLS_LANG_ID_LANG), 0XFFFFFFFF, 0xFFFFFF, LV_TEXT_ALIGN_CENTER, lv_font_large);
	}

	/***********************************************
	** 作者: leo.liu
	** 日期: 2023-2-3 14:13:25
	** 说明: 返回按钮
	***********************************************/
	{
		lv_common_img_btn_create(sat_cur_layout_screen_get(), setting_language_obj_id_cancel_btn, 35, 15, 48, 48,
														 setting_language_cancel_btn_client, true, LV_OPA_TRANSP, 0, LV_OPA_TRANSP, 0x808080,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 0, 0, LV_BORDER_SIDE_NONE, LV_OPA_TRANSP, 0,
														 resource_ui_src_get("btn_title_back.png"), LV_OPA_COVER, 0x00a8ff, LV_ALIGN_CENTER);
	}
	setting_language_list_create();
}

static void sat_layout_quit(setting_language)
{
}

sat_layout_create(setting_language);
