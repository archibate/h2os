#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(int,sys_nbsend, l4id_t,epid)
_SYS10(int,sys_send, l4id_t,epid)
_SYS10(int,sys_call, l4id_t,epid)
_SYS10(int,sys_recv, l4id_t,epid)
