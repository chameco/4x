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

void draw_map(map *m)
{
	for (int ix = 0; ix < MAP_DIM; ++ix) {
		for (int iy = 0; iy < MAP_DIM; ++iy) {
			draw_hex(&(m->hexes[ix][iy]), ix*(HEX_DIM-32), iy*(HEX_DIM-17) + (ix % 2 == 0 ? 0 : 64 - 8));
		}
	}
}
