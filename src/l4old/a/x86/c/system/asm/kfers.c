#include <l4/machine/asm/msr.h>
#include <l4/system/asm/gdt.h>
#include <l4/system/kstack.h>

void init_kfers(void)
{
	extern void sysenter_entry(void);
	wrmsr(IA32_SYSENTER_CS,  SEG_KCODE);
	wrmsr(IA32_SYSENTER_EIP, (ulong)&sysenter_entry);
	wrmsr(IA32_SYSENTER_ESP, KernSEStackTop);
}
