#pragma once

typedef enum weapon_id {
	WEAPON_TEST=0,
	WEAPON_END_PLACEHOLDER
} weapon_id;

typedef struct weapon_type {
	char name[256];
	int power;
} weapon_type;

typedef struct weapon {
	weapon_id t;
} weapon;

void create_weapon(weapon *w, weapon_id t);
