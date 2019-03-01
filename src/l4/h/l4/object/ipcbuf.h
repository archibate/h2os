#pragma once

#include <l4/enum/ipcbuf.h>

struct ipc_buf
{
	char buf[IPCBUF_SIZE];
};
