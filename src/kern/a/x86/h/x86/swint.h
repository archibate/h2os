#pragma once

static void x86_int3(unsigned char nr)
{
	asm volatile ("int %0" :: "Nr" (nr) : "cc");
}

static void x86_int3(void)
{
	asm volatile ("int3");
}

static void x86_int13(void)
{
	asm volatile ("int $13");
}
