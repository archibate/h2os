#pragma once

#include <h4/sys/ipcflags.h>

void ipc_rewind(void);
void ipc_begin(void);
void *ipc_getbuf(void);
int ipc_write(const void *buf, size_t n);
int ipc_read(void *buf, size_t n);
int ipc_seek_set(size_t off);
int ipc_seek_cur(ssize_t off);
size_t ipc_tell(void);
int ipc_send(int epid);
int ipc_nbsend(int epid);
int ipc_call(int epid);
int ipc_recv(int epid);
int ipc_open(key_t key, unsigned int flags);
int ipc_close(int epid);
