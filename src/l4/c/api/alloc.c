#include <l4/api/alloc.h>
#include <l4/generic/thread.h>
#include <l4/generic/idget.h>
#include <l4/generic/rtmethod.h>
#include <l4/generic/slab.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>
#include <l4/misc/panic.h>

sl4id_t sys_new(unsigned int rtype)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;
	BUG_ON(rtype_get_sizeof(rtype) <= current->slabs[rtype]->rsize);

	void *p = slab_new(current->slabs[rtype]);
	if (!p)
		return -ENOMEM;

	rtype_init(p, rtype);

	size_t off = rtype_offsetof_ide(rtype);

	l4id_t id = idg_new_entry(p + off, rtype);
	return id;
}

int sys_delete(unsigned int rtype, l4id_t id)
{
	void *p = id_get_object(rtype, id);
	rtype_delete(p, rtype);

	slab_free(current->slabs[rtype], p);

	idg_free_id(rtype, id);
	return 0;
}

int sys_revoke(unsigned int rtype, l4id_t id)
{
	void *p = id_get_object(rtype, id);
	rtype_revoke(p, rtype);
	return 0;
}

/*每一个线程启动个时候给他在内核空间奉陪几个虚拟页(此所谓slab)
然后他要奉陪的时候就从这里面奉陪*/
