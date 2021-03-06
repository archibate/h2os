#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int ipc_seek_setw(size_t cnt)
{
	ipc_seek_set(sizeof(uintptr_t) * cnt);
	return 0;
}

int ipc_seek_curw(ssize_t cnt)
{
	ipc_seek_cur(sizeof(uintptr_t) * cnt);
	return 0;
}

int ipc_rconnect(key_t key, unsigned int flags)
{
	ipc_rewind();
	ipc_rconnect(key, flags);
}

void ipc_replyw(uintptr_t w)
{
	ipc_rewindw(w);
	ipc_reply();
}

void ipc_rewindw(uintptr_t w)
{
	ipc_rewind();
	ipc_putw(w);
}

uintptr_t ipc_getw(void)
{
	uintptr_t w;
	ipc_read(&w, sizeof(w));
	return w;
}

int ipc_putw(uintptr_t w)
{
	ipc_write(&w, sizeof(w));
	return 0;
}

uint32_t ipc_get32(void)
{
	uint32_t w;
	ipc_read(&w, sizeof(w));
	return w;
}

int ipc_put32(uint32_t w)
{
	ipc_write(&w, sizeof(w));
	return 0;
}

uint16_t ipc_get16(void)
{
	uint16_t w;
	ipc_read(&w, sizeof(w));
	return w;
}

int ipc_put16(uint16_t w)
{
	ipc_write(&w, sizeof(w));
	return 0;
}

uint8_t ipc_get8(void)
{
	uint8_t w;
	ipc_read(&w, sizeof(w));
	return w;
}

int ipc_put8(uint8_t w)
{
	ipc_write(&w, sizeof(w));
	return 0;
}
