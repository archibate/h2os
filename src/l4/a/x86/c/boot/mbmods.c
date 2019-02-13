#include <l4/boot/mboot.h>
#include <l4/boot/ldmod.h>
#include <l4/misc/panic.h>
#include <l4/misc/printk.h>
#ifdef CONFIG_DUMP_MODULES
#define tprintk(...) printk(__VA_ARGS__)
#else
#define tprintk(...) /* Nothing */
#endif

void init_usermods(void)
{
	if (!mboot)
		panic("Not loaded by a multiboot loader ):");
	if (!(mboot->flags & MBI_MODULES) || !mboot->mods_count)
		panic("No multiboot modules loaded ):");

	struct mboot_module *m = (void *)(mboot->mods_addr);

	int i;
	tprintk("#n  StartAddr   EndAddr     Name");
	for (i = 0; i < mboot->mods_count; i++) {
		const char *name = (void*)m[i].mod_string;
		const void *begin = (void*)m[i].mod_start;
		const void *end = (void*)m[i].mod_end;
		tprintk("#%d  %#p->%#p  %s", i, begin, end, name);
		load_module(begin, end);
	}
}
