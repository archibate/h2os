#pragma once


static uint32_t rdmsr_lo(uint32_t ecx)
{
	uint32_t eax;
	asm volatile ("rdmsr" : "=a" (eax) : "c" (ecx) : "edx");
	return eax;
}

static uint32_t rdmsr_hi(uint32_t ecx)
{
	uint32_t edx;
	asm volatile ("rdmsr" : "=d" (edx) : "c" (ecx) : "eax");
	return edx;
}

static uint64_t rdmsr(uint32_t ecx)
{
	uint64_t res;
	asm volatile ("rdmsr" : "=A" (res) : "c" (ecx));
	return res;
}

static void wrmsr(uint32_t ecx, uint64_t val)
{
	asm volatile ("wrmsr" :: "A" (val), "c" (ecx));
}

#include "msr-adrs.h"
