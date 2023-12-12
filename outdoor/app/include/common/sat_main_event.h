

#ifndef _MAIN_EVENT_H_
#define _MAIN_EVENT_H_
#include <stdbool.h>

enum
{
	MSG_EVENT_CMD_SD_CHANGE = 0X0000,
	/****************************************************************
	2022-09-21 author:leo.liu 说明:开始，0：拍照完成
	*****************************************************************/
	MSG_EVENT_CMD_SNAPSHORT,

	/****************************************************************
	2022-09-21 author:leo.liu 说明:arg1:开始录制，0：录制完成
	*****************************************************************/
	MSG_EVENT_CMD_RECORD,
	/****************************************************************
	2022-09-21 author:leo.liu 说明:arg1 1:online,arg2:online num
										 2:rtsp streadm,arg2:avlid num
	*****************************************************************/
	MSG_EVENT_CMD_IPCAMERA,
	/****************************************************************
	2022-09-21 author:leo.liu 说明:arg1 :0:空闲，1:播放，2:暂停s
	*****************************************************************/
	MSG_EVENT_CMD_VIDEO_PLAY_STATE,
	/****************************************************************
	2022-09-21 author:leo.liu 说明:arg1 :cur,arg2:total
	*****************************************************************/
	MSG_EVENT_CMD_VIDEO_PLAY_DURATION,
	/***********************************************
	** 作者: leo.liu
	** 日期: 2022-12-28 13:41:34
	** 说明: 按键呼叫
	***********************************************/
	MSG_EVENT_CMD_KEY_CALL,
	/***********************************************
	** 作者: leo.liu
	** 日期: 2022-12-28 13:41:34
	** 说明:视频流状态
	***********************************************/
	MSG_EVNET_CMD_VIDEO_STREAM,
	/***********************************************
	 ** 作者: leo.liu
	 ** 日期: 2022-12-28 13:41:34
	 ** 说明:CALL RING
	 ***********************************************/
	MSG_EVNET_CMD_CALL_RING,
	/****************************************************************
	 2022-09-21 author:leo.liu ring play:arg1:0,start,1:finish
	*****************************************************************/
	MSG_EVENT_CMD_AUDIO_RING_PLAY,
	/*linphone 通话状态*/
	MSG_EVENT_CMD_LINPHONE_STATE,
};

typedef struct
{
	void (*enter)(void);

	void (*quit)(void);
} sat_layout_info;

/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @功能: 主任务处理
 * @return:
 */
bool sat_mian_task_init(void);

/***
** 日期: 2022-04-25 16:14
** 作者: leo.liu
** 函数作用：创建一个页面
** 返回参数说明：
***/
#define sat_layout_create(x) sat_layout_info layout_##x = {   \
																 .enter = layout_##x##_enter, \
																 .quit = layout_##x##_quit};

/***
** 日期: 2022-04-25 16:14
** 作者: leo.liu
** 函数作用：进入此页面的第一个执行的函数
** 返回参数说明：
***/
#define sat_layout_enter(x) layout_##x##_enter(void)

/***
** 日期: 2022-04-25 16:14
** 作者: leo.liu
** 函数作用：退出此页面的最后一个函数
** 返回参数说明：
***/
#define sat_layout_quit(x) layout_##x##_quit(void)

/***
** 日期: 2022-04-25 16:15
** 作者: leo.liu
** 函数作用：页面地址
** 返回参数说明：
***/
#define sat_playout_get(x) (&layout_##x)

/***
** 日期: 2022-04-25 16:15
** 作者: leo.liu
** 函数作用：声明页面
** 返回参数说明：
***/
#define sat_layout_define(x) extern sat_layout_info layout_##x

/***
** 日期: 2022-04-25 16:31
** 作者: leo.liu
** 函数作用：页面跳转
** 返回参数说明：
***/
bool sat_layout_goto(const sat_layout_info *layout);

/***
**	 日期:2022-05-23 13:41:22
**	 作者: leo.liu
**	 函数作用：获取当前页面
**	 参数说明:
***/
const sat_layout_info *sat_cur_layout_get(void);

/***
** 日期: 2022-05-12 10:34
** 作者: leo.liu
** 函数作用：发送消息到主线程
** 返回参数说明：
***/
bool sat_msg_send_cmd(unsigned int cmd, unsigned int arg1, unsigned int arg2);

/*
 * @日期: 2022-08-12
 * @作者: leo.liu
 * @功能: sd卡状态处理函数注册
 * @return:
 */
void sd_state_channge_callback_register(void (*callback)(void));

/*
 * @日期: 2022-09-08
 * @作者: leo.liu
 * @注释: bool ,true,开始，false 完成
 */
void snapshot_state_callback_register(void (*callback)(bool ready));

/*
 * @日期: 2022-09-15
 * @作者: leo.liu
 * @注释: bool,true,开始，false 完成
 */
void record_state_callback_register(void (*callback)(bool reday));

/****************************************************************
**@日期: 2022-09-21
**@作者: leo.liu
**@功能:ipcamera相关的注册
*****************************************************************/
void ipcamera_state_callback_register(void (*callback)(unsigned int type, unsigned int num));

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-17 13:39:21
** 说明: 注册视频播放状态变化的事件
***********************************************/
void video_play_state_callback_register(void (*callback)(unsigned int state));

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-17 13:39:55
** 说明: 注册视频播放时长获取变化的事件
***********************************************/
void video_play_duration_callback_register(void (*callback)(unsigned int cur, unsigned int total));

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:42:13
** 说明: 按键呼叫
***********************************************/
void key_call_callback_register(void (*callback)(unsigned int, unsigned int));
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:42:13
** 说明: 注册定时器函数
***********************************************/
void system_timer_callback_register(void (*callback)(void));
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:42:13
** 说明: 视频流状态注册
***********************************************/
void video_stream_status_callback_register(void (*callback)(bool));
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:42:13
** 说明: 视频流状态注册
***********************************************/
void call_ring_event_fun_register(void (*callback)(void));
/*铃声播放状态*/
void call_ring_play_status_func_register(void (*callback)(int));
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 13:42:13
** 说明: linphone通话状态
***********************************************/
void linphone_call_status_event_func_register(void (*callback)(int));
#endif
