#include <l4/types.h>
#include <l4/api/sched.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <h4/fs/sysnr.h>
#include <h4/proc.h>
#include <printk.h>
#include <bug.h>
#include "minix.h"
#include "bcache.h"
#include "hd.h"
#include "file.h"
#include "p2i.h"
#include "errno.h"

static int fs;

void fs_server_init(void)
{
	fs = ipc_open(SVID_ROOTFS, IPC_CREAT | IPC_SERVER);
	BUG_ON(fs < 0);
}

void fs_serve_ipc(void)
{
	ipc_recv(fs);
	struct file *fp = ipc_getbadge();
	int nr = ipc_getw();

	if (!fp) switch (nr) {
	case _FS_open:
		{
			size_t len = ipc_getw();
			len++;
			char *path = ipc_getbuf(&len);
			len--;
			path[len] = 0;
			struct inode *ip = p2i(path);
			if (ip == NULL) {
				iput(ip);
				ipc_rewindw(-errno);
				break;
			}
			fp = falloc();
			if (fp == NULL) {
				ipc_rewindw(-EMFILE);
				break;
			}
			fp->ftype = F_INODE;
			fp->ip = ip;
			ipc_setbadge(fp);
			ipc_rewindw(0);
		} break;
	} else switch (nr) {
	case _FILE_read:
		{
			size_t len = ipc_getw();
			ipc_seek_curw(1);
			char *buf = ipc_getbuf(&len);
			ssize_t ret = fread(badge, buf, len);
			ipc_rewindw(ret);
		} break;
	case _FILE_write:
		{
			size_t len = ipc_getw();
			const char *buf = ipc_getbuf(&len);
			ssize_t ret = fwrite(badge, buf, len);
			ipc_rewindw(ret);
		} break;
	case _FILE_close:
		{
			fclose(badge);
			ipc_rewindw(0);
		} break;
	/*case _FILE_lseek:
		{
			off_t off = ipc_getw();
			struct file *fp = badge;
			fp->off += off;
		} break;*/
	default:
		ipc_rewindw(-ENOTSUP);
	}
}

void main(void)
{
	fs_server_init();
	hd_init();
	bcache_init();

	while (1) {
		fs_serve_ipc();
	}

	_exit(0);
}
