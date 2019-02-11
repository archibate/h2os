#pragma once

#include <l4/object/thread.h>

extern bool curr_idle;
extern unsigned char curr_priority;
#define MAX_PRIORITY  256
extern struct list_head *running_heads[MAX_PRIORITY];
extern struct ktcb *current;

#define running_head  running_heads[curr_priority]
#define sched_get_curr()  list_entry(running_head, struct ktcb, list)

void thread_set_priority(struct ktcb *tcb, unsigned int priority);
void thread_active(struct ktcb *x);
void thread_suspend(struct ktcb *x);

void sched_enter(void);
void sched_leave(void);
void sched_next(void);
void sched_timer_callback(void);
