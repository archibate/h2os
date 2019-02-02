#include <k/panic.h>
#include <k/printk.h>
#include <l4/errors.h>
#include <l4/invoke.h>
#include <l4/captypes.h>
#include <l4/services.h>
#include <l4/arguments.h>
#include <l4/capability.h>
#include <l4/asm/shortmsg.h>
#include <ovtools.h>
#include <lohitools.h>
#include <assert.h>
#include <l4/a/mkvpage.h>
#include <l4/thread.h>
#include <conio.h>
#include <asm/clsti.h>
#include <x86/io.h>
#include <bittools.h>
#include <roundtools.h>
#include <numtools.h>
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

#define prefombr(obj, p) p
#define pnewslab(p) p
#define pslabnew(p) p

static size_t sysObjSizeOf(byte_t objType);
int sysInvoke(cap_t *target, cap_t *capDest, word_t *shortMsg, word_t *extraMsg)
{
#define L4_RWFragSize ((L4_ShortMsgWords - L4_RWFragArg_DataBegin) * sizeof(word_t))
#define L4_ShortMsgBytes (L4_ShortMsgWords * sizeof(word_t))
#define L4_MaxExtraBytes (L4_MaxExtraWords * sizeof(word_t))
#define getword(nr) (*(((nr) < L4_ShortMsgWords) ? \
			&(((word_t*)shortMsg)[(nr)]) : \
			&(((word_t*)extraMsg)[(nr) - L4_ShortMsgWords])))
#define getbyte8(i) (*(((i) < L4_ShortMsgBytes) ? \
			&(((byte_t*)shortMsg)[(i)]) : \
			&(((byte_t*)extraMsg)[(i) - L4_ShortMsgBytes])))
#define getbyte(nr, i) getbyte8((nr) * sizeof(word_t) + (i))

	word_t service = getword(L4_Arg_Service);

	//dprintk("target=%p, ctype=%d, service=%d", target, target->c_type, service);
	//dprintk("shortMsg+1=[%8s]", shortMsg + 1);

	word_t i, length = MIN(getword(L4_RWArg_Length), L4_ShortMsgBytes + L4_MaxExtraBytes);

	switch (target->c_type)
	{
	case L4_ConsoleCap:
		{
			switch (service)
			{
			case L4_Write:
				for (i = 0; i < length; i++) {
					char ch = getbyte(L4_RWArg_DataBegin, i);
					cputchar(ch);
				}
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_BufferCap:
		{
			switch (service)
			{
			case L4_RewindWriteFrag:
			case L4_RewindReadFrag:
				target->c_water = 0;
			case L4_WriteFrag:
			case L4_ReadFrag:
				if (target->c_water + L4_RWFragSize > target->c_limit)
					return -L4_ELength;
				if (service == L4_ReadFrag || service == L4_RewindReadFrag)
					memcpy(shortMsg + L4_RWFragArg_DataBegin,
							target->c_objptr + target->c_water,
							L4_RWFragSize);
				else
					memcpy(target->c_objptr + target->c_water,
							shortMsg + L4_RWFragArg_DataBegin,
							L4_RWFragSize);
				target->c_water += L4_RWFragSize;
				return 0;
			default:
				return -L4_EService;
			}
		}
#if 0 // {{{
	case L4_TestObjCap:
		{
			switch (service)
			{
			case L4_Write:
				cprintf("<TestObj at %#p>: [", target->c_objptr);
				for (i = 0; i < getword(L4_RWArg_Length); i++) {
					char ch = getbyte(L4_RWArg_DataBegin, i);
					cputchar(ch);
				}
				cputs("]\n");
				return 0;
			default:
				return -L4_EService;
			}
		}
#endif // }}}
	case L4_IOPortCap:
		{
			assertSegmentValid(&target->seg);

			switch (service)
			{
			case L4_PWrite:
			case L4_PRead:
				{
					int res = verifySegment(&target->seg, length, 0);
					if (res)
						return res;
				}
			}
			switch (service)
			{
			case L4_PWrite:
				for (i = 0; i < length; i++) {
					byte_t data = getbyte(L4_PRWArg_DataBegin, i);
					word_t port = target->c_base + getword(L4_PRWArg_Offset);
					outb(port, data);
				}
				return 0;
			case L4_PRead:
				for (i = 0; i < length; i++) {
					word_t port = target->c_base + getword(L4_PRWArg_Offset);
					byte_t data = inb(port);
					getbyte(L4_PRWArg_DataBegin, i) = data;
				}
				return 0;
			default:
				return -L4_EService;
			}
		}
#ifdef CONFIG_DEBUG_OBJECT
	case L4_DebugCap:
		{
			switch (service)
			{
			case L4_Debug_Halt:
				printk("System Halted by L4DEBUG");
				clihlt();
				return 0;
			case L4_Write:
				cputs("(L4DEBUG) ");
				for (i = 0; i < length; i++) {
					char ch = getbyte(L4_RWArg_DataBegin, i);
					cputchar(ch);
				}
				cputs("\n");
				return 0;
			case L4_Read:
				for (i = 0; i < length; i++) {
					getbyte(L4_ReadRet_DataBegin, i) = 'a' + i % 26;
				}
				cputs("\n");
				return 0;
			default:
				return -L4_EService;
			}
		}
#endif
#if 0 // {{{
	case L4_PageCap:
		{
			switch (service)
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
	case L4_CSpaceCap:
		{
			switch (service)
			{
			case L4_CSpace_SetDestSlot:
				target->c_water = getword(L4_CSpace_SetDestSlot_Arg_SlotCPtr);
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_TCBCap:
		{
			tcb_t *tcb = target->c_objptr;
			switch (service)
			{
			case L4_TCB_GetExtraBuffer:
				memset(capDest, 0, sizeof(cap_t));
				capDest->c_type = L4_BufferCap;
				capDest->c_objptr = prefombr(tcb, &tcb->extraBuf);
				capDest->c_limit = sizeof(tcb->extraBuf);
				return 0;
			case L4_TCB_SetContext:
				for (i = 0; i < L4_ContextWords; i++) {
					word_t value = getword(L4_TCB_SetContext_Arg_ContextBegin + i);
					tcb->context[i] = value;
				}
				return 0;
			case L4_TCB_SetPriority:
				tcb->priority = getword(L4_TCB_SetPriority_Arg_Priority);
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_SegmentCap:
		{
			assertSegmentValid(&target->seg);
			assert(target->c_water <= target->c_limit);

			switch (service)
			{
			case L4_Segment_Split:
				{
					word_t point = getword(L4_Segment_Split_Arg_Point);
					if (point <= target->c_water)
						return -L4_EWater;
					// T: verify(dest)
					cap_t *dest = capDest;
					target->c_limit = point;
					memset(dest, 0, sizeof(cap_t));
					dest->c_base = target->c_base + target->c_limit;
					dest->c_limit = target->c_limit - point;
					return 0;
				}
			case L4_Segment_AllocSlab:
				{
					cap_t *dest = capDest;
					//assert(target != dest);
					word_t num = getword(L4_Segment_AllocSlab_Arg_Count);
					word_t water = PageUp(target->c_base + target->c_water);
					word_t end = PageDown(target->c_base + target->c_limit);
					if (end < water)
						return num;
					while (num > 0) {
						if (water >= end)
							break;
						void *virtPage = Arch_makeVirtPage(water);
						water += PageSize;
						virtPage = pnewslab(virtPage);
						//dprintk("L4_Segment_AllocSlab: virtPage = %p", virtPage);
						assert(virtPage);
						memset(dest, 0, sizeof(cap_t));
						dest->c_type = L4_SlabCap;
						dest->c_objptr = virtPage;
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
			switch (service)
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
					byte_t objType = getword(L4_Slab_Retype_Arg_ObjType);
					size_t objSize = sysObjSizeOf(objType);
					if (!objSize)
						return -L4_EObjType;
					assert(objSize < PageSize);
					target->c_retype = objType;
					return 0;
				}
			case L4_Slab_Allocate:
				{
					if (!target->c_retype)
						return -L4_ERetype;
					// T: verify(dest)
					cap_t *dest = capDest;
					//assert(target != dest);
					word_t num = getword(L4_Slab_Allocate_Arg_Count);
					byte_t objType = target->c_retype;
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
						objVirtPtr = pslabnew(objVirtPtr);
						memset(objVirtPtr, 0, objSize);
						//dprintk("L4_Slab_Allocate: objVirtPtr = %p", objVirtPtr);
						memset(dest, 0, sizeof(cap_t));
						dest->c_objptr = objVirtPtr;
						dest->c_type = objType;
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
		panic("L4_ECapType: %d (%#x)", target->c_type, target->c_type);
		return -L4_ECapType;
	}
}

size_t sysObjSizeOf(byte_t objType)
{
	switch (objType)
	{
	case L4_TCBCap:
		return sizeof(tcb_t);
	default:
		return 0;
	}
}
