#ifndef _USER_MONITOR_H_
#define _USER_MONITOR_H_
typedef enum
{
	MON_CH_NONE = 0x00,
	MON_CH_DOOR1,
	MON_CH_DOOR2,
	MON_CH_CCTV1,
	MON_CH_CCTV2,
	MON_CH_INTERCOM,
	MON_CH_TOTAL
} MON_CH;
typedef enum{

	MON_ENTER_CALL_FLAG,
	MON_ENTER_MANUAL_DOOR_FLAG,
	MON_ENTER_MANUAL_CCTV_FLAG
}MON_ENTER_FLAG;
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道
** 返回参数说明：
***/
MON_CH monitor_channel_get(void);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 8:37:5
** 说明: 设置通道 
***********************************************/
void monitor_channel_set(MON_CH ch);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:43:34
** 说明: 进入监控标志 
***********************************************/
void monitor_enter_flag_set(MON_ENTER_FLAG flag);
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 9:44:13
** 说明: 获取进入监控标志 
***********************************************/
MON_ENTER_FLAG monitor_enter_flag_get(void);

#endif