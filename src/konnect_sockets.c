#include "konnect_sockets.h"
#include "platform.h"

int konnect_sockets_init()
{
#if defined KONNECT_OS_WINDOWS
	int result;
	WSADATA wsa_data;

	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		return -1;
	}

#endif

	return 0;

}
