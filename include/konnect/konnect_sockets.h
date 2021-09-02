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
#ifndef KONNECT_SOCKETS_H
#define KONNECT_SOCKETS_H

#include "konnect_platform.h"

#if defined KONNECT_OS_WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#if defined KONNECT_OS_WINDOWS
#define KONNECT_SOCKET_HANDLE SOCKET
#define KONNECT_SOCKET_CLOSE closesocket
#define KONNECT_INPROGRESS_ERROR WSAEINPROGRESS
#define KONNECT_WOULDBLOCK_ERROR WSAEWOULDBLOCK
#define KONNECT_SHUTDOWN_READ_WRITE SD_BOTH
#define KONNECT_INVALID_SOCKET_HANDLE INVALID_SOCKET
#define KONNECT_GET_ERROR WSAGetLastError()
#else
#define KONNECT_SHUTDOWN_READ_WRITE SHUT_RDWR
#define KONNECT_INPROGRESS_ERROR EINPROGRESS
#define KONNECT_WOULDBLOCK_ERROR EINPROGRESS
#define KONNECT_SOCKET_HANDLE int
#define KONNECT_INVALID_SOCKET_HANDLE (-1)
#include <unistd.h>
#define KONNECT_SOCKET_CLOSE close
#endif
int konnect_sockets_init();

typedef unsigned char uint8_t;

#endif
