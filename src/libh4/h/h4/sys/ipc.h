#pragma once

#include <h4/sys/ipcflags.h>

void ipc_rewind(void);
void *ipc_getbuf(void);
int ipc_write(const void *buf, size_t n);
int ipc_read(void *buf, size_t n);
int ipc_seek_set(size_t off);
int ipc_seek_cur(ssize_t off);
size_t ipc_tell(void);
int ipc_send(int fd);
int ipc_nbsend(int fd);
int ipc_call(int fd);
int ipc_recv(int fd);
int ipc_open(key_t key, unsigned int flags);
int ipc_close(int fd);
uintptr_t ipc_getw(void);
int ipc_putw(uintptr_t w);
uint32_t ipc_get32(void);
int ipc_put32(uint32_t w);
uint16_t ipc_get16(void);
int ipc_put16(uint16_t w);
uint8_t ipc_get8(void);
int ipc_put8(uint8_t w);
