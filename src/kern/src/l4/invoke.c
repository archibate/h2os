#include <errno.h>
#include <l4/types.h>
#include <l4/ipcbuf.h>
#include <l4/thread.h>
#include <l4/untyped.h>
#include <l4/objects.h>
#include <l4/invocations.h>
#include <l4/capability.h>

CNode_t *lookupCNode(CNode_t *root, cptr_t nodePtr)
{
	cap_t *nodeCap = cget(root, nodePtr);
	if (!nodeCap)
		return 0;

	switch (getCapType(nodeCap))
	{
	case L4_CNodeObject:
		return Cap_CNode(nodeCap);
	default:
		return 0;
	}
}

int sysInvoke(cap_t *target, tcb_t *sender, word_t *tag)
{
	IPCBuffer_t *sendBuf = sender->ipcbuf;

	switch (getCapType(target))
	{
#if 0 // {{{
	case L4_EndpointObject:
		{
			int i;
			endpoint_t *ep = Cap_Endpoint(target);
			IPCBuffer_t *recvBuf = ep->ipcbuf;

			memcpy(ep->ep_recvTag, tag, sizeof(ep->ep_recvTag));

			recvBuf->extraWords = sendBuf->extraWords;
			memcpy(recvBuf->words, sendBuf->words, sendBuf->extraWords);

			recvBuf->extraCaps = sendBuf->extraCaps;
			for (i = 0; i < sendBuf->extraCaps; i++) {
				cap_t *cap = lookupCap(sender->root, sendBuf->caps[i]);
				moveCap(recver->root, recvBuf->caps[i], cap);
			}
			// ...
			// L4_IPCBuffer_CreateEndpoint...
		}
		return 0;
#endif // }}}

	case L4_UntypedObject:
		{
			Untyped_t *unt = Cap_Untyped(target);

			word_t type      = tag[1] % L4_ObjectTypesNR;
			size_t sizeMaybe = tag[2];
			word_t num       = tag[3];
			cptr_t offset    = tag[4];
			cptr_t nodePtr   = tag[5]; // T: from sendBuf!
			CNode_t *node;

			switch (tag[0])
			{
			case L4_Untyped_Retype:
				node = lookupCNode(sender->root, nodePtr);
				if (!node)
					return -ENOCAP;
				Untyped_Retype(unt, node, offset, type, sizeMaybe, num);
				return 0;
			default:
				return -ENOSYS;
			}
		}
		return 0;

	case L4_IPCBufferObject:
		{
			IPCBuffer_t *ipcbuf = Cap_IPCBuffer(target);

			int index = tag[1] % IPCRegisters;
			switch (tag[0])
			{
			case L4_IPCBuffer_setRegister:
				ipcbuf->regs[index] = tag[2];
				return 0;
			case L4_IPCBuffer_getRegister:
				tag[0] = ipcbuf->regs[index];
				return 0;
			default:
				return -ENOSYS;
			}
		}
		return 0;

	case L4_CNodeObject:
		{
			CNode_t *cnode = Cap_CNode(target);
			cap_t *dest, *src;
		       
			dest = cget(cnode, tag[1]);
			switch (tag[0])
			{
			case L4_CNode_Delete:
				if (!dest)
					return -ENOCAP;
				deleteCap(dest);
				return 0;
			};

			src = cget(cnode, tag[2]);
			switch (tag[0])
			{
			case L4_CNode_Move:
				if (!dest || !src)
					return -ENOCAP;
				moveCap(dest, src);
				return 0;
			case L4_CNode_Swap:
				if (!dest || !src)
					return -ENOCAP;
				swapCap(dest, src);
				return 0;
			default:
				return -ENOSYS;
			}
		}
		return 0;

	default:
		return -ENOSYS;
	}
}
