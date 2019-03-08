#pragma once

#include <l4/object/thread.h>
#include <l4/enum/priority.h>

extern bool curr_idle;
extern unsigned char curr_priority;
extern struct list_head *running_heads[MAX_PRIORITY];
extern struct ktcb *current;

#define running_head  running_heads[curr_priority]
#define sched_get_curr()  list_entry(running_head, struct ktcb, list)

void sched_enter(void);
void sched_leave(void);
void sched_next(void);
void sched_timer_callback(void);
