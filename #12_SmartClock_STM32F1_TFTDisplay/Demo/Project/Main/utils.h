#pragma once

#include "sys.h"
#include "stdbool.h"

typedef struct
{
	int16_t x;
	int16_t y;
} vec;
vec vec_inst( int16_t x, int16_t y );
void vec_copy( vec* own, vec other );
void vec_add( vec* own, vec* other );

typedef struct 
{
	vec topleft;
	vec dimens;
	uint8_t scale;
	const Tie_ANIM* sAnim;
	uint16_t anim_cnt;
	uint16_t anim_dura;
	uint8_t anim_idx;
} entity;

void ent_init( entity* ent, const Tie_ANIM* sAnim );
void ent_scale( entity* ent, uint16_t scale );
void ent_update( entity* ent, uint16_t dtime );
void ent_draw( entity* ent );
void ent_draw_at( int16_t x, int16_t y, entity* ent );
void ent_set_idx( entity* ent, uint8_t idx );

void gfx__rect_thin( vec topleft, vec dimens, COLOR color );
void gfx__rect_fill( vec topleft, vec dimens, COLOR color );
// void gfx__line( int16_t x1, int16_t y1, int16_t x2, int16_t y2, COLOR color );
uint16_t util_random( uint16_t begin, uint16_t end );

bool str_to_u64(const char* str, uint64_t* num);

#define bit16Read(value, bit)   (((value) << (bit)) & 0x8000)
	//Read Left -> Right increasing
#define bit16Set(value, bit)    ((value) |= (0x8000 >> (bit)))
#define bit16Clear(value, bit)  ((value) &= ~(0x8000 >> (bit))) 
#define bit16Write(value, bit, bitvalue)  ((bitvalue) ? bit16Set(value, bit) : bit16Clear(value, bit))
