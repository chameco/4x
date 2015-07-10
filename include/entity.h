#pragma once

#include <stdbool.h>

#include "texture.h"
#include "weapon.h"
#include "armor.h"

typedef enum entity_id {
	ENTITY_TEST=0,
	M_PROGENITOR,
	MT_PROGENITOR,
	TM_PROGENITOR,
	T_PROGENITOR,
	TP_PROGENITOR,
	PT_PROGENITOR,
	P_PROGENITOR,
	PM_PROGENITOR,
	MP_PROGENITOR,
	ENTITY_END_PLACEHOLDER
} entity_id;

typedef enum entity_tag_primary {
	PROGENITOR=0,
	CITY,
	STRUCTURE,
	BIOLOGICAL,
	MECHANICAL,
	INCORPOREAL,
	ENTITY_TAG_PRIMARY_PLACEHOLDER
} entity_tag_primary;

typedef enum entity_tag_secondary {
	NONE=0,
	BUILDER,
	INFANTRY,
	LIGHT,
	HEAVY,
	ENTITY_TAG_SECONDARY_PLACEHOLDER
} entity_tag_secondary;

typedef struct entity_type {
	char name[256];
	char path[256];
	int w, h;
	texture *t;
} entity_type;

typedef struct entity {
	entity_id t;
	int count;
	int current_health;
	weapon w;
	armor a;
} entity;

void create_entity(entity *h, entity_id t, int count);
void draw_entity(entity *h, double x, double y);
