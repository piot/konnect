#include "konnect_socket.h"


typedef struct konnect_listen {
	konnect_socket socket;
} konnect_listen;

int konnect_listen_init(konnect_listen* self, int port);
konnect_socket* konnect_listen_accept(konnect_listen* self);

