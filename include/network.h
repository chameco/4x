#pragma once

typedef struct network_context { //could just use typedef to void*, might want more fields later
	void *context;
} network_context;

typedef struct network_socket {
	void *socket;
} network_socket;

typedef struct network_connection {
	network_socket outgoing;
	network_socket incoming;
} network_connection;

network_context make_context();
network_socket make_socket(network_context c, int type);
void set_socket_topic(network_socket s, char *topic);
void set_socket_topic_all(network_socket s);
void socket_connect(network_socket s, char *addr);
void socket_bind(network_socket s, char *addr);
void socket_send(network_socket s, void *msg, int len);
void socket_recieve(network_socket s, void *msg, int len);

network_connection connect_to_server(network_context c, char *addr);
network_connection bind_clients(network_context c);

void push_output(network_connection c, void *msg, int len);
void pull_input(network_connection c, void *msg, int len);
