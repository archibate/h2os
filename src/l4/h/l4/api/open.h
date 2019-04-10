#pragma once

sl4fd_t sys_connect(l4id_t tid);
int sys_close(l4fd_t fd);
int sys_dup(int fd);
int sys_dup2(int fd, int dirfd);
