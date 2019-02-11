#include <l4/i/slab.h>
#include <l4/captypes.h>
#include <l4/errors.h>
#include <mmu/page.h>
#include <l4/thread.h>
#include <l4/a/mkvpage.h>
#include <l4/cdep.h>
#include <memory.h>
#include <assert.h>

static size_t sysObjSizeOf(byte_t objType)
{
	switch (objType)
	{
	case L4_TCBCap:
		return sizeof(tcb_t);
	/*case L4_PageCap:
		return PageSize;
	case L4_PgtabCap:
		return PgtabSize;
	case L4_PgdirCap:
		return PgdirSize;*/
	default:
		return 0;
	}
}

int do_Page_RetypeToSlab(cap_t *target, byte_t toType, byte_t objType)
{
	size_t objSize = sysObjSizeOf(objType);
	if (!objSize)
		return -L4_ERetype;
	assert(objSize <= PageSize);
	target->ctype = L4_SlabCap;
	target->c_slab.retype = objType;
	word_t pa = target->c_page.objaddr;
	void *vip = Arch_makeVirtPage(pa);
	target->c_slab.objptr = vip;
	return 0;
}

int do_Slab_Allocate(CSlab_t *slab, cap_t *capDest, word_t num)
	// TODO: make me clean!!
{
	/*if (!slab->retype)
	  return -L4_ERetype;*/
	// T: verify(dest)
	cap_t *dest = capDest;
	//assert(slab != dest);
	byte_t objType = slab->retype;
	size_t objSize = sysObjSizeOf(objType);
	if (!objSize)
		return num;//
	assert(objSize <= PageSize);
	assert(PageOffset((word_t)slab->objptr) == 0);
	assert(slab->water % objSize == 0);
	while (num > 0) {
		if (slab->water >= PageSize)
			break;
		void *objVirtPtr = slab->objptr + slab->water;
		slab->water += objSize;
		memset(objVirtPtr, 0, objSize);
		//dprintk("L4_Slab_Allocate: objVirtPtr = %p", objVirtPtr);
		memset(dest, 0, sizeof(cap_t));
		dest->c_objptr = objVirtPtr;
		dest->ctype = objType;
		cdepend(&dest->c_ko, &slab->ko);//??
		dest++;
		num--;
	}
	assert(!num);
	return num;
}
