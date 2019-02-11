#include <l4/api/syscall.h>

_SYS30(int,action, l4id_t,capid, unsigned int,action, unsigned long,argument);

#define _$E(x) x
#define _SYS44(rett,func, t1,x1, t2,x2, t3,x3, t4,x4, u1,y1, u2,y2, u3,y3, u4,y4) \
rett sys_##func(t1 x1, t2 x2, t3 x3, t4 x4, \
		u1*y1, u2*y2, u3*y3, u4*y4) \
{ \
	asm volatile ( \
		"push %%ebp\n" \
		"push %%ecx\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%ebp, %%edx\n" \
		"pop %%ecx\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (y1) \
		, "=D" (y2) \
		, "=S" (y3) \
		, "=d" (y4) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x1) \
		, "D" (x2) \
		, "S" (x3) \
		, "d" (x4) \
		: "cc", "memory");
