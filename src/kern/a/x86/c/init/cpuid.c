#include <x86/cpuid.h>
#include <k/printk.h>
#include <stddef.h>

#ifdef CONFIG_LOG_CPUID
#define tprintk(...) printk(...)
#else
#define tprintk(...) /* Nothing */
#endif

#define _$EXPAND(x) x
#define _CPUID_FEATURE(reg, nr, name) {_$EXPAND(CPUID_##reg), nr, #name}

typedef struct CPUID_Entry
{
	int reg;
	int nr;
	const char *name;
}
CPUID_Entry_t;

static CPUID_Entry_t cpuid1_table[] =
{
	_CPUID_FEATURE(ECX, 0, SSE3),
	_CPUID_FEATURE(ECX, 3, MONITOR),
	_CPUID_FEATURE(ECX, 5, MSR),
	_CPUID_FEATURE(ECX, 9, SSSE3),
	_CPUID_FEATURE(ECX, 12, FMA),
	_CPUID_FEATURE(ECX, 13, CMPXCHG16B),
	_CPUID_FEATURE(ECX, 22, MOVBE),
	_CPUID_FEATURE(ECX, 23, POPCNT),
	_CPUID_FEATURE(ECX, 25, AES),
	_CPUID_FEATURE(ECX, 26, XSAVE),
	_CPUID_FEATURE(ECX, 27, OSXSAVE),
	_CPUID_FEATURE(EDX, 11, SEP),
	_CPUID_FEATURE(EDX, 15, CMOV),
	_CPUID_FEATURE(EDX, 23, MMX),
	_CPUID_FEATURE(EDX, 24, FXSR),
	_CPUID_FEATURE(EDX, 25, SSE),
	_CPUID_FEATURE(EDX, 26, SSE2),
};

void test_cpuid(void)
{
	int i, ena;
	uint32_t r[5];
	char brand[49];

	r[4] = 0;
	cpuid(r, 0, 0);
	tprintk("CPUID: Vendor: %s", r+1);

	brand[48] = 0;
	for (i = 0; i < 3; i++)
		cpuid(brand + i*16, 0x80000002 + i, 0);
	tprintk("CPUID: Brand: %s", r+1);

	tprintk("CPUID: Features:");
	cpuid(r, 1, 0);
	for (i = 0; i < array_sizeof(cpuid1_table); i++)
	{
		CPUID_Entry_t *ent = &cpuid1_table[i];
		ena = r[ent->reg] & (1 << ent->nr);
		if (ena)
			tprintk("[*] %s", ent->name);
		else
			tprintk("[ ] %s", ent->name);
	}
}
