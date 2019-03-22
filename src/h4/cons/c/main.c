#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <l4/stdafx.h>
#include <l4/api/hello.h>
#include <l4/api/asyncep.h>
#include <l4/api/softirq.h>
#include <l4/enum/irq-nrs.h>
#include <conio.h>
#include <fifo.h>
#include <errno.h>
#include <numtools.h>
#include <console.h>

void con_serve_ipc(void)
{
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_read:
	{
		ipc_rewindw(-EPERM);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		printk("con_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = con_write(buf, len);
		ipc_rewindw(ret);
	} break;

	case _FILE_pread:
	case _FILE_pwrite:
	case _FILE_lseek:
	{
		ipc_rewindw(-ESPIPE);
	} break;

	default:
		ipc_putw(-ENOTSUP);
	}
	ipc_reply();
}

int main(void)
{
	ipc_serve(SVID_CONS);
	ipc_recv();
	while (1) {
		con_serve_ipc();
	}
}
