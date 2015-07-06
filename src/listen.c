#include "listen.h"
#include "konnect_socket.h"

int konnect_listen_init(konnect_listen* self, int port)
{
	konnect_socket_tcp_create_handle(&self->socket);
	konnect_socket_socket_option(self->socket.handle, SO_REUSEADDR, 1);
	konnect_socket_bind_to_local_port(&self->socket, port);
	int error = listen(self->socket.handle, SOMAXCONN);
	return error;
}

konnect_socket* konnect_listen_accept(konnect_listen* self)
{
	struct sockaddr_in incoming_address;
	int len = sizeof(incoming_address);
	int handle = accept(self->socket.handle, (struct sockaddr*)&incoming_address, &len);
	if (handle == -1) {
		return 0;
	}
	konnect_socket_tcp_options(handle, TCP_NODELAY, 1);

	konnect_socket* accepted_socket = konnect_socket_tcp_new();
	konnect_socket_tcp_init(accepted_socket, handle);

	return accepted_socket;
}
