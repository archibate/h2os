#include <l4/kobject.h>

static void kobject_revoke(struct kobject *this)
{
	struct kobject *child;
	hlist_for_each_entry2(child, &this->children, hlist) {
		kobject_delete(child);
	}
}

static void kobject_delete(struct kobject *this)
{
	kobject_revoke(this);
	if (this->parent) {
		kobject_put(&this->parent);
		this->parent = NULL;
	}
}
