#include <l4/stdafx.h>
#include <l4/sys/sysnr.h>
#include <l4/sys/sysdef.h>
#define bool int

_SYS00(int,hello)
_SYS10(int,putchar, char,c)
_SYS10(int,print, const char *,s)
_SYS00(int,halt)