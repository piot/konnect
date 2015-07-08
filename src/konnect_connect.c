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

#if !defined KONNECT_OS_WINDOWS
#include <arpa/inet.h>
#endif

int konnect_connect_init(konnect_socket *self, const char *address, int port)
{
	KONNECT_ZERO_MEMORY(self);

	int create_error = konnect_socket_tcp_create_handle(self);
	if (create_error) {
		konnect_error(create_error, "connect: create_handle");
		return create_error;
	}

	struct sockaddr_in	addr;
	addr.sin_family = AF_INET;

	int convert_error = !inet_pton(AF_INET, address, &addr.sin_addr);
	if (convert_error) {
		konnect_error(convert_error, "connect: inet_aton");
		return convert_error;
	}

	addr.sin_port = htons(port);

	int connect_error = connect(self->handle, (struct sockaddr *) &addr, sizeof(addr));
	if (connect_error) {
		konnect_error(connect_error, "connect:connect");
	}

	return connect_error;
}
