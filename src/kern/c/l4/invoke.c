#include <k/panic.h>
#include <k/printk.h>
#include <l4/errors.h>
#include <l4/invoke.h>
#include <l4/captypes.h>
#include <l4/eptypes.h>
#include <l4/services.h>
#include <l4/arguments.h>
#include <l4/capability.h>
#include <l4/asm/shortmsg.h>
#include <l4/clookup.h>
#include <k/a/dumpuser.h>
#include <ovtools.h>
#include <lohitools.h>
#include <assert.h>
#include <l4/a/mkvpage.h>
#include <l4/a/pgdir.h>
#include <l4/a/utcb.h>
#include <l4/endpoint.h>
#include <l4/sched.h>
#include <l4/i/tcbcap.h>
#include <l4/i/tcbctx.h>
#include <l4/i/tcbsch.h>
#include <l4/i/segment.h>
#include <l4/i/slab.h>
#include <conio.h>
#include <asm/clsti.h>
#include <x86/io.h>
#include <bittools.h>
#include <roundtools.h>
#include <numtools.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/mmu.h>
#include <mmu/pte.h>
#include <memory.h>
#include <l4/cdep.h>

#define assertSegmentValid(seg) assert(!ovadd((seg)->base, (seg)->limit))

int verifySegment(CSegment_t const *seg, word_t offset, word_t length)
{
	assertSegmentValid(seg);

	if (offset >= seg->limit)
		return -L4_ELimit;

	if (length)
		if (ov_or_add(offset, length-1) >= seg->limit)
			return -L4_ELength;

	return 0;
}

int sysInvoke(cap_t *target, cap_t *capDest, word_t *shortMsg, word_t *extraMsg)
	// T: verify(capDest)
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
#define getcap(nr)  capLookup(getword(nr))

	word_t service = getword(L4_Arg_Service);

	//dprintk("target=%p, ctype=%d, service=%d", target, target->ctype, service);
	//dprintk("shortMsg+1=[%8s]", shortMsg + 1);

	word_t i, length = MIN(getword(L4_RWArg_Length), L4_ShortMsgBytes + L4_MaxExtraBytes);

	switch (target->ctype)
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
			CBuffer_t *buffer = &target->c_buffer;
			switch (service)
			{
			case L4_RewindWriteFrag:
			case L4_RewindReadFrag:
				buffer->water = 0;
			case L4_WriteFrag:
			case L4_ReadFrag:
				if (buffer->water + L4_RWFragSize > buffer->limit)
					return -L4_ELength;
				if (service == L4_ReadFrag || service == L4_RewindReadFrag)
					memcpy(shortMsg + L4_RWFragArg_DataBegin,
							buffer->objptr + buffer->water,
							L4_RWFragSize);
				else
					memcpy(target->c_objptr + buffer->water,
							shortMsg + L4_RWFragArg_DataBegin,
							L4_RWFragSize);
				buffer->water += L4_RWFragSize;
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
			CSegment_t *ioseg = &target->c_segment;
			assertSegmentValid(ioseg);

			switch (service)
			{
			case L4_PWrite:
			case L4_PRead:
				{
					int res = verifySegment(ioseg, length, 0);
					if (res)
						return res;
				}
			}
			switch (service)
			{
			case L4_PWrite:
				for (i = 0; i < length; i++) {
					byte_t data = getbyte(L4_PRWArg_DataBegin, i);
					word_t port = ioseg->base + getword(L4_PRWArg_Offset);
					outb(port, data);
				}
				return 0;
			case L4_PRead:
				for (i = 0; i < length; i++) {
					word_t port = ioseg->base + getword(L4_PRWArg_Offset);
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
						dest->ctype = objType;
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
			CCSpace_t *cspace = &target->c_cspace;
			switch (service)
			{
			case L4_CSpace_SetDestSlot:
				{
					word_t water = getword(L4_CSpace_SetDestSlot_Arg_SlotCPtr);
					if (water >= cspace->limit)
						return -L4_ELimit;
					cspace->water = water;
					return 0;
				}
			default:
				return -L4_EService;
			}
		}
	case L4_EndpointCap:
		{
			CEndpoint_t *cep = &target->c_endpoint;
			endpoint_t *ep = cep->objptr;
			switch (cep->retype)
			{
			case L4_EPUntyped:
				switch (service)
				{
				case L4_Endpoint_Retype:
					cep->retype = getword(L4_Endpoint_Retype_Arg_EPType);
					return 0;
				default:
					return -L4_EService;
				};
			case L4_EPNBSend:
				epCall(ep, schedGetCurr(), 0, 0);
				return 0;
			case L4_EPSend:
				epCall(ep, schedGetCurr(), 1, 0);
				return 0;
			case L4_EPCall:
				epCall(ep, schedGetCurr(), 1, 1);
				return 0;
			case L4_EPWait:
				epWait(ep, schedGetCurr());
				return 0;
			default:
				return -L4_ERetype;
			}
		}
	case L4_TCBCap:
		{
			tcb_t *tcb = target->c_objptr;
			switch (service)
			{
			case L4_TCB_SetCap:
				return do_TCB_SetCap(tcb,
						getword(L4_TCB_SetCap_Arg_CapIdx),
						getcap(L4_TCB_SetCap_Arg_CPtr));
			case L4_TCB_GetCap:
				return do_TCB_GetCap(tcb,
						getword(L4_TCB_GetCap_Arg_CapIdx),
						capDest);
			case L4_TCB_GetExtraBuffer:
				if (tcb->state != TCB_NullState)
					return -L4_EActived;
				memset(capDest, 0, sizeof(cap_t));
				capDest->ctype = L4_BufferCap;
				capDest->c_buffer.objptr = tcb->extraBuf;
				capDest->c_buffer.limit = sizeof(tcb->extraBuf);
				cdepend(capDest, target);
				return 0;
			case L4_TCB_SetPCSP:
				return do_TCB_SetPCSP(tcb,
						getword(L4_TCB_SetPCSP_Arg_PC),
						getword(L4_TCB_SetPCSP_Arg_SP));
			case L4_TCB_SetPriority:
				return do_TCB_SetPriority(tcb,
						getword(L4_TCB_SetPriority_Arg_Priority));
			case L4_TCB_Active:
				return do_TCB_Active(tcb);
			case L4_TCB_Suspend:
				return do_TCB_Suspend(tcb);
			default:
				return -L4_EService;
			}
		}
	case L4_SegmentCap:
		{
			CSegment_t *segment = &target->c_segment;
			assertSegmentValid(segment);
			assert(segment->water <= segment->limit);

			switch (service)
			{
			case L4_Segment_Split:
				return do_Segment_Split(segment, capDest, getword(L4_Segment_Split_Arg_Point));
			case L4_Segment_AllocPage:
				return do_Segment_AllocPage(segment, capDest, getword(L4_Segment_AllocPage_Arg_Count));
			default:
				return -L4_EService;
			}
		}
	case L4_PgdirCap:
		{
			pde_t *pgdir = target->c_objptr;
			assert(PgdirOffset((pa_t)pgdir) == 0);
			switch (service)
			{
			case L4_Pgdir_MapPage:
				{
					word_t va = getword(L4_Pgdir_MapPage_Arg_VAddr);
					cap_t *pgcap = getcap(L4_Pgdir_MapPage_Arg_PageCPtr);
					if (!pgcap)
						return -L4_ECLookup;
					if (pgcap->ctype != L4_PageCap)
						return -L4_ECapType;
					pa_t page = pgcap->c_page.objaddr;
					pde_t pde = pgdir[PdeIndex(va)];
					if (!PdeIsValid(pde))
						return -L4_EPgtab;
					pte_t *pt = (pte_t*)PdePgtabAddr(pde);
					//dprintk("L4_Pgdir_MapPage: pt=%p", pt);
					pte_t pte = pt[PteIndex(va)];
					if (PteIsValid(pte))
						return -L4_EMapped;
					pt[PteIndex(va)] = Pte(page, PtePerm_UserRW);
					//dprintk("!!pte=%p", pt[PteIndex(va)]);
					if (mmu_getPgdirPaddr() == (pa_t)pgdir)
						mmu_invalidatePage(va);//,*((volatile int*)va);
					return 0;
				}
			case L4_Pgdir_MapPgtab:
				{
					word_t va = getword(L4_Pgdir_MapPgtab_Arg_VAddr);
					cap_t *pgcap = getcap(L4_Pgdir_MapPgtab_Arg_PgtabCPtr);
					if (!pgcap)
						return -L4_ECLookup;
					if (pgcap->ctype != L4_PgtabCap)
						return -L4_ECapType;
					pa_t ptaddr = pgcap->c_pgtab.objaddr;
					assert(PgdirOffset((pa_t)pgdir) == 0);
					pde_t pde = pgdir[PdeIndex(va)];
					if (PdeIsValid(pde))
						return -L4_EMapped;
					//dprintk("L4_Pgdir_MapPgtab: pt=%p", ptaddr);
					pgdir[PdeIndex(va)] = PdePgtab(ptaddr);
					return 0;
				}
			case L4_Pgdir_UnmapPage:
				{
					word_t va = getword(L4_Pgdir_UnmapPage_Arg_VAddr);
					pde_t pde = pgdir[PdeIndex(va)];
					if (!PdeIsValid(pde))
						return -L4_EPgtab;
					pte_t *pt = (pte_t*)PdePgtabAddr(pde);
					pte_t pte = pt[PteIndex(va)];
					if (!PteIsValid(pte))
						return -L4_EFault;
					pt[PteIndex(va)] = 0;
					if (mmu_getPgdirPaddr() == (pa_t)pgdir)
						mmu_invalidatePage(va);
					return 0;
				}
			case L4_Pgdir_UnmapPgtab:
				{
					word_t va = getword(L4_Pgdir_UnmapPgtab_Arg_VAddr);
					pde_t pde = pgdir[PdeIndex(va)];
					if (!PdeIsValid(pde))
						return -L4_EFault;
					pgdir[PdeIndex(va)] = 0;
					return 0;
				}
			default:
				return -L4_EService;
			}
		}
	case L4_PageCap:
		{
			switch (service)
			{
#if 0
			case L4_Page_Map:
				{
					word_t va = getword(L4_Page_Map_Arg_VAddr);
					...
					return 0;
				}
#endif
			case L4_Page_Retype:
				{
					byte_t toType = getword(L4_Page_Retype_Arg_ToType);
					byte_t objType = getword(L4_Page_Retype_Arg_ObjType);
					switch (toType) {
					case L4_PageCap:
					case L4_PgtabCap:
					case L4_PgdirCap:
						target->ctype = toType;
						//dprintk("!!pa=%p", target->c_pgdir.objaddr);
						return 0;
					case L4_SlabCap:
						return do_Page_RetypeToSlab(target, toType, objType);
					default:
						return -L4_ERetype;
					}
				}
			default:
				return -L4_EService;
			};
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
						dest->ctype = objType;
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
						dest->ctype = L4_PageCap;
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
			case L4_Slab_Allocate:
				return do_Slab_Allocate(target, capDest, getword(L4_Slab_Allocate_Arg_Count));
			default:
				return -L4_EService;
			};
		}
	default:
		panic("L4_ECapType: %d (%#x)", target->ctype, target->ctype);
		return -L4_ECapType;
	}
}
