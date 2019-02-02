#include <multiboot.h>
#include <k/panic.h>
#include <k/printk.h>
#include <ccutils.h>
#include <string.h>

_SECTION(".mboot_header") struct mboot_header multiboot_header =
{
	MAKE_MBOOT_HEADER(MBH_MEMINFO | MBH_PGALIGN),
};

struct mboot_info *mboot;

void save_mboot_pointer(struct mboot_info *mb_ptr, int mb_magic)
{
	mboot = mb_magic == MB_MAGIC_EAX ? mb_ptr : 0;
}

ulong biInitimg, biInitimgEnd;

void parse_bootinfo(void)
{
	if (!mboot)
		panic("Not loaded by a multiboot loader ):");
	if (!(mboot->flags & MBI_MODULES) || !mboot->mods_count)
		panic("No multiboot modules loaded ):");

	struct mboot_module *m = (void *)(mboot->mods_addr);

	biInitimg = m[0].mod_start;
	biInitimgEnd = m[0].mod_end;

#ifdef CONFIG_DUMP_MODULES
	int i;
	for (i = 0; i < mboot->mods_count; i++) {
		printk("M#%d: %#p-%#p %s", i, m[i].mod_start, m[i].mod_end, m[i].mod_string);
	}
#endif
}
