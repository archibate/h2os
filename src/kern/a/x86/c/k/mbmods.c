#include <multiboot.h>
#include <k/panic.h>
#include <k/printk.h>
#include <k/ldmod.h>
#include <k/mboot.h>
#include <string.h>

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
		const void *begin = (void*)m[0].mod_start;
		const void *end = (void*)m[0].mod_end;
		tprintk("#%d  %#p->%#p  %s", i, begin, end, name);
		load_module(begin, end);
	}
}
