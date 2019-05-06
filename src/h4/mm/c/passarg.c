#include "passarg.h"
#include "emmap.h"
#include <l4/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/api/mmap.h>
#include <l4/api/mmctl.h>
#include <h4/mm/prot.h>
#include <h4/file.h>
#include <h4/fs.h>
#include <h4/sys/ubase.h>
#include <bug.h>
#include <string.h>

static int psa_mmc = 0, psa_fd = -1;
static size_t psa_size;
static uintptr_t psa_addr;

static void psa_putpage(size_t size)
{
	if (size == 0)
		return;
	BUG_ON(PageOffset(size) != 0);
	BUG_ON(PageOffset(psa_addr) != 0);
	BUG_ON(lseek(psa_fd, 0, 0) != 0);
	//printk("emmap(%d, %d, %p, %p, %d)", psa_mmc, psa_fd, psa_addr, PageUp(size), PROT_READ | PROT_WRITE);
	BUG_ON(emmap(psa_mmc, psa_fd, psa_addr, PageUp(size), PROT_READ | PROT_WRITE) < 0);
	psa_addr += size;
	close(psa_fd);
	psa_fd = open("/dev/zero", O_RDWR);
	BUG_ON(psa_fd < 0);
}

static void psa_init(uintptr_t addr, int mmc)
{
	psa_addr = addr;
	psa_mmc = mmc;
	psa_fd = open("/dev/zero", O_RDWR);
	BUG_ON(psa_fd < 0);
	psa_size = 0;
}

static void psa_dtor(void)
{
	psa_putpage(PageUp(psa_size));
	psa_addr = 0;
	psa_mmc = 0;
	close(psa_fd);
	psa_fd = -1;
	psa_size = 0;
}

static void psa_write(const void *s, size_t size)
{
	psa_size += size;
	size_t posiz = PageOffset(psa_size);
	if (psa_size >= PageSize) {
		if (size > posiz)
			BUG_ON(write(psa_fd, s, size - posiz) != size - posiz);
		s += size - posiz;
		psa_putpage(PageDown(psa_size));
	}
	psa_size = posiz;
	if (posiz > size)
		posiz = size;
	if (posiz != 0)
		BUG_ON(write(psa_fd, s, posiz) != posiz);
	s += posiz;
}

static void psa_putstr(const char *s)
{
	psa_write(s, strlen(s) + 1);
}

static void psa_putw(uintptr_t w)
{
	psa_write(&w, sizeof(w));
}

void stack_init(int mmc, char *const *argv, char *const *envp, uintptr_t *sp)
{
	psa_init(UserStackTop, mmc);
	char *const *p;
	for (p = argv; *p; p++)
		psa_putstr(*p);
	psa_putstr("");
	for (p = envp; *p; p++)
		psa_putstr(*p);
	psa_putstr("");
	psa_dtor();
	psa_init(UserStackBottom, mmc);
	psa_putpage(UserStackTop - UserStackBottom);
	psa_dtor();
	*sp = UserStackTop;
}
