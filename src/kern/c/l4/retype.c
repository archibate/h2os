#include <l4/untyped.h>
#include <l4/ipcbuf.h>
#include <l4/cnode.h>
#include <l4/retype.h>
#include <bittools.h>
#include <memory.h>

size_t doRetype(cap_t *dest, void *ptr, word_t type, bits_t sizeBits)
{
	dest->objType = type;
	switch (type) {
	case L4_UntypedObject:
		{
			Untyped_t *unt = Cap_Untyped(dest);
			unt->base = ptr;
			unt->used = 0;
			unt->size = SizeOfBits(sizeBits);
			return unt->size;
		}
	case L4_IPCBufferObject:
		{
			setCapPtr_PTRED(dest, ptr);
			IPCBuffer_t *ipcbuf = Cap_IPCBuffer(dest);
			memset(ipcbuf, 0, sizeof(IPCBuffer_t));
			return sizeof(IPCBuffer_t);
		}
	case L4_CNodeObject:
		{
			CNode_t *cnode = Cap_CNode(dest);
			cnode->depth = sizeBits;
			cnode->cs = ptr;
			size_t size = SizeOfBits(cnode->depth) * sizeof(cap_t);
			memset(cnode->cs, 0, size);
			return size;
		}
	default:
		return 0;
	}
}
