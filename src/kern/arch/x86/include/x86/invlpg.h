#pragma once

static void invlpg(void *p)
{
	asm volatile ("invlpg (%0)" :: "p" (p) : "memory");
}
