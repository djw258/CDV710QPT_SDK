#ifndef _AK_VIDEO_H
#define _AK_VIDEO_H

#include "ak_common.h"
#include "ak_venc.h"

enum ak_app_video_error_type
{
	ERROR_APP_VIDEO_WRONG_SRC_MODULE = (MODULE_ID_APP_VIDEO << 24) + 0,
	ERROR_APP_VIDEO_WRONG_MODULE_ID,
	ERROR_APP_VIDEO_INCORRECT_BIND_RELATION,
	ERROR_APP_VIDEO_DST_CHN_MUTLEPLE_BINDED,
	ERROR_APP_VIDEO_DST_CHN_NOT_BINDED,
	ERROR_APP_VIDEO_SRC_HHN_NOT_BINDED,
	ERROR_APP_VIDEO_SRC_CHN_OUTOF_RANGE,
	ERRPR_APP_VIDEO_SRC_DST_CHN_PARAM_NOT_MATCH,
	ERROR_APP_VIDEO_GENERATE_GOBJ_FAILED,
	ERROR_APP_VIDEO_GENERATE_OBJ_FAILED,
	ERROR_APP_VIDEO_ENCODRE_HAS_NO_DATA,
	ERROR_APP_VIDEO_FRAME_COUNT_OUTOF_LIMITATION,
	ERROR_APP_VIDEO_DST_CHN_NOT_DISABLED,
};

typedef struct ak_app_chn_s
{
	enum module_id	mid;			/* module id  */
	int				oid;			/* object id, chn id for vi, encoder id for venc, decoder id for vdec, layer id for vo */
}APP_CHN_S;

typedef struct ak_app_bind_param
{
	int 	frame_rate;				/* frame rate control */
	int 	frame_depth;			/* frame list max length*/
//	int		frame_count;			/* frame count */
}APP_BIND_PARAM;

/* define the callback to handle the attr chn data */
typedef void (*ak_data_cb)(APP_CHN_S *Chn, void *input, void *output);

/**
 * ak_app_video_get_version - get version
 * return: version string
 */
const char* ak_app_video_get_version(void);

/* ak_app_video_set_dst_chn_active - set stat to dst chn
 * @Chn[IN]			:	dst info of bind obj
 * @enable[IN]		:	activate status of Chn
 * return AK_SUCCESS is successful,  error code otherwise.
 * */
int ak_app_video_set_dst_chn_active(const APP_CHN_S *Chn, int enable);

/* ak_app_video_venc_get_stream	--	get the venc stream from video lib
 * @Chn[IN]		:	dst info of bind obj
 * @stream[OUT] :   output the stream
 * return AK_SUCCESS is successful,  error code otherwise.
 */
int ak_app_video_venc_get_stream(const APP_CHN_S *Chn, struct video_stream *stream);

/* ak_app_video_unbind_chn	--	chn bind function for video
 * @pstSrcChn[IN]	:	src info of bind obj
 * @pstDestChn[IN]	:	dest info of bind obj
 * return AK_SUCCESS is successful,  error code otherwise.
 */
int ak_app_video_unbind_chn(const APP_CHN_S *pstSrcChn, const APP_CHN_S *pstDestChn);

/* ak_app_video_bind_chn	--	chn bind function for video
 * @pstSrcChn[IN]	:	src info of bind obj
 * @pstDestChn[IN]	:	dest info of bind obj
 * @param[IN]		:	param of bind obj
 * return AK_SUCCESS is successful,  error code otherwise.
 */
int ak_app_video_bind_chn(const APP_CHN_S *pstSrcChn, const APP_CHN_S *pstDestChn, const APP_BIND_PARAM *param);

/* ak_app_video_set_cb	--	set the callback to handle the chn video data
 * @Chn[IN]	    :	dest src obj
 * @fn_cb[IN]	:	func to handle the data
 * @output[IN]		:	param of output for use
 * return AK_SUCCESS is successful,  error code otherwise.
 */
int ak_app_video_set_cb(const APP_CHN_S *Chn, ak_data_cb fn_cb, void *output);

/* ak_app_video_unset_cb	--	unset the callback func
 * @Chn[IN]	    :	dest src obj
 * return AK_SUCCESS is successful,  error code otherwise.
 */
int ak_app_video_unset_cb(const APP_CHN_S *Chn);

/* ak_app_video_set_chn_attr	--	set the dst chn attr
 * @pstDestChn[IN]	    :	dest chn
 * @param[IN]	        :	dest param to set
 * return AK_SUCCESS is successful,  error code otherwise.
 * NOTE : This api is only to modify the frame rate output from vi. for the venc wanted, call the  corresponding venc interface
 */
int ak_app_video_set_chn_attr(const APP_CHN_S *pstDestChn, const APP_BIND_PARAM *param);

#endif
