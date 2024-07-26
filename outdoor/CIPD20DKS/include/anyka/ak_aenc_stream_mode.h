#ifndef _AK_AENC_STREAM_MODE_H_
#define _AK_AENC_STREAM_MODE_H_



enum ak_app_video_error_type
{
	ERROR_APP_AUDIO_WRONG_SRC_MODULE = (MODULE_ID_APP_AUDIO << 24) + 0,
	ERROR_APP_AUDIO_WRONG_MODULE_ID,
	ERROR_APP_AUDIO_INCORRECT_BIND_RELATION,
	ERROR_APP_AUDIO_DST_CHN_MUTLEPLE_BINDED,
	ERROR_APP_AUDIO_DST_CHN_NOT_BINDED,
	ERROR_APP_AUDIO_SRC_HHN_NOT_BINDED,
	ERROR_APP_AUDIO_SRC_CHN_OUTOF_RANGE,
	ERRPR_APP_AUDIO_SRC_DST_CHN_PARAM_NOT_MATCH,
	ERROR_APP_AUDIO_GENERATE_GOBJ_FAILED,
	ERROR_APP_AUDIO_GENERATE_OBJ_FAILED,
	ERROR_APP_AUDIO_ENCODRE_HAS_NO_DATA,
	ERROR_APP_AUDIO_FRAME_COUNT_OUTOF_LIMITATION,
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

int ak_app_audio_set_dst_chn_active(const APP_CHN_S *Chn, int enable);
int ak_app_audio_aenc_get_stream(const APP_CHN_S *Chn, struct audio_stream *stream);
int ak_app_audio_unbind_chn(const APP_CHN_S *pstSrcChn, const APP_CHN_S *pstDestChn);
int ak_app_audio_bind_chn(const APP_CHN_S *pstSrcChn, const APP_CHN_S *pstDestChn, const APP_BIND_PARAM *param);
int ak_app_audio_set_chn_attr(const APP_CHN_S *pstDestChn, const APP_BIND_PARAM *param);
int ak_app_audio_set_cb(const APP_CHN_S *Chn, ak_data_cb fn_cb, void *output);
int ak_app_audio_unset_cb(const APP_CHN_S *Chn);














#endif
