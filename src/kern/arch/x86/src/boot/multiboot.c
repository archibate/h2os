#include <inttypes.h>
#include <multiboot.h>
#include <ccutils.h>

_SECTION(".phys.header") ulong multiboot_header[] =
{
  MAKE_MBOOT_HEADER(MB_MEMINFO | MB_PGALIGN),
};

struct mboot_info *mboot_ptr;

_SECTION(".phys.text") void check_mboot(struct mboot_info *mb_ptr,
                 int mb_magic)
{
  mboot_ptr = mb_magic == MB_MAGIC_EAX ? mb_ptr : 0;
}
