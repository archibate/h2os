#pragma once

#include <inttypes.h>
#include <compiler.h>

#define _JMPBUF_REGS 5 /* ebx, edi, esi, ebp, eip */

typedef uintptr_t jmp_buf[_JUMBUF_REGS];

long setjmp(jmp_buf jb);
void _NORETURN longjmp(jmp_buf jb, long val);
