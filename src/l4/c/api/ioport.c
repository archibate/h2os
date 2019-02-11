#include <l4/api/ioport.h>
#include <l4/machine/asm/io.h>

#if 0
int sys_io_out(unsigned short port)
{
	while (size--)
		outb(port, *buf++);
	return 0;
}

int sys_io_in(unsigned short port)
{
	while (size--)
		*buf++ = inb(port);
	return 0;
}
#endif
