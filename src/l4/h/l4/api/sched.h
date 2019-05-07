#pragma once

int sys_exit(void);
l4id_t sys_getpid(void);
int sys_sched_next(void);
int sys_wait_first(void);
sl4id_t sys_fork(void);
