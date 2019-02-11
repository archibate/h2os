#include <l4/boot/mboot.h>

_SECTION(".mboot_header") struct mboot_header multiboot_header =
{
	MAKE_MBOOT_HEADER(MBH_MEMINFO | MBH_PGALIGN),
};

struct mboot_info *mboot;

void save_mboot_pointer(struct mboot_info *mb_ptr, uint mb_magic)
{
	mboot = mb_magic == MB_MAGIC_EAX ? mb_ptr : 0;
}
