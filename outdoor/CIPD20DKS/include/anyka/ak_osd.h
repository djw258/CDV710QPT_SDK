#ifndef _AK_OSD_H_
#define _AK_OSD_H_

#include "ak_vi.h"

/* 驱动色彩模式，分为RGB和YUV编码格式，目前只支持YUV */
enum color_mode
{
    AK_OSD_COLOR_FMT_YUV = 0,
    AK_OSD_COLOR_FMT_RGB,
    AK_OSD_COLOR_FMT_SUPPORT_NUM
};

enum ak_osd_errno {
    ERROR_OSD_NOT_INIT                              = ( MODULE_ID_OSD << 24 ) + 0,
    ERROR_OSD_CREATE_CANVAS_FAILED					= ( MODULE_ID_OSD << 24 ) + 1,
    ERROR_OSD_SET_CANVAS_SIZE_FAILED                = ( MODULE_ID_OSD << 24 ) + 2,

};


/* osd 属性初始化设置结构参数 
 *
 * 注：若不使用自定义颜色表，请将
 *     color_table设置为NULL。
 */
struct osd_attr
{
    enum color_mode color_mode_index;	// 色彩模式值，根据枚举值选
    unsigned int *color_table;					// 色彩映射表，
};

/* 像素图位置坐标 */
struct vector
{
    int x;
    int y;
};

/* 画板创建属性参数 */
struct osd_canvas_info
{
    int channel_id;             // 通道编号
    int region_id;              // 画布区域编号
    struct vector pos_in_yuv;   // 在yuv图像上的位置，单位为像素
    int region_size_height;		// 画布高度，单位像素
    int region_size_width;		// 画布宽度，单位像素
    int canvas_color_index;		// 画布的背景颜色
    int alpha;				    // 画布的透明度
};

/* 画出的点阵位图属性参数 */
struct osd_drawing_info
{
    int channel_id;               // 目标vi通道编号
    int region_id;                // 目标画布区域编号

    const unsigned char *buffer;  // 点阵数据
    int buffer_size;			  // 点阵数据的大小，byte
    int bitmap_size_height;		  // 点阵高度，单位像素
    int bitmap_size_width;		  // 点阵宽度，单位像素

    struct vector pos_in_region;  // 在画布区域上的位置，单位为像素
};

/**
 * ak_osd_get_version - get the version of osd module.
 * return: a string parameter;
 */
const char* ak_osd_get_version(void);

/**
 * ak_osd_init - init osd para, include screen resolution,
 *			default color table and color id ;malloc osd buf.
 * @osd_attr_value[IN]: a pointer contains the attribute of OSD
 * return: 0 for success; other value for failed;
 */
int ak_osd_init(struct osd_attr *osd_attr_value);

/**
 * ak_osd_create_canvas - create the destinational canvas on screen.
 * @canvas_info[IN]: a handle include the all information to create the 
 *                   canvas.That struct contains the attribute of canvas to be create.
 * return: 0 for success; other value for failed;
 */
int ak_osd_create_canvas(struct osd_canvas_info *canvas_info);

/**
 * ak_osd_draw_bitmap_to_canvas - draw the bitmap data to the destinational region on screen.
 * @draw_info[IN]: a handle saves the information to drawing the matrix data to destinational
 *                 canvas created by calling the function above.
 * return: 0 - success; other value for failed;
 */
int ak_osd_draw_bitmap_to_canvas(struct osd_drawing_info *draw_info);

/**
 * ak_osd_set_to_vi - set all completed regions to the driver.
 * return: 0 - success; other value for failed;
 */
int ak_osd_set_to_vi(void);

/**
 * ak_osd_set_alpha - set alpha to the dst region.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * @alpha[IN]: alpha value from 0 to 100;
 * return: 0 for success; other value for failed;
 */
int ak_osd_set_alpha(int channel_id, int region_id, int alpha);

/**
 * ak_osd_set_canvas_color - set canvas color.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * @bg_color[IN]: bg color index;
 * return: 0 for success; other value for failed;
 */
int ak_osd_set_canvas_color(int channel_id, int region_id, int bg_color);

/**
 * ak_osd_set_canvas_position - set canvas position.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * @x_pos_in_yuv[IN]: position in width;
 * @y_pos_in_yuv[IN]: position in height;
 * return: 0 for success; other value for failed;
 */
int ak_osd_set_canvas_position(int channel_id, int region_id, int x_pos_in_yuv, int y_pos_in_yuv);

/**
 * ak_osd_set_canvas_size - set canvas size.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * @height[IN]: size in width;
 * @width[IN]: size in height;
 * return: 0 for success; other value for failed;
 */
int ak_osd_set_canvas_size(int channel_id, int region_id, int height, int width);

/**
 * ak_osd_clean_canvas - clean canvas context.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * return: 0 for success; other value for failed;
 */
int ak_osd_clean_canvas(int channel_id, int region_id);

/**
 * ak_osd_set_canvas_enable - enable or disable the canvas display.
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * @enable[IN]: 0-disble, 1-enable;
 * return: 0 for success; other value for failed;
 */
int ak_osd_set_canvas_enable(int channel_id, int region_id, int enable);

/**
 * ak_osd_destroy_canvas - destroy the display region 
 * @channel[IN]: channel number;
 * @region_id[IN]: region index;
 * return: 0 - success; other value for failed;
 */
int ak_osd_destroy_canvas(int channel_id, int region_id);

/**
 * ak_osd_uninit - uninit the osd
 * return: 0 for success; other value for failed;
 */
int ak_osd_uninit(void);

#endif

