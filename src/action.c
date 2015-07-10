#include "action.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"
#include "map.h"

void dispatch_action(map *m, action_message a, void *data)
{
	switch (a.a) {
		case ACTION_LOGIN: // handled in main server loop
			break;
		case ACTION_MOVE:
			move_entity(m, a.es, ((action_move_data *) data)->x, ((action_move_data *) data)->y);
			break;
	}
}
