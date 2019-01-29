#include <string.h>

char *strcpy(char *_dst, const char *src)
{
	char *dst = _dst;
	while ((*dst++ = *src++));
	return dst;
}

char *strncpy(char *_dst, const char *src, unsigned long n)
{
	char *dst = _dst;
	while (n-- && (*dst++ = *src++));
	return dst;
}

char *strcat(char *dst, const char *src)
{
	return strcpy(dst + strlen(dst), src);
}

char *strncat(char *dst, const char *src, unsigned long n)
{
	return strncpy(dst + strlen(dst), src, n);
}

unsigned long strlen(const char *src)
{
	unsigned long res = 0;
	while (*src++)
		res++;
	return res;
}

unsigned long strnlen(const char *src, unsigned long n)
{
	unsigned long res = 0;
	while (n-- && *src++)
		res++;
	return res;
}

int strcmp(const char *dst, const char *src)
{
	int res = 0;
	while (!(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

int strncmp(const char *dst, const char *src, unsigned long n)
{
	int res = 0;
	while (n-- && !(res = *dst - *src))
		if (*dst++ == 0 || *src++ == 0)
			break;
	return res;
}

char *strchr(const char *src, int ch)
{
	int i = 0;
	while (*src) {
		if (*src == ch)
			return (char*)src;
		i++, src++;
	}
	return 0;
}

char *strchrl(const char *src, int ch)
{
	char *res = strchr(src, ch);
	return res ? res : (void*)src;
}

char *strnchr(const char *src, int ch, unsigned long n)
{
	int i = 0;
	while (n-- && *src) {
		if (*src == ch)
			return (char*)src;
		i++, src++;
	}
	return 0;
}

char *strnchrl(const char *src, int ch, unsigned long n)
{
	char *res = strnchr(src, ch, n);
	return res ? res : (void*)src;
}

char *strchrin(const char *src, const char *chrs)
{
	int i = 0;
	while (*src) {
		if (strchr(chrs, *src))
			return (char*)src;
		i++, src++;
	}
	return 0;
}

char *strchrlin(const char *src, const char *chrs)
{
	char *res = strchrin(src, chrs);
	return res ? res : (void*)src;
}

char *strskipin(const char *dst, const char *chrs)
{
	while (strchr(chrs, *dst))
		dst++;
	return (char *) dst;
}

int strchop(char *dst, const char *chrs)
{
	int i = 0;
	unsigned long len = strlen(dst) - 1;
	while (len > 0 && strchr(chrs, dst[len])) {
		dst[len--] = 0;
		i++;
	}
	return len + 1;
}

char *strtrim(char *dst, const char *chrs)
{
	dst = strskipin(dst, chrs);
	strchop(dst, chrs);
	return dst;
}
