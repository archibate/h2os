#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>
#define bool int

_SYS10(sl4id_t,rt_new, unsigned int,rtype)
_SYS30(sl4id_t,rt_open, unsigned int,rtype,  l4id_t,id,  unsigned int,flags)
_SYS20(int,rt_check, unsigned int,rtype,  l4id_t,id)
_SYS20(int,rt_delete, unsigned int,rtype,  l4id_t,id)
_SYS20(int,rt_revoke, unsigned int,rtype,  l4id_t,id)
