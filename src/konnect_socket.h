#ifndef KONNECT_SOCKET_H
#define KONNECT_SOCKET_H

#include "konnect_sockets.h"

typedef struct konnect_socket {
	KONNECT_SOCKET_HANDLE handle;
} konnect_socket;

int konnect_socket_tcp_init(konnect_socket* self, KONNECT_SOCKET_HANDLE handle);
konnect_socket* konnect_socket_tcp_new();
int konnect_socket_tcp_create_handle(konnect_socket* self);
int konnect_socket_tcp_options(KONNECT_SOCKET_HANDLE handle, int option_name, int flag);
int konnect_socket_socket_option(KONNECT_SOCKET_HANDLE handle, int option_name, int flag);
int konnect_socket_bind_to_local_port(konnect_socket* self, int port);

#endif
