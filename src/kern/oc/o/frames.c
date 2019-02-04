#include <o/frames.h>
#include <k/panic.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <memory.h>

pte_t _Frames_GetFramePte(Frames_t *fs)
{
	pa_t pa;
	switch (fs->fsType)
	{
	case Frames_PhysRegion:
		pa = fs->fs_pa0;
		fs->fs_pa0 += PageSize;
		if (fs->fs_needZero)
			memset((void*)pa, 0, PageSize);//P2V?
		return Pte(pa, fs->ptePerm);
	default:
		panic("Invalid Frames type: %d", fs->fsType);
	}
}
