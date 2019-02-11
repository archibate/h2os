#pragma once

#include <l4/object/capability.h>

int cap_thread_action(struct capability *cap, unsigned int action, unsigned long argument);
