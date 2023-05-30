#ifndef _LAYOUT_DOOR_CAMERA_SEARCH_H_
#define _LAYOUT_DOOR_CAMERA_SEARCH_H_
#include <stdbool.h>
#define IPC_CAMERA_FLAG_PWD_ERROR 0X01
#define IPC_CAMERA_FLAG_REGISTER 0X02
#define IPC_CAMERA_FLAG_INPUT_USER 0X04
#define IPC_CAMERA_FLAG_INPUT_PWD 0X08
#define IPC_CAMERA_FLAG_CHANGE_NAME 0X10
#define IPC_CAMERA_FLAG_CHANGE_PWD 0X20
#define IPC_CAMERA_FLAG_SEARCH 0X40

typedef enum
{

    DOOR_CAMERA_SEARCH_ID_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_ID_DOOR_CAMERA_SEARCH,
    DOOR_CAMERA_SEARCH_ID_IF_YOUT_PRESS,
    DOOR_CAMERA_SEARCH_ID_SEARCHED_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_ID_SEARCHED_CCTV,
    DOOR_CAMERA_SEARCH_ID_IF_YOUT_CANNOT_SEE,
    DOOR_CAMERA_SEARCH_ID_REGISTRATION,
    DOOR_CAMERA_SEARCH_ID_OPERATION_SERVER,
    DOOR_CAMERA_SEARCH_ID_DHCP,
    DOOR_CAMERA_SEARCH_ID_STATIC,
    DOOR_CAMERA_SEARCH_ID_REGISTERED_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_ID_PASSWORD,
    DOOR_CAMERA_SEARCH_ID_DEFAULT_PASSWORD_IS_SET,
    DOOR_CAMERA_SEARCH_ID_INPUT_PASSWORD_IS_SET,
    DOOR_CAMERA_SEARCH_ID_CHANGE_PASSWORD,
    DOOR_CAMERA_SEARCH_ID_CONFIRM_CHANGE_PASSWORD,
    DOOR_CAMERA_SEARCH_ID_ENTER_AT_LEAST_9_DIGITS_OF_NUMBER,
    DOOR_CAMERA_SEARCH_ID_SET_THE_PASSWORD_TO_A_MIXTURE,
    DOOR_CAMERA_SEARCH_ID_CHANGE_DOOR_CAMERA_INFOMATION,
    DOOR_CAMERA_SEARCH_ID_CHANGE_NAME,
    DOOR_CAMERA_SEARCH_ID_CHANGE_TH_CONNETION_PASSWORD,
    DOOR_CAMERA_SEARCH_ID_DO_YOU_WANT_DELETE_DOOR_CAMERA,
    DOOR_CAMERA_SEARCH_ID_DO_YOU_WANT_DELETE_CCTV,
    DOOR_CAMERA_SEARCH_ID_PASSWORD_MODIY_SUCCESS,
    DOOR_CAMERA_SEARCH_ID_NAME_MODIY_SUCCESS,
    DOOR_CAMERA_SEARCH_ID_NAME_MODIY_FAIL,
    DOOR_CAMERA_SEARCH_ID_TOTAL,
} LAYOUT_DOOR_CAMERA_SEARCH_LANG_ID;

static const char *layout_door_camera_search_language[DOOR_CAMERA_SEARCH_ID_TOTAL][LANGUAGE_ID_TOTAL] =
    {
        {
            "Door camera",
            "Door camera",
            "Door camera",
        },
        {
            "Door camera search",
            "Door camera search",
            "Door camera search",
        },
        {
            "If you press the button of front door camera for 5 seconds, \ncamera information is displayed..",
            "If you press the button of front door camera for 5 seconds, \ncamera information is displayed..",
            "If you press the button of front door camera for 5 seconds, \ncamera information is displayed..",
        },
        {
            "Searched door camera",
            "Searched door camera",
            "Searched door camera",
        },
        {
            "Searched CCTV",
            "Searched CCTV",
            "Searched CCTV",
        },
        {
            "If you cannot see the video, please enter the correct IP using the IP setting function at the top right.\nIf the video is not visible even after setting the IP, press the reset button on the door camera.",
            "If you cannot see the video, please enter the correct IP using the IP setting function at the top right.\nIf the video is not visible even after setting the IP, press the reset button on the door camera.",
            "If you cannot see the video, please enter the correct IP using the IP setting function at the top right.\nIf the video is not visible even after setting the IP, press the reset button on the door camera.",
        },
        {
            "Registration",
            "Registration",
            "Registration",
        },
        {
            "Operation server IP address",
            "Operation server IP address",
            "Operation server IP address",
        },
        {
            "DHCP",
            "DHCP",
            "DHCP",
        },
        {
            "Static",
            "Static",
            "Static",
        },
        {
            "Registered Door camera",
            "Registered Door camera",
            "Registered Door camera",
        },
        {
            "Password",
            "Password",
            "Password",
        },
        {
            "A default password is set. You must\nchange your password before you can\nproceed to the next procedure.Do you\nwant to change your password?",
            "A default password is set. You must\nchange your password before you can\nproceed to the next procedure.Do you\nwant to change your password?",
            "A default password is set. You must\nchange your password before you can\nproceed to the next procedure.Do you\nwant to change your password?",
        },
        {
            "Please enter the password for the\ndoor camera device. If the password is\nincorrect, calls cannot be monitored",
            "Please enter the password for the\ndoor camera device. If the password is\nincorrect, calls cannot be monitored",
            "Please enter the password for the\ndoor camera device. If the password is\nincorrect, calls cannot be monitored",
        },
        {
            "Change Password",
            "Change Password",
            "Change Password",
        },
        {
            "Confirm change password",
            "Confirm change password",
            "Confirm change password",
        },
        {
            "Enter at least 9 digits of numbers, special characters and alphabet characters",
            "Enter at least 9 digits of numbers, special characters and alphabet characters",
            "Enter at least 9 digits of numbers, special characters and alphabet characters",
        },
        {
            "Set the password to a mixture of is displayed.\nnumbers, letters and special\ncharacters to be at least 9 digits.",
            "Set the password to a mixture of is displayed.\nnumbers, letters and special\ncharacters to be at least 9 digits.",
            "Set the password to a mixture of is displayed.\nnumbers, letters and special\ncharacters to be at least 9 digits.",
        },
        {
            "Change Door camera information",
            "Change Door camera information",
            "Change Door camera information",
        },
        {
            "Change Name",
            "Change Name",
            "Change Name",
        },
        {
            "Change the connection password",
            "Change the connection password",
            "Change the connection password",
        },
        {
            "Are you sure you want to\ndelete the front door camera?",
            "Are you sure you want to\ndelete the front door camera?",
            "Are you sure you want to\ndelete the front door camera?",
        },
        {
            "Are you sure you want to\ndelete the CCTV?",
            "Are you sure you want to\ndelete the CCTV?",
            "Are you sure you want to\ndelete the CCTV?",
        },
        {
            "Password modified successfully",
            "Password modified successfully",
            "Password modified successfully",
        },
        {
            "Name modified successfully",
            "Name modified successfully",
            "Name modified successfully",
        },
        {
            "Name modified fail",
            "Name modified fail",
            "Name modified fail",
        },

};

/***
** 日期: 2022-04-26 09:16
** 作者: leo.liu
** 函数作用：获取home页面的字符串资源
** 返回参数说明：年/년 !@
***/
static inline const char *layout_ipc_camera_search_language_get(int id)
{
    return layout_door_camera_search_language[id][language_id_get()];
}

void layout_ipc_camera_input_flag_set(int flg);
int layout_ipc_camera_input_flag_get(void);
/*进入模式设置:door camera /ipc*/
bool layout_ipc_cmeara_is_doorcamera_get(void);
void layout_ipc_cmeara_is_doorcamera_set(bool yeno);

/*当前编辑的索引号*/
void layout_ipc_camera_edit_index_set(int index);
int layout_ipc_camera_edit_index_get(void);
#endif