#include <l4/api/rtalloc.h>
#include <l4/generic/idget.h>
#include <l4/generic/rtmethod.h>
#include <l4/generic/gfopen.h>
#include <l4/generic/kcget.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>
#include <l4/enum/rflags.h>
#include <l4/misc/bug.h>
#include <l4/misc/panic.h>

sl4fd_t sys_rt_open(unsigned int rtype, l4id_t id, unsigned int flags)
{
	//printk("sys_rt_open(%d, %d, %d)", rtype, id, flags);

	if (rtype > RTYPE_MAX)
		return -EINVAL;

	struct ids_entry *ide = id_get_object(rtype, id);
	if (ide && (flags & R_EXCL))
		return -EEXIST;

	size_t off = rtype_offsetof_ide(rtype);
	void *p = NULL;

	if ((flags & R_CREAT) && ide == NULL) {
		p = kcg_new(rtype);
		if (p == NULL)
			return -ENOMEM;

		rtype_init(p, rtype);
		ide = p + off;

		ide->id = id;
		BUG_ON(!idg_set_entry(ide, rtype));

	} else if (ide == NULL) {
		return -ENOENT;

	} else {
		id = ide->id;
		p = (void*)ide - off;
	}

	return gf_open(p, rtype);
}

int sys_rt_close(l4fd_t fd)
{
	return gf_close(fd);
}

#if 0//{{{
int sys_rt_check(unsigned int rtype, l4id_t id)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = id_get_object(rtype, id);
	if (!p) return -ESRCH;

	return 0;
}

sl4id_t sys_rt_new(unsigned int rtype)
{
	if (rtype > RTYPE_MAX)
		return -EINVAL;

	void *p = kcg_new(rtype);
	if (p == NULL)
		return -ENOMEM;

	size_t off = rtype_offsetof_ide(rtype);
	struct ids_entry *ide = p + off;
	l4id_t id = idg_new_entry(ide, rtype);
	return id;
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
#endif

/*（⊙ｖ⊙）嗯..
 * 每一个线程启动个时候给他在内核空间奉陪几个虚拟页(此所谓slab)
 * 然后他要奉陪的时候就从这里面奉陪.?.*///}}}
