#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>
#define bool int

_SYS10(int,softirq_done, unsigned int,irq)
_SYS20(int,softirq_set_enable, unsigned int,irq,  bool,enable)
