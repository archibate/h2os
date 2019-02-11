#include <l4/syscall.h>
#include <l4/kobject.h>
#include <l4/clookup.h>

typedef struct tak_info
{
	byte_t words;
	byte_t shares; // origin, derived
	byte_t grants; // null, origin
	byte_t givens; // derived, origin
	word_t data[L4_ShortMsgWords];
}
tak_info_t;

word_t take_word(tak_info_t *ti, int i)
{
	if (i > ti->words)
		return 0;
	word_t r = ti->data[i];
	return r;
}

cap_t *take_share(tak_info_t *ti, int i)
{
	if (i > ti->maps)
		return 0;
	cptr_t r = ti->data[ti->words + i];
	return capLookup(r);
}

cap_t *take_grant(tak_info_t *ti, int i)
{
	if (i > ti->grants)
		return 0;
	cptr_t r = ti->data[ti->words + ti->shares + i];
	return capLookup(r);
}

cap_t *take_given(tak_info_t *ti, int i)
{
	if (i > ti->grants)
		return 0;
	cptr_t r = ti->data[ti->words + ti->shares + ti->grants + i];
	return capLookup(r);
}

int invocation(int service, tak_info_t *ti)
{
	switch (service)
	{
	case L4_Page_Map:
	};
}

@class Page
{
	struct kobject ko;

	struct hlist_node child_list;
	struct hlist_head children;

	word_t pte;
	Page *parent;
	VSpace *vspace;

	@constructor()
	{
		hlist_head_init(this.children);
		this.parent = 0;
		this.vspace = 0;
	}

	@method Page_Map(in VSpace *vspace, in word_t vaddr)
	{
		vs_getpe(currvs, out pte);
		vs_map(vsid, vaddr, pte);
		child.parent = this;
		child.vspace = kospar(child, vspace);
		child.vaddr = vaddr;
		children += child;
	}

	@method Page_Unmap()
	{
		hlist_for_each_entry(child, children, child_list)
		{
			child.Revoke();
			vs_unmap(child.vspace, child.vaddr);
		}
	}
}

/*
 * prototype page.Page_Map(addr)
 *
 * map into current thread vspace
 */
