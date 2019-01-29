#include <inttypes.h>
#include <multiboot.h>
#include <ccutils.h>

_SECTION(".mboot_header") ulong multiboot_header[] =
{
  MAKE_MBOOT_HEADER(MB_MEMINFO | MB_PGALIGN),
};

struct mboot_info *mboot_ptr;

void save_mboot_pointer(struct mboot_info *mb_ptr, int mb_magic)
{
  mboot_ptr = mb_magic == MB_MAGIC_EAX ? mb_ptr : 0;
}
