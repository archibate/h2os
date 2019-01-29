#pragma once

#include <l4/ipcbuf.h>

typedef struct Endpoint
{
	IPCBuffer_t *ipcbuf;
}
endpoint_t;
