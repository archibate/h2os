#pragma once


static uint32_t rdtsc_lo(void)
{
	uint32_t res;
	asm volatile ("rdtsc" : "=a" (res) :: "edx");
	return res;
}

static uint32_t rdtsc_hi(void)
{
	uint32_t res;
	asm volatile ("rdtsc" : "=d" (res) :: "eax");
	return res;
}

static uint64_t rdtsc(void)
{
	uint64_t res;
	asm volatile ("rdtsc" : "=A" (res) :);
	return res;
}
