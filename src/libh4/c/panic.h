#pragma once

#define _PANIC_PREFIX "fatal error: "
#define panic(...) do { \
	cprintf(_PANIC_PREFIX __VA_ARGS__); \
	UNREACHABLE(); } while (0)
