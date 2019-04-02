#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/proc/sysnr.h>
#include <h4/servers.h>
#include <errno.h>
#include <case.h>
#include <printk.h>
#include <bug.h>

struct proc
{
	uintptr_t ebss;
};

uintptr_t do_sbrk(ptrdiff_t incptr)
{
	if (incptr > 0) {
		printk("heap grow %#x", incptr);
		curr->ebss += incptr;
	} else if (incptr < 0) {
		incptr = -incptr;
		printk("heap shrink %#x", incptr);
		curr->ebss -= incptr;
	}
	return curr->ebss;
}

struct proc *new_proc(uintptr_t ebss)
{
	struct proc *proc = malloc(sizeof(struct proc));
	proc->ebss = ebss;
	return proc;
}

int main(void)
{
	ipc_serve(SVID_PROC);

	while (1) {
		ipc_recv();
		int nr = ipc_getw();

		struct proc *curr = (void*)ipc_getbadge();
		if (curr == NULL && nr == _PROC_rootclinit) {
			uintptr_t ebss = ipc_getw();
			struct proc *proc = new_proc(ebss);
			ipc_setbadge(proc);
			ipc_rewindw(proc == NULL ? -errno : 0);
			ipc_reply();
			continue;
		}

		if (curr == NULL)
			ipc_rewindw(-EINVAL);

		switch (nr) {
		/*CASE(_PROC_execve) {
			int ret = do_execve(path, argv, envp);
			ipc_rewindw(ret);
		}*/
		CASE(_PROC_sbrk) {
			ptrdiff_t incptr = ipc_getw();
			uintptr_t ret = do_sbrk(curr, incptr);
			ipc_rewindw(ret);
		}
		DEFAULT {
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
