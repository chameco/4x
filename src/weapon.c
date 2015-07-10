#include "weapon.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"

weapon_type WEAPON_TYPES[WEAPON_END_PLACEHOLDER] = {
	[WEAPON_TEST] = {"Test Weapon", 10},
};

void create_weapon(weapon *w, weapon_id t)
{
	if (w == NULL) log_err("Invalid address on weapon creation");
	w->t = t;
}
