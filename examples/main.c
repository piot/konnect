/*$6*/


#include "konnect/konnect_sockets.h"
#include "konnect/konnect_listen.h"
#include "konnect/konnect_connect.h"
#include "konnect/konnect_log.h"

#include <string.h>

int server()
{
	konnect_listen	listen;
	int				error = konnect_listen_init(&listen, 23000);
	if (error) {
		konnect_log("couldn't listen:%d", error);
		return error;
	}

	while (1) {
		konnect_socket	*socket = konnect_listen_accept(&listen);
		if (socket) {
			konnect_log("connected!");
			while (1) {
				char	data[1024];

				int		octets_received = konnect_socket_receive(socket, data, sizeof(data));
				if (octets_received == 0) {
					konnect_log("Disconnected");
					return -1;
				}

				data[octets_received] = 0;

				const char	*reply = "Got it!";
				konnect_socket_send(socket, reply, strlen(reply));

				char	*command;
				if ((command = strchr(data, '\r')) != 0) {
					konnect_log("chopping");
					*command = '\0';
				}

				konnect_log("Incoming '%s'", data);
				if (strcmp("quit", data) == 0) {
					konnect_socket_close(socket);
					return -2;
				}
			}
		}
		else {
			konnect_log("Accept failed");
			return -1;
		}
	}
}

int client()
{
	konnect_listen	listen;
	int				error = konnect_listen_init(&listen, 23001);
	if (error) {
		konnect_log("couldn't listen:%d", error);
		return error;
	}

	konnect_connect connection;
	int				connect_error = konnect_connect_init(&connection, "127.0.0.1", 23001);
	if (connect_error) {
		konnect_log("Couldn't connect:%d", connect_error);
		return connect_error;
	}

	const char	*reply = "Connection Received!";
	int			octets_sent = konnect_socket_send(&connection.socket, reply, strlen(reply));
	if (octets_sent != strlen(reply)) {
		konnect_log("Konnect_send %d", octets_sent);
		return -2;
	}

	konnect_socket_close(&connection.socket);

	return 0;
}

int main(int argc, const char *argv[])
{
	konnect_sockets_init();
	server();

	return 0;
}
