#include "user_monitor.h"
static MON_CH montor_channel = MON_CH_NONE;
/***
** 日期: 2022-05-12 11:33
** 作者: leo.liu
** 函数作用：获取监控通道
** 返回参数说明：
***/
MON_CH monitor_channel_get(void)
{
	return montor_channel;
}