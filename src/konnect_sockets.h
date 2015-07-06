#ifndef KONNECT_SOCKETS_H
#define KONNECT_SOCKETS_H
#include "platform.h"


#if defined KONNECT_OS_WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#endif

#if defined KONNECT_OS_WINDOWS
#define KONNECT_SOCKET_HANDLE SOCKET
#define KONNECT_SOCKET_CLOSE closesocket
#else
#define KONNECT_SOCKET_HANDLE int
#define KONNECT_SOCKET_CLOSE close
#endif

int konnect_sockets_init();

#endif
