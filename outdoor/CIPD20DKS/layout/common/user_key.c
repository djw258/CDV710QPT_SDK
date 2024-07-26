
#include "user_key.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include "common/sat_user_common.h"
#include "common/sat_main_event.h"
#include "common/sat_user_time.h"
#include "common/gpio_ctrl.h"
#include "user_data.h"
#include "user_gpio.h"

#define KEY_DEVICE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define KEY_RESET_SYSTEM_PIN 41
static int user_key_state = KEY_STATE_RELEASE;
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:54:32
** 说明读取按键值
***********************************************/
static int user_key_code_read(int fd)
{
	char key_map_code[10];
	int reslut = read(fd, key_map_code, sizeof(key_map_code));
	if (reslut < 1)
	{
		return -1;
	}
	int key_code = 0;
	sscanf(key_map_code, "%d", &key_code);
	return key_code;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:54:32
** 说明: 复位系统检测
***********************************************/
static void reset_system_gpio_detection(void)
{
	GPIO_LEVEL level;
	gpio_level_read(KEY_RESET_SYSTEM_PIN, &level);
	unsigned long long timestap = user_timestamp_get();
	while (level == GPIO_LEVEL_LOW)
	{
		if (abs(user_timestamp_get() - timestap) > 3000)
		{
			user_data_reset();
			led_ctrl_blink(3);
			system("reboot");
		}
		gpio_level_read(KEY_RESET_SYSTEM_PIN, &level);
		usleep(10 * 1000);
	}
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:54:32
** 说明: 按键检测任务
***********************************************/
static void *user_key_task(void *arg)
{
	int key_value = 0;
	unsigned long long key_down_timestamp = 0;
	while (1)
	{
		int fd = open(KEY_DEVICE_PATH, O_RDONLY);
		if ((key_value = user_key_code_read(fd)) >= 0)
		{
			if (abs(key_value - 3000) < 200)
			{
				if (user_key_state != KEY_STATE_RELEASE)
				{
					if (user_key_state == KEY_STATE_SLOT_DOWN)
					{
						sat_msg_send_cmd(MSG_EVENT_CMD_KEY_CALL, user_key_state, key_value);
					}
					user_key_state = KEY_STATE_RELEASE;
					SAT_DEBUG("KEY RELEASE");
				}
			}
			else if (user_key_state == KEY_STATE_RELEASE)
			{
				key_down_timestamp = user_timestamp_get();
				user_key_state = KEY_STATE_SLOT_DOWN;
				SAT_DEBUG("KEY SLOT DOWN");
			}
			else if ((user_key_state == KEY_STATE_SLOT_DOWN) && ((user_timestamp_get() - key_down_timestamp) > 1500))
			{
				user_key_state = KEY_STATE_LONG_DOWN;
				SAT_DEBUG("KEY LONG DOWN");
			}
			else if ((user_key_state == KEY_STATE_LONG_DOWN) && ((user_timestamp_get() - key_down_timestamp) > 10000))
			{
				SAT_DEBUG("restart app");
				exit(0);
			}
		}
		close(fd);

		reset_system_gpio_detection();
		usleep(100 * 1000);
	}
	return NULL;
}

/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:50:54
** 说明: 初始化按键检测线程
***********************************************/
bool user_key_init(void)
{
	if (access(KEY_DEVICE_PATH, F_OK))
	{
		printf("%s not find\n", KEY_DEVICE_PATH);
		return false;
	}
	gpio_direction_set(KEY_RESET_SYSTEM_PIN, GPIO_DIR_IN);
	pthread_t thread_id;
	pthread_create(&thread_id, sat_pthread_attr_get(), user_key_task, NULL);
	return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-12-28 11:50:54
** 说明: 按键状态
***********************************************/
int user_key_state_get(void)
{
	return user_key_state;
}