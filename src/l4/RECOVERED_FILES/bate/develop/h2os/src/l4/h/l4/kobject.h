#pragma once

#include <list.h>
#include <atomic.h>
#include <spinlock.h>

struct kobject
{
	struct hlist_node hlist;
	struct hlist_head children;
	struct kobject *parent;
	atomic_t refcount;
	spinlock_t spinlock;
};

static void kobject_init(struct kobject *this)
{
	hlist_node_init(&this->hlist);
	hlist_head_init(&this->children);
	atomic_set(&this->refcount, 1);
	spinlock_init(&this->spinlock);
	this->parent = NULL;
}

static struct kobject *kobject_get(struct kobject *this)
{
	atomic_inc(&this->refcount);
	return this;
}

static void kobject_put(struct kobject *this)
{
	atomic_dec(&this->refcount);
}

static struct kobject *kobject_lock(struct kobject *this)
{
	spin_lock(&this->spinlock);
	return this;
}

static void kobject_unlock(struct kobject *this)
{
	spin_unlock(&this->spinlock);
}

static struct kobject *kobject_set_parent(struct kobject *this, struct kobject *parent)
{
	this->parent = kobject_get(parent);
	return parent;
}

void kobject_revoke(struct kobject *this);
void kobject_delete(struct kobject *this);

#define koget(this) ({ kobject_get(&(this)->ko); (this); })
#define koput(this) ({ kobject_put(&(this)->ko); (void)0; })
#define kospar(this, parent) ({ kobject_set_parent(&(this)->ko, &(parent)->ko); (parent); })
