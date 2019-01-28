#include <l4/untyped.h>
#include <l4/retype.h>
#include <l4/cnode.h>
#include <bittools.h>

size_t Untyped_Retype(Untyped_t *unt,
		CNode_t *root, word_t offset,
		word_t type, size_t sizeMaybe,
		word_t num)
{
	int i;
	void *ptr;
	cap_t *dest;
	size_t cnodeSize = SizeOfBits(root->depth);

	for (i = 0; i < num && unt->used < unt->size &&
			offset + num < cnodeSize; i++)
	{
		ptr = unt->base + unt->used;
		dest = &root->cs[offset + i];
		deleteCap(dest);
		unt->used += doRetype(dest, ptr, type, sizeMaybe);
	}

	return unt->used;
}
