#include <l4/sys/sysnr.h>
#include <l4/api/asyncep.h>
#include <l4/api/hello.h>
#include <l4/api/ipc.h>
#include <l4/api/rtalloc.h>
#include <l4/api/sched.h>
#include <l4/api/softirq.h>
#include <l4/api/thread.h>

void *_systab[] = {
	[_SYS_async_pulse] = sys_async_pulse,
	[_SYS_async_poll] = sys_async_poll,
	[_SYS_async_listen] = sys_async_listen,
	[_SYS_hello] = sys_hello,
	[_SYS_print] = sys_print,
	[_SYS_con_putchar] = sys_con_putchar,
	[_SYS_con_write] = sys_con_write,
	[_SYS_halt] = sys_halt,
	[_SYS_nbsend] = sys_nbsend,
	[_SYS_send] = sys_send,
	[_SYS_call] = sys_call,
	[_SYS_recv] = sys_recv,
	[_SYS_reply] = sys_reply,
	[_SYS_connect] = sys_connect,
	[_SYS_rt_open] = sys_rt_open,
	[_SYS_rt_close] = sys_rt_close,
	[_SYS_exit] = sys_exit,
	[_SYS_getpid] = sys_getpid,
	[_SYS_sched_next] = sys_sched_next,
	[_SYS_softirq_done] = sys_softirq_done,
	[_SYS_softirq_set_enable] = sys_softirq_set_enable,
	[_SYS_thread_check] = sys_thread_check,
	[_SYS_thread_suspend] = sys_thread_suspend,
	[_SYS_thread_active] = sys_thread_active,
	[_SYS_thread_set_register] = sys_thread_set_register,
	[_SYS_thread_set_priority] = sys_thread_set_priority,
	[_SYS_thread_get_priority] = sys_thread_get_priority,
	[_SYS_thread_get_state] = sys_thread_get_state,
};
