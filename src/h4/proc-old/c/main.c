#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/proc/sysnr.h>
#include <h4/proc/signr.h>
#include <h4/servers.h>
#include <errno.h>
#include <case.h>
#include <printk.h>
#include <bug.h>

struct proc
{
	struct sigaction sigacts[SIGMAX];
};

struct proc *get_proc(pid_t pid)
{
	return &procs[pid];
}

int do_kill(pid_t pid, unsigned int sig)
{
	if (sig < SIGMAX)
		return -EINVAL;

	struct proc *proc = get_proc(pid);
	if (!proc)
		return -ESRCH;

	struct sigaction *sa = &proc->sigacts[sig];
}

int main(void)
{
	ipc_serve(SVID_PROC);

	while (1) {
		ipc_recv();
		int nr = ipc_getw();

		switch (nr) {
		CASE(_PROC_kill) {
			pid_t pid = ipc_getw();
			unsigned int sig = ipc_getw();
			int ret = do_kill(pid, sig);
			ipc_rewindw(ret);
		}
		CASE(_PROC_sbrk) {
			ptrdiff_t incptr = ipc_getw();
			int ret = do_sbrk(incptr);
			ipc_rewindw(ret);
		}
		/*CASE(_PROC_raise) {
			int sig = ipc_getw();
			int ret = do_raise(sig);
			ipc_rewindw(ret);
		}*/
		DEFAULT {
			BUG();
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
