#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/api/endpoint.h>

int ipc_send(int epid)
{
	sys_send(epid);
	ipc_rewind();
}

int ipc_nbsend(int epid)
{
	sys_nbsend(epid);
	ipc_rewind();
}

int ipc_call(int epid)
{
	sys_call(epid);
	ipc_rewind();
}

int ipc_recv(int epid)
{
	sys_recv(epid);
	ipc_rewind();
}
