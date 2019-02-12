#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(int,nbsend, l4id_t,epid)
_SYS10(int,send, l4id_t,epid)
_SYS10(int,call, l4id_t,epid)
_SYS10(int,recv, l4id_t,epid)
