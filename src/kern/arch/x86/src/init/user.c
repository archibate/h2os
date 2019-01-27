#include <k/printk.h>
#include <k/panic.h>
#include <k/bootml.h>
#include <l4/can.h>
#include <mmu/mmu.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <memory.h>
#include <assert.h>
#include <elf32.h>


static int alloc_page_pa = kern_ptpool_beg;
static pa_t alloc_page(void)
{
	pa_t pa = alloc_page_pa;
	alloc_page_pa += PageSize;
	return pa;
}


static void *loadelf(const char *data, const char *edata);
void setup_user(void)
{
	void *ep;
	extern const char initImage[], initImageEnd[];
	if (!(ep = loadelf(initImage, initImageEnd)))
		panic("init image ELF format wrong");
}

static void map_zero(va_t va0, size_t size, int can);
static void loadprog(const char *data, struct Proghdr *ph)
{
	assert(ph->p_align >= PageSize);
	assert(PageOffset(ph->p_pa) == 0);
	map_zero(ph->p_pa, PageUp(ph->p_memsz), ph->p_flags & PF_W ? CanRW : CanRO);
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


static pte_t *touch_pdi(pde_t *pd, uint pdi)
{
	pte_t *pt;
	if (!PdeIsValid(pd[pdi])) {
		pt = (pte_t*)alloc_page();
		memset(pt, 0, PgtabSize);
		pd[pdi] = PdePgtab((va_t)pt);
	} else {
		pt = (pte_t*)PdePgtabAddr(pd[pdi]);
	}
	return pt;
}

void map_zero(va_t va0, size_t size, int can)
{
	pde_t *pd = (pde_t*)mmu_getPgdirPaddr();
	pte_t *pt = touch_pdi(pd, PdeIndex(va0));
	va_t va = va0;
	pa_t pa;

	assert(PageOffset(va) == 0);
	assert(PageOffset(size) == 0);

	//createRegionCap(&rootCNode, va, size, can);

	size_t secoff = SectionOffset(va);

	for (; va < va0 + size; va += PageSize, secoff += PageSize)
	{
		if (secoff >= SectionSize) {
			pt = touch_pdi(pd, PdeIndex(va));
			secoff = 0;
		}
		pa = alloc_page();
		pt[PteIndex(va)] = Pte(pa, can & CanWrite ? PtePerm_UserRW : PtePerm_UserRO);
		mmu_invalidatePage(va);
		memset((void*)va, 0, PageSize);
	}
}
