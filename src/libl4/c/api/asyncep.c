#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(int,async_pulse, l4id_t,id)
_SYS10(int,async_poll, l4id_t,id)
_SYS10(int,async_listen, l4id_t,id)
