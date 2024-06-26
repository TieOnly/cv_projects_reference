#include "utils.h"

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


void ent_init( entity* ent, const Tie_ANIM* sAnim )
{
	ent->sAnim = sAnim;
	ent->topleft = vec_inst(0, 0);

	if ( sAnim != NULL )
		ent->dimens = vec_inst(sAnim->w, (8*sAnim->amount_b)/sAnim->w);
	else
		ent->dimens = vec_inst(0, 0);

	ent->anim_cnt = 0;
	ent->anim_idx = 0;
	ent->anim_dura = 150;
	ent->scale = 1;
}
void ent_scale( entity* ent, uint16_t scale )
{
	if ( ent->sAnim == NULL ) return;
	ent->scale = scale;
	ent->dimens = vec_inst( 
		scale*(ent->sAnim->w), 
		scale*((8*ent->sAnim->amount_b)/ent->sAnim->w)
	);
}
void ent_update( entity* ent, uint16_t dtime )
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
	gfx_draw_by_arr (
		ent->topleft.x, ent->topleft.y, ent->sAnim->w,
		ent->sAnim->anim[ent->anim_idx].data, ent->sAnim->amount_b,
		COLOR_BG, ent->scale
	);
}
void ent_draw_at( int16_t x, int16_t y, entity* ent )
{
	if ( ent->sAnim == NULL ) return;
	gfx_draw_by_arr (
		x, y, ent->sAnim->w,
		ent->sAnim->anim[ent->anim_idx].data, ent->sAnim->amount_b,
		COLOR_BG, ent->scale
	);
}
void ent_set_idx( entity* ent, uint8_t idx )
{
	if ( idx >= ent->sAnim->frame ) return;
	ent->anim_idx = idx;
}

void gfx__rect_thin( vec topleft, vec dimens, COLOR color )
{
	for ( int i = topleft.x; i <= (topleft.x + dimens.x); ++i )
	{
		gfx_drawpixel(i, topleft.y, color);
		gfx_drawpixel(i, topleft.y + dimens.y, color);
	}
	for ( int i = topleft.y; i <= (topleft.y + dimens.y); ++i )
	{
		gfx_drawpixel(topleft.x, i, color);
		gfx_drawpixel(topleft.x + dimens.x, i, color);
	}
}
void gfx__rect_fill( vec topleft, vec dimens, COLOR color )
{
	for ( int i = topleft.y; i < (topleft.y + dimens.y); ++i )
		for ( int j = topleft.x; j < (topleft.x + dimens.x); ++j )
			gfx_drawpixel(j, i, color);
}
uint16_t util_random( uint16_t begin, uint16_t end )
{
	if ( begin > end ) return 0;
	return ( begin + ((end-begin)*ft_dtime_micro())/100 );
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
