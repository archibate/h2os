#include <l4/types.h>
#include <l4/enum/rtype.h>
#include <l4/enum/thread-registers.h>
#include <l4/api/hello.h>
#include <l4/api/rtalloc.h>
#include <l4/api/thread.h>
#include <l4/api/sched.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>

#if 0//{{{
void task_a(void)
{
	sys_print("TASK_A!!!");

	for (;;)
		sys_putchar('a');
}

void task_b(void)
{
	sys_print("TASK_B!!!");

	for (;;)
		sys_putchar('b');
}

static char fsf_a[2048], fsf_b[2048];
#endif//}}}

//
static int kbd;

void kbd_init(void)
{
	kbd = ipc_open(SVID_KEYBD, IPC_CREAT | IPC_RECV);
	if (kbd < 0) {
		sys_print("error in open keyboard server");
		sys_halt();
	}
}

int kbd_getchar(void)
{
	ipc_recv(kbd);
	int ch = ipc_get32();
	return ch;
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
		sys_putchar('m');
#endif//}}}

	kbd_init();

	int ch;
	for (;;) {
		ch = kbd_getchar();
		sys_putchar(ch);
	}

	sys_exit();
}
