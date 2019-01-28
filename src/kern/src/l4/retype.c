#include <l4/untyped.h>
#include <l4/ipcbuf.h>
#include <l4/cnode.h>
#include <l4/retype.h>
#include <bittools.h>
#include <memory.h>

int doRetype(cap_t *dest, void *ptr, word_t type, size_t sizeMaybe)
{
	dest->objType = type;
	switch (type) {
	case L4_UntypedObject:
		{
			Untyped_t *unt = Cap_Untyped(dest);
			unt->base = ptr;
			unt->used = 0;
			unt->size = sizeMaybe;
			return sizeMaybe;
		}
	case L4_IPCBufferObject:
		{
			IPCBuffer_t *ipcbuf = Cap_IPCBuffer(dest);
			memset(ipcbuf, 0, sizeof(IPCBuffer_t));
			return sizeof(IPCBuffer_t);
		}
	case L4_CNodeObject:
		{
			CNode_t *cnode = Cap_CNode(dest);
			cnode->depth = sizeMaybe;
			cnode->cs = ptr;
			memset(cnode->cs, 0, SizeOfBits(cnode->depth));
			return SizeOfBits(cnode->depth);
		}
	default:
		return 0;
	}
}
