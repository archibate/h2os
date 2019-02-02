#include <asm/clsti.h>
#include <k/printk.h>

#define CALL(func,...) extern void func(); func(__VA_ARGS__);

void kern_start(void)
{
	*(char*)0xb8001 = *(char*)0xb8003 = 0xb;

	CALL(init_mmu);
	CALL(con_init);
	CALL(parse_bootinfo);
	CALL(test_cpuid);
	CALL(init_gdt);
	CALL(init_idt);
	CALL(init_tss);
	CALL(init_kdrvs);
	CALL(init_kfers);
	CALL(setup_user);

	cli();
	hlt();
}
