#pragma once

int sys_nbsend(l4fd_t fd);
int sys_send(l4fd_t fd);
int sys_call(l4fd_t fd);
int sys_recv(l4fd_t fd);
int sys_reply(uintptr_t badge);
//int SYS_connect(l4id_t id, unsigned int flags);
