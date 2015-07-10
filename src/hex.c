#include "hex.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"
#include "texture.h"
#include "entity.h"

hex_type HEX_TYPES[HEX_END_PLACEHOLDER] = {
	[HEX_EMPTY] = {"Empty", "assets/textures/hexes/hex.png", 0, NULL},
};

void create_hex(hex *h, hex_id t)
{
	if (h == NULL) log_err("Invalid address on hex creation");
	h->t = t;
	h->entity_count = 0;
	memset(h->entities, 0, sizeof(h->entities));
}

entity *add_entity(hex *h)
{
	return &(h->entities[h->entity_count++]);
}

void draw_hex(hex *h, double x, double y)
{
	if (HEX_TYPES[h->t].t == NULL) {
		HEX_TYPES[h->t].t = load_texture(HEX_TYPES[h->t].path, HEX_DIM, HEX_DIM);
	}
	draw_texture(HEX_TYPES[h->t].t, x, y);
}
