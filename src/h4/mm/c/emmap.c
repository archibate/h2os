#include <h4/mm.h>
#include "emmap.h"
#include <l4/types.h>
#include <l4/api/mmap.h>
#include <l4/enum/mmprot.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pagefault.h>

int emmap(int mmc, int fd, uintptr_t addr, size_t size, int prot)
{
	int ret = sys_mmap(mmc, fd, (void*)addr, size);
	if (ret < 0)
		return ret;
	int pfc = PageFaultErrorCode_User;
	if (prot & PROT_EXEC)
		pfc |= PageFaultErrorCode_Instr;
	if (prot & PROT_WRITE)
		pfc |= PageFaultErrorCode_Write;
	uintptr_t a;
	for (a = addr; a < addr + size; a += PageSize)
		sys_test_fault(mmc, (void*)a, pfc);
	return 0;
}
