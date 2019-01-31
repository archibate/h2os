#include <k/printk.h>
#include <l4/errors.h>
#include <l4/invoke.h>
#include <l4/captypes.h>
#include <l4/services.h>
#include <l4/capability.h>
#include <ovtools.h>
#include <lohitools.h>
#include <assert.h>
#include <l4/a/mkvpage.h>
#include <drv/console.h>
#include <conio.h>
#include <asm/clsti.h>
#include <x86/io.h>
#include <bittools.h>
#include <roundtools.h>
#include <mmu/page.h>
#include <memory.h>

#define assertSegmentValid(seg) assert(!ovadd((seg)->base, (seg)->limit))

int verifySegment(segment_t const *seg, word_t offset, word_t length)
{
	assertSegmentValid(seg);

	if (offset >= seg->limit)
		return -L4_ELimit;

	if (length)
		if (ov_or_add(offset, length-1) >= seg->limit)
			return -L4_ELength;

	return 0;
}

static size_t sysObjSizeOf(byte_t objType);
int sysInvoke(cap_t *target, Invo_t *invo)
{
	//dprintk("sysInvoke: target = %p", target);
	switch (target->c_type)
	{
	case L4_ConsoleCap:
		{
			switch (invo->service)
			{
			case L4_Write:
				con_write(invo->dataSend, invo->length);
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_TestObjCap:
		{
			switch (invo->service)
			{
			case L4_Write:
				cprintf("<TestObj at %#p>: [", target->c_objptr);
				con_write(invo->dataSend, invo->length);
				cputs("]\n");
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_IOPortCap:
		{
			assertSegmentValid(&target->seg);

			switch (invo->service)
			{
			case L4_Write:
			case L4_Read:
				{
					int res = verifySegment(&target->seg, invo->offset, 0);
					if (res)
						return res;
				}
			}
			word_t i;
			switch (invo->service)
			{
			case L4_Write:
				for (i = 0; i < invo->length; i++)
					outb(target->c_base + invo->offset, invo->byteSend[i]);
				return 0;
			case L4_Read:
				for (i = 0; i < invo->length; i++)
					invo->byteRecv[i] = inb(target->c_base + invo->offset);
				return 0;
			default:
				return -L4_EService;
			}
		}
#ifdef CONFIG_DEBUG_OBJECT
	case L4_DebugCap:
		{
			switch (invo->service)
			{
			case L4_Debug_Halt:
				dprintk("System Halted by L4DEBUG");
				clihlt();
				return 0;
			case L4_Write:
				dprintk("(L4DEBUG) %*s", invo->length, invo->dataSend);
				return 0;
			default:
				return -L4_EService;
			}
		}
#endif
#if 0 // {{{
	case L4_PageCap:
		{
			switch (invo->service)
			{
#if 0 //～＼（≧▽≦）／～啦啦啦
			case L4_Retype:
				{
					size_t sysRetypeSizeOf(word_t objType);
					void sysRetypeInit(void *p, word_t objType, size_t objSize);
					word_t num = invo->wordSend[0];
					word_t objType = invo->wordSend[1];
					word_t objSize = invo->wordSend[2];
					if (objType != L4_SegmentCap)
						objSize = sysRetypeSizeOf(objType);
					if (!objSize)
						return num;
					// assert(gcd(objSize, PageSize) == 0);
					while (num-- > 0) {
						word_t objPhys;
						if (objType == L4_SegmentCap) {
							if (target->c_water >= target->c_limit)
								break;
							objPhys = target->c_base + target->c_water;
						} else {
							objPhys = target->c_base + target->c_water;
							objPhys = RoundUp(objSize, objPhys);
							if (objPhys - target->c_base >= target->c_limit)
								break;
						}
						dest->c_physptr = objPhys;
						dest->c_type = objType;
						if (objType != L4_SegmentCap) {
							void *p = phymmap(objPhys, objSize, 1);
							sysRetypeInit(p, objType, objSize);
							phyunmap(p);
						}
						target->c_water += objSize;
					}
					return num;
				}
#endif
			case L4_Retype:
				{
					if (target->c_pgRetype)
						return -L4_ERetype;
					byte_t retype = invo->wordSend[0]; 
					target->c_pgRetype = retype;
					return 0;
				}
			default:
				return -L4_EService;
			};
		}
#endif // }}}
	case L4_SegmentCap:
		{
			switch (invo->service)
			{
			case L4_Segment_Split:
				{
					word_t point = invo->offset;
					if (point <= target->c_water)
						return -L4_EWater;
					// T: verify(dest)
					cap_t *dest = invo->capDest;
					assert(target->c_water <= target->c_limit);
					target->c_limit = point;
					memset(dest, 0, sizeof(cap_t));
					dest->c_base = target->c_base + target->c_limit;
					dest->c_limit = target->c_limit - point;
					return 0;
				}
			case L4_Segment_AllocSlab:
				{
					cap_t *dest = invo->capDest;
					//assert(target != dest);
					word_t num = invo->capCount;
					assert(target->c_water < target->c_limit);
					word_t water = PageUp(target->c_base + target->c_water);
					word_t end = PageDown(target->c_base + target->c_limit);
					if (end < water)
						return num;
					while (num > 0) {
						if (water >= end)
							break;
						memset(dest, 0, sizeof(cap_t));
						dest->c_type = L4_SlabCap;
						dest->c_objptr = Arch_makeVirtPage(water);
						dprintk("L4_Segment_AllocSlab: virtPage = %p", dest->c_objptr);
						assert(dest->c_objptr);
						water += PageSize;
						dest++;
						num--;
					}
					assert(!num);
					target->c_water = water - target->c_base;
					return num;
				}
			default:
				return -L4_EService;
			}
		}
	case L4_SlabCap:
		{
			//assertSegmentValid(&target->seg);

			switch (invo->service)
			{
#if 0 // {{{
			case L4_Retype:
				{
					size_t sysRetypeSizeOf(word_t objType);
					void sysRetypeInit(void *p, word_t objType, size_t objSize);
					word_t num = invo->wordSend[0];
					word_t objType = invo->wordSend[1];
					word_t objSize = invo->wordSend[2];
					if (objType != L4_SegmentCap)
						objSize = sysRetypeSizeOf(objType);
					if (!objSize)
						return num;
					// assert(gcd(objSize, PageSize) == 0);
					while (num-- > 0) {
						word_t objPhys;
						if (objType == L4_SegmentCap) {
							if (target->c_water >= target->c_limit)
								break;
							objPhys = target->c_base + target->c_water;
						} else {
							objPhys = target->c_base + target->c_water;
							objPhys = RoundUp(objSize, objPhys);
							if (objPhys - target->c_base >= target->c_limit)
								break;
						}
						dest->c_physptr = objPhys;
						dest->c_type = objType;
						if (objType != L4_SegmentCap) {
							void *p = phymmap(objPhys, objSize, 1);
							sysRetypeInit(p, objType, objSize);
							phyunmap(p);
						}
						target->c_water += objSize;
					}
					return num;
				}
#endif
#if 0
			case L4_Map:
				{
					cap_t *dest = xxx;
					word_t i;
					for (i = 0; i < target->c_limit; i++) {
						word_t pfn = target->c_base + i;
						dest->c_pgPfn = pfn;
						dest->c_type = L4_PageCap;
						dest->c_pgRetype = 0;
						dest++;
					}
					return 0;
				}
			case L4_Split:
				{
					word_t point = invo->offset;
					if (point <= target->c_water)
						return -L4_EWater;
					// T: verify(dest)
					cap_t *dest = invo->capDest;
					assert(target->c_water <= target->c_limit);
					target->c_limit = point;
					memset(dest, 0, sizeof(cap_t));
					dest->c_base = target->c_base + target->c_limit;
					dest->c_limit = target->c_limit - point;
					return 0;
				}
#endif // }}}
			case L4_Slab_Retype:
				{
					if (target->c_retype)
						return -L4_ERetype;
					byte_t objType = invo->offset;
					size_t objSize = sysObjSizeOf(objType);
					if (!objSize)
						return -L4_EObjType;
					assert(objSize < PageSize);
					target->c_retype = objType;
					return 0;
				}
			case L4_Slab_Allocate:
				{
					// T: verify(dest)
					cap_t *dest = invo->capDest;
					//assert(target != dest);
					word_t num = invo->capCount;
					byte_t objType = target->c_retype;
					if (!objType)
						return -L4_ERetype;
					size_t objSize = sysObjSizeOf(objType);
					if (!objSize)
						return num;
					assert(objSize < PageSize);
					assert(PageOffset((word_t)target->c_objptr) == 0);
					assert(target->c_water % objSize == 0);
					while (num > 0) {
						if (target->c_water >= PageSize)
							break;
						void *objVirtPtr = target->c_objptr + target->c_water;
						target->c_water += objSize;
						memset(dest, 0, sizeof(cap_t));
						dest->c_type = objType;
						dest->c_objptr = objVirtPtr;
						dprintk("L4_Slab_Allocate: objVirtPtr = %p", dest->c_objptr);
						dest++;
						num--;
					}
					assert(!num);
					return num;
				}
			default:
				return -L4_EService;
			};
		}
	default:
		dprintk("L4_ECapType: %d (%#x)", target->c_type, target->c_type);
		return -L4_ECapType;
	}
}

size_t sysObjSizeOf(byte_t objType)
{
	switch (objType)
	{
	case L4_TestObjCap:
		return 16;
	default:
		return 0;
	}
}
