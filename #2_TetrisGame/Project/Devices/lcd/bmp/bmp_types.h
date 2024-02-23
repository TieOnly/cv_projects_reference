#pragma once
#include <stdint.h>

typedef struct
{
	uint8_t w;
	uint16_t amount;
	const uint8_t *bmp;
} Tie_BMP;

typedef struct 
{
	const uint8_t *data;
} Tie_BMP_DATA;

typedef struct 
{
	uint8_t w;
	//amount element each of bmp
	uint16_t amount_b;
	//amount frame of animation
	uint16_t frame;
	//duration and counting active of each frame
	const Tie_BMP_DATA *anim;
} Tie_ANIM;
