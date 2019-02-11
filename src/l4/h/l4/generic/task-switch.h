#pragma once

#include <l4/object/thread.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task);
