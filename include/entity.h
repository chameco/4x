#pragma once

#include <stdbool.h>

#include "texture.h"

typedef enum entity_id {
	ENTITY_TEST=0,
	ENTITY_END_PLACEHOLDER
} entity_id;

typedef struct entity_type {
	char name[256];
	char path[256];
	int w, h;
	texture *t;
} entity_type;

typedef struct entity {
	entity_id t;
} entity;

void create_entity(entity *h, entity_id t);
void draw_entity(entity *h, double x, double y);
