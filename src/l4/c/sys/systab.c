#include <l4/sys/sysnr.h>
#include <l4/api/sched.h>
#include <l4/api/rtalloc.h>
#include <l4/api/endpoint.h>
#include <l4/api/thread.h>
#include <l4/api/hello.h>

void *_systab[] = {
	[_SYS_exit] = sys_exit,
	[_SYS_getpid] = sys_getpid,
	[_SYS_sched_next] = sys_sched_next,
	[_SYS_rt_new] = sys_rt_new,
	[_SYS_rt_delete] = sys_rt_delete,
	[_SYS_rt_revoke] = sys_rt_revoke,
	[_SYS_nbsend] = sys_nbsend,
	[_SYS_send] = sys_send,
	[_SYS_call] = sys_call,
	[_SYS_recv] = sys_recv,
	[_SYS_thread_check] = sys_thread_check,
	[_SYS_thread_suspend] = sys_thread_suspend,
	[_SYS_thread_active] = sys_thread_active,
	[_SYS_thread_set_register] = sys_thread_set_register,
	[_SYS_thread_set_priority] = sys_thread_set_priority,
	[_SYS_thread_get_priority] = sys_thread_get_priority,
	[_SYS_thread_get_state] = sys_thread_get_state,
	[_SYS_hello] = sys_hello,
	[_SYS_putchar] = sys_putchar,
	[_SYS_print] = sys_print,
	[_SYS_halt] = sys_halt,
};
