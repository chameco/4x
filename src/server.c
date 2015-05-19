#include "server.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"
#include "network.h"
#include "hex.h"
#include "map.h"

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
	add_entity(&(m.hexes[1][1]), ENTITY_TEST);
	RUNNING = true;
	while (RUNNING) {
		push_output(CONN, &m, sizeof(m));
	}
}

int main(int argc, char *argv[])
{
	initialize_server();
	main_server_loop();
}
