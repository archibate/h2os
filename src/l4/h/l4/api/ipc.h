#pragma once

int sys_nbsend(l4fd_t fd);
int sys_send(l4fd_t fd);
int sys_call(l4fd_t fd);
int sys_recv(void);
int sys_poll(void);
int sys_reply(uintptr_t badge, uintptr_t offset);
