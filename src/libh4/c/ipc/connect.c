#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int ipc_accept(key_t key)
{
	return ipc_open(key, IPC_CREAT | IPC_SERVER);
}

int ipc_connect(key_t key)
{
	return ipc_open(key, IPC_CREAT | IPC_CLIENT);
}
