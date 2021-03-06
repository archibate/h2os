#pragma once

#include <l4/object/thread.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task);
void set_idle_task(struct ktcb *old_task);
void task_newly_forked(struct ktcb *task);
