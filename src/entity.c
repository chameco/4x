#include "entity.h"

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

entity_type ENTITY_TYPES[ENTITY_END_PLACEHOLDER] = {
	[ENTITY_TEST] = {"Test Entity", "assets/textures/entities/star.png", 128, 128, NULL},
	// PROGENITOR UNITS - These are the initial settlers
	[M_PROGENITOR] = {"Research Vessel", "assets/textures/entities/m/progenitor.png", 64, 64, NULL},
	[MT_PROGENITOR] = {"Clockwork Ark", "assets/textures/entities/mt/progenitor.png", 64, 64, NULL},
	[TM_PROGENITOR] = {"Ancient Beacon", "assets/textures/entities/tm/progenitor.png", 64, 64, NULL},
	[T_PROGENITOR] = {"Network Probe", "assets/textures/entities/t/progenitor.png", 64, 64, NULL},
	[TP_PROGENITOR] = {"Transport Hulk", "assets/textures/entities/tp/progenitor.png", 64, 64, NULL},
	[PT_PROGENITOR] = {"Gray Goo", "assets/textures/entities/pt/progenitor.png", 64, 64, NULL},
	[P_PROGENITOR] = {"Nomadic Tribe", "assets/textures/entities/p/progenitor.png", 64, 64, NULL},
	[PM_PROGENITOR] = {"Bones of the Long Dead", "assets/textures/entities/pm/progenitor.png", 64, 64, NULL},
	[MP_PROGENITOR] = {"Wandering Conclave", "assets/textures/entities/mp/progenitor.png", 64, 64, NULL},
};

char *ENTITY_TAG_PRIMARY_NAMES[ENTITY_TAG_PRIMARY_PLACEHOLDER] = {
	[PROGENITOR] = "Progenitor",
	[CITY] = "City",
	[STRUCTURE] = "Structure",
	[BIOLOGICAL] = "Biological",
	[MECHANICAL] = "Mechanical",
	[INCORPOREAL] = "Incorporeal",
};

void create_entity(entity *e, entity_id t, int count)
{
	if (e == NULL) log_err("Invalid address on entity creation");
	e->t = t;
	e->count = count;
}

void draw_entity(entity *e, double x, double y)
{
	if (ENTITY_TYPES[e->t].t == NULL) {
		ENTITY_TYPES[e->t].t = load_texture(ENTITY_TYPES[e->t].path, ENTITY_TYPES[e->t].w, ENTITY_TYPES[e->t].h);
	}
	draw_texture(ENTITY_TYPES[e->t].t, x, y);
}
