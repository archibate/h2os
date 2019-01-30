#include <k/printk.h>
#include <l4/errors.h>
#include <l4/invoke.h>
#include <l4/captypes.h>
#include <l4/services.h>
#include <l4/capability.h>
#include <ovtools.h>
#include <lohitools.h>
#include <assert.h>
#include <l4/a/fpgmapwin.h>
#include <l4/memory.h>
#include <drv/console.h>
#include <asm/clsti.h>
#include <x86/io.h>
#include <bittools.h>
#include <memory.h>


int verifySegment(segment_t const *seg, word_t offset, word_t length)
{
	assert(!ovadd(seg->base, seg->limit));

	if (offset >= seg->limit)
		return -L4_ELimit;

	if (length)
		if (ov_or_add(offset, length-1) >= seg->limit)
			return -L4_ELength;

	return 0;
}


int sysInvoke(cap_t *target, Invo_t *invo)
{
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
#if 0
	case L4_SegmentCap:
		{
			int res = verifySegment(&target->seg, invo->offset, invo->length);
			if (res < 0)
				return res;
			mem_t *mem = (mem_t*)target->ptr;
			switch (invo->service)
			{
			case L4_Write:
				return mwrite(mem, target->c_base + invo->offset, invo->dataSend, invo->length);
			case L4_Read:
				return mread(mem, target->c_base + invo->offset, invo->dataRecv, invo->length);
			default:
				return -L4_EService;
			}
		}
#endif
	case L4_IOPortCap:
		{
			int res = verifySegment(&target->seg, invo->offset, 0);
			if (res < 0)
				return res;
			int i;
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
			case L4_Debug_Puts:
				dprintk("(L4DEBUG) %s", invo->dataSend);
				return 0;
			default:
				return -L4_EService;
			}
		}
#endif
	case L4_FPageCap:
		{
			switch (invo->service)
			{
			case L4_Untyped_Retype:
				{
					size_t sysObjSizeBitsOf(word_t objType);
					void sysRetype(cap_t *dest, void *p, word_t objType, size_t objSize);
					word_t num = invo->wordSend[0];
					word_t objType = invo->wordSend[1];
					bits_t objSizeBits = invo->wordSend[2];
					objSizeBits = sysObjSizeBitsOf(objType, objSizeBits);
					if (!objSizeBits)
						return num;
					while (num-- > 0) {
						if (target->c_labUsed >= SizeOfBits(target->c_sizeBits))
							break;
						void *p = fpgmapwin(&target->fpage, target->c_labUsed, 1);
						cap_t *dest = xxxx;
						sysRetype(dest, p, objType, objSize);
						target->c_labUsed += objSize;
					}
					return num;
				}
			default:
				return -L4_EService;
			};
		}
	default:
		return -L4_ECap;
	}
}

bits_t sysObjSizeBitsOf(word_t objType, word_t objSizeBits)
{
	switch (objType)
	{
	case L4_UntypedCap:
		return objSizeBits;
	default:
		return 0;
	}
}

void sysRetype(cap_t *dest, void *p, word_t objType, size_t objSize)
{
	dest->c_ptr = p;
	dest->c_type = objType;
	if (objType != L4_UntypedCap)
		memset(p, 0, objSize);
}
