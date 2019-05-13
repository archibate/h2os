#pragma once

#include <l4/object/thread.h>

void thread_init(struct ktcb *tcb);
void __thread_init(struct ktcb *tcb);
void thread_revoke(struct ktcb *tcb);
void thread_delete(struct ktcb *tcb);
void thread_set_priority(struct ktcb *tcb, unsigned char priority);
void thread_active(struct ktcb *x);
void thread_suspend(struct ktcb *x);
struct ktcb *thread_new(void);
