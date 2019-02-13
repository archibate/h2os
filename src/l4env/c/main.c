#include <l4/types.h>
#include <l4/enum/rtype.h>
#include <l4/enum/thread-registers.h>
#include <l4/api/hello.h>
#include <l4/api/rtalloc.h>
#include <l4/api/thread.h>

void func(void)
{
	sys_print("FUNC!!!");
	sys_halt();
}

static char fsf[2048];

void main(void)
{
	sys_print("MAIN!!!");
	sys_hello();

	l4id_t id = sys_rt_new(RTYPE_THREAD);

	sys_thread_set_register(id, THREAD_REG_PC, (word_t)func);

	sys_thread_set_register(id, THREAD_REG_SP, (word_t)fsf+2048);

	sys_thread_active(id);

	sys_sched_next();

	sys_halt();
}
