#pragma once


static uint8_t inb(uint16_t port)
{
	uint8_t data;
	asm volatile ("in %%dx, %%al\n" : "=a" (data) : "d" (port));
	return data;
}

static void outb(uint16_t port, uint8_t data)
{
	asm volatile ("out %%al, %%dx\n" :: "a" (data), "d" (port));
}

static uint16_t inw(uint16_t port)
{
	uint16_t data;
	asm volatile ("in %%dx, %%ax\n" : "=a" (data) : "d" (port));
	return data;
}

static void outw(uint16_t port, uint16_t data)
{
	asm volatile ("out %%ax, %%dx\n" :: "a" (data), "d" (port));
}

static uint32_t inl(uint16_t port)
{
	uint32_t data;
	asm volatile ("in %%dx, %%eax\n" : "=a" (data) : "d" (port));
	return data;
}

static void outl(uint16_t port, uint32_t data)
{
	asm volatile ("out %%eax, %%dx\n" :: "a" (data), "d" (port));
}
