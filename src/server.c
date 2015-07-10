#include "server.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"
#include "network.h"
#include "hex.h"
#include "map.h"
#include "action.h"

static bool RUNNING = false;

static network_context CONTEXT;
static network_connection CONN;

void initialize_server()
{
	CONTEXT = make_context();
	CONN = bind_clients(CONTEXT);
}

void set_server_running(bool b)
{
	RUNNING = b;
}

void main_server_loop()
{
	map m;
	create_map(&m, "Test Map");
	create_entity(add_entity(&(m.hexes[1][1])), T_PROGENITOR, 1);
	entity_selector s = {1, 1, 0};
	move_entity(&m, s, 2, 2);
	RUNNING = true;
	action_message buf;
	char data[512] = {0};
	while (RUNNING) {
		SDL_Delay(200);
		pull_input(CONN, &buf, sizeof(buf));
		if (buf.len > 0 && buf.len < 512) {
			pull_input(CONN, data, buf.len);
		}
		dispatch_action(&m, buf, data);
		push_output(CONN, &m, sizeof(m));
	}
}

int main(int argc, char *argv[])
{
	initialize_server();
	main_server_loop();
}
