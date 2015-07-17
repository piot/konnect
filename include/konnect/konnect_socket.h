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
#ifndef KONNECT_SOCKET_H
#define KONNECT_SOCKET_H

#include "konnect_sockets.h"

typedef struct konnect_socket {
	KONNECT_SOCKET_HANDLE handle;
	int should_be_freed;
} konnect_socket;

int konnect_socket_tcp_init(konnect_socket* self, KONNECT_SOCKET_HANDLE handle);
konnect_socket* konnect_socket_tcp_new();
int konnect_socket_tcp_create_handle(konnect_socket* self);
int konnect_socket_tcp_options(KONNECT_SOCKET_HANDLE handle, int option_name, int flag);
int konnect_socket_socket_option(KONNECT_SOCKET_HANDLE handle, int option_name, int flag);
int konnect_socket_bind_to_local_port(konnect_socket* self, int port);
int konnect_socket_receive(konnect_socket* self, void* buffer, size_t max_length);
int konnect_socket_send(konnect_socket* self, const void* buffer, size_t length);
int konnect_socket_close(konnect_socket* self);
int konnect_socket_non_blocking(konnect_socket* self, int non_blocking);
int konnect_socket_select_write(konnect_socket* self, int seconds);

#endif
