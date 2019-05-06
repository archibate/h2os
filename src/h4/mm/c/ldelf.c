#include "loader.h"
#include <elf32.h>
#include <l4/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/api/mmap.h>
#include <l4/api/mmctl.h>
#include <h4/mm/prot.h>
#include <h4/file.h>
#include <h4/fs.h>
#include <h4/mm.h>
#include <emmap.h>
#include <stddef.h>
#include <printk.h>
#include <assert.h>
#include <memory.h>
#include <errno.h>
#include <bug.h>
#if 0
#define tprintk(...) printk(__VA_ARGS__)
#else
#define tprintk(...) /* Nothing */
#endif

static void loadprog(int mmc, int fd, struct Proghdr *ph);
// https://github.com/archibate/jos/blob/master/lab1/code/boot/main.c
int loadelf(int mmc, int fd)
{
	struct Elfhdr e;
	static struct Proghdr ph;
	int i;

	pread(fd, &e, sizeof(e), 0);

	if (e.e_magic != ELF_MAGIC)
		return -ENOEXEC;

	//printk("mmc_destroy");//
	sys_mmctl_destroy(mmc);

	tprintk("Offset     PhysAddr   VirtAddr   Filesz     Memsz      Flg Align");
	for (i = 0; i < e.e_phnum; i++) {
		//printk("mpread");//
		pread(fd, &ph, sizeof(ph), e.e_phoff + i * sizeof(ph));
		//printk("mprest");//
		if (ph.p_type == PT_LOAD)
			loadprog(mmc, fd, &ph);
	}

	tprintk("Executable Entry Point at %#p", e.e_entry);
	sys_mmctl_setpcsp(mmc, e.e_entry, 0);
	return 0;
}

void loadprog(int mmc, int fd, struct Proghdr *ph)
{
	tprintk("%#p %#p %#p %#p %#p %c%c%c %#x",
		ph->p_offset, ph->p_pa, ph->p_va,
		ph->p_filesz, ph->p_memsz,
		ph->p_flags & PF_R ? 'R' : '-',
		ph->p_flags & PF_W ? 'W' : '-',
		ph->p_flags & PF_X ? 'X' : '-',
		ph->p_align);
	assert(ph->p_align >= PageSize);
	assert(PageOffset(ph->p_pa) == 0);
	size_t filesz = PageUp(ph->p_filesz);
	//assert(PageOffset(filesz) == 0);
	size_t memsz = PageUp(ph->p_memsz);
	lseek(fd, ph->p_offset, SEEK_SET);
	uintptr_t pa = ph->p_pa;
	int prot = 0;
	if (ph->p_flags & PF_R)
		prot |= PROT_READ;
	if (ph->p_flags & PF_W)
		prot |= PROT_WRITE;
	if (ph->p_flags & PF_X)
		prot |= PROT_EXEC;
	emmap(mmc, fd, pa, filesz, prot);
	if (filesz < memsz) {
		int zero = open("/dev/zero", O_RDONLY);
		BUG_ON(zero < 0);
		emmap(mmc, zero, pa + filesz, memsz - filesz, prot);
		close(zero);
	}

	//ph->p_filesz, ph->p_pa, PageUp(ph->p_memsz), ph->p_flags & PF_W;
	//pread(fd, pa, ph->p_filesz, ph->p_offset);
}
