#include <k/printk.h>
#include <l4/errors.h>
#include <l4/invoke.h>
#include <l4/objects.h>
#include <l4/services.h>
#include <l4/capability.h>
#include <ovtools.h>
#include <assert.h>


#include <l4/memory.h>
#include <drv/console.h>
#include <x86/io.h>


int sysInvoke(cap_t *target, Invo_t *invo)
{
	printk("base:limit=%#x:%#x", target->base, target->limit);
	assert(!ovadd(target->base, target->limit));

	if (invo->offset >= target->limit)
		return -L4_ELimit;

/*	if (!(target->permask & (1 << (invo->service % 32))))
		return -L4_EPerm;*/

	switch (invo->service)
	{
	case L4_Write:
	case L4_Read:
		if (ov_or_add(invo->offset, invo->length) >= target->limit)
			return -L4_ELength;
	}

	switch (target->objType)
	{
#if 0
	case L4_UntypedObject:
		{
			switch (invo->service)
			{
			case L4_Retype:
				args = invo->dataSend;
				return 0;
			default:
				return -L4_EService;
			}
		}
#endif
	case L4_ConsoleObject:
		{
			switch (invo->service)
			{
			case L4_Send:
				con_write((void*)invo->dataSend, invo->length);
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_MemoryObject:
		{
			switch (invo->service)
			{
			case L4_Write:
				mwrite((mem_t*)target->object, target->base + invo->offset, invo->dataSend, invo->length);
				return 0;
			case L4_Read:
				mread((mem_t*)target->object, target->base + invo->offset, invo->dataRecv, invo->length);
				return 0;
			default:
				return -L4_EService;
			}
		}
	case L4_IOPortObject:
		{
			switch (invo->service)
			{
			case L4_Send:
				for (int i = 0; i < invo->length; i++)
					outb(target->base + invo->offset, invo->dataSend[i]);
				return 0;
			case L4_Recv:
				for (int i = 0; i < invo->length; i++)
					invo->dataRecv[i] = inb(target->base + invo->offset);
				return 0;
			default:
				return -L4_EService;
			}
		}
	default:
		return -L4_EObject;
	}
}
