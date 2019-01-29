#include <l4/untyped.h>
#include <l4/retype.h>
#include <l4/cnode.h>
#include <bittools.h>

size_t Untyped_Retype(Untyped_t *unt,
		word_t type, bits_t sizeBits,
		word_t num, word_t offset, CNode_t *root)
{
	int i;
	void *ptr;
	cap_t *dest;
	size_t cnodeSize = SizeOfBits(root->depth);

	for (i = 0; i < num && offset + num < cnodeSize; i++)
	{
		ptr = unt->base + unt->used;
		dest = &root->cs[offset + i];
		unt->used += SizeOfBits(sizeBits);
		if (unt->used > unt->size) {
			unt->used = unt->size;
			break;
		}
		deleteCap(dest);
		if (!doRetype(dest, ptr, type, sizeBits))
			break;
	}

	return unt->used;
}
