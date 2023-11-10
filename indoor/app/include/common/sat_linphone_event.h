

#ifndef _SAT_LINPHONE_EVENT_H
#define _SAT_LINPHONE_EVENT_H
#include <stdbool.h>

typedef bool (*snapshot_write_callback)(unsigned char *data, int size, int ch, int mode);
typedef bool (*record_video_write_callback)(const char *path, int ch, int mode);
typedef bool (*media_thumb_disp_callback)(const char *data, int x, int y, int w, int h);

typedef struct
{
        bool enable;
        int channel;
        long call_id;
} linphone_incomming_info;

/*
 * @日期: 2022-09-05
 * @作者: leo.liu
 * @注释: linephone消息结构体
 */
typedef struct
{
        long size;
        char msg[1024];
} linphone_queue_msg;

/*
 * @日期: 2022-09-05
 * @作者: leo.liu
 * @功能: 获取linphone消息队列句柄
 * @return:
 */
int linphone_queue_handle_get(void);

/*
 * @日期: 2022-09-05
 * @作者: leo.liu
 * @功能: 呼叫用户
 * @return:
 */
bool sat_linphone_call(const char *url, bool video, bool early_media, char *multicase_ip);

/*
 * @日期: 2022-09-06
 * @作者: leo.liu
 * @功能: 注册到服务器
 * @return:
 */
bool sat_linphone_register(const char *name, const char *user, const char *pwd, const char *proxy);

/*
 * @日期: 2022-09-06
 * @作者: leo.liu
 * @注释: 同意通话
 */
bool sat_linphone_answer(long id, bool is_tuya_answer);
/*
 * @日期: 2022-09-06
 * @作者: leo.liu
 * @注释: 是否添加背景音乐
 */
bool sat_linphone_alarm_backgound_sound(bool en);
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-21 14:44:28
** 说明: 挂断sip通话
***********************************************/
bool sat_linphone_handup(int id);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 重新渲染下一个接入的call
***********************************************/
bool sat_linphone_incomming_refresh(long id);
/*
 * @日期: 2022-09-08
 * @作者: leo.liu
 * @注释: 抓拍
 */
bool sat_linphone_snap(int ch, int mode, snapshot_write_callback callback);

/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 录制视频
 */
bool sat_linphone_start_record_video(int has_audio, int ch, int mode, record_video_write_callback callback);

/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: 停止录制
 */
bool sat_linphone_stop_record_video(void);

/****************************************************************
**@日期: 2022-09-22
**@作者: leo.liu
**@功能:查看camera
*****************************************************************/
bool sat_linphone_ipcamera_start(const char *url);


/****************************************************************
**@日期: 2022-09-22
**@作者: leo.liu
**@功能:关闭camera
*****************************************************************/
bool sat_linphone_ipcamera_stop(void);

/****************************************************************
**@日期: 2022-10-06
**@作者: leo.liu
**@功能:缩略图显示
ex:filepath x y w h
*****************************************************************/
bool sat_linphone_media_thumb_display(char arry[][128], int num, media_thumb_disp_callback callback);

/****************************************************************
**@日期: 2022-10-06
**@作者: leo.liu
**@功能:销毁缩略图现实
*****************************************************************/
bool sat_linphone_media_thumb_destroy(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-9 15:44:18
** 说明: 播放视频
***********************************************/
bool sat_linphone_video_play_start(char *file, int x, int y, int w, int h);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-9 15:47:56
** 说明: 停止播放视频
***********************************************/
bool sat_linphone_video_play_stop(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-9 15:48:58
** 说明: 暂停播放
***********************************************/
bool sat_linphone_video_play_pause(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-17 11:55:52
** 说明: 播放状态获取
***********************************************/
bool sat_linphone_video_play_state_get(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-17 11:57:32
** 说明: 播放时长获取
***********************************************/
bool sat_linphone_video_play_duration_get(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 铃声播放
***********************************************/
bool sat_linphone_audio_play_start(const char *file, int reount);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 停止铃声
***********************************************/
bool sat_linphone_audio_play_stop(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 停止铃声
***********************************************/
bool sat_linphone_message_cmd_send(const char *user, const char *cmd);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 查询通话状态
***********************************************/
bool sat_linphone_calls_cmd_send(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 移动侦测:
** threshold(0-255)
** sensivity(0-3600)
***********************************************/
bool sat_linphone_motion_detection_start(int threshold, int sensivity);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 9:0:4
** 说明: 移动侦测停止
***********************************************/
bool sat_linphone_motion_detection_stop(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 铃声音量
***********************************************/
bool sat_linphone_audio_play_volume_set(int volume);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 通话音量
***********************************************/
bool sat_linphone_audio_talk_volume_set(int volume);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 获取一个已经存入的节点
***********************************************/
linphone_incomming_info *linphone_incomming_used_node_get(bool is_doorcamera);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 获取一个已经存入的节点
***********************************************/
linphone_incomming_info *linphone_incomming_used_node_get_by_call_id(long call_id);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 获取一个没有存入的节点
***********************************************/
linphone_incomming_info *linphone_incomming_unused_node_get(bool is_doorcamera);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 释放其中的节点
***********************************************/
bool linphone_incomming_node_release(const linphone_incomming_info *node);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 释放所有的节点
***********************************************/
bool linphone_incomming_node_release_all(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-10-19 8:57:42
** 说明: 获取所有的有效通道号
***********************************************/
bool linphone_incomming_vaild_channel_get(bool is_doorcamera, linphone_incomming_info valid_group[8], int *total);
#endif
