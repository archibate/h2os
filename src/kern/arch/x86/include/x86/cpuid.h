#pragma once

#include <inttypes.h>

enum CPUID_Index
{
	CPUID_EAX = 0,
	CPUID_EBX,
	CPUID_EDX,
	CPUID_ECX,
};

static void cpuid(void *_r, uint32_t eax, uint32_t ecx)
{
	uint32_t *r = _r;
	asm volatile ("cpuid"
		: "=a" (r[CPUID_EAX])
		, "=b" (r[CPUID_EBX])
		, "=d" (r[CPUID_EDX])
		, "=c" (r[CPUID_ECX])
		: "a" (eax)
		, "c" (ecx)
		);
}
