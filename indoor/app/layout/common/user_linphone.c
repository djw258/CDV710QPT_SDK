
#include "user_linphone.h"
#include "common/sat_main_event.h"
#include "common/user_data.h"
#include "common/sat_linphone_event.h"
#include "common/user_monitor.h"
#include "common/sat_user_common.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef struct
{

	char *cmd;
	user_linphone_func func;
} user_linphone_func_s;

static bool linphone_call_online_query_func_defalut(char *args)
{
	const char *user = monitor_channel_get_url(monitor_channel_get(), false);
	if (strstr(args, user) != NULL)
	{
		char cmd[128] = {0};
		sprintf(cmd, "SAT_CMD %s", LinphoneCallOnlineQueryStr);
		sat_linphone_message_cmd_send(user, cmd);
		return true;
	}
	return false;
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:13:8
** 说明: 用户呼入处理
***********************************************/
static user_linphone_func_s user_linphone_func_array[] =
		{
				{
						LinphoneCallIncomingReceivedStr,
						NULL,
				},
				{LinphoneCallStreamsRunningStr,
				 NULL},
				{LinphoneCallStreamsStoppingStr,
				 NULL},
				{LinphoneCallConnectedStr,
				 NULL},
				{LinphoneCallPasswordErrorStr,
				 NULL},
				{
						LinphoneCallOnlineQueryStr,
						linphone_call_online_query_func_defalut,
				},
				{
						LinphoneCallOutgoingInitStr,
						NULL,
				},
				{
						LinphoneCallEndStr,
						NULL,
				},
				{
						LinphoneCallAnswerStr,
						NULL,
				},
				{LinphoneCallBusyStr,
				 NULL},
				{LinphoneCallErrorStr,
				 NULL},
				{LinphoneCallOutgoingEarlyMediaStr,
				 NULL},

};

static void user_linphone_event_func(unsigned char *info)
{
	char *ptr = (char *)info;
	char *args = NULL;

	while (*ptr && !isspace(*ptr))
	{

		++ptr;
	}

	if (*ptr)
	{
		*ptr = '\0';

		/* set args to first nonblank */
		args = ptr + 1;

		while (*args && isspace(*args))
		{
			++args;
		}
	}
	else
	{

		printf("[%s]:%d error linphone sip event:%s\n", __func__, __LINE__, info);
	}

	int n_func = sizeof(user_linphone_func_array) / sizeof(user_linphone_func_s);

	for (int i = 0; i < n_func; i++)
	{

		if (strcmp(user_linphone_func_array[i].cmd, (char *)info) == 0)
		{

			if (user_linphone_func_array[i].func != NULL)
			{

				user_linphone_func_array[i].func(args);
			}

			break;
		}
	}
}

/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: sip incomoning call register
***********************************************/
static void user_linphone_event_received_register(const char *str, user_linphone_func callback)
{
	int n_func = sizeof(user_linphone_func_array) / sizeof(user_linphone_func_s);

	for (int i = 0; i < n_func; i++)
	{

		if (strcmp(user_linphone_func_array[i].cmd, str) == 0)
		{

			user_linphone_func_array[i].func = callback;
		}
	}
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: sip 呼入处理函数
***********************************************/
void user_linphone_call_incoming_received_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallIncomingReceivedStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 流建立后的处理函数
***********************************************/
void user_linphone_call_streams_running_receive_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallStreamsRunningStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 流停止后的处理函数
***********************************************/
void user_linphone_call_streams_stopping_receive_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallStreamsStoppingStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 同意通话后建立后的处理函数
***********************************************/
void user_linphone_call_streams_connected_receive_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallConnectedStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 密码错误提示
***********************************************/
void user_linphone_call_password_error_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallPasswordErrorStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 呼叫应答成功后
***********************************************/
void user_linphone_call_outgoing_call_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallOutgoingInitStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 通话结束
***********************************************/
void user_linphone_call_end_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallEndStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 通话结束
***********************************************/
void user_linphone_call_answer_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallAnswerStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 通话结束
***********************************************/
void user_linphone_call_busy_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallBusyStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 呼叫失败
***********************************************/
void user_linphone_call_error_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallErrorStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 10:46:3
** 说明: 呼叫失败
***********************************************/
void user_linphone_call_outgoing_early_media_register(user_linphone_func callback)
{
	user_linphone_event_received_register(LinphoneCallOutgoingEarlyMediaStr, callback);
}
/***********************************************
** 作者: leo.liu
** 日期: 2023-1-5 9:59:5
** 说明: 初始化user linphone相关
***********************************************/
void user_linphone_init(void)
{
	/*
	 * @日期: 2022-08-20
	 * @作者: leo.liu
	 * @注释: 开启pjsip
	 */
	extern int linphone_init(int argc, char *argv[]);

	linphone_init(0, NULL);

	sip_cmd_callback_register(user_linphone_event_func);
}
