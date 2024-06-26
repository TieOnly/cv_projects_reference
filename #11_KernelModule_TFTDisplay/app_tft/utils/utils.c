#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vec vec_inst( int16_t x, int16_t y )
{
	vec v = {.x = x, .y = y};
	return v;
}
void vec_add( vec* own, vec* other )
{
	own->x += other->x;
	own->y += other->y;
}
void vec_copy( vec* own, vec other )
{
	own->x = other.x;
	own->y = other.y;
}


void ent_init( entity* ent, const t_anim* sAnim )
{
	if (ent == NULL) return;

	ent->sAnim = sAnim;
	ent->topleft = vec_inst(0, 0);

	if ( sAnim != NULL )
		ent->dimens = vec_inst(sAnim->w, sAnim->h);
	else
		ent->dimens = vec_inst(0, 0);

	ent->anim_cnt = 0;
	ent->anim_idx = 0;
	ent->anim_dura = 150;
	ent->scale = 1;
}
void ent_scale( entity* ent, uint8_t scale )
{
	if ( ent->sAnim == NULL ) return;
	ent->scale = scale;
	ent->dimens = vec_inst( 
		scale*(ent->sAnim->w), 
		scale*(ent->sAnim->h)
	);
}
void ent_update( entity* ent, uint32_t dtime )
{
	if ( ent->sAnim == NULL ) return;
	ent->anim_cnt += dtime;

	if( ent->anim_cnt >= ent->anim_dura )
	{
		ent->anim_cnt %= ent->anim_dura;
		ent->anim_idx += 1;
		ent->anim_idx %= ent->sAnim->frame;
	}
}
void ent_draw( entity* ent )
{
	if ( ent->sAnim == NULL ) return;
	gfx__draw_pixels(
		ent->topleft, ent->sAnim->w,
		ent->sAnim->anim[ent->anim_idx].pixels, ent->sAnim->len_of_frame, ent->scale
	);
}
void ent_draw_at( vec topleft, entity* ent )
{
	if ( ent->sAnim == NULL ) return;
	gfx__draw_pixels(
		topleft, ent->sAnim->w,
		ent->sAnim->anim[ent->anim_idx].pixels, ent->sAnim->len_of_frame, ent->scale
	);
}
void ent_set_idx( entity* ent, uint8_t idx )
{
	if ( idx >= ent->sAnim->frame ) return;
	ent->anim_idx = idx;
}

void gfx__rect_thin( vec topleft, vec dimens, uint16_t color )
{
	for ( int i = topleft.x; i <= (topleft.x + dimens.x); ++i )
	{
		tft_draw_point(i, topleft.y, color);
		tft_draw_point(i, topleft.y + dimens.y, color);
	}
	for ( int i = topleft.y; i <= (topleft.y + dimens.y); ++i )
	{
		tft_draw_point(topleft.x, i, color);
		tft_draw_point(topleft.x + dimens.x, i, color);
	}
}
void gfx__rect_fill( vec topleft, vec dimens, uint16_t color )
{
	for ( int i = topleft.y; i < (topleft.y + dimens.y); ++i )
		for ( int j = topleft.x; j < (topleft.x + dimens.x); ++j )
			tft_draw_point(j, i, color);
}
void gfx__draw_pixels( vec topleft, uint16_t width, const uint16_t* pixels, uint64_t len, uint8_t scale )
{
	int16_t x = topleft.x;
	int16_t y = topleft.y;
	uint32_t idx = 0;
	uint32_t idy = 0;
	for( uint32_t i = 0; i < len; ++i, ++idx )
	{
		if( idx >= width ) { idy++; idx = 0; }

		if (*(pixels+i) != COLOR_BG)
		{
			scale == 1 
			? tft_draw_point(x+idx, y+idy, *(pixels+i)) 
			: tft_draw_point_scale(x+(idx*scale), y+(idy*scale), *(pixels+i), scale);
		}
	}
}
uint16_t util_random( uint16_t begin, uint16_t end )
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	uint32_t ms = ts.tv_nsec / 1e6;
	return ( begin + ((end-begin)*ms)/1000 );
}
uint32_t tick_dtime()
{
	static uint32_t ms_last = 0;
	static uint32_t ms_old = 0;

	ms_old = ms_last;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ms_last = ts.tv_nsec / 1e6;

	if (ms_last < ms_old)
		return (1000+ms_last)-ms_old;
	return ms_last-ms_old;
}
static struct signal_ctrl_t
{
	bool val[S_CTRL_COUNT];
	pthread_mutex_t mtx;
} signal_ctrl;
void signal_ctrl_set( t_ctrl ctrl )
{
	printf("set signal %d\n", (int)ctrl);
	signal_ctrl.val[ctrl] = 1;
}
bool signal_ctrl_get( t_ctrl ctrl )
{
	return signal_ctrl.val[ctrl];
}
void signal_ctrl_clear()
{
	for (int i = 0; i < S_CTRL_COUNT; ++i)
		signal_ctrl.val[i] = 0;
}
void signal_mtx_lock()
{
	pthread_mutex_lock(&signal_ctrl.mtx);
}
void signal_mtx_unlock()
{
	pthread_mutex_unlock(&signal_ctrl.mtx);
}

bool str_to_u64(const char* str, uint64_t* num)
{
	if (strlen(str) == 0) return false;
	*num = 0;
	int i = 0;
	while (str[i] != '\0' && i <= 10)
	{
		if (str[i] < '0' || str[i] > '9') return false;
		*num *= 10;
		*num += str[i] - '0';
		i++;
	}
	return true;
}

static struct assets_t
{
	pool_t tetris;
} assets;

void assets_load()
{
	pool_create(&assets.tetris, 100, sizeof(t_anim));
	pool_add(&assets.tetris, load_anim(IDA_CLOUD, "./assets/cloud.png", 2), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_YTB, "./assets/ytb.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_TITLE, "./assets/tet_title.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_START_BTN, "./assets/start_btn.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_OPT_BTN, "./assets/opt_btn.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_BACK_BTN, "./assets/back_btn.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_FRAME_BTN, "./assets/frame_btn.png", 1), POOL_PACK_FROM_VAR);
	pool_add(&assets.tetris, load_anim(IDA_TET_ARROW, "./assets/arrow.png", 2), POOL_PACK_FROM_VAR);
}
void* _anim_by_id(ppack_t pack, void* args)
{
	SPREAD_VA_LIST( *(va_list*)args,
		id, id_anim
	);
	t_anim* anim = (t_anim*)pack;
	if (anim->id == id) return anim;
	return NULL;
}
t_anim* assets_tetris_get( id_anim id )
{
	t_anim* anim = (t_anim*)pool_for_each(&assets.tetris, _anim_by_id, id);
	if (anim == NULL) printf("Asset get fail (id)\n");
	return anim;
}

t_anim* load_anim( id_anim id, const char* filename, int num_of_crop )
{
	int width, height, channels;
	uint8_t *image_data = stbi_load(filename, &width, &height, &channels, 0);
	if (image_data == NULL) 
	{
		printf("Load animation fail (file error)\n");
		return NULL;
	}

	int w_frame = width / num_of_crop;


	t_anim* anim = malloc(sizeof(t_anim));
	anim->w = w_frame;
	anim->h = height;
	anim->len_of_frame = w_frame*height;
	anim->frame = num_of_crop;
	anim->id = id;
	anim->anim = malloc(num_of_crop*sizeof(t_bmp_data));

	for ( int i_frame = 0; i_frame < num_of_crop; ++i_frame )
	{
		uint16_t* pixels = malloc(anim->len_of_frame*sizeof(uint16_t));
		int p_len = 0;

		for ( int y = 0; y < height; ++y )
		{
			for ( int x = (i_frame*w_frame); x < ((i_frame+1)*w_frame); ++x )
			{
				int p_index = (y * width + x) * channels;

				// uint8_t r = (image_data[p_index+0] >> 3) & 0x1f;
				// uint8_t g = (image_data[p_index+1] >> 2) & 0x3f;
				// uint8_t b = (image_data[p_index+2] >> 3) & 0x1f;
				uint8_t r = image_data[p_index+0];
				uint8_t g = image_data[p_index+1];
				uint8_t b = image_data[p_index+2];
				// pixels[p_len++] = (r << 11) + (g << 5) + b;
				pixels[p_len++] = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
			}
		}

		anim->anim[i_frame].pixels = pixels;
	}
	// printf("Loading: %d %d %d %d\n", width, height, channels, w_frame);
	return anim;
}