#include <k/printk.h>
#include <k/kstack.h>
#include <k/movusr.h>
#include <ccutils.h>

#define CALL(func,...) extern void func(); func(__VA_ARGS__);

void _NORETURN kern_start(void)
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
	move_to_user(kIFrame);
}
