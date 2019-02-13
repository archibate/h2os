#include <l4/api/rtalloc.h>
#include <l4/generic/idget.h>
#include <l4/generic/rtmethod.h>
#include <l4/generic/kcget.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>
#include <l4/misc/panic.h>

sl4id_t sys_rt_new(unsigned int rtype)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = kcg_new(rtype);
	if (!p)
		return -ENOMEM;

	size_t off = rtype_offsetof_ide(rtype);
	l4id_t id = idg_new_entry(p + off, rtype);
	return id;
}

int sys_rt_check(unsigned int rtype, l4id_t id)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = id_get_object(rtype, id);
	if (!p) return -ESRCH;

	return 0;
}

int sys_rt_delete(unsigned int rtype, l4id_t id)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = id_get_object(rtype, id);
	if (!p) return -ESRCH;

	kcg_delete(rtype, p);

	idg_free_id(rtype, id);
	return 0;
}

int sys_rt_revoke(unsigned int rtype, l4id_t id)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = id_get_object(rtype, id);
	if (!p) return -ESRCH;

	rtype_revoke(p, rtype);

	return 0;
}

/*（⊙ｖ⊙）嗯..
 * 每一个线程启动个时候给他在内核空间奉陪几个虚拟页(此所谓slab)
 * 然后他要奉陪的时候就从这里面奉陪.?.*/
