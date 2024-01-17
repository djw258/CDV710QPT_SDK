#include "user_gpio.h"
#include "common/sat_user_common.h"
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: ir 检测初始化
***********************************************/
#define IR_FEED_GPIO_PIN 65
static void ir_feed_init(void)
{
        gpio_direction_set(IR_FEED_GPIO_PIN, GPIO_DIR_IN);
        gpio_level_set(IR_FEED_GPIO_PIN, GPIO_LEVEL_LOW);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: 读取ir的高低电平
***********************************************/
GPIO_LEVEL ir_feed_read(void)
{
        GPIO_LEVEL level;
        gpio_level_read(IR_FEED_GPIO_PIN, &level);
        return level;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: led 始化
***********************************************/
#define LED_CTRL_GPIO_PIN 81
static void led_ctrl_init(void)
{
        gpio_direction_set(LED_CTRL_GPIO_PIN, GPIO_DIR_OUT);
        gpio_level_set(LED_CTRL_GPIO_PIN, GPIO_LEVEL_LOW);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: 设置led的电平
***********************************************/
void led_ctrl_enable(bool en)
{
        gpio_level_set(LED_CTRL_GPIO_PIN, en == true ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW);
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: 读取gpio 电平
***********************************************/
GPIO_LEVEL led_ctrl_level_read(void)
{
        GPIO_LEVEL level;
        gpio_level_read(LED_CTRL_GPIO_PIN, &level);
        return level;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: gpio初始化
***********************************************/
bool user_gpio_init(void)
{
        ir_feed_init();
        led_ctrl_init();
        return true;
}
/***********************************************
** 作者: leo.liu
** 日期: 2022-11-9 10:15:48
** 说明: 控制灯闪烁
***********************************************/
void led_ctrl_blink(int num)
{
        for (int i = 0; i < num; i++)
        {
                led_ctrl_enable(true);
                usleep(500 * 1000);
                led_ctrl_enable(false);
                usleep(500 * 1000);
        }
}