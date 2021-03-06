#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <memory.h>
#include <stdlib.h>
#ifdef _CC_HAS_ALLOCA
#include <alloca.h>
#endif


static
void _geutoa
	( void (*putch)(char, void *)
	, void *arg
	, const char *digsel
	, unsigned long u
	, unsigned int base
	, unsigned long len
	)
{
#ifdef _CC_HAS_ALLOCA
	char *buf = alloca(len);
#else
	char buf[40];
#endif
	char *p = buf + len;
	do {
		*--p = digsel[u % base];
		u /= base;
	} while (p > buf);
	while (p < buf + len)
		putch(*p++, arg);
}


static
unsigned long geusslen
	( unsigned long u
	, unsigned int base
	)
{
	unsigned long len = 0;
	do {
		len++;
	} while ((u /= base));
	return len;
}


static inline
long get_lon_arg
	( va_list *pap
	, int lon
	)
{
	if (!lon)
		return va_arg(*pap, int);
	else
		return va_arg(*pap, long);
}


static
void gputtimes
	( void (*putch)(char, void *)
	, void *arg
	, int times
	, char ch
	)
{
	while (times-- > 0)
		putch(ch, arg);
}


// reference: https://baike.so.com/doc/5432134-5670428.html
int vgprintf
	( void (*putch)(char, void *)
	, void *arg
	, const char *fmt
	, va_list ap
	)
{
	char c;
	wchar_t wc;

	while ((c = *fmt++)) {
		if (c != '%') {
			putch(c, arg);
			continue;
		}

		const char *s = NULL;
		const wchar_t *ws = NULL;
		unsigned long x = 0;
		long l = 0;
		int err = 0;
		int lon = 0;
		const char *digsel = "0123456789abcdef";
		int base = 10;
		int algnr = 0;
		unsigned long len, maxlen = -1;
		char possign = 0;
		char prefill = ' ';
		int sharp = 0, maxitied = 0;
		int rightalg = 0;
again:
		switch (c = *fmt++) {
		case 0:
			goto break_while;

		default:
			if ('1' <= c && c <= '9') {
				len = strtoul(fmt - 1, &fmt, 10);
				goto setalg;
			}
			putch(c, arg);
			break;

		case '*':
			len = va_arg(ap, int);
setalg:
			if (!maxitied)
				algnr = len;
			else
				maxlen = len;
			goto again;

		case '%':
			putch(c, arg);
			break;

		case 'l':
			lon++;
			goto again;

		case '.':
			maxitied = 1;
			goto again;

		case '#':
			sharp = 1;
			goto again;

		case '0':
			prefill = c;
			goto again;

		case ' ':
		case '+':
			possign = c;
			goto again;

		case '-':
			rightalg = 1;
			goto again;

		case 'c':
			c = va_arg(ap, int);
			putch(c, arg);
			break;

		case 'm':
			err = va_arg(ap, int);
			s = strerror(err);
			goto prstr;

		case 's':
			s = va_arg(ap, const char *);
prstr:
			if (algnr && !rightalg) {
				len = strlen(s);
				gputtimes(putch, arg, algnr - len, prefill);
			}
			len = 0;
			while (maxlen-- && (c = *s++))
				putch(c, arg), len++;
			if (algnr && rightalg) {
				gputtimes(putch, arg, algnr - len, prefill);
			}
			break;

		case 'S':
			ws = va_arg(ap, const wchar_t *);
			if (algnr && !rightalg) {
				len = wcslen(ws);
				gputtimes(putch, arg, algnr - len, prefill);
			}
			len = 0;
			while (maxlen-- && (wc = *ws++))
				putch(wc & 0xff, arg), len++;
			if (algnr && rightalg) {
				gputtimes(putch, arg, algnr - len, prefill);
			}
			break;

		case 'i':
		case 'd':
			l = get_lon_arg(&ap, lon);
			if (l < 0) {
				l = -l;
				putch('-', arg);
			} else if (possign) {
				putch(possign, arg);
			}
			x = l;
			goto utos;

		case 'o':
			base = 8;
			goto utos;

		case 'p':
			algnr = sizeof(long) * 2;
			prefill = '0';
			c = 'x';
		case 'X':
			digsel = "0123456789ABCDEF";
		case 'x':
			base = 16;
		case 'u':
			x = get_lon_arg(&ap, lon);
		utos:
			if (sharp) {
				putch('0', arg);
				if (c != 'o')
					putch(c, arg);
			}
			len = geusslen(x, base);
			if (algnr && !rightalg) {
				gputtimes(putch, arg, algnr - len, prefill);
			}
			_geutoa(putch, arg, digsel, x, base, len);
			if (algnr && rightalg) {
				gputtimes(putch, arg, algnr - len, prefill);
			} // TODO: consider another algl??
			break;
		}
	}
break_while:
	return 0;
}


int gprintf
	( void (*putch)(char, void *)
	, void *arg
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vgprintf(putch, arg, fmt, ap);
	va_end(ap);
	return res;
}
