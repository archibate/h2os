#include <errno.h>
#include <l4/sched.h>
#include <l4/invoke.h>
#include <l4/ipcbuf.h>
#include <l4/thread.h>
#include <l4/untyped.h>
#include <l4/objects.h>
#include <l4/invocations.h>
#include <l4/capability.h>
#include <bittools.h>

int sysInvoke(cap_t *target, word_t *tag)
{
	tcb_t *sender = schedGetCurrent();
	IPCBuffer_t *sendBuf = sender->ipcbuf;

	Range_t range;
	getCapRange(target, &range);

	switch (getCapType(target))
	{
#if 0 // {{{
	case L4_EndpointObject:
		{
			int i;
			endpoint_t *ep = Cap_Endpoint(target);
			IPCBuffer_t *recvBuf = ep->ipcbuf;

			recvBuf->extraCaps = sendBuf->extraCaps;
			for (i = 0; i < sendBuf->extraCaps; i++) {
				cap_t *cap = CNode_lookup(&sender->root, sendBuf->caps[i]);
				if (!cap)
					return -ENOCAP;
				moveCap(recver->root, recvBuf->caps[i], cap);
			}

			memcpy(ep->ep_recvTag, tag, sizeof(ep->ep_recvTag));

			recvBuf->extraWords = sendBuf->extraWords;
			memcpy(recvBuf->words, sendBuf->words, sendBuf->extraWords);

			// ...
			// L4_IPCBuffer_CreateEndpoint...
		}
		return 0;
#endif // }}}

	case L4_UntypedObject:
		{
			Untyped_t *unt = Cap_Untyped(target);
			word_t type     = tag[1] >> 8;
			bits_t sizeBits = tag[1] & 0xff;
			word_t num      = tag[2];
			cptr_t offset   = tag[3];
			cptr_t nodePtr  = sendBuf->regs[L4_IPCReg_Word0];
			cap_t *node;

			switch (tag[0])
			{
			case L4_Untyped_Retype:
				node = CNode_lookup(&sender->root, nodePtr, 32);
				if (!node)
					return -ENOCAP;
				Untyped_Retype(unt, type, sizeBits, num, offset, node);
				return 0;
			default:
				return -ENOSYS;
			}
		}
		return 0;

#if 0
	case L4_IPCBufferObject:
		{
			IPCBuffer_t *ipcbuf = Cap_IPCBuffer(target);

			int index = tag[1] % L4_IPCRegisters;
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
#endif

	case L4_CNodeObject:
		{
			CNode_t *cnode = Cap_CNode(target);
			cap_t *dest, *src;
		       
			dest = CNode_getAt(cnode, tag[1]);
			switch (tag[0])
			{
			case L4_Node_Delete:
				if (!dest)
					return -ENOCAP;
				deleteCap(dest);
				return 0;
			};

			src = CNode_getAt(cnode, tag[2]);
			switch (tag[0])
			{
			case L4_Node_Move:
				if (!dest || !src)
					return -ENOCAP;
				moveCap(dest, src);
				return 0;
			case L4_Node_Swap:
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

int sysSubInvoke(cap_t *root, cptr_t cptr, bits_t depth, word_t *tag)
{
	cap_t *cap;
	switch (getCapType(cap))
	{
	case L4_CNodeObject:
		cap = CNode_lookup(Cap_CNode(root), cptr, depth);
		return sysInvoke(cap, tag);
	default:
		return -EROOTCAP;
	}
}
