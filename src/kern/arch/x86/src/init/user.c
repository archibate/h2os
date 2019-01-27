#include <k/printk.h>
#include <k/panic.h>
#include <o/region.h>
#include <o/frames.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <memory.h>
#include <assert.h>
#include <elf32.h>


static void *loadelf(const char *data, const char *edata);
void setup_user(void)
{
	void *ep;
	extern const char initImage[], initImageEnd[];
	if (!(ep = loadelf(initImage, initImageEnd)))
		panic("init image ELF format wrong");
}

static void map_zero(va_t va0, size_t size, int rw);
static void loadprog(const char *data, struct Proghdr *ph)
{
	assert(ph->p_align >= PageSize);
	assert(PageOffset(ph->p_pa) == 0);
	map_zero(ph->p_pa, PageUp(ph->p_memsz), ph->p_flags & PF_W);
	printk("%#p %#p %#p %#p %#p %c%c%c %#x",
		ph->p_offset, ph->p_pa, ph->p_va,
		ph->p_filesz, ph->p_memsz,
		ph->p_flags & PF_R ? 'R' : '-',
		ph->p_flags & PF_W ? 'W' : '-',
		ph->p_flags & PF_X ? 'X' : '-',
		ph->p_align);
	memcpy((void*)ph->p_pa, data + ph->p_offset, ph->p_filesz);
}


// https://github.com/archibate/jos/blob/master/lab1/code/boot/main.c
void *loadelf(const char *data, const char *edata)
{
	struct Elfhdr *e = (struct Elfhdr *)data;
	struct Proghdr *ph, *eph;

	if (e->e_magic != ELF_MAGIC)
		return 0;

	ph = (struct Proghdr *) (data + e->e_phoff);
	eph = ph + e->e_phnum;

	printk("Offset     PhysAddr   VirtAddr   Filesz     Memsz      Flg Align");
	for (; ph < eph; ph++) {
		if (ph->p_type == PT_LOAD)
			loadprog(data, ph);
	}

	printk("load init image done");
	return (void*)e->e_entry;
}


void map_zero(va_t va0, size_t size, int rw)
{
	Frames_t fsMap   = {ptePerm: (rw ? PtePerm_UserRW : PtePerm_UserRO)};
	Frames_t fsPgtab = {ptePerm: PtePerm_KernRW};
	Region_t reg     = {va0: va0, va1: va0 + size};
	Region_Map(&reg, &fsMap, &fsPgtab);
}
