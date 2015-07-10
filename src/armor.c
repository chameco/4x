#include "armor.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"

armor_type ARMOR_TYPES[ARMOR_END_PLACEHOLDER] = {
	[ARMOR_TEST] = {"Test Armor", 10},
};

void create_armor(armor *a, armor_id t)
{
	if (a == NULL) log_err("Invalid address on armor creation");
	a->t = t;
}
