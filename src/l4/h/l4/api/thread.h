#pragma once

l4id_t sys_getpid(void);
int sys_sched_next(void);
int sys_thread_check(l4id_t tid);
int sys_thread_suspend(l4id_t tid);
int sys_thread_active(l4id_t tid);
int sys_thread_set_register(l4id_t tid, unsigned int reg, word_t pc);
int sys_thread_set_priority(l4id_t tid, unsigned char priority);
int sys_thread_get_priority(l4id_t tid);
int sys_thread_get_state(l4id_t tid);
