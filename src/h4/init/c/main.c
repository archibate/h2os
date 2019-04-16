#if 1
#include <l4/types.h>
#include <l4/api/hello.h>
#else
#define sys_con_write(y,z) write(cons, y, z)
#define sys_con_putchar(y) putchar(cons, y)
#endif
/*#include <l4/api/rtalloc.h>
#include <l4/enum/rtype.h>
#include <l4/enum/thread-registers.h>
#include <l4/api/thread.h>
#include <l4/api/sched.h>*/
#include <l4/api/mmap.h>
#include <l4/stdafx.h>
#include <l4/machine/mmu/page.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/fs.h>
#include <h4/file.h>
#include <h4/sys/sched.h>
#include <c4/liballoc.h>
#include <printk.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <bug.h>

#if 0//{{{
void task_a(void)
{
	sys_print("TASK_A!!!");

	for (;;)
		sys_con_putchar('a');
}

void task_b(void)
{
	sys_print("TASK_B!!!");

	for (;;)
		sys_con_putchar('b');
}

static char fsf_a[2048], fsf_b[2048];
#endif//}}}

const int libh4_serve_id = -1;

void main(void)
{
#if 0//{{{
	l4id_t id;

	id = sys_rt_new(RTYPE_THREAD);
	sys_thread_set_register(id, THREAD_REG_PC, (word_t)task_a);
	sys_thread_set_register(id, THREAD_REG_SP, (word_t)fsf_a+2048);
	sys_thread_active(id);

	id = sys_rt_new(RTYPE_THREAD);
	sys_thread_set_register(id, THREAD_REG_PC, (word_t)task_b);
	sys_thread_set_register(id, THREAD_REG_SP, (word_t)fsf_b+2048);
	sys_thread_active(id);

	int i;
	for (i = 0; i < 10000; i++)
		sys_con_putchar('m');
#endif//}}}

	static char heap[4096*32];
	liballoc_set_memory(&heap, sizeof(heap));
	pause();
	pause();

	int fs, kbd, hello, cons, hda, fd;
again:
	cons = open("/dev/cons", O_WRONLY);
	BUG_ON(cons < 0);
	hda = open("/dev/hda", O_RDONLY);
	BUG_ON(hda < 0);
	kbd = open("/dev/keybd", O_RDONLY);
	BUG_ON(kbd < 0);
	hello = open("/dev/hello", O_RDONLY);
	BUG_ON(hello < 0);
	fd = open("README.md", O_RDONLY);
	BUG_ON(fd < 0);
	FILE *out = fdopen(cons, "w");
	BUG_ON(out == NULL);
	FILE *fp = fdopen(fd, "r");
	BUG_ON(fp == NULL);
	FILE *kb = fdopen(kbd, "r");
	BUG_ON(kb == NULL);

	char buf[128];
	int ch;
	for (;;) {
		ch = fgetc(kb);
		BUG_ON(ch <= 0);
		if (ch == 'z') {
			ssize_t ret = pread(hello, buf, sizeof(buf), 0);
			if (ret > 0)
				sys_con_write(buf, ret);
		} else if (ch == 'x') {
			ssize_t ret = read(hello, buf, sizeof(buf));
			if (ret > 0)
				sys_con_write(buf, ret);
			lseek(hello, -7, 2);
			//if (!ret) lseek(hello, 0, 0);
		} else if (ch == 'r') {
			if (NULL == fgets(buf, sizeof(buf), fp)) {
				BUG_ON(0 > rewind(fp));
				BUG_ON(NULL == fgets(buf, sizeof(buf), fp));
			}
			fputs(buf, out);
			//write(cons, buf, strlen(buf));
		} else if (ch == 'c') {
			ssize_t ret = pread(hda, buf, sizeof(buf), 0);
			if (ret > 0)
				sys_con_write(buf, ret);
		} else if (ch == 'm') {
			char *page = (void*)0xd000000;
			sys_mmap(hda, page, PageSize, 0);
			fputc(page[19], out);
		}
	}

	_exit(0);
}
