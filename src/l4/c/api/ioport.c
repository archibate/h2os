#include <l4/api/ioport.h>
#include <l4/machine/asm/io.h>

int cap_ioport_send(struct capability *cap, unsigned long addr, const char *buf, size_t size)
{
	unsigned short port = addr;
	while (size--)
		outb(port, *buf++);
	return 0;
}

int cap_ioport_recv(struct capability *cap, unsigned long addr, char *buf, size_t size)
{
	unsigned short port = addr;
	while (size--)
		*buf++ = inb(port);
	return 0;
}
