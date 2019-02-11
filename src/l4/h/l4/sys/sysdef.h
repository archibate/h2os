#pragma once

#define _$E(x) x

#define _SYS00(rett, func) \
rett sys_##func() \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		: "a" (_$E(_SYS_##func)) \
                : "ecx", "ebx", "edi", "esi", "cc", "memory"); \
}

#define _SYS01(rett, func, u0, y0) \
rett sys_##func(, u0*y0) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		: "a" (_$E(_SYS_##func)) \
                : "ecx", "edi", "esi", "cc", "memory"); \
}

#define _SYS02(rett, func, u0, y0, u1, y1) \
rett sys_##func(, u0*y0, u1*y1) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		: "a" (_$E(_SYS_##func)) \
                : "ecx", "esi", "cc", "memory"); \
}

#define _SYS03(rett, func, u0, y0, u1, y1, u2, y2) \
rett sys_##func(, u0*y0, u1*y1, u2*y2) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		: "a" (_$E(_SYS_##func)) \
                : "ecx", "cc", "memory"); \
}

#define _SYS04(rett, func, u0, y0, u1, y1, u2, y2, u3, y3) \
rett sys_##func(, u0*y0, u1*y1, u2*y2, u3*y3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%edx, %%ebp\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		, "=d" (*y3) \
		: "a" (_$E(_SYS_##func)) \
                : "ecx", "cc", "memory"); \
}

#define _SYS10(rett, func, t0, x0) \
rett sys_##func(, t0 x0) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
                : "ecx", "ebx", "edi", "esi", "cc", "memory"); \
}

#define _SYS11(rett, func, t0, x0, u0, y0) \
rett sys_##func(, t0 x0, u0*y0) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
                : "ecx", "edi", "esi", "cc", "memory"); \
}

#define _SYS12(rett, func, t0, x0, u0, y0, u1, y1) \
rett sys_##func(, t0 x0, u0*y0, u1*y1) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
                : "ecx", "esi", "cc", "memory"); \
}

#define _SYS13(rett, func, t0, x0, u0, y0, u1, y1, u2, y2) \
rett sys_##func(, t0 x0, u0*y0, u1*y1, u2*y2) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
                : "ecx", "cc", "memory"); \
}

#define _SYS14(rett, func, t0, x0, u0, y0, u1, y1, u2, y2, u3, y3) \
rett sys_##func(, t0 x0, u0*y0, u1*y1, u2*y2, u3*y3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%edx, %%ebp\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		, "=d" (*y3) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
                : "ecx", "cc", "memory"); \
}

#define _SYS20(rett, func, t0, x0, t1, x1) \
rett sys_##func(, t0 x0, t1 x1) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
                : "ecx", "ebx", "edi", "esi", "cc", "memory"); \
}

#define _SYS21(rett, func, t0, x0, t1, x1, u0, y0) \
rett sys_##func(, t0 x0, t1 x1, u0*y0) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
                : "ecx", "edi", "esi", "cc", "memory"); \
}

#define _SYS22(rett, func, t0, x0, t1, x1, u0, y0, u1, y1) \
rett sys_##func(, t0 x0, t1 x1, u0*y0, u1*y1) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
                : "ecx", "esi", "cc", "memory"); \
}

#define _SYS23(rett, func, t0, x0, t1, x1, u0, y0, u1, y1, u2, y2) \
rett sys_##func(, t0 x0, t1 x1, u0*y0, u1*y1, u2*y2) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
                : "ecx", "cc", "memory"); \
}

#define _SYS24(rett, func, t0, x0, t1, x1, u0, y0, u1, y1, u2, y2, u3, y3) \
rett sys_##func(, t0 x0, t1 x1, u0*y0, u1*y1, u2*y2, u3*y3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%edx, %%ebp\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		, "=d" (*y3) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
                : "ecx", "cc", "memory"); \
}

#define _SYS30(rett, func, t0, x0, t1, x1, t2, x2) \
rett sys_##func(, t0 x0, t1 x1, t2 x2) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
                : "ecx", "ebx", "edi", "esi", "cc", "memory"); \
}

#define _SYS31(rett, func, t0, x0, t1, x1, t2, x2, u0, y0) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, u0*y0) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
                : "ecx", "edi", "esi", "cc", "memory"); \
}

#define _SYS32(rett, func, t0, x0, t1, x1, t2, x2, u0, y0, u1, y1) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, u0*y0, u1*y1) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
                : "ecx", "esi", "cc", "memory"); \
}

#define _SYS33(rett, func, t0, x0, t1, x1, t2, x2, u0, y0, u1, y1, u2, y2) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, u0*y0, u1*y1, u2*y2) \
{ \
	rett res; \
	asm volatile ( \
		"call libl4_do_syscall\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
                : "ecx", "cc", "memory"); \
}

#define _SYS34(rett, func, t0, x0, t1, x1, t2, x2, u0, y0, u1, y1, u2, y2, u3, y3) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, u0*y0, u1*y1, u2*y2, u3*y3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%edx, %%ebp\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		, "=d" (*y3) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
                : "ecx", "cc", "memory"); \
}

#define _SYS40(rett, func, t0, x0, t1, x1, t2, x2, t3, x3) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, t3 x3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
		, "d" (x3) \
                : "ecx", "ebx", "edi", "esi", "cc", "memory"); \
}

#define _SYS41(rett, func, t0, x0, t1, x1, t2, x2, t3, x3, u0, y0) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, t3 x3, u0*y0) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
		, "d" (x3) \
                : "ecx", "edi", "esi", "cc", "memory"); \
}

#define _SYS42(rett, func, t0, x0, t1, x1, t2, x2, t3, x3, u0, y0, u1, y1) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, t3 x3, u0*y0, u1*y1) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
		, "d" (x3) \
                : "ecx", "esi", "cc", "memory"); \
}

#define _SYS43(rett, func, t0, x0, t1, x1, t2, x2, t3, x3, u0, y0, u1, y1, u2, y2) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, t3 x3, u0*y0, u1*y1, u2*y2) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
		, "d" (x3) \
                : "ecx", "cc", "memory"); \
}

#define _SYS44(rett, func, t0, x0, t1, x1, t2, x2, t3, x3, u0, y0, u1, y1, u2, y2, u3, y3) \
rett sys_##func(, t0 x0, t1 x1, t2 x2, t3 x3, u0*y0, u1*y1, u2*y2, u3*y3) \
{ \
	rett res; \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"mov %%edx, %%ebp\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		, "=b" (*y0) \
		, "=D" (*y1) \
		, "=S" (*y2) \
		, "=d" (*y3) \
		: "a" (_$E(_SYS_##func)) \
		, "b" (x0) \
		, "D" (x1) \
		, "S" (x2) \
		, "d" (x3) \
                : "ecx", "cc", "memory"); \
}
