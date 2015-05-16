#pragma once

#include <stdbool.h>

#include "hex.h"

#define MAP_DIM 128

typedef struct map {
	char name[256];
	hex hexes[MAP_DIM][MAP_DIM];
} map;

void create_map(map *m, char *name);

void draw_map(map *m);
