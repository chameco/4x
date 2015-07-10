#pragma once

#include <stdbool.h>

#include "hex.h"

#define MAP_DIM 128

typedef struct entity_selector {
	int x, y;
	int index;
} entity_selector;

typedef struct map {
	char name[256];
	hex hexes[MAP_DIM][MAP_DIM];
} map;

void create_map(map *m, char *name);
bool is_adjacent(int x1, int y1, int x2, int y2);
entity *pick_entity(map *m, entity_selector s);
void move_entity(map *m, entity_selector s, int x, int y);
void draw_map(map *m);
