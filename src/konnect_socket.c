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
#include "konnect/konnect_socket.h"
#include <stdlib.h>
#include <string.h>
#include "konnect/konnect_utils.h"
#include "konnect/konnect_log.h"

#include <fcntl.h>

static int konnect_socket_option(KONNECT_SOCKET_HANDLE handle, int level, int option_name, int flag)
{
	int error = setsockopt(handle, level, option_name, (char*) &flag, sizeof(flag));

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
	konnect_socket* socket = (konnect_socket*) calloc(1, sizeof(konnect_socket));
	socket->should_be_freed = 1;

	return socket;
}

int konnect_socket_tcp_init(konnect_socket* self, KONNECT_SOCKET_HANDLE handle)
{
	KONNECT_ZERO_MEMORY(self);
	self->handle = handle;
	return 0;
}

int konnect_socket_tcp_create_handle(konnect_socket* self)
{
	KONNECT_SOCKET_HANDLE handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == KONNECT_INVALID_SOCKET_HANDLE) {
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

int konnect_socket_receive(konnect_socket* self, void* buffer, size_t max_length)
{
	int length = recv(self->handle, buffer, max_length, 0);
	if (length < 0) {
		int err = KONNECT_GET_ERROR;
		if (err == KONNECT_WOULDBLOCK_ERROR) {
			return 0;
		}
		if (err == KONNECT_NOT_CONNECTED_ERROR) {
			return 0;
		}
		return -1;
	}
	return length;
}

int konnect_socket_send(konnect_socket* self, const void* buffer, size_t length)
{
	int octets_sent = send(self->handle, buffer, length, 0);
	return octets_sent;
}

int konnect_socket_close(konnect_socket* self)
{
	konnect_log("closing socket:%d", self->handle);

	int close_error = KONNECT_SOCKET_CLOSE(self->handle);
	konnect_log("close error %d", close_error);
	self->handle = 0;
	if (self->should_be_freed) {
		konnect_log("freeing %d", self->handle);
		free(self);
	}

	return close_error;
}

int konnect_socket_non_blocking(konnect_socket* self, int non_blocking)
{
#if defined KONNECT_OS_WINDOWS
	u_long mode = non_blocking;
	int result = ioctlsocket(self->handle, FIONBIO, &mode);
	if (result != NO_ERROR) {
		return konnect_error(result, "ioctlsocket failed with error");
	}
	return 0;
#else
	int flags = fcntl(self->handle, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}
	if (non_blocking) {
		flags |= O_NONBLOCK;
	} else {
		flags &= ~O_NONBLOCK;
	}
	return fcntl(self->handle, F_SETFL, flags);
#endif
}

int konnect_socket_select_write(konnect_socket* self, int seconds)
{
	fd_set set;

	FD_ZERO(&set);
	FD_SET(self->handle, &set);

	struct timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = 0;

	int descriptors_ready = select((int) self->handle + 1, 0, &set, 0, &timeout);
	if (descriptors_ready != 1) {
		return -1;
	}

	return 0;
}
