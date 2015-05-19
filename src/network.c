#include "network.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include <zmq.h>

#include <cuttle/debug.h>
#include <cuttle/utils.h>

#include "utils.h"

network_context make_context()
{
	network_context c;
	c.context = zmq_ctx_new();
	if (!c.context) {
		log_err("Context creation failed with message '%s'", zmq_strerror(errno));
	}
	return c;
}

network_socket make_socket(network_context c, int type)
{
	network_socket s;
	s.socket = zmq_socket(c.context, type);
	if (!s.socket) {
		log_err("Socket creation failed with message '%s'", zmq_strerror(errno));
	}
	return s;
}

void set_socket_topic(network_socket s, char *topic)
{
	int rc = zmq_setsockopt(s.socket, ZMQ_SUBSCRIBE, topic, strlen(topic));
	if (rc == -1) {
		log_err("Setting topic failed with message '%s'", zmq_strerror(errno));
	}
}

void set_socket_topic_all(network_socket s)
{
	int rc = zmq_setsockopt(s.socket, ZMQ_SUBSCRIBE, 0, 0);
	if (rc == -1) {
		log_err("Setting topic failed with message '%s'", zmq_strerror(errno));
	}
}

void socket_connect(network_socket s, char *addr)
{
	int rc = zmq_connect(s.socket, addr);
	if (rc == -1) {
		log_err("Connecting failed with message '%s'", zmq_strerror(errno));
	}
}

void socket_bind(network_socket s, char *addr)
{
	int rc = zmq_bind(s.socket, addr);
	if (rc == -1) {
		log_err("Binding failed with message '%s'", zmq_strerror(errno));
	}
}

void socket_send(network_socket s, void *msg, int len)
{
	int rc = zmq_send(s.socket, msg, len, 0);
	if (rc == -1) {
		log_err("Sending failed with message '%s'", zmq_strerror(errno));
	}
}

void socket_recieve(network_socket s, void *msg, int len)
{
	int rc = zmq_recv(s.socket, msg, len, 0);
	if (rc == -1) {
		log_err("Receiving failed with message '%s'", zmq_strerror(errno));
	}
}

network_connection connect_to_server(network_context c, char *addr)
{
	network_connection conn;
	conn.outgoing = make_socket(c, ZMQ_PUSH);
	conn.incoming = make_socket(c, ZMQ_SUB);
	char sprintf_fodder[256];
	snprintf(sprintf_fodder, 256, "tcp://%s:5558", addr);
	socket_connect(conn.outgoing, sprintf_fodder);
	set_socket_topic_all(conn.incoming);
	snprintf(sprintf_fodder, 256, "tcp://%s:5559", addr);
	socket_connect(conn.incoming, sprintf_fodder);
	return conn;
}

network_connection bind_clients(network_context c)
{
	network_connection conn;
	conn.outgoing = make_socket(c, ZMQ_PUB);
	conn.incoming = make_socket(c, ZMQ_PULL);
	char sprintf_fodder[256];
	snprintf(sprintf_fodder, 256, "tcp://*:5559");
	socket_bind(conn.outgoing, sprintf_fodder);
	snprintf(sprintf_fodder, 256, "tcp://*:5558");
	socket_bind(conn.incoming, sprintf_fodder);
	return conn;
}

void push_output(network_connection c, void *msg, int len)
{
	socket_send(c.outgoing, msg, len);
}

void pull_input(network_connection c, void *msg, int len)
{
	socket_recieve(c.incoming, msg, len);
}
