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
	[ENTITY_TEST] = {"Test Entity", "assets/textures/wizard.png", 32, 32, NULL},
};

void create_entity(entity *e, entity_id t)
{
	if (e == NULL) log_err("Invalid address on entity creation");
	e->t = t;
}

void draw_entity(entity *e, double x, double y)
{
	if (ENTITY_TYPES[e->t].t == NULL) {
		ENTITY_TYPES[e->t].t = load_texture(ENTITY_TYPES[e->t].path, ENTITY_TYPES[e->t].w, ENTITY_TYPES[e->t].h);
	}
	draw_texture(ENTITY_TYPES[e->t].t, x, y);
}
