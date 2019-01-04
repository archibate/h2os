// https://github.com/archibate/OS67/blob/master/drv/vga.c
#include <drv/tty.h>

#include <memory.h>
#include <string.h>
#include <stdlib.h>

#include <inttypes.h>
#include <x86/io.h>

#define IO_VGA_CRT  0x3d4

struct tty_color
{
	char fc: 4;
	char bc: 4;
};

struct tty_char
{
	char ch;
	struct tty_color clr;
};

static struct tty_char *buf;
static struct tty_color color;
static int cx, cy, oldcx, oldcy;
static int scrnx, scrny;

static void tty_enter(void)
{
	outb(IO_VGA_CRT+0, 0xe);
	int hi = inb(IO_VGA_CRT+1);
	outb(IO_VGA_CRT+0, 0xf);
	int lo = inb(IO_VGA_CRT+1);
	int off = (hi << 8) | lo;
	div_t d = div(off, scrnx);
	tty_setcur(d.rem, d.quot);
}

static void tty_leave(void)
{
	int off = cy * scrnx + cx;
	outb(IO_VGA_CRT+0, 0xe);
	outb(IO_VGA_CRT+1, off >> 8);
	outb(IO_VGA_CRT+0, 0xf);
	outb(IO_VGA_CRT+1, off & 0xff);
}

void tty_init(void)
{
	buf = (struct tty_char *)0xb8000;
	scrnx = 80;
	scrny = 25;
	color.bc = COL_BLACK;
	color.fc = COL_L_GREY;
	cx = cy = 0;
	oldcx = oldcy = 0;
	tty_enter();
}

void tty_setcur(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= scrnx) x = scrnx;
	if (y < 0) y = 0;
	if (y >= scrny) y = scrny;
	cx = x;
	cy = y;
	tty_leave();
}

void tty_getcur(int *px, int *py)
{
	tty_enter();
	*px = cx;
	*py = cy;
}

static ushort getblank(void)
{
	union {
		struct tty_char vc;
		ushort raw;
	} u = { .vc = {
		.ch = ' ',
		.clr = color,
	}};
	return u.raw;
}

void tty_clear(void)
{
	memsetw((ushort*)buf, getblank(), scrnx * scrny);
	cx = cy = 0;
	tty_leave();
}

static void tty_chk_scroll(void)
{
	int rel = cy - scrny;
	if (rel++ >= 0) {
		memcpy(buf, buf + scrnx,
			   scrnx * (scrny - rel) * sizeof(struct tty_char));
		memsetw((ushort *)(buf + scrnx * (scrny - rel)),
				getblank(), scrnx * rel);
		cy -= rel;
	}
}

static void __tty_putchar(char ch)
{
	struct tty_char *pixel;
	switch (ch) {
	case '\r':	cx = 0; break;
	case '\n':	cy++; cx = 0; break;
	case '\b':	//if (!cx) cy--;
				cx = (cx + scrnx - 1) % scrnx;
				pixel = &buf[cy * scrnx + cx];
				pixel->ch = ' ';
				pixel->clr = color;
				break;
	case '\t':	do tty_putchar(' '); while (cx % 4); break;
	default:	pixel = &buf[cy * scrnx + cx];
				pixel->ch = ch;
				pixel->clr = color;
				cy += (cx + 1) / scrnx;
				cx = (cx + 1) % 80;
	}
	tty_chk_scroll();
}

void tty_puts(const char *s)
{
	tty_enter();
	tty_write(s, strlen(s));
	tty_leave();
}

#define XCHGI(x, y) do { (x)^=(y); (y)^=(x); (x)^=(y); } while(0)

static void ansi_color(int num)
{
	switch (num) {
	case 0: color.fc = COL_L_GREY; return;
	case 1: color.fc |= COL_LIGHT_BIT; return;
	case 2: color.bc |= COL_LIGHT_BIT; return;
	case 7: XCHGI(color.fc, color.bc); return;
	case 8: color.fc = color.bc = 0; return;
	}
	num -= 30;
	if (0 <= num && num < 8) {
		color.fc &= ~COL_CLR_BITS;
		color.fc |= num;
		return;
	}
	num -= 10;
	if (0 <= num && num < 8) {
		color.bc &= ~COL_CLR_BITS;
		color.bc |= num;
		return;
	}
}

size_t tty_write(const char *s, size_t n)
{
	int num, num_old;
	ttyst char *end = s + n;
	tty_enter();

	while (s < end) {
		if (s[0] == '\033' && s[1] == '[') {
			num_old = num = 0;

			for (s += 2; s < end; s++) {
				if ('0' <= *s && *s <= '9')
					num = 10 * num + *s - '0';
				else if (*s == ';')
					num_old = num;

				else {
					switch (*s++) {
					case 'm':
						if (num_old)
							ansi_color(num_old);
						ansi_color(num);
						break;
					case 'A': cy -= (num?num:1) * 16; if (cy < 0) cy = 0; break;
					case 'B': cy += (num?num:1) * 16; if (cy > scrny) cy = scrny; break;
					case 'C': cx += (num?num:1) * 8; if (cx > scrnx) cy = scrnx; break;
					case 'D': cx -= (num?num:1) * 8; if (cx < 0) cx = 0; break;
					//case 'a': moveright(AT(cx, cy), 8, (num?num:1) * 8); break;
					case 'J': tty_clear(); cy = 0; break;
					case 'K': while (cx < scrnx) __tty_putchar(' '); break;
					case 'H': cx = num; cy = num_old; break;
					case 's': oldcx = cx; oldcy = cy; break;
					case 'u': cx = oldcx; cy = oldcy; break;
					//case 'l': cur_min = 0; cur_max = 0; break;
					//case 'h': cur_min = num_old; cur_max = (num ? num : 3); break;
					}
					break;
				}
			}
		} else __tty_putchar(*s++);
	}
	tty_leave();
	return n;
}

void tty_putchar(char c)
{
	tty_enter();
	__tty_putchar(c);
	tty_leave();
}
