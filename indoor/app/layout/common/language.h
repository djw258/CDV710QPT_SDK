#ifndef _LANGUAGE_kdfjlskd_H_
#define _LANGUAGE_kdfjlskd_H_

#include <stdint.h>
//#include "XLS_LANG_xls.h"

typedef enum
{
	LANGUAGE_ID_HANYU = 0,
	LANGUAGE_ID_CHINESE,
	LANGUAGE_ID_ENGLISH,
	LANGUAGE_ID_ELUOSI,
	LANGUAGE_ID_XIBANYA,
	LANGUAGE_ID_YUENAN,
	LANGUAGE_ID_ALABOYU,

    
    LANGUAGE_ID_TOTAL,
} LANGUAGE_ID;

/***
** 日期: 2022-04-26 08:55
** 作者: leo.liu
** 函数作用：设置当前语言
** 返回参数说明：
***/
void language_id_set(LANGUAGE_ID id);

/***
** 日期: 2022-04-26 08:56
** 作者: leo.liu
** 函数作用：获取当前语言ID
** 返回参数说明：
***/
LANGUAGE_ID language_id_get(void);
typedef enum LANGUAGE_STRING_TYPE_ID
{
    #if 1
    XLS_LANG_ID_HEAD_INDEX,
    XLS_LANG_ID_LANGUAGE_TYPE,
    // /*******  layout_power_setting  *******/
    POWER_SETTING_XLS_LANG_ID_SKIP,
    POWER_SETTING_XLS_LANG_ID_WELCOM,
    POWER_SETTING_XLS_LANG_ID_RUN,
    POWER_SETTING_XLS_LANG_ID_POWER_SET,
    /*******  layout_setting_complete *******/
    SETTING_COMPLETE_XLS_LANG_ID_START_SETTING_IS_COMPLETE,
    SETTING_COMPLETE_XLS_LANG_ID_CONFIRM,
    SETTING_COMPLETE_XLS_LANG_ID_SETTING_COMPLETE,


    /*******  layout_logo  *******/
    LAYOUT_LOGO_XLS_LANG_ID_MODEL,
    LAYOUT_LOGO_XLS_LANG_ID_VERSION,

    /*******  layout_setting_language *******/
    XLS_LANG_ID_LANG,

 /*******  layout_operating_structure *******/
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_STRUCTURE,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE_SETTING_INFO,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE_SETTING_INFO_MSTAR,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE_SETTING_ENABLE,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SLAVE_SETTING,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SERVER_SYSTEM,
    SIGNLE_OPERATION_STRUCTURE_XLS_LANG_ID_SINGLE,

    /*******  layout_single_operation_network  *******/
    SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_OPERATION_NETWORK, 
    SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_OPERATION_STRCUUTRE,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SINGLE,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SERVER_SYSTEM,
    SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_SLAVE,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_GATEWAY,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_IP,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_MASK,
	SIGNLE_OPERATION_NETWORK_XLS_LANG_ID_DNS,

    /*******  layout_server_operating_network  *******/
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_SIP_SERVER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_HOUSEHOLD_NUMBER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_LOCAL_SERVER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_SERVER_SYSTEM,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_GUARD_STATION_NUMBER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_NUMBER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_UPDATE_SERVER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_CCTV_SERVER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_BUILD_HOUSE_NUMBER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_OPERATING_SERVER,
    SERVER_OPERATION_NETWORK_XLS_LANG_ID_GUARD_STATION,

    /*******  layout_slave_type_setting  *******/
    LAYOUT_SLAVE_TYPE_SETTING_XLS_LANG_ID_SLAVE_TYPE_SETTING,
    LAYOUT_SLAVE_TYPE_SETTING_XLS_LANG_ID_NOT_SET,
    LAYOUT_SLAVE_TYPE_SETTING_XLS_LANG_ID_MASTTER_INDOOR,
    LAYOUT_SLAVE_TYPE_SETTING_XLS_LANG_ID_EXTENSION_NUMBER,


    /*******  layout_setting_wifi *******/
    WIFI_SETTING_XLS_LANG_ID_DISCOVERED_NETWORKS,
    WIFI_SETTING_XLS_LANG_ID_WIFI_WIRELESS_NETWORK, 
    WIFI_SETTING_XLS_LANG_ID_WIFI,
    WIFI_SETTING_XLS_LANG_ID_ADD_NETWORKS,

 /*******  layout_wifi_info *******/
	WIFI_INFO_XLS_LANG_ID_SECURITY,
    WIFI_INFO_XLS_LANG_ID_IP_ADDRES,

 /*******  layout_wifi_input *******/
    WIFI_INPUT_XLS_LANG_ID_INPUT_USER,
 	WIFI_INPUT_XLS_LANG_ID_INPUT_PASSWORD,
	WIFI_INPUT_XLS_LANG_ID_CONNECTING,
	WIFI_INPUT_XLS_LANG_ID_CONNECT_FAILED,



 /*******  layout_setting_time *******/
    SETTING_TIME_XLS_LANG_ID_THE_TIME_IS_SET_AUTOMATICALLY,
    SETTING_TIME_XLS_LANG_ID_SET_DATE_AND_TIME,
    SETTING_TIME_XLS_LANG_ID_DATE_TIME,

 /*******  layout_ipc_camera *******/
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DOOR_CAMERA_SEARCH,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_IF_YOUT_PRESS,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_SEARCHED_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTERED_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_SEARCHED_CCTV,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTER_CCTV,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTER_FAILDED,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_REGISTRATION,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_IF_YOUT_CANNOT_SEE,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_IF_YOUT_CANNOT_REGISTER,



    DOOR_CAMERA_SEARCH_XLS_LANG_ID_OPERATION_SERVER,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_UPDATE_SERVER_IP,


    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DHCP,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_STATIC,


    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DEFAULT_PASSWORD_IS_SET,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_PASSWORD,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_ENTER_AT_LEAST_9_DIGITS_OF_NUMBER,

    DOOR_CAMERA_SEARCH_XLS_LANG_ID_SET_THE_PASSWORD_TO_A_MIXTURE,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_CONFIRM_CHANGE_PASSWORD,
    DOOR_CAMERA_XLS_LANG_ID_PASSWORD_NOT_MATCH,


    DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_DOOR_CAMERA_INFOMATION,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_NAME,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_CHANGE_TH_CONNETION_PASSWORD,


/*******  layout_home *******/
    HOME_XLS_LANG_ID_JAN,
    HOME_XLS_LANG_ID_FEB,
    HOME_XLS_LANG_ID_MAR,
    HOME_XLS_LANG_ID_APRIAL,
    HOME_XLS_LANG_ID_MAY,
    HOME_XLS_LANG_ID_JUN,
    HOME_XLS_LANG_ID_JULY,
    HOME_XLS_LANG_ID_AUG,
    HOME_XLS_LANG_ID_SEP,
    HOME_XLS_LANG_ID_OCT,
    HOME_XLS_LANG_ID_NOV,
    HOME_XLS_LANG_ID_DEC,

    HOME_XLS_LANG_ID_MON,
    HOME_XLS_LANG_ID_TUE,
    HOME_XLS_LANG_ID_WED,
    HOME_XLS_LANG_ID_THU,
    HOME_XLS_LANG_ID_FRI,
    HOME_XLS_LANG_ID_SAT,
    HOME_XLS_LANG_ID_SUN,

    HOME_XLS_LANG_ID_MONITORING,
    HOME_XLS_LANG_ID_CALL,
    HOME_XLS_LANG_ID_CCTV,
    HOME_XLS_LANG_ID_AWAY,
    HOME_XLS_LANG_ID_BURGLAR,
    HOME_XLS_LANG_ID_ELEVATOR,
    HOME_XLS_LANG_ID_EMERGENCY,
    HOME_XLS_LANG_ID_RECENT_CALL,
    HOME_XLS_LANG_ID_RECENT_VIDEO, 

    HOME_XLS_LANG_ID_USE_MOBILE_APP,
    HOME_XLS_LANG_ID_COMMON_ENTRANCE,
    HOME_XLS_LANG_ID_DOORCAMERA,

    HOME_XLS_LANG_ID_THE_COMMUNICATION_LINE,


    /*******  layout_intercom_call*******/
    INTERCOM_XLS_LANG_ID_CAll_CALL,
    INTERCOM_XLS_LANG_ID_CAll_CHANNEL,
    INTERCOM_XLS_LANG_ID_CAll_LOG,
    INTERCOM_XLS_LANG_ID_CAll_LOG_SELECTED,
    INTERCOM_XLS_LANG_ID_EXTENSION,
    INTERCOM_XLS_LANG_ID_GUARD,


    /*******  layout_away*******/
    LAYOUT_AWAY_XLS_LANG_ID_AWAY,
    LAYOUT_AWAY_XLS_LANG_ID_RUNNING,

    LAYOUT_AWAY_XLS_LANG_ID_DETECTS_INSTRUDERS,
    LAYOUT_AWAY_XLS_LANG_ID_OUT_GOING,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE,

    LAYOUT_AWAY_XLS_LANG_ID_FUNCRION_SETTING_TIME,
    LAYOUT_AWAY_XLS_LANG_ID_FUNCRION_RELEASE_TIME,

    LAYOUT_AWAY_XLS_LANG_ID_SAVE_VISITORS_PHORO,
    LAYOUT_AWAY_XLS_LANG_ID_BYPASS_CALL,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_0,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_10,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_20,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_30,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_40,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_50,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_60,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_70,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_80,
    LAYOUT_AWAY_XLS_LANG_ID_RELEASE_TIME_SECOND_90,
    LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_1_MINUTE,
    LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_2_MINUTE,
    LAYOUT_AWAY_XLS_LANG_ID_SETTING_TIME_3_MINUTE,

    /*******  layout_alarm_security*******/
    LAYOUT_SECURITY_XLS_LANG_ID_HEAD,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR1,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR2,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR3,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR4,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR5,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR6,
    LAYOUT_SECURITY_XLS_LANG_ID_SENSOR7,
    LAYOUT_SECURITY_XLS_LANG_ID_TIP,
    LAYOUT_SECURITY_XLS_LANG_ID_AUTO_RECORD,
    LAYOUT_SECURITY_XLS_LANG_ID_END,
    LAYOUT_SECURITY_XLS_LANG_ID_EXECUTION,
    LAYOUT_SECURITY_XLS_LANG_ID_RUN,
    LAYOUT_SECURITY_XLS_LANG_ID_ENTER_PASSSWORD,

    /*******  layout_alarm*******/
    LAYOUT_ALARM_XLS_LANG_ID_STOP,
    LAYOUT_ALARM_XLS_LANG_ID_RETURN,
    LAYOUT_ALARM_XLS_LANG_ID_SECURITY_EMERGENCY,
    LAYOUT_ALARM_XLS_LANG_ID_EMERHENCY,
    LAYOUT_ALARM_XLS_LANG_ID_HOUSEHOLD_OPERATION,

    LAYOUT_ALARM_XLS_LANG_ID_SENSOR1,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR2,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR3,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR4,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR5,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR6,
    LAYOUT_ALARM_XLS_LANG_ID_SENSOR7,

    /*******  layout_setting_emergency*******/
    LAYOUT_EMERGENCY_XLS_LANG_ID_CONFIRM,
    LAYOUT_EMERGENCY_XLS_LANG_ID_ACTIVE_EMERGENCY,

    /*******  LAYOUT_ALARM_LIST*******/
    LAYOUT_ALARM_LIST_XLS_LANG_ID_DELETED_ALL,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_LOG,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_OCCURS,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_STOP,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_EMERGENCY_RETURN,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY_STOP,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_EMERGENCY_RETURN,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_INDOOR_OPERATION,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_USER_OPERATION,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR1,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR2,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR3,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR4,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR5,
    LAYOUT_ALARM_LIST_XLS_LANG_ID_SECURITY_SENSOR6,

    /*******  layout_intercom_out/talk*******/
    INTERCOM_XLS_LANG_ID_OUTCOMING_CALL,
    INTERCOM_XLS_LANG_ID_INCOMMING_CALL,
    INTERCOM_XLS_LANG_ID_BUZZER_CALL,
    INTERCOM_XLS_LANG_ID_CONNECTED_CALL,
    INTERCOM_XLS_LANG_ID_DOOR_CAMERA,
    // INTERCOM_XLS_LANG_ID_COMMON_TRANCE, 
    // INTERCOM_XLS_LANG_ID_GUARD_STATION, 

    /*******  layout_intercom_log_del *******/
    LAYOUT_CALL_LOG_XLS_LANG_ID_DEL,
    LAYOUT_CALL_LOG_XLS_LANG_ID_DEL_TIPS,


    /*******  layout_playback *******/
    PLAYBACK_XLS_LANG_ID_PLAYBACK,

    /*******  layout_setting_general*******/
    SETTING_GENERAL_XLS_LANG_ID_SETTINGS,
    SETTING_GENERAL_XLS_LANG_ID_GENERAL,
    SETTING_GENERAL_XLS_LANG_ID_WIFI,
    SETTING_GENERAL_XLS_LANG_ID_SOUND,
    SETTING_GENERAL_XLS_LANG_ID_SCREEN,
    SETTING_GENERAL_XLS_LANG_ID_RECORD_MONITORING,
    SETTING_GENERAL_XLS_LANG_ID_INSTALLATION_CONFIGURATION,
    SETTING_GENERAL_XLS_LANG_ID_DATA_AND_TIME,
    SETTING_GENERAL_XLS_LANG_ID_AUTO_SETUP,
    SETTING_GENERAL_XLS_LANG_ID_LANG,
    SETTING_GENERAL_XLS_LANG_ID_CHANNGE_PASSWORD_FOR_SECURITY,
    SETTING_GENERAL_XLS_LANG_ID_PASSWORD,
    SETTING_GENERAL_XLS_LANG_ID_STORAGE_SPACE,
    /*******  layout_setting_storage_space*******/
    SETTING_STORAGE_XLS_LANG_ID_SD_FORMAT,
    SETTING_STORAGE_XLS_LANG_ID_REMAINING_DATA,
    SETTING_STORAGE_XLS_LANG_ID_INTERNAL_STORAGE,
    SETTING_STORAGE_XLS_LANG_ID_EXTERNAL_STORAGE,
    SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_FORMAT,
    SETTING_STORAGE_XLS_LANG_ID_WOULD_YOU_LIKE_DEL,

    SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENING_METHOD,
    SETTING_GENERAL_XLS_LANG_ID_OPEN_WHEN_CALL,
    SETTING_GENERAL_XLS_LANG_ID_OPEN_AFTER_CALL,
    SETTING_GENERAL_XLS_LANG_ID_DOOR_OPENER_MODULE,
    SETTING_GENERAL_XLS_LANG_ID_DOOR2_OPEN_NUM,
    SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_NUM1,
    SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_NUM2,
    SETTING_GENERAL_XLS_LANG_ID_DOOR_OPEN_NUM,
    SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_LOCK,
    SETTING_GENERAL_XLS_LANG_ID_DIGITAL_DOOR_CAMERA,
    SETTING_GENERAL_XLS_LANG_ID_CALL_TIMER,
    SETTING_GENERAL_XLS_LANG_ID_1_MINUTE,
    SETTING_GENERAL_XLS_LANG_ID_3_MINUTE,
    SETTING_GENERAL_XLS_LANG_ID_5_MINUTE,

    SETTING_GENERAL_XLS_LANG_ID_SENSOR_USAGE_SETTING,
    SETTING_GENERAL_XLS_LANG_ID_DOWNLOAD_MOBILE_APP,
    /*******  layout_setting_download*******/
    SETTING_DOWNLOAD_XLS_LANG_ID_GOOGLE_PLAY_STORE,
    SETTING_DOWNLOAD_XLS_LANG_ID_GOOGLE_APP_STORE,
    SETTING_DOWNLOAD_XLS_LANG_ID_VIDEO_CALL_WITH_MOBILE_APP,




    SETTING_GENERAL_XLS_LANG_ID_INITIALIZATION_USER_DATA,
    /*******  layout_setting_initialize*******/
    SETTING_INITIALIZE_XLS_LANG_ID_INIT_DATA,
    SETTING_INITIALIZE_XLS_LANG_ID_DATA_CANNOT_BE,
    SETTING_INITIALIZE_XLS_LANG_ID_INITIALZATION_LIST,
    SETTING_INITIALIZE_XLS_LANG_ID_INITIALZATION_IS_COMPLETE,
    SETTING_INITIALIZE_XLS_LANG_ID_CALL_LOG,
    SETTING_INITIALIZE_XLS_LANG_ID_SAVED_VIDEO,
    SETTING_INITIALIZE_XLS_LANG_ID_EMERGENCY_RECORD,
    SETTING_INITIALIZE_XLS_LANG_ID_COMPLETE,
    SETTING_INITIALIZE_XLS_LANG_ID_RESET,
    SETTING_INITIALIZE_XLS_LANG_ID_FAILED,


    SETTING_GENERAL_XLS_LANG_ID_VERSION_INFORMATION,
    /*******  layout_setting_version_information*******/
    SETTING_VERSION_INFORMATION_XLS_LANG_ID_CURRENT_VERSION,
    SETTING_VERSION_INFORMATION_XLS_LANG_ID_UPDATE_VERSION,
    SETTING_VERSION_INFORMATION_XLS_LANG_ID_UPDATE,
    SETTING_VERSION_INFORMATION_XLS_LANG_ID_UPDATE_ING,



    /*******  layout_setting_sound*******/
    SOUND_XLS_LANG_ID_VOLUME,
    SOUND_XLS_LANG_ID_THE_VOLUME_OF_THE_CALLING,
    SOUND_XLS_LANG_ID_RINGTONE1,
    SOUND_XLS_LANG_ID_RINGTONE2,
    SOUND_XLS_LANG_ID_RINGTONE3,
    SOUND_XLS_LANG_ID_RINGTONE4,
    SOUND_XLS_LANG_ID_RINGTONE5,
    SOUND_XLS_LANG_ID_RINGTONE6,
    SOUND_XLS_LANG_ID_BUZZER,
    SOUND_XLS_LANG_ID_FRONT_DOOR,
    SOUND_XLS_LANG_ID_ENTRANCE,
    SOUND_XLS_LANG_ID_COMMON_ENTRANCE,
    SOUND_XLS_LANG_ID_SECURITY_OFFICE,
    SOUND_XLS_LANG_ID_EXTENSION,
    SOUND_XLS_LANG_ID_GUARD_STATION,
    SOUND_XLS_LANG_ID_TOUCH_NOTIFICATION,


    /*******  layout_setting_screen*******/
    SCREEN_XLS_LANG_ID_LCD_SCREEN_BRIGHNESS,
    SCREEN_XLS_LANG_ID_LCD_STANDBY_SCREEN,
    SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_15SEC,
    SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_30SEC,
    SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_60SEC,
    SCREEN_XLS_LANG_ID_LCD_SCREEN_AFTER_180SEC,
    SCREEN_XLS_LANG_ID_LCD_SCREEN_CLEANING_MODE,

    /*******  layout_setting_recording*******/
    RECORDING_XLS_LANG_ID_AUTO_RECORIDNG_CALLS,
    RECORDING_XLS_LANG_ID_SAVE_VIDEO,
    RECORDING_XLS_LANG_ID_SAVE_PICTURE,
    RECORDING_XLS_LANG_ID_SAVE_OFF,
    RECORDING_XLS_LANG_ID_SAVE_ON,
    RECORDING_XLS_LANG_ID_MOTION_DETECTION,
    RECORDING_XLS_LANG_ID_ALWAYS_MONITORING,
    RECORDING_XLS_LANG_ID_10_SEC_PER_CAMERA,
    RECORDING_XLS_LANG_ID_30_SEC_PER_CAMERA,
    RECORDING_XLS_LANG_ID_60_SEC_PER_CAMERA,

    /*******  layout_setting_installation*******/
    INSTALLATION_XLS_LANG_ID_THE_MENU_IS_TO_OPERATE,
    INSTALLATION_XLS_LANG_ID_WE_ARE_NOT_RESPONSIBLE,
    INSTALLATION_XLS_LANG_ID_I_HAVE_READ_AND_UNDERSTOOD,
    INSTALLATION_XLS_LANG_ID_CONFIRM,
    INSTALLATION_XLS_LANG_ID_CACCEL,
    INSTALLATION_XLS_LANG_ID_OPERATING_STRUCT,
    INSTALLATION_XLS_LANG_ID_BUILDING_HOUSE_NO,
    INSTALLATION_XLS_LANG_ID_IP_ADDRES,
    INSTALLATION_XLS_LANG_ID_IP_ADDRES_GATEWAY_DNS_SETTING,
    INSTALLATION_XLS_LANG_ID_OPERATIONG_SERVER_IP_ADDRES,
    INSTALLATION_XLS_LANG_ID_LOCAL_SERVER_SIP_SERVER_UPDATE,
    INSTALLATION_XLS_LANG_ID_COMMON_ENTRANCE_IP,
    INSTALLATION_XLS_LANG_ID_COMMON_ENTRANCE_IP_FOR_INTERLOCKING,
    INSTALLATION_XLS_LANG_ID_GUARD_STATION_NUMBER,
    INSTALLATION_XLS_LANG_ID_NOT_SET,
    INSTALLATION_XLS_LANG_ID_SENSOR,
    INSTALLATION_XLS_LANG_ID_SENSOR_TEST,
    INSTALLATION_XLS_LANG_ID_FRONT_DOOR_CAMERA,
    INSTALLATION_XLS_LANG_ID_FRONT_CCTV,
    INSTALLATION_XLS_LANG_ID_FACTORY_RESET,


    /*******  layout_setting_master_indoor_unit_ip*******/
    _SETTING_MASTER_INDOOR_UNIT_IP_XLS_LANG_ID_MASTER_WALLPAD_IP,


    /*******  layout_setting_sensor_usage*******/
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_SETTINGS,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_1,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_2,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_3,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_4,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_5,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_6,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_CONTACT_7,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_RUNNNG_TIPS,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_NOT_USED,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_NO,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_NC,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_ALWAYS,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_USAGE_SETTINGS,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_SETTINGS_WHEN_GOING_OUT,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_SETTING_WHEN_SECURITY_MODE,
    SETTING_SENSOR_USAGE_XLS_LANG_ID_SENSOR_FOR_SECURITY,

    /*******  layout_setting_sensor_test*******/
    SETTING_SENSOR_TEST_XLS_LANG_ID_EMERGENCY,
    SETTING_SENSOR_TEST_XLS_LANG_ID_MAGNETIC_SENSOR,
    SETTING_SENSOR_TEST_XLS_LANG_ID_RIP_SENSOR,
    SETTING_SENSOR_TEST_XLS_LANG_ID_ALARM,

    /*******  layout_ip_setting*******/
    IP_SETTING_XLS_LANG_ID_IP_SETTING,

    /*******  layout_setting_motion*******/
    SETTING_MOTION_XLS_LANG_ID_USE_MOTION_DETECTION,
    SETTING_MOTION_XLS_LANG_ID_SELECT_CAMERA,
    SETTING_MOTION_XLS_LANG_ID_STORAGE_FORMAT,
    SETTING_MOTION_XLS_LANG_ID_HIGH,
    SETTING_MOTION_XLS_LANG_ID_LOW,
    SETTING_MOTION_XLS_LANG_ID_MIDDLE,
    SETTING_MOTION_XLS_LANG_ID_SENSITIVTY_SETTING,
    SETTING_MOTION_XLS_LANG_ID_MOTION_DETECTING_SCHEDULE,
    SETTING_MOTION_XLS_LANG_ID_LCD_ON_WHEN_MOTION_DETECTED,
    SETTING_MOTION_XLS_LANG_ID_SCHEDULE_SETTING,
    SETTING_MOTION_XLS_LANG_ID_START,
    SETTING_MOTION_XLS_LANG_ID_END,


    /*******  layout_setting_standby_screen*******/
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_SCREEN_OFF,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_OPEN_FRAME,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_OPEN_FRAME_APPLY_CONFIRM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_WAIT_DISPLAY_TIME,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_DISPLAY_TIME,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_ALWAYS,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_TIME_DESIGNATION,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_NIGHT_MODE,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_ITEM_DISPLAY,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_0_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_1_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_2_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_3_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_4_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_5_ITEM,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_BACKROUND,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_RANDOM_IMAGE,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_BASIC,

    SETTING_STANDBY_SCREEN_XLS_LANG_ID_USER_NIGHT_MODE,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_ADJUSTS_THE_BRIGHTNESS,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_HOUR,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_CALENDAR,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_RECENTLY_SAVED_VIDEO,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_FRONT_DOOR_VIDEO,
    SETTING_STANDBY_SCREEN_XLS_LANG_ID_CCTV_FOOTAGE,

    SETTING_STANDBY_SCREEN_XLS_LANG_ID_END_TIME,


    /*******  layout_setting_password*******/
    SETTING_PASSWORD_XLS_LANG_ID_RESET_HOUSEHOLD_PASSWORD,
    SETTING_PASSWORD_XLS_LANG_ID_RESET_COMMON_ENTRANCE_PASSWORD,
    SETTING_PASSWORD_XLS_LANG_ID_NEW_PASSWORD,
    SETTING_PASSWORD_XLS_LANG_ID_VERIFY_PASSWORD,
    SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_RESET,
    SETTING_PASSWORD_XLS_LANG_ID_APPLE,
    SETTING_PASSWORD_XLS_LANG_ID_PASSWORD_NOT_MATCH,
    SETTING_PASSWORD_XLS_LANG_ID_OLD_PASSWORD,


    /*******  layout_ipc_camera_edit *******/

    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DO_YOU_WANT_DELETE_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_DO_YOU_WANT_DELETE_CCTV,

    DOOR_CAMERA_SEARCH_XLS_LANG_ID_PASSWORD_MODIY_SUCCESS,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_NAME_MODIY_SUCCESS,
    DOOR_CAMERA_SEARCH_XLS_LANG_ID_NAME_MODIY_FAIL,



    UNKNOW_1,
    UNKNOW_2,
    UNKNOW_3,
    UNKNOW_4,
    UNKNOW_5,
    UNKNOW_6,
#endif
 
    LANGUAGE_STRING_TOTAL
} xls_str_id_t;

/***
**   日期:2022-05-24 10:04:38
**   作者: leo.liu
**   函数作用：根据通道类型获取字符串
**   参数说明:
***/
const char *language_common_ch_string_get(int ch);

const char *lang_str_get(int XLS_LANG_str_id);

char *lang_str_get_by_language(xls_str_id_t index, LANGUAGE_ID language);

int  language_total_get(void);

void write_language_string_to_xls(void);


#endif // _LANGUAGE_H_