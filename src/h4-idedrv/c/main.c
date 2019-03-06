// https://github.com/archibate/OS67/blob/master/drv/kb.c
#include <l4/types.h>
#include <l4/defines.h>
#include <l4/api/hello.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>

static int hds;

void hd_init(void)
{
	hds = ipc_open(SVID_IDEHD, IPC_CREAT | IPC_SERVER);
	if (hds < 0) {
		sys_print("error in opening hd endpoint");
		sys_halt();
	}
}

void hd_serve(void)
{
	ipc_recv(hds);
}

int main(void)
{
	hd_init();
	while (1) {
		hd_serve();
	}
}
