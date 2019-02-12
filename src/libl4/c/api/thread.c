#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(int,thread_suspend, l4id_t,tid)
_SYS10(int,thread_active, l4id_t,tid)
_SYS20(int,thread_set_priority, l4id_t,tid,  unsigned int,priority)
