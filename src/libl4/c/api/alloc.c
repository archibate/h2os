#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(sl4id_t,sys_new, unsigned int,rtype)
_SYS20(int,sys_delete, unsigned int,rtype,  l4id_t,id)
_SYS20(int,sys_revoke, unsigned int,rtype,  l4id_t,id)
