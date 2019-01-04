#include <memory.h>

void *memcpy(void *_dst, const void *_src, unsigned long n)
{
	char *dst = _dst;
	const char *src = _src;
	while (n--)
		*dst++ = *src++;
	return _dst;
}

void *memrcpy(void *_dst, const void *_src, unsigned long n)
{
	char *dst = _dst;
	const char *src = _src;
	dst += n;
	src += n;
	while (n--)
		*--dst = *--src;
	return _dst;
}

int memcmp(const void *_dst, const void *_src, unsigned long n)
{
	const char *dst = _dst;
	const char *src = _src;
	int res = 0;
	while (n-- && (res = *dst++ - *src++));
	return res;
}

void *memset(void *_dst, int val, unsigned long n)
{
	char *dst = _dst;
	while (n--)
		*dst++ = val;
	return _dst;
}

void *memsetw(void *_dst, int val, unsigned long n)
{
	short *dst = _dst;
	while (n--)
		*dst++ = val;
	return _dst;
}

void *memchr(const void *_dst, int chr, unsigned long n)
{
	const char *dst = _dst;
	while (n--)
		if (*dst == chr)
			return (void*)dst;
		else
			dst++;
	return 0;
}

void *memchrl(const void *dst, int chr, unsigned long n)
{
	void *res = memchr(dst, chr, n);
	return res ? res : (void*)dst;
}
