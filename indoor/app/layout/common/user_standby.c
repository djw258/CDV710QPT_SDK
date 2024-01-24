#include "user_standby.h"
#include "common/sat_user_time.h"

/***
** 日期: 2022-05-10 08:45
** 作者: leo.liu
** 函数作用：超时时间
** 返回参数说明：
***/
static int standby_timeout = 30000;

/***
** 日期: 2022-05-10 08:45
** 作者: leo.liu
** 函数作用：待机使能
** 返回参数说明：
***/
static bool standby_timer_enable = false;
/***
** 日期: 2022-05-10 08:33
** 作者: leo.liu
** 函数作用：待机跳转页面
** 返回参数说明：
***/
static sat_layout_info *standby_goto_playout = NULL;

/***
** 日期: 2022-05-10 08:38
** 作者: leo.liu
** 函数作用：定时器溢出时间
** 返回参数说明：
***/
static unsigned long long standby_timeout_timestamp = 0;

/***
** 日期: 2022-05-10 08:33
** 作者: leo.liu
** 函数作用：初始化待机检测
** 返回参数说明：
***/
bool standby_timer_init(sat_layout_info *playout, int timeout)
{
    if (playout != NULL)
    {
        standby_goto_playout = playout;
    }
    standby_timeout = timeout;
    standby_timer_enable = false;
    return true;
}

/***
** 日期: 2022-05-10 08:47
** 作者: leo.liu
** 函数作用：待机检测重新及时
** 返回参数说明：
***/
bool standby_timer_restart(bool fouce_enable)
{
    if (fouce_enable)
    {
        standby_timer_enable = true;
    }
    standby_timeout_timestamp = user_timestamp_get() + standby_timeout;
    return true;
}

/************************************************************
** 函数说明: 重置待机时间
** 作者: xiaoxiao
** 日期: 2023-06-07 14:52:47
** 参数说明:
** 注意事项:
************************************************************/
bool standby_timer_reset(int timeout)
{
    standby_timeout = timeout;
    standby_timeout_timestamp = user_timestamp_get() + standby_timeout;
    return true;
}
/***
** 日期: 2022-05-10 08:42
** 作者: leo.liu
** 函数作用：关闭待机检测
** 返回参数说明：
***/
bool standby_timer_close(void)
{
    if (standby_timer_enable == false)
    {
        return false;
    }

    standby_timer_enable = false;
    return true;
}
/***
** 日期: 2022-05-10 08:42
** 作者: leo.liu
** 函数作用：待机检测状态获取
** 返回参数说明：
***/
bool standby_timer_status_get(void)
{
    return standby_timer_enable;
}

/***
** 日期: 2022-05-10 08:41
** 作者: leo.liu
** 函数作用：检测待机超时处理函数
** 返回参数说明：
***/
bool standby_timeout_check_and_process(void)
{
    if (standby_timer_enable == false)
    {
        return false;
    }
    unsigned long long timestamp = user_timestamp_get();
    if (timestamp > standby_timeout_timestamp)
    {
        if (standby_goto_playout != NULL)
        {
            _sat_layout_goto(standby_goto_playout, LV_SCR_LOAD_ANIM_FADE_IN);
            return true;
        }
    }
    return false;
}
