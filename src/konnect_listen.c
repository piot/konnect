/*
The MIT License (MIT)

Copyright (c) 2015 Peter Bjorklund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "konnect/konnect_listen.h"
#include "konnect/konnect_socket.h"
#include "konnect/konnect_error.h"
#include "konnect/konnect_utils.h"

int konnect_listen_init(konnect_listen* self, int port)
{
	KONNECT_ZERO_MEMORY(self);

	int create_handle_error = konnect_socket_tcp_create_handle(&self->socket);
	if (create_handle_error) {
		return konnect_error(create_handle_error, "listen_init: create_handle");
	}

	int option_error = konnect_socket_socket_option(self->socket.handle, SO_REUSEADDR, 1);
	if (option_error) {
		return konnect_error(option_error, "listen_init: socket_option");
	}

	int bind_error = konnect_socket_bind_to_local_port(&self->socket, port);
	if (bind_error) {
		return konnect_error(bind_error, "listen_init: bind error");
	}

	int listen_error = listen(self->socket.handle, SOMAXCONN);
	if (listen_error) {
		return konnect_error(listen_error, "listen_init: listen");
	}

	return 0;
}

konnect_socket* konnect_listen_accept(konnect_listen* self)
{
	struct sockaddr_in incoming_address;
	socklen_t len = sizeof(incoming_address);
	KONNECT_SOCKET_HANDLE handle = accept(self->socket.handle, (struct sockaddr*) &incoming_address, &len);
	if (handle == KONNECT_INVALID_SOCKET_HANDLE) {
		int err = KONNECT_GET_ERROR;
		if (err == EAGAIN || err == KONNECT_WOULDBLOCK_ERROR) {
			return 0;
		}
		konnect_error(errno, "listen_accept:accept");
		return 0;
	}

	int options_error = konnect_socket_tcp_options(handle, TCP_NODELAY, 1);
	if (options_error) {
		konnect_error(options_error, "listen_accept: TCP_NODELAY");
		return 0;
	}

	konnect_socket* accepted_socket = konnect_socket_tcp_new();
	int init_error = konnect_socket_tcp_init(accepted_socket, handle);
	if (init_error) {
		konnect_error(init_error, "listen_accept: tcp_init");
		return 0;
	}

	return accepted_socket;
}

int konnect_listen_close(konnect_listen* self)
{
	int shutdown_error = shutdown(self->socket.handle, KONNECT_SHUTDOWN_READ_WRITE);
	if (shutdown_error) {
		konnect_error(shutdown_error, "konnect_listen_close: shutdown");
	}
	return konnect_socket_close(&self->socket);
}
