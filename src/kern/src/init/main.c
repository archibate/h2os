#include <asm/clsti.h>

_SECTION(".phys.text") void kern_phys_start(void)
{
  cli();
  hlt();
}
