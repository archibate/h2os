#include <loader.h>
#include <elf32.h>
#include <l4/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/api/mmap.h>
#include <h4/file.h>
#include <h4/fs.h>
#include <printk.h>
#include <assert.h>
#include <memory.h>
#include <bug.h>
#if 1
#define tprintk(...) printk(__VA_ARGS__)
#else
#define tprintk(...) /* Nothing */
#endif

static void loadprog(int fd, struct Proghdr *ph);
// https://github.com/archibate/jos/blob/master/lab1/code/boot/main.c
void *loadelf(int fd)
{
	struct Elfhdr e;
	struct Proghdr ph;
	int i;

	pread(fd, &e, sizeof(e), 0);

	if (e.e_magic != ELF_MAGIC)
		return 0;

	tprintk("Offset     PhysAddr   VirtAddr   Filesz     Memsz      Flg Align");
	for (i = 0; i < e.e_phnum; i++) {
		pread(fd, &ph, sizeof(ph), e.e_phoff + i * sizeof(ph));
		if (ph.p_type == PT_LOAD)
			loadprog(fd, &ph);
	}

	tprintk("Executable Entry Point at %#p", e.e_entry);
	return (void*)e.e_entry;
}

void loadprog(int fd, struct Proghdr *ph)
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
	void *pa = (void*)ph->p_pa;
	sys_mmap(fd, pa, filesz, 0);
	if (filesz < memsz) {
		int zero = open("/dev/zero", O_RDONLY);
		BUG_ON(zero < 0);
		sys_mmap(zero, pa + filesz, memsz - filesz, 0);
		close(zero);
	}

	void *p;
	for (p = pa; p < pa + memsz; p += PageSize) {
		//printk("sys_test_fault(%p, 6)", p);
		sys_test_fault(p, 6);
	}
	//ph->p_filesz, ph->p_pa, PageUp(ph->p_memsz), ph->p_flags & PF_W;
	//pread(fd, pa, ph->p_filesz, ph->p_offset);
}
