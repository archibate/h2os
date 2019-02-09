#include <stdlib.h>

static
int ctoi(int c)
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'z')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'Z')
		return c - 'A' + 10;
	else
		return -1;
}


unsigned long strtoul
	( const char *s
	, const char **endp
	, int base
	)
{
	int c;
	unsigned long res = 0;
	while ((c = *s)) {
		c = ctoi(c);
		if (c < 0 || c >= base)
			break;
		res *= base;
		res += c;
		s++;
	}
	if (endp)
		*endp = s;
	return res;
}

long strtol
	( const char *s
	, const char **endp
	, int base
	)
{
	int sign = 1;
	switch (*s) {
	case '-': sign = -1;
	case '+': s++; break;
	}
	return sign * strtoul(s, endp, base);
}

int atoi(const char *s)
{
	return (int)strtol(s, 0, 10);
}

long atol(const char *s)
{
	return strtol(s, 0, 10);
}
