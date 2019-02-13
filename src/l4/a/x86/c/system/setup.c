#include <l4/machine/asm/clsti.h>

#define CALL(func,...) extern void func(); func(__VA_ARGS__);

void _NORETURN arch_startup(void)
{
	*(char*)0xb8001 = *(char*)0xb8003 = 0xb;

	CALL(init_mmu);
	CALL(con_init);
	CALL(test_cpuid);
	CALL(init_gdt);
	CALL(init_idt);
	CALL(init_tss);
	CALL(init_kdrvs);
	CALL(init_kfers);
	CALL(init_usermods);
	CALL(generic_setup);
	CALL(move_to_user);
	UNREACHABLE();
}
