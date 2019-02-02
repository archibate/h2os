#include <k/printk.h>
#include <k/panic.h>
#include <k/kbase.h>
#include <o/region.h>
#include <o/frames.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <memory.h>
#include <assert.h>
#include <elf32.h>
#include <k/x86/gdt.h>
#include <k/asm/iframe.h>

#ifdef CONFIG_LOG_INITIMAGE
#define tprintk(...) printk(...)
#else
#define tprintk(...) /* Nothing */
#endif


static Frames_t fsz = {
	.fsType = Frames_PhysRegion,
	.fs_pa0 = KernPoolBegin,
	.fs_pa1 = KernPoolEnd,
};


extern void _NORETURN move_to_user(const ulong *uc); // intrents.asm
static void _NORETURN goto_user_entry(void *pc, void *sp)
{
	ulong uc[IFrameWords];
	memset(uc, 0, sizeof(uc));
	uc[IFrame_PC] = (ulong)pc;
	uc[IFrame_SP] = (ulong)sp;
	uc[IFrame_EFLAGS] = 0x202;
	uc[IFrame_CS] = SEG_UCODE;
	uc[IFrame_SS] = SEG_UDATA;
	uc[IFrame_DS] = SEG_UDATA;
	uc[IFrame_ES] = SEG_UDATA;
	uc[IFrame_FS] = SEG_UDATA;
	uc[IFrame_GS] = SEG_UDATA;
	move_to_user(uc);
}

void setup_mycaps(void);
static void *loadelf(const char *data, const char *edata);
void setup_user(void)
{
	void *pc, *sp = 0;
	extern const char initImage[], initImageEnd[];
	if (!(pc = loadelf(initImage, initImageEnd)))
		panic("init image ELF format wrong");
	tprintk("load init image done");

	setup_mycaps();
	goto_user_entry(pc, sp);
}

static void map_zero(va_t va0, size_t size, int rw);
static void loadprog(const char *data, struct Proghdr *ph)
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


// https://github.com/archibate/jos/blob/master/lab1/code/boot/main.c
void *loadelf(const char *data, const char *edata)
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

	tprintk("Entry Point at %#p", e->e_entry);
	return (void*)e->e_entry;
}


static void map_zero(va_t va0, size_t size, int rw)
{
	fsz.fs_needZero = 1;
	fsz.ptePerm = (rw ? PtePerm_UserRW : PtePerm_UserRO);
	Region_t reg = {va0: va0, va1: va0 + size};
	Region_Map(&reg, &fsz, &fsz);
}
