#define USEFAT 1
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/sysnr.h>
#include <h4/fs/defines.h>
#include <h4/file/sysnr.h>
#include <h4/servers.h>
#include <errno.h>
#include <case.h>
#include <printk.h>
#include <bug.h>
#include <h4/fs/oflags.h>
#include <numtools.h>
#ifdef USEFAT
#include "vn.h"
#include "dir.h"
#endif

int dev_resolve(const char *name)
{
	if (!strcmp(name, "hello")) 
		return SVID_HELLO;
	else if (!strcmp(name, "cons")) 
		return SVID_CONS;
	else if (!strcmp(name, "keybd")) 
		return SVID_KEYBD;
	else if (!strcmp(name, "hda")) 
		return SVID_IDEDRV;
	else 
		return -ENOENT;
}

int do_open(const char *path, unsigned int flags)
{
	printk("do_open(%s, %d)", path, flags);

	if (!strncmp(path, "/dev/", strlen("/dev/")))
		return dev_resolve(path + strlen("/dev/"));

#ifdef USEFAT
	extern vn_t *fat_root;
	vn_t *v = dir_vopen(fat_root, path, flags);
	if (!v)
		return -errno;

	ipc_setbadge((uintptr_t)v);
	return 0;
#else
	return -ENOENT;
#endif
}

#ifdef USEFAT
ssize_t file_write(vn_t *v, const char *buf, size_t len, off_t off)
{
	if (!(v->exflags & O_WRONLY))
		return -EPERM;

	return vwrite(v, buf, len, off);
}

ssize_t file_read(vn_t *v, char *buf, size_t len, off_t off)
{
	if (!(v->exflags & O_RDONLY))
		return -EPERM;

	return vread(v, buf, len, off);
}

off_t file_lseek(vn_t *v, off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: off = v->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	CLMAX(off, v->size);
	return off;
}

void file_close(vn_t *v)
{
	free(v);
}

void file_serve_ipc(vn_t *v)
{
	unsigned int nr = ipc_getw();
	//printk("!!!nr=%d", nr);
	switch (nr) {

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = file_read(v, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = file_write(v, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("file_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_read(v, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("file_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_write(v, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_lseek:
	{
		off_t off = ipc_getw();
		int whence = ipc_getw();
		off_t now_off = ipc_getoffset();
		off_t ret = file_lseek(v, now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}
#endif

int main(void)
{
	static char buffer[4096*32];
	liballoc_set_memory(buffer, sizeof(buffer));

#ifdef USEFAT
	extern void fat_init(void);
	fat_init();
#endif

	ipc_serve(SVID_ROOTFS);

	while (1) {
		ipc_recv();

#ifdef USEFAT
		vn_t *v = (void*)ipc_getbadge();
		if (v != NULL) {
			file_serve_ipc(v);
			ipc_reply();
			continue;
		}
#endif

		int nr = ipc_getw();
		switch (nr) {
		CASE(_FS_open) {
			size_t len = MAXPATH;
			unsigned int flags = ipc_getw();
			const char *path = ipc_getbuf(&len);
			if (strnlen(path, len) >= MAXPATH) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
			int ret = do_open(path, flags);
			ipc_rewindw(ret);
		}
		DEFAULT {
			BUG();
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
