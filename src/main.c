#include "konnect_sockets.h"
#include "Listen.h"

#include <stdio.h>

int main(int argc, const char* argv[])
{
	konnect_sockets_init();

	konnect_listen listen;
	konnect_listen_init(&listen, 23000);
	while (1) {
		konnect_socket* socket = konnect_listen_accept(&listen);
		if (socket) {
			printf("\nconnected!");
		}
	}
	return 0;
}
