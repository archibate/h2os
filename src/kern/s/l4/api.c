#include <l4/types.h>
#include <l4/invocations.h>

#ifdef _KERNEL
typedef cap_t *api_cptr_t;
#else
typedef cptr_t api_cptr_t;
#endif


void l4IPCBuffer_setRegister(api_cptr_t ipcbuf, word_t regnr, word_t value)
{
	l4Invoke(ipcbuf, L4_IPCBuffer_setRegister, 2,
			regnr, value);
}

word_t l4IPCBuffer_getRegister(api_cptr_t ipcbuf, word_t regnr)
{
	return l4Invoke(ipcbuf, L4_IPCBuffer_setRegister, 1,
			regnr);
}

word_t l4CNode_Delete(api_cptr_t cnode, cptr_t dest)
{
	return l4Invoke(cnode, L4_CNode_Delete, 1,
			dest);
}

word_t l4CNode_Move(api_cptr_t cnode, cptr_t dest, cptr_t src)
{
	return l4Invoke(cnode, L4_CNode_Move, 2,
			dest, src);
}

word_t l4CNode_Swap(api_cptr_t cnode, cptr_t dest, cptr_t src)
{
	return l4Invoke(cnode, L4_CNode_Move, 2,
			dest, src);
}

word_t l4IPCBuffer_setDestCNode(api_cptr_t ipcbuf, cptr_t cnode)
{
	l4IPCBuffer_setRegister(ipcbuf, L4_IPCReg_Word0, cnode);
}

word_t l4Untyped_Retype(api_cptr_t untyped,
		word_t type, bits_t sizeBits,
		word_t num, word_t offset)
{
	return l4Invoke(untyped, L4_Untyped_Retype, 3,
			(type << 8) + sizeBits, num, offset);
}
