#pragma once

int sys_thread_suspend(l4id_t tid);
int sys_thread_active(l4id_t tid);
int sys_thread_set_priority(l4id_t tid, unsigned int priority);
