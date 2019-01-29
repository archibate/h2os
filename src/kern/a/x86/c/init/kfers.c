#include <x86/msr.h>
#include <k/x86/gdt.h>
#include <k/kstack.h>

void init_kfers(void)
{
	extern void sysenter_entry(void);
	wrmsr(IA32_SYSENTER_CS,  SEG_KCODE);
	wrmsr(IA32_SYSENTER_EIP, (ulong)&sysenter_entry);
	wrmsr(IA32_SYSENTER_ESP, KernSEStackTop);
}
