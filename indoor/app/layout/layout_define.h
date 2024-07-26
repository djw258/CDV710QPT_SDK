#ifndef _LAYOUT_DEFIEN_H_
#define _LAYOUT_DEFIEN_H_
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../share/lvgl-8.3.1/lvgl.h"
#include "common/sat_main_event.h"
#include "common/sat_user_file.h"
#include "common/sat_linphone_event.h"
#include "common/sat_ipcamera.h"
#include "common/user_record.h"
#include "common/user_data.h"
#include "common/language.h"
#include "common/user_monitor.h"
#include "common/sat_user_time.h"
#include "common/user_network.h"
#include "common/lv_common.h"
#include "common/sat_wifi_api.h"
#include "common/sat_user_common.h"
#include "common/user_linphone.h"
#include "common/sat_user_file.h"
#include "common/ring_common.h"
#include "common/user_gpio.h"
#include "common/pwm_ctrl.h"
#include "common/layout_common.h"
#include "common/tuya_common.h"
#include "common/user_standby.h"
#include "common/user_alarm_list.h"
#include "common/user_call_list.h"
#include "common/layout_mac_common.h"
#define SYSTEM_VERSION "Ver:" VERSION_NO
#define VERSION_NO "1.4.0"          // 此版本实际为1.2.2，客户要求和生产完成的产品版本号一致
#define TUYA_PID "veyd1kjvzmot7iks" //"nhi8jlgj989p9l5b" // 7.0 "wrw4vog3vosbq4yd" // 10.1  "iet0idk0yw3fgren"

#define ALM_HIGHT 1.8
#define ALM_LOW 1.1
/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 获取ui资源的路径
 * @return:
 */
void *resource_ui_src_get(const char *file);

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 获取背景图片P
 * @return:
 */
void *resource_wallpaper_src_get(char *file, uint32_t w, uint32_t h);

/*
 * @日期: 2022-08-12
 * @作者: leo.liu
 * @功能: 文件路径获取
 * @return:
 */
void *file_path_src_get(char *path, const char *file, uint32_t w, uint32_t h);

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 创建一个ui资源文件
 * @return:
 */
void *resource_ui_src_alloc(char *file, uint32_t w, uint32_t h);

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 创建一个壁纸资源文件
 * @return:
 */
void *resource_wallpaper_src_alloc(char *file, uint32_t w, uint32_t h);

/*
 * @日期: 2022-08-11
 * @作者: leo.liu
 * @功能: 释放文件
 * @return:
 */
bool resouce_file_src_free(void *pstr);

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:58
** 说明:
***********************************************/
void lv_layout_touch_callback(lv_event_t *e);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: 获取编译日期
***********************************************/
bool platform_build_date_get(struct tm *tm);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-26 16:48:12
** 说明: sip账号获取其IP和房号
***********************************************/
bool sip_user_get_number_and_ip(const char *user, char *ip, char *number);
/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @注释: 定义layout
 */
sat_layout_define(logo);    // 语言已完成
sat_layout_define(monitor); // 语言已完成
sat_layout_define(home);
sat_layout_define(power_setting);
sat_layout_define(setting_language);
sat_layout_define(single_operation_network);
sat_layout_define(server_operation_network);
sat_layout_define(operating_structure);
sat_layout_define(slave_type_setting);
sat_layout_define(setting_wifi);

sat_layout_define(wifi_info);
sat_layout_define(wifi_input);
sat_layout_define(setting_time);
sat_layout_define(ipc_camera_register);
sat_layout_define(ipc_camera_search);
sat_layout_define(ipc_camera_input);
sat_layout_define(ipc_camera_edit);
sat_layout_define(ipc_camera_display);

sat_layout_define(setting_complete);
sat_layout_define(setting_general);
sat_layout_define(setting_user_wifi);
sat_layout_define(setting_sound);
sat_layout_define(setting_screen);
sat_layout_define(setting_recording);
sat_layout_define(setting_installation);
sat_layout_define(setting_password);
sat_layout_define(setting_storage_space);
sat_layout_define(setting_sensor_usage);
sat_layout_define(setting_sensor_switch);
sat_layout_define(setting_download);
sat_layout_define(setting_initialize);
sat_layout_define(setting_version_information);
sat_layout_define(setting_volume);
sat_layout_define(setting_standby_screen);
sat_layout_define(setting_frame_display_time);
sat_layout_define(setting_frame_night_mode);
sat_layout_define(setting_frame_item);
sat_layout_define(setting_frame_backgorund);
sat_layout_define(setting_screen_clean);
sat_layout_define(setting_motion);
sat_layout_define(setting_motion_timer);
sat_layout_define(setting_master_indoor_unit_ip);
sat_layout_define(setting_building_house_number);
sat_layout_define(setting_ipaddress);
sat_layout_define(setting_server_ipaddress);
sat_layout_define(setting_common_entrance_ip);
sat_layout_define(setting_guard_station_number);
sat_layout_define(setting_sensor_settings);
// sat_layout_define(door_camera_password_input);
// sat_layout_define(door_camera_edit);
sat_layout_define(playback);
sat_layout_define(photo);
sat_layout_define(video);
sat_layout_define(intercom_call);
sat_layout_define(intercom_talk);
sat_layout_define(emergency_setting);
sat_layout_define(alarm);
sat_layout_define(alarm_list);
sat_layout_define(security);        // 语言已完成,有缺漏
sat_layout_define(away);            // 语言已完成,有缺漏
sat_layout_define(away_count);      // 语言已完成,有缺漏
sat_layout_define(sensor_settings); // 语言已完成,有缺漏
sat_layout_define(close);           // 语言已完成
sat_layout_define(frame_show);      // 语言已完成
sat_layout_define(always_record);   // 语言已完成
sat_layout_define(sensors_test);
sat_layout_define(buzzer_call);
sat_layout_define(factory_reset);

sat_layout_define(indoor_register);
sat_layout_define(setting_app_integration);
sat_layout_define(outdoor_mac_register);
sat_layout_define(wifi_test);
sat_layout_define(setting_open_duration_time);
#endif
