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
#include "konnect/konnect_connect.h"
#include "konnect/konnect_error.h"
#include "konnect/konnect_log.h"
#include "konnect/konnect_utils.h"
#include "konnect/konnect_socket.h"

#if !defined KONNECT_OS_WINDOWS
#include <sys/errno.h>
#include <arpa/inet.h>
#endif

int konnect_get_last_error()
{
#if defined KONNECT_OS_WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif
}

int konnect_connect_init(konnect_socket* self, const char* address, int port)
{
	KONNECT_ZERO_MEMORY(self);

	int create_error = konnect_socket_tcp_create_handle(self);
	if (create_error) {
		return konnect_error(create_error, "connect: create_handle");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;

	int convert_error = !inet_pton(AF_INET, address, &addr.sin_addr);
	if (convert_error) {
		return konnect_error(convert_error, "connect: inet_aton");
	}

	addr.sin_port = htons(port);

	int non_block_result = konnect_socket_non_blocking(self, 1);
	if (non_block_result) {
		return konnect_error(create_error, "non blocking");
	}

	int connect_error = connect(self->handle, (struct sockaddr*) &addr, sizeof(addr));
	if (connect_error) {
		int socket_error = konnect_get_last_error();
		if ((socket_error != KONNECT_INPROGRESS_ERROR) && (socket_error != KONNECT_WOULDBLOCK_ERROR)) {
			konnect_log("socket_error:%d", socket_error);
			return konnect_error(connect_error, "connect:connect");
		}
	}

	int select_error = konnect_socket_select_write(self, 2);
	if (select_error) {
		return konnect_error(select_error, "select");
	}

	int block_result = konnect_socket_non_blocking(self, 0);
	if (block_result) {
		return konnect_error(create_error, "blocking");
	}

	return 0;
}
