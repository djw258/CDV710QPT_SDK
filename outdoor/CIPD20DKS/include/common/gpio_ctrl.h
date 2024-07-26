#ifndef _GPIO_CTRL_H_
#define _GPIO_CTRL_H_
#include <stdbool.h>


typedef enum 
{
GPIO_DIR_IN = 0, 
GPIO_DIR_OUT = 1
} GPIO_DIR;


typedef enum 
{
GPIO_LEVEL_LOW = 0x01, 
GPIO_LEVEL_HIGH = 0x02
} GPIO_LEVEL;

/***
** 日期: 2022-04-28 10:02 
** 作者: leo.liu
** 函数作用：设置gpio输入/输出
** 返回参数说明：
***/
bool gpio_direction_set(const int pin, GPIO_DIR dir);

/***
** 日期: 2022-04-28 10:02 
** 作者: leo.liu
** 函数作用：使能/失能gpio内部上拉
** 返回参数说明：
***/
bool gpio_pull_enable(const int pin, bool enable);

/***
** 日期: 2022-04-28 10:02 
** 作者: leo.liu
** 函数作用：设置IO口电平
** 返回参数说明：在设置IO电平之前,需要调用 gpio_direction_set 
***/
bool gpio_level_set(const int pin, GPIO_LEVEL level);


/***
** 日期: 2022-04-28 10:03 
** 作者: leo.liu
** 函数作用：读取IO电平
** 返回参数说明：在读取IO电平之前,需要调用 gpio_direction_set 
***/
bool gpio_level_read(const int pin, GPIO_LEVEL * level);

#endif