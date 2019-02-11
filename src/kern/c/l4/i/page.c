#include <l4/i/page.h>

void page_unmap(page_t *page)
{
	struct kobject *child;
	hlist_for_each_entry2(child, &page->children, hlist) {
		page_unmap(page);
		if (page->parent) {
			kobject_put(&page->parent);
			page->parent = NULL;
		}
	}
}
