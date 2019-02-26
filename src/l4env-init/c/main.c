#include <l4/types.h>
#include <l4/enum/rtype.h>
#include <l4/enum/thread-registers.h>
#include <l4/api/hello.h>
#include <l4/api/rtalloc.h>
#include <l4/api/thread.h>
#include <l4/api/sched.h>
#include <l4/api/endpoint.h>
#include <l4/enum/rflags.h>
#include <l4/enum/rtype.h>

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

void main(void)
{
	sys_print("MAIN!!!");

	l4id_t id;

#if 0
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
#endif

	int kb = sys_rt_open(RTYPE_ENDPOINT, 2333, R_CREAT | R_RDONLY);
	if (kb < 0) {
		sys_print("error in open keyboard endpoint");
		sys_halt();
	}

	int i = 0;
	for (;;) {
		sys_recv(kb);
		sys_putchar("/-\\|"[i++%4]);
	}

	sys_exit();
}
