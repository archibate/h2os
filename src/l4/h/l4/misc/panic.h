#pragma once

#include <l4/misc/printk.h>
#include <l4/machine/asm/clsti.h>
#include <l4/driver/console.h>

#define _PANIC_PREFIX KL_PANIC "PANIC: "
#define panic(...) do { \
	printk(_PANIC_PREFIX __VA_ARGS__); \
	clihlt(); UNREACHABLE(); } while (0)
