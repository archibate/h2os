#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/sysnr.h>
#include <h4/fs/defines.h>
#include <h4/servers.h>
#include <errno.h>
#include <case.h>
#include <printk.h>
#include <bug.h>
#include <h4/fs/oflags.h>

int dev_path_resolve(const char *path)
{
	if (!strcmp(path, "/dev/hello")) 
		return SVID_HELLO;
	else if (!strcmp(path, "/dev/keybd")) 
		return SVID_KEYBD;
	else if (!strcmp(path, "/dev/hda")) 
		return SVID_IDEDRV;
	else 
		return -ENOENT;
}

int do_open(const char *path, unsigned int flags)
{
	printk("do_open(%s, %d)", path, flags);

	int id = dev_path_resolve(path);
	if (id < 0)
		return id;
	return id;
}

int main(void)
{
	ipc_serve(SVID_ROOTFS);

	while (1) {
		ipc_recv();
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
