#include "map.h"

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
#include "hex.h"

void create_map(map *m, char *name)
{
	if (m == NULL) log_err("Invalid address on map creation");
	strncpy(m->name, name, 256);
	for (int ix = 0; ix < MAP_DIM; ++ix) {
		for (int iy = 0; iy < MAP_DIM; ++iy) {
			create_hex(&(m->hexes[ix][iy]), HEX_EMPTY);
		}
	}
}

bool is_adjacent(int x1, int y1, int x2, int y2)
{
	if (x1 % 2 != 0) {
		return
			(x1 - 1 == x2 && y1 == y2) ||
			(x1 == x2 && y1 - 1 == y2) ||
			(x1 + 1 == x2 && y1 == y2) ||
			(x1 - 1 == x2 && y1 + 1 == y2) ||
			(x1 == x2 && y1 + 1 == y2) ||
			(x1 + 1 == x2 && y1 + 1 == y2);
	} else {
		return
			(x1 - 1 == x2 && y1 - 1 == y2) ||
			(x1 == x2 && y1 - 1 == y2) ||
			(x1 + 1 == x2 && y1 == y2) ||
			(x1 - 1 == x2 && y1 == y2) ||
			(x1 == x2 && y1 + 1 == y2) ||
			(x1 + 1 == x2 && y1 - 1 == y2);
	}
}

entity *pick_entity(map *m, entity_selector s)
{
	return &(m->hexes[s.x][s.y].entities[s.index]);
}

void move_entity(map *m, entity_selector s, int x, int y)
{
	if (is_adjacent(s.x, s.y, x, y)) {
		entity *src = pick_entity(m, s);
		entity *dest = add_entity(&(m->hexes[x][y]));
		memcpy(dest, src, sizeof(entity));
		m->hexes[s.x][s.y].entity_count--;
		memmove(src, src + sizeof(entity), (16 - s.index - 1) * sizeof(entity));
	}
}

void draw_map(map *m)
{
	int ix, iy;
	for (ix = 0; ix < MAP_DIM; ++ix) {
		for (iy = 0; iy < MAP_DIM; ++iy) {
			draw_hex(&(m->hexes[ix][iy]), ix*(HEX_DIM-32), iy*(HEX_DIM-17) + (ix % 2 == 0 ? 0 : 64 - 8));
		}
	}
	for (ix = 0; ix < MAP_DIM; ++ix) {
		for (iy = 0; iy < MAP_DIM; ++iy) {
			for (int i = 0; i < m->hexes[ix][iy].entity_count; ++i) {
				draw_entity(&(m->hexes[ix][iy].entities[i]), ix*(HEX_DIM-32) + 32, iy*(HEX_DIM-17) + (ix % 2 == 0 ? 0 : 64 - 8) + 32);
			}
		}
	}
}
