#pragma once

#include <printk.h>
#include <abort.h>
#include <console.h>

#define _PANIC_PREFIX KL_PANIC "PANIC: "
#define panic(...) do { \
	printk(_PANIC_PREFIX __VA_ARGS__); \
	abort(); UNREACHABLE(); } while (0)
