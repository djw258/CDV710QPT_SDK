
#include "user_monitor.h"
#include "user_data.h"
#include "common/sat_linphone_event.h"
#include "common/lv_common.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "common/sat_ipcamera.h"

static int monitor_channel = -1;
static MON_ENTER_FLAG monitor_enter_flag = MON_ENTER_MANUAL_DOOR_FLAG;

const char *monitor_channel_get_url(int index, bool rtsp)
{
        static char rtsp_uri[128] = {0};
        memset(rtsp_uri, 0, sizeof(rtsp_uri));
        if (index < 8)
        {
                if (rtsp)
                {
                        sprintf(rtsp_uri, "%s %s %s", network_data_get()->door_device[index].rtsp[0].rtsp_url, network_data_get()->door_device[index].username, network_data_get()->door_device[index].password);
                }
                else
                {
                        return network_data_get()->door_device[index].sip_url;
                }
        }
        else
        {
                sprintf(rtsp_uri, "%s %s %s", network_data_get()->cctv_device[index - 8].rtsp[0].rtsp_url, network_data_get()->cctv_device[index - 8].username, network_data_get()->cctv_device[index - 8].password);
        }
        return rtsp_uri;
}
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：依据房号获取通道(用户door camera)
** 返回参数说明：
***/
int monitor_index_get_by_user(const char *user)
{
        /*格式："CIP-D20YS" <sip:202@10.1.1.11:5066>,*/
        char *s = strstr(user, "sip:20");
        if (s == NULL)
        {
                printf("[%s:%d] user string parse failed(%s)\n", __func__, __LINE__, user);
                return -1;
        }

        s += 6;
        if (((*s) < '1') || ((*s) > '9'))
        {
                printf("[%s:%d] user string aprse failed(%s)\n", __func__, __LINE__, user);
                return -1;
        }

        return ((*s) - '0');
}

/************************************************************
** 函数说明: 依据sip消息获取分机号
** 作者: xiaoxiao
** 日期: 2023-08-10 10:51:32
** 参数说明:
** 注意事项:
************************************************************/
int extern_index_get_by_user(const char *user)
{
        /*格式："user:"50x" <sip:71x@10.1.1.11:5066>,*/
        char *s = strstr(user, "user:\"50");
        if (s == NULL)
        {
                s = strstr(user, "sip:50");
                if (s == NULL)
                {
                        printf("[%s:%d] user string parse failed(%s)\n", __func__, __LINE__, user);
                        return -1;
                }
                else
                {
                        s += 6;
                }
        }
        else
        {
                s += 8;
        }
        if (((*s) < '1') || ((*s) > '9'))
        {
                printf("[%s:%d] user string aprse failed(%s)\n", __func__, __LINE__, user);
                return -1;
        }
        return ((*s) - '0');
}

/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取上一个有效通道
** 返回参数说明：
***/
int monitor_channel_prev_get(void)
{
        /* 往下获取当前通道的上一个有效通道*/
        int ch = monitor_channel - 1;
        int min = is_channel_ipc_camera(monitor_channel) == false ? 0 : 8;
        while (ch >= min)
        {
                if (monitor_valid_channel_check(ch) == true)
                {
                        return ch;
                }
                ch--;
        }
        /*往上获取有效通道*/
        ch = is_channel_ipc_camera(monitor_channel) == false ? (DEVICE_MAX - 1) : (DEVICE_MAX * 2 - 1);
        while (ch != monitor_channel)
        {
                if (monitor_valid_channel_check(ch) == true)
                {
                        return ch;
                }

                ch--;
        }
        return -1;
}

/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取下一个有效通道
** 返回参数说明：
***/
int monitor_channel_next_get(void)
{
        /* 往下获取当前通道的上一个有效通道*/
        int ch = monitor_channel + 1;
        int total = is_channel_ipc_camera(monitor_channel) == false ? (DEVICE_MAX - 1) : (DEVICE_MAX * 2 - 1);
        while (ch <= total)
        {
                if (monitor_valid_channel_check(ch) == true)
                {
                        return ch;
                }

                ch++;
        }

        /*往上获取有效通道*/
        ch = is_channel_ipc_camera(monitor_channel) == false ? 0 : 8;
        while (ch != monitor_channel)
        {
                if (monitor_valid_channel_check(ch) == true)
                {
                        return ch;
                }

                ch++;
        }
        return -1;
}
/*获取第一个有效的通道*/
int monitor_door_first_valid_get(bool is_door_camera)
{
        if (is_door_camera)
        {
                for (int i = 0; i < DEVICE_MAX; i++)
                {

                        if (network_data_get()->door_device[i].sip_url[0] != '\0')
                        {
                                return i;
                        }
                }
        }
        else
        {
                for (int i = 0; i < DEVICE_MAX; i++)
                {
                        if (network_data_get()->cctv_device[i].rtsp[0].rtsp_url[0] != '\0')
                        {
                                return i + 8;
                        }
                }
        }
        return -1;
}

/*获取最后一个有效通道*/
int monitor_door_last_valid_get(bool is_door_camera)
{
        if (is_door_camera)
        {
                for (int i = DEVICE_MAX - 1; i >= 0; i--)
                {
                        if (network_data_get()->door_device[i].sip_url[0] != '\0')
                        {
                                return i;
                        }
                }
        }
        else
        {
                for (int i = DEVICE_MAX - 1; i >= 0; i--)
                {
                        if (network_data_get()->cctv_device[i].rtsp[0].rtsp_url[0] != '\0')
                        {
                                return i + 8;
                        }
                }
        }
        return -1;
}

// 获取门口机和CCTV的注册状态
bool monitor_door_registered_status_get(void)
{
        if (monitor_door_first_valid_get(true) == -1 && monitor_door_first_valid_get(false) == -1)
        {
                return false;
        }
        return true;
}
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道
** 返回参数说明：
***/
int monitor_channel_get(void)
{
        return monitor_channel;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 8:37:5
** 说明: 设置通道
***********************************************/
void monitor_channel_set(int ch)
{
        monitor_channel = ch;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:43:34
** 说明: 进入监控标志
***********************************************/
void monitor_enter_flag_set(MON_ENTER_FLAG flag)
{
        monitor_enter_flag = flag;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 获取进入监控标志
***********************************************/
MON_ENTER_FLAG monitor_enter_flag_get(void)
{
        return monitor_enter_flag;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 开启监控 flag bit0 =1 close sip ,bit1= 1 close rtsp
***********************************************/
static void monitor_reset(char flag)
{
        if (flag & 0x01)
        {
                sat_linphone_handup(-1);
        }

        if (flag & 0x02)
        {
                sat_linphone_ipcamera_stop();
        }
}
void monitor_open(bool refresh, bool rtsp)
{
        SAT_DEBUG("monitor_enter_flag is %d\n", monitor_enter_flag);
        if ((monitor_enter_flag == MON_ENTER_MANUAL_DOOR_FLAG) || (monitor_enter_flag == MON_ENTER_MANUAL_CCTV_FLAG))
        {
                monitor_reset(0x03);
                usleep(10 * 1000);
                if (is_channel_ipc_camera(monitor_channel_get()) == true)
                {
                        sat_linphone_ipcamera_start(monitor_channel_get_url(monitor_channel, rtsp));
                }
                else
                {
                        if (rtsp == true)
                        {
                                sat_linphone_ipcamera_start(monitor_channel_get_url(monitor_channel, rtsp));
                        }
                        else
                        {
                                char uri[128] = {0};
                                sprintf(uri, "%s:5066", monitor_channel_get_url(monitor_channel, rtsp));
                                sat_linphone_call(uri, true, true, NULL);
                        }
                }
        }
        //   char url[128] = {0};
        //  sprintf(url, "sip:%s", "172.16.0.131@172.16.0.131")       ;
        //  sat_linphone_call(url, true, true, NULL);
        lv_common_video_mode_enable(refresh);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明:关闭监控
***********************************************/
void monitor_close(char flag)
{
        monitor_reset(flag);
        lv_common_video_mode_enable(false);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 获取门口机信息
** 010.001.001.011
BYTE1. 010(固定部分).
                BYTE2. 001 -> 0x01
                                bit[7]=0:
                                bit[6:0] 栋号

                                bit[7]=1:
                                        分机(门口机/室内分机)
                                        bit[6:5]=00:室内分机
                                        bit[6:5]=10:户外机)

                BYTE3. 001 楼层号
                BYTE4. 001 百分位和十分位 - 房号
                           个位 -BYTE2 bit[7]=0
                                                1:室内机
                                                3:Gateway IP system
                                                7:Lobby IP system
                                                9:Guard Phone IP system
                                                5:KIOSK IP system
                                   -BYTE2 bit[7]=1
                                                个位分机ID号
***********************************************/
/************************************************************
** 函数说明: 判断通道是否有效
** 作者: xiaoxiao
** 日期: 2023-05-26 08:28:22
** 参数说明:
** 注意事项:
************************************************************/
bool monitor_valid_channel_check(int channel)
{
        if (channel < 8)
        {
                if ((network_data_get()->door_device[channel].sip_url[0] != 0))
                {
                        return true;
                }
        }
        else if ((channel >= 8) && (channel < 16))
        {
                channel -= 8;
                if (network_data_get()->cctv_device[channel].rtsp[0].rtsp_url[0] != 0)
                {
                        return true;
                }
        }
        return false;
}

/************************************************************
** 函数说明: 门口机注册数量获取
** 作者: xiaoxiao
** 日期: 2023-08-08 14:27:13
** 参数说明:
** 注意事项:
************************************************************/
int door_camera_register_num_get()
{
        int num = 0;
        for (int i = 0; i < 8; i++)
        {
                if (monitor_valid_channel_check(i) == true)
                {
                        num++;
                }
        }
        return num;
}

/************************************************************
** 函数说明: CCTV注册数量获取
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:
** 注意事项:
************************************************************/
int cctv_register_num_get()
{
        int num = 0;
        for (int i = 0; i < 8; i++)
        {
                if (monitor_valid_channel_check(8 + i) == true)
                {
                        num++;
                }
        }
        return num;
}
/************************************************************
** 函数说明: 门口机在线查询
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:ch:门口机通道；totoal:注册的门口机在线个数
** 注意事项:
************************************************************/
bool outdoor_online_check(int ch, int *total)
{
        int online_num = 0;
        bool result = false;
        const asterisk_register_info *p_register_info = asterisk_register_info_get();
        for (int j = 0; j < 20; j++)
        {
                if ((strncmp("20", p_register_info[j].name, 2) == 0) && (p_register_info[j].timestamp != 0)) // 获取在线的门口机
                {
                        if (network_data_get()->door_device[p_register_info[j].name[2] - '0' - 1].rtsp[0].rtsp_url[0] != 0) // 如果此在线的门口机是已经注册的
                        {
                                online_num++;
                                if (p_register_info[j].name[2] - '0' == (ch + 1)) // 判断在线的门口机是否和传入的参数一致
                                {
                                        result = true;
                                }
                        }
                }
        }
        if (total != NULL)
        {
                *total = online_num;
        }
        return result;
}

/************************************************************
** 函数说明: 分机在线查询
** 作者: xiaoxiao
** 日期: 2023-08-08 14:29:29
** 参数说明:ch:分机id1-8；total分机在线个数统计
** 注意事项:
************************************************************/
bool extension_online_check(int ch, int *total)
{
        int online_num = 0;
        bool result = false;
        const asterisk_register_info *p_register_info = asterisk_register_info_get();
        char user_name[64] = {0};
        sprintf(user_name, "50%d", user_data_get()->system_mode & 0x0f);
        for (int j = 0; j < 20; j++)
        {
                if (strncmp(user_name, p_register_info[j].name, strlen(user_name)))
                {
                        if ((strncmp("50", p_register_info[j].name, 2) == 0) && (p_register_info[j].timestamp != 0)) // 分机在线
                        {
                                online_num++;
                                if (p_register_info[j].name[2] - '0' == ch) // 判断在线的分机是否和传入的参数一致
                                {
                                        result = true;
                                }
                        }
                }
        }
        if (total != NULL)
        {
                *total = online_num;
        }
        return result;
}
