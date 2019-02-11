#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>

_SYS10(int,sys_io_in, unsigned short,port)
_SYS10(int,sys_io_out, unsigned short,port)
