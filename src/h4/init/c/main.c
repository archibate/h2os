#if 1
#include <l4/types.h>
#include <l4/api/hello.h>
#else
#define sys_con_write(y,z) write(cons, y, z)
#define sys_con_putchar(y) putchar(cons, y)
#endif
/*#include <l4/api/rtalloc.h>
#include <l4/enum/rtype.h>
#include <l4/enum/thread-registers.h>
#include <l4/api/thread.h>
#include <l4/api/sched.h>*/
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/api.h>
#include <h4/fs/api.h>
#include <h4/proc.h>
#include <h4/proc.h>
#include <printk.h>
#include <stddef.h>
#include <bug.h>

#if 0//{{{
void task_a(void)
{
	sys_print("TASK_A!!!");

	for (;;)
		sys_con_putchar('a');
}

void task_b(void)
{
	sys_print("TASK_B!!!");

	for (;;)
		sys_con_putchar('b');
}

static char fsf_a[2048], fsf_b[2048];
#endif//}}}

//
int getchar(int fd)
{
	char ch;
	return read(fd, &ch, 1) != 1 ? EOF : 0;
}

int putchar(int fd, int ch)
{
	return write(fd, &ch, 1) != 1 ? EOF : 0;
}
//

void main(void)
{
#if 0//{{{
	l4id_t id;

	id = sys_rt_new(RTYPE_THREAD);
	sys_thread_set_register(id, THREAD_REG_PC, (word_t)task_a);
	sys_thread_set_register(id, THREAD_REG_SP, (word_t)fsf_a+2048);
	sys_thread_active(id);

	id = sys_rt_new(RTYPE_THREAD);
	sys_thread_set_register(id, THREAD_REG_PC, (word_t)task_b);
	sys_thread_set_register(id, THREAD_REG_SP, (word_t)fsf_b+2048);
	sys_thread_active(id);

	int i;
	for (i = 0; i < 10000; i++)
		sys_con_putchar('m');
#endif//}}}

	pause();
	pause();
	pause();
	pause();
	pause();

	int fs, kbd, hello, cons;

	fs = ipc_open(SVID_ROOTFS);
	BUG_ON(fs < 0);
	/*cons = fs_open(fs, "/dev/cons", O_WRONLY);
	BUG_ON(cons < 0);*/
	kbd = fs_open(fs, "/dev/keybd", O_RDONLY);
	BUG_ON(kbd < 0);
	hello = fs_open(fs, "/dev/hello", O_RDONLY);
	BUG_ON(hello < 0);

	int ch;
	for (;;) {
		ch = getchar(kbd);
		BUG_ON(ch <= 0);
		if (ch == 'z') {
			BUG();
			char buf[128];
			ssize_t ret = pread(hello, buf, sizeof(buf), 0);
			if (ret > 0)
				sys_con_write(buf, ret);
		} else if (ch == 'x') {
			char buf[128];
			ssize_t ret = read(hello, buf, sizeof(buf));
			if (ret > 0)
				sys_con_write(buf, ret);
			lseek(hello, -7, 2);
		} else if (ch > 0)
			BUG_ON(0 > sys_con_putchar(ch));
	}

	_exit();
}
