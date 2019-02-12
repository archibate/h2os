#include <l4/sys/sysnr.h>
#include <l4/api/endpoint.h>
#include <l4/api/thread.h>
#include <l4/api/alloc.h>

void *_systab[] = {
	[_SYS_nbsend] = sys_nbsend,
	[_SYS_send] = sys_send,
	[_SYS_call] = sys_call,
	[_SYS_recv] = sys_recv,
	[_SYS_thread_suspend] = sys_thread_suspend,
	[_SYS_thread_active] = sys_thread_active,
	[_SYS_thread_set_priority] = sys_thread_set_priority,
	[_SYS_new] = sys_new,
	[_SYS_delete] = sys_delete,
	[_SYS_revoke] = sys_revoke,
};
