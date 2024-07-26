#ifndef _AK_APP_MEM_EX_H
#define _AK_APP_MEM_EX_H
/*
 * The APP mem lib is based on the V3.0.16 and above version of libakv_encode.a/so.
 */

#include "ak_common.h"
#include "ak_common_video.h"
#include "ak_vi.h"

enum ak_app_mem_error_type
{
	ERROR_APP_MEM_IS_ALREADY_ACTIVATED	=	(MODULE_ID_APP_MEM << 24) + 0,
};

struct vi_policy{
    int     crop_width;					/* crop width of sensor*/
    int     crop_height;				/* crop height of sensor */
    		
	int		main_max_width;				/* width of main chn */
	int		main_max_height;			/* height of main chn  */
    int		main_frame_depth;			/* frame buf depth of main chn */
    
	int		sub_max_width;				/* width of sub chn */
	int		sub_max_height;				/* height of sub chn  */
	int		sub_frame_depth;			/* frame buf depth of sub chn */ 

    int		thr_max_width;      		/*if no need third channel , set 0.*/
	int		thr_max_height;     		/*if no need third channel , set 0.*/
	int		thr_frame_depth;    		/*if no need third channel , set 0.*/

    void    *info;  					/* reserved */
};

struct venc_policy{
    enum video_channel video_chn_id;	/* video channel id*/
    int		width;						/* encoder width */
    int		height;						/* encoder height */
    enum encode_output_type type;		/* encode type */
    int		smart_mode;					/* smart mode for encoder */
    int     max_picture_size;   		/* kbits. 0 means default.*/
    int     roi_en;						/* ROI enable flag */
    void    *info;  					/* reserved */
};

/**
 * ak_app_mem_ex_get_version - get app_mem_ex version
 * return: version string
 * notes: 
 */
const char* ak_app_mem_ex_get_version(void);


/* ak_app_mem_ex_dma_pool_set_vi_policy 	-- set vi policy
 * policy[IN] :	vi policy 
 * return AK_SUCCESS if success, Error Code if failed
 */
int ak_app_mem_ex_dma_pool_set_vi_policy(struct vi_policy *policy);


/* ak_app_mem_ex_dma_pool_set_venc_policy 	-- set venc policy
 * policy[IN] :	venc policy 
 * return AK_SUCCESS if success, Error Code if failed
 */
int ak_app_mem_ex_dma_pool_set_venc_policy(struct venc_policy *policy);

/* ak_app_mem_ex_dma_pool_activate 	-- init the dma memory pool
 * void
 * return AK_SUCCESS if success, Error Code if failed
 */
int ak_app_mem_ex_dma_pool_activate(void);

/* ak_app_mem_ex_dma_pool_exit 	-- exit the dma memory pool
 * void
 * return AK_SUCCESS if success, Error Code if failed
 */
int ak_app_mem_ex_dma_pool_exit(void);


#endif
