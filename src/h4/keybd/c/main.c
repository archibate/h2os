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

struct fifo tin;

void kb_handler(void);

ssize_t tty_read(void *buf, size_t len)
{
	int i;
	char *b = buf;
	CLMAX(len, fifo_rdmax(&tin));
	for (i = 0; i < len; i++) {
		b[i] = fifo_get(&tin);
	}
	return len;
}

void tty_serve_ipc(void)
{
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_read:
	{
		size_t len = ipc_getw();
		printk("tty_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = tty_read(buf, len);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		ipc_rewindw(-EPERM);
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
	ipc_serve(SVID_KEYBD);
	sys_softirq_set_enable(IRQ_KEYBD, true);
	while (1) {
		while (fifo_empty(&tin)) {
			sys_async_listen(IRQ_KEYBD);
			do {
				kb_handler();
				sys_softirq_done(IRQ_KEYBD);
			} while (sys_async_poll(IRQ_KEYBD));
		}
		while (ipc_poll() >= 0) {
			tty_serve_ipc();
		}
		while (!fifo_empty(&tin)) {
			ipc_recv();
			while (sys_async_poll(IRQ_KEYBD)) {
				kb_handler();
				sys_softirq_done(IRQ_KEYBD);
			}
			tty_serve_ipc();
		}
	}
}
