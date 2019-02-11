#pragma once

extern const char *__ntEnum(const char **nt, int n);
#define ntEnum(e, n) do { \
	extern const char *__ntNameTableOfEnum_##e[];\
	__ntEnum(__ntNameTableOfEnum_##e, n); \
} while (0)
