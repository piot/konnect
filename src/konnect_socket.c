#include "konnect_socket.h"

static int konnect_socket_option(KONNECT_SOCKET_HANDLE handle, int level, int option_name, int flag)
{
	int error = setsockopt(handle, level, option_name, (char*)&flag, sizeof(flag));

	return error;
}

int konnect_socket_tcp_options(KONNECT_SOCKET_HANDLE handle, int option_name, int flag)
{
	return konnect_socket_option(handle, IPPROTO_TCP, option_name, flag);
}

int konnect_socket_socket_option(KONNECT_SOCKET_HANDLE handle, int option_name, int flag)
{
	return konnect_socket_option(handle, SOL_SOCKET, option_name, flag);
}

konnect_socket* konnect_socket_tcp_new()
{
	return (konnect_socket*) calloc(1, sizeof(konnect_socket));
}
int konnect_socket_tcp_init(konnect_socket* self, KONNECT_SOCKET_HANDLE handle)
{
	self->handle = handle;

	return 0;
}

int konnect_socket_tcp_create_handle(konnect_socket* self)
{
	int handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1) {
		return -1;
	}

	return konnect_socket_tcp_init(self, handle);
}


int konnect_socket_bind_to_local_port(konnect_socket* self, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int error = bind(self->handle, (const struct sockaddr*) &addr, sizeof(addr));

	return error;

}