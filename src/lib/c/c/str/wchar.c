#include <wchar.h>

wchar_t *wcscpy(wchar_t *_dst, const wchar_t *src)
{
	wchar_t *dst = _dst;
	while ((*dst++ = *src++));
	return dst;
}

wchar_t *wcsncpy(wchar_t *_dst, const wchar_t *src, unsigned long n)
{
	wchar_t *dst = _dst;
	while (n-- && (*dst++ = *src++));
	return dst;
}

wchar_t *wcscat(wchar_t *dst, const wchar_t *src)
{
	return wcscpy(dst + wcslen(dst), src);
}

wchar_t *wcsncat(wchar_t *dst, const wchar_t *src, unsigned long n)
{
	return wcsncpy(dst + wcslen(dst), src, n);
}

unsigned long wcslen(const wchar_t *src)
{
	unsigned long res = 0;
	while (*src++)
		res++;
	return res;
}

unsigned long wcsnlen(const wchar_t *src, unsigned long n)
{
	unsigned long res = 0;
	while (n-- && *src++)
		res++;
	return res;
}

int wcscmp(const wchar_t *dst, const wchar_t *src)
{
	int res = 0;
	while (!(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n)
{
	int res = 0;
	while (n-- && !(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

wchar_t *wcschr(const wchar_t *src, wchar_t ch)
{
	int i = 0;
	while (*src) {
		if (*src == ch)
			return (wchar_t*)src;
		i++, src++;
	}
	return 0;
}

wchar_t *wcschrl(const wchar_t *src, wchar_t ch)
{
	wchar_t *res = wcschr(src, ch);
	return res ? res : (void*)src;
}

wchar_t *wcsnchr(const wchar_t *src, wchar_t ch, unsigned long n)
{
	int i = 0;
	while (n-- && *src) {
		if (*src == ch)
			return (wchar_t*)src;
		i++, src++;
	}
	return 0;
}

wchar_t *wcsnchrl(const wchar_t *src, wchar_t ch, unsigned long n)
{
	wchar_t *res = wcsnchr(src, ch, n);
	return res ? res : (void*)src;
}

wchar_t *wcschrin(const wchar_t *src, const wchar_t *chrs)
{
	int i = 0;
	while (*src) {
		if (wcschr(chrs, *src))
			return (wchar_t*)src;
		i++, src++;
	}
	return 0;
}

wchar_t *wcschrlin(const wchar_t *src, const wchar_t *chrs)
{
	wchar_t *res = wcschrin(src, chrs);
	return res ? res : (void*)src;
}

wchar_t *wcsskipin(const wchar_t *dst, const wchar_t *chrs)
{
	while (wcschr(chrs, *dst))
		dst++;
	return (wchar_t *) dst;
}

int wcschop(wchar_t *dst, const wchar_t *chrs)
{
	int i = 0;
	unsigned long len = wcslen(dst) - 1;
	while (len > 0 && wcschr(chrs, dst[len])) {
		dst[len--] = 0;
		i++;
	}
	return len + 1;
}

wchar_t *wcstrim(wchar_t *dst, const wchar_t *chrs)
{
	dst = wcsskipin(dst, chrs);
	wcschop(dst, chrs);
	return dst;
}
