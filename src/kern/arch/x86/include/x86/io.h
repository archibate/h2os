#pragma once

#include <inttypes.h>

static uchar inb(ushort port)
{
	uchar data;
	asm volatile ("in %%dx, %%al\n" : "=a" (data) : "d" (port));
	return data;
}

static void outb(ushort port, uchar data)
{
	asm volatile ("out %%al, %%dx\n" :: "a" (data), "d" (port));
}

static ushort inw(ushort port)
{
	ushort data;
	asm volatile ("in %%dx, %%ax\n" : "=a" (data) : "d" (port));
	return data;
}

static void outw(ushort port, ushort data)
{
	asm volatile ("out %%ax, %%dx\n" :: "a" (data), "d" (port));
}

static ulong inl(ushort port)
{
	ulong data;
	asm volatile ("in %%dx, %%eax\n" : "=a" (data) : "d" (port));
	return data;
}

static void outl(ushort port, ulong data)
{
	asm volatile ("out %%eax, %%dx\n" :: "a" (data), "d" (port));
}
