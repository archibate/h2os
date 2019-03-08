// https://github.com/archibate/OS67/blob/master/drv/kb.c
#include <l4/types.h>
#include <l4/defines.h>
#include <l4/api/hello.h>
#include <l4/api/asyncep.h>
#include <l4/api/softirq.h>
#include <l4/enum/irq-nrs.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>

static int kbds;

void kbds_init(void)
{
	kbds = ipc_open(SVID_KEYBD, IPC_CREAT | IPC_SEND);
	if (kbds < 0) {
		sys_print("error in opening kb endpoint");
		sys_halt();
	}
}

void kbds_putchar(int ch)
{
	ipc_rewind();
	ipc_put32(ch);
	ipc_send(kbds);//todo: impl an fifo buffer inside this server process
	//t!: use two threads in this server, one for intrhand, one for sendbuf
}

void kb_handler(void);
int main(void)
{
	kbds_init();
	sys_softirq_set_enable(IRQ_KEYBD, true);
	while (1) {
		sys_async_listen(IRQ_KEYBD);
		//sys_con_putchar('K');
		sys_softirq_done(IRQ_KEYBD);
		kb_handler();
	}
}

// Keyboard Map {{{
/* maybe scancode set 1 ?*/
static char kb_map[128] = {
	0,
	0x1b,   /* esc */
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0,      /* left ctrl */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',
	0,      /* left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/',
	0,    /* right shift */
	'*',
	0,  /* alt */
	' ',/* space*/
	0,  /* capslock */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* f1 ... f10 */
	0,    /* num lock*/
	0,    /* scroll Lock */
	0,    /* home key */
	0,    /* up arrow */
	0,    /* page up */
	'-',
	0,    /* left arrow */
	0,
	0,    /* right arrow */
	'+',
	0,    /* end key*/
	0,    /* down arrow */
	0,    /* page down */
	0,    /* insert key */
	0,    /* delete key */
	0,   0,   0,
	0,    /* f11 key */
	0,    /* f12 key */
	0,    /* all other keys are undefined */
};

static char kb_shift_map[128] =
{
	0,
	0x1b, /* esc */
	'!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0,  /* left control */
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~',
	0,    /* left shift */
	'|', 'Z', 'X', 'C', 'V', 'B', 'N',
	'M', '<', '>', '?',
	0,    /* right shift */
	'*',
	0,  /* alt */
	' ',/* space bar */
	0,  /* caps lock */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* f1 ... f10 */
	0,    /* num lock*/
	0,    /* scroll lock */
	0,    /* home key */
	0,    /* up arrow */
	0,    /* page up */
	'-',
	0,    /* left arrow */
	0,
	0,    /* right arrow */
	'+',
	0,    /* end key*/
	0,    /* down arrow */
	0,    /* page down */
	0,    /* insert key */
	0,    /* delete key */
	0, 0, 0,
	0,    /* f11 key */
	0,    /* f12 key */
	0,    /* all other keys are undefined */
};
// }}}

#define SHIFT    (1<<0)
#define CTRL     (1<<1)
#define ALT      (1<<2)
#define E0ESC    (1<<3)

static char kb_mode;
static bool kb_caps;

static char shift(char uc)
{
	if (kb_mode & E0ESC) {
		switch (uc) {
		case 0x1D: return CTRL;
		case 0x38: return ALT;
		}
	} else {
		switch (uc) {
		case 0x2A:
		case 0x36: return SHIFT;
		case 0x1D: return CTRL;
		case 0x38: return ALT;
		}
	}
	return 0;
}

#define TTY_EOF (-1)
#define TTY_INT (-2)

int ctrl(int ch)
{
	switch (ch) {
	case 'd': return TTY_EOF;
	case 'c': return TTY_INT;
	};
	return ch;
}

int alt(int ch)
{
	return ch;
}

#include <l4/machine/asm/io.h>

#define KB_DATA     0x60
#define KB_STAT     0x64
#define KB_CMD      0x64

#define KB_STAT_OBF 0x01 // output buffer full

#define KB_IS_RELEASE(sc)   ((sc) & 0x80)
#define KB_IS_ESCAPE(sc)    ((sc) == 0xe0)

void kb_handler(void)
{
	if (!(inb(KB_STAT) & KB_STAT_OBF))
		return;

	unsigned char sc = inb(KB_DATA);

	if (KB_IS_ESCAPE(sc)) {
		kb_mode |= E0ESC;
		return;
	}

	switch (sc) {
	case 0x3A:
		kb_caps = !kb_caps;
		return;
	}

	int uc = sc & 0x7f;
	char m = shift(uc);
	if (m) {
		if (KB_IS_RELEASE(sc))
			kb_mode &= ~m;
		else
			kb_mode |= m;
		return;
	}

	int ch;
	if (kb_mode & SHIFT)
		ch = kb_shift_map[uc];
	else
		ch = kb_map[uc];

	if (kb_mode & CTRL)
		ch = ctrl(ch);

	if (kb_mode & ALT)
		ch = alt(ch);

	if (kb_caps) {
		if (ch >= 'a' && ch <= 'z')
			ch -= 'a' - 'A';
		else if (ch >= 'A' && ch <= 'Z')
			ch -= 'A' - 'a';
	}

	if (KB_IS_RELEASE(sc))
		kb_mode &= ~E0ESC;

	else if (ch)
		kbds_putchar(ch);
}
