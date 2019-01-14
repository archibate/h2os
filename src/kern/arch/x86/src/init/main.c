#include <asm/clsti.h>

void kern_phys_start(void)
{
  *(char*)0xb8001 = *(char*)0xb8003 = 0xf;
  cli();
  hlt();
}
