

#ifndef _USER_COMMON_H_
#define _USER_COMMON_H_
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "ssd20x/mi_sys.h"
#include "ssd20x/mi_gfx.h"
#include "ssd20x/mi_panel.h"
#include "ssd20x/mi_divp_datatype.h"
#include "ssd20x/mi_divp.h"
#include "ssd20x/mi_disp_datatype.h"
#include "ssd20x/mi_disp.h"

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 食品输入端口属性设置
 * @return:
 */
bool platform_video_port_init(int src_w, int src_h);

/*
 * @日期: 2022-09-03
 * @作者: leo.liu
 * @功能: 绑定端口
 * @return:
 */
bool platform_video_port_bind(void);

/*
* @日期: 2022-09-03
* @作者: leo.liu
* @功能: disabl video port
* @return: 
*/
bool platform_video_layer_disable(void);

/*
 * @日期: 2022-09-03
 * @作者: leo.liu
 * @功能: 取消绑定
 * @return:
 */
bool platform_video_port_unbind(void);

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 获取线程栈内存大小
 * @return:
 */
pthread_attr_t * sat_pthread_attr_get(void);

/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 初始化平台
 * @return:
 */
bool platform_sdk_init(void);

/***********************************************
** 作者: leo.liu
** 日期: 2022-11-21 10:29:2
** 说明: 读取本机的UUID 
***********************************************/
char * platform_read_device_uuid(void);
/*
 * @日期: 2022-08-06
 * @作者: leo.liu
 * @功能: 图形混合
 * @return:
 */
bool platform_gfx_bitblt(int s_x, int s_y, int s_w, int s_h, int src_w, int src_h, int src_stride, MI_PHY src_phy, MI_GFX_ColorFmt_e src_fmt, 
	int 		d_x, int d_y, int d_w, int d_h, int dst_w, int dst_h, int dst_stride, MI_PHY dst_phy, MI_GFX_ColorFmt_e dst_fmt);

/*
 * @日期: 2022-08-08
 * @作者: leo.liu
 * @功能: 填充一个矩形
 * @return:
 */
bool platform_gfx_fill(MI_PHY phy, int x, int y, int w, int h, int dst_w, int dst_h, int stride, MI_GFX_ColorFmt_e fmt, unsigned int color);

/***********************************************
** 作者: leo.liu
** 日期: 2022-10-8 16:16:24
** 说明: rgb888 转yuv420sp
***********************************************/
bool platform_rgb888_to_yuv420sp(const unsigned char * src, unsigned char * dst, int w, int h);

#endif

