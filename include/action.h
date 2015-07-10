#pragma once

#include <stdbool.h>

#include "map.h"

typedef enum action {
	ACTION_LOGIN=0,
	ACTION_MOVE,
} action;

typedef struct action_message {
	entity_selector es;
	action a;
	int len;
} action_message;

typedef struct action_move_data {
	int x, y;
} action_move_data;

void dispatch_action(map *m, action_message a, void *data);
