#pragma once

typedef enum armor_id {
	ARMOR_TEST=0,
	ARMOR_END_PLACEHOLDER
} armor_id;

typedef struct armor_type {
	char name[256];
	int resistance;
} armor_type;

typedef struct armor {
	armor_id t;
} armor;

void create_armor(armor *w, armor_id t);
