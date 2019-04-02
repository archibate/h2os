#include <wchar.h>
#include <malloc.h>

wchar_t *wcsdup(const wchar_t *s)
{
	unsigned long n = wcslen(s) + 1;
	wchar_t *d = malloc(n * sizeof(wchar_t));
	wcscpy(d, s);
	return d;
}

wchar_t *wcsndup(const wchar_t *s, unsigned long n)
{
	n = wcsnlen(s, n) + 1;
	wchar_t *d = malloc(n * sizeof(wchar_t));
	wcsncpy(d, s, n);
	return d;
}
