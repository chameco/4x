#pragma once

#include <stdbool.h>

#include "texture.h"
#include "entity.h"

#define HEX_DIM 128

typedef enum hex_id {
	HEX_EMPTY=0,
	HEX_END_PLACEHOLDER
} hex_id;

typedef struct hex_type {
	char name[256];
	char path[256];
	int move;
	texture *t;
} hex_type;

typedef struct hex {
	hex_id t;
	int entity_count;
	entity entities[16];
} hex;

void create_hex(hex *h, hex_id t);
entity *add_entity(hex *h);
void draw_hex(hex *h, double x, double y);
