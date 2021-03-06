#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/sys/sched.h>
#include <h4/servers.h>
#include <h4/filedes.h>
#include <panic.h>
#include <bug.h>

extern int libh4_serve_id;

static void try_openat(key_t key, int fd)
{
	int timeout = 100;
	while (ipc_openat(key, fd) < 0) {
		pause();
		if (timeout-- <= 0)
			panic("(%d) failed in try_openat(%d, %d)",
					libh4_serve_id, key, fd);
	}
}

void __libh4_init(void)
{
	printk("libh4_serve_id=%d", libh4_serve_id);
	if (libh4_serve_id >= 0)
		ipc_serve(libh4_serve_id);
	pause();
	pause();

	if (libh4_serve_id != SVID_FS) {
		try_openat(SVID_FS, SVFD_FS);
		if (libh4_serve_id != SVID_MM) {
			try_openat(SVID_MM, SVFD_MM);
		}
	}
}
