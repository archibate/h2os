#include <l4/boot/ldelf.h>
#include <l4/lib/elf32.h>
//#include <l4/boot/alloc.h>
#include <l4/generic/allocpage.h>
#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pte.h>
#include <l4/machine/mmu/mmu.h>
#include <l4/misc/printk.h>
#include <l4/misc/assert.h>
#include <memory.h>
#ifdef CONFIG_DUMP_LDMOD
#define tprintk(...) printk(__VA_ARGS__)
#else
#define tprintk(...) /* Nothing */
#endif

static void loadprog(const void *data, struct Proghdr *ph);
// https://github.com/archibate/jos/blob/master/lab1/code/boot/main.c
void *loadelf(const void *data, const void *end)
{
	struct Elfhdr *e = (struct Elfhdr *)data;
	struct Proghdr *ph, *eph;

	if (e->e_magic != ELF_MAGIC)
		return 0;

	ph = (struct Proghdr *) (data + e->e_phoff);
	eph = ph + e->e_phnum;

	tprintk("Offset     PhysAddr   VirtAddr   Filesz     Memsz      Flg Align");
	for (; ph < eph; ph++) {
		if (ph->p_type == PT_LOAD)
			loadprog(data, ph);
	}

	tprintk("Executable Entry Point at %#p", e->e_entry);
	return (void*)e->e_entry;
}

static void map_zero(va_t va0, size_t size, bool rw);
void loadprog(const void *data, struct Proghdr *ph)
{
	assert(ph->p_align >= PageSize);
	assert(PageOffset(ph->p_pa) == 0);
	map_zero(ph->p_pa, PageUp(ph->p_memsz), ph->p_flags & PF_W);
	tprintk("%#p %#p %#p %#p %#p %c%c%c %#x",
		ph->p_offset, ph->p_pa, ph->p_va,
		ph->p_filesz, ph->p_memsz,
		ph->p_flags & PF_R ? 'R' : '-',
		ph->p_flags & PF_W ? 'W' : '-',
		ph->p_flags & PF_X ? 'X' : '-',
		ph->p_align);
	memcpy((void*)ph->p_pa, data + ph->p_offset, ph->p_filesz);
}

/////////////////////////////////////////////////

static pte_t *touch_pdi(pde_t *pd, uint pdi);
void map_zero(va_t va0, size_t size, bool rw)
{
	uint ptePerm = rw ? PtePerm_UserRW : PtePerm_UserRO;
	va_t va1 = va0 + size;
	pde_t *pd = (pde_t*)mmu_getPgdirPaddr();
	pte_t *pt = touch_pdi(pd, PdeIndex(va0));
	size_t secoff = SectionOffset(va0);
	va_t va;
	pa_t page;
	assert(PageOffset(va0) == 0);
	assert(PageOffset(va1) == 0);
	for (va = va0; va < va1; va += PageSize, secoff += PageSize)
	{
		if (secoff >= SectionSize) {
			pt = touch_pdi(pd, PdeIndex(va));
			secoff = 0;
		}
		page = (pa_t)alloc_page();
		pt[PteIndex(va)] = Pte(page, ptePerm);
		mmu_invalidatePage(va);
	}
}

pte_t *touch_pdi(pde_t *pd, uint pdi)
{
	pte_t *pt;
	if (!PdeIsValid(pd[pdi])) {
		pt = alloc_page();
		pd[pdi] = PdePgtab((va_t)pt);
	} else {
		pt = (pte_t*)PdePgtabAddr(pd[pdi]);
	}
	return pt;
}
