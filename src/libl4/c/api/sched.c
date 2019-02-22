#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>
#define bool int

_SYS00(int,exit)
_SYS00(l4id_t,getpid)
_SYS00(int,sched_next)
