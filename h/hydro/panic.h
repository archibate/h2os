#pragma once

#include <hydro/printk.h>
#include <asm/clsti.h>
#include <drv/console.h>
#include <ccutils.h>

#define _PANIC_PREFIX KL_PANIC "PANIC: "
#define panic(...) do { \
	printk(_PANIC_PREFIX __VA_ARGS__); \
	clihlt(); UNREACHABLE(); } while (0)
