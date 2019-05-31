// vim: ts=4 sts=4 sw=4 fdm=marker
// reference: http://wiki.0xffffff.org/posts/hurlex-11.html
#include <malloc.h> // impelementation
#include <c4/liballoc.h> // impelementation
#include <memory.h> // memcpy in realloc
#include <l4/machine/mmu/page.h> // PageSize
#define PGSIZE PageSize // machi
#define PGMASK (-PGSIZE)
#include <roundtools.h> // RoundUp in amalloc
#include <inttypes.h> // size_t, uintptr_t
#include <h4/mm.h> // brk
typedef uintptr_t addr_t;

#include <assert.h> // assert
#include <bug.h> // BUG_ON

#undef free // TODO: bug on free

typedef struct HNODE {
	struct HNODE *next, *prev;
	size_t allocated;
} HNODE;

static HNODE *heap_head;

static void *currbrk, *maxbrk;

static void *hard_sbrk(ptrdiff_t incptr)
{
	//printk("hard_sbrk(%#x): %p->%p", incptr, currbrk, currbrk + incptr);
	void *oldbrk = currbrk;
	void *noldbrk = sbrk(incptr);
	//printk("%p:%p", noldbrk, oldbrk);
	BUG_ON(noldbrk != oldbrk);
	currbrk += incptr;
	return oldbrk;
}

static void *first_hard_sbrk(ptrdiff_t incptr);
static void *(*my_sbrk)(ptrdiff_t incptr) = first_hard_sbrk;

static void *first_hard_sbrk(ptrdiff_t incptr)
{
	//printk("first_hard_sbrk(%#x)", incptr);
	void *oldbrk = sbrk(incptr);
	BUG_ON(oldbrk == (void*)-1);
	currbrk = oldbrk + incptr;
	//printk("first_hard_sbrk: %p", currbrk);
	my_sbrk = hard_sbrk;
	return oldbrk;
}

static void *soft_sbrk(ptrdiff_t incptr)
{
	//printk("ss: [%p->%p]%p", currbrk, currbrk + incptr, maxbrk);
	void *oldbrk = currbrk;
	currbrk += incptr;
	BUG_ON(currbrk > maxbrk);
	return oldbrk;
}

static void set_break(void *p)
{
	p = p + PGSIZE;
	if (currbrk != p) {
		my_sbrk(p - currbrk);
	}
}

void liballoc_init(void) // not called in h4 init modules
{
	//printk("liballoc_init");
	heap_head = (HNODE*)my_sbrk(PGSIZE);
	heap_head->allocated = 0;
	heap_head->next = 0;
	heap_head->prev = heap_head;
}

void liballoc_set_memory(void *begin, size_t size) // used in h4 init modules
{
	//printk("liballoc_set_memory(size=%d)", size);
	my_sbrk = soft_sbrk;
	currbrk = begin;
	maxbrk = begin + size;
	currbrk = (void*)RoundUp(PGSIZE, (uintptr_t)currbrk);//
	maxbrk = (void*)RoundDown(PGSIZE, (uintptr_t)maxbrk);//
	liballoc_init();
}

#ifdef dbg_printf // {{{
void print_heap_status(void)
{
	dbg_printf("heap status %p:\n", heap_head);
	for (HNODE *curr = heap_head; curr; curr = curr->next) {
		size_t len = curr->next ?
			(void*)curr->next - (void*)curr : 0;
		dbg_printf("%c:%p:%d\n", curr->allocated ? 'M' : '-', curr, len);
	}
}
#else
#define print_heap_status()
#endif // }}}

static HNODE *get_aligned(HNODE *curr, size_t align)
{
	HNODE *icurr = (HNODE*)RoundUp(align, (uintptr_t)(curr + 1)) - 1;
	//printk("curr:%p->%p for align=%d", curr, icurr, align);//
	return icurr;
}

static void cope2_new_node(HNODE **pprev, HNODE *icurr, HNODE *curr)
{
	if (icurr != curr) {
		memrcpy(icurr, curr, sizeof(HNODE));
		*pprev = icurr;
	}
}

void *amalloc(size_t len, size_t align)
{
	len = (len + sizeof(int) - 1) & (~0UL ^ (sizeof(int) - 1));
	len += sizeof(HNODE);

	if (align < sizeof(int))
		align = sizeof(int);

	//printk("amalloc: len=%d, align=%d", len, align);

	HNODE *curr, *icurr, **pprev = &heap_head;
	for (curr = heap_head; curr->next; curr = curr->next)
	{
		if (!curr->allocated) {
			icurr = get_aligned(curr, align);
			ssize_t rest_len = (void*)curr->next - (void*)icurr - len;
			if (rest_len > 0) {
				cope2_new_node(pprev, icurr, curr);
				curr = icurr;
				curr->allocated = len;
				if (rest_len > sizeof(HNODE)) {
					HNODE *rest = (HNODE*)((void*)curr + len);
					rest->allocated = 0;
					rest->next = curr->next;
					curr->next->prev = rest;
					curr->next = rest;
					rest->prev = curr;
				}
				return curr + 1;
			}
		}
		pprev = &curr->next;
	}

	icurr = get_aligned(curr, align);
	cope2_new_node(pprev, icurr, curr);
	curr = icurr;

	HNODE *next = (HNODE*)((void*)curr + len);
	//printk("curr:%p, next:%p", curr, next);
	set_break(next + 1);
	curr->next = next;
	next->prev = curr;
	next->next = 0;
	next->allocated = 0;
	curr->allocated = len;
	return curr + 1;
}

void free(void *p)
{
	assert(p);
	HNODE *curr = (HNODE*)p - 1;
	assert(curr->allocated);

	if (curr->next && !curr->next->allocated) {
		HNODE *nnext = curr->next->next;
		if (nnext)
			nnext->prev = curr;
		curr->next = nnext;
		//list_remove_nextof_s3(curr);
	}

	if (curr->prev && !curr->prev->allocated) {
		curr->prev->next = curr->next;
		if (curr->next)
			curr->next->prev = curr->prev;
		//list_remove_s2(curr);
	}

	/*if (!curr->next) {// && curr->prev && curr->prev->allocated) { // {{{
	  curr->prev->next = 0;
	  FIXME!
	  setbrk(curr->prev + 1);
	  }*/ // }}}

	curr->allocated = 0;
}

void *malloc(size_t len)
{
	return amalloc(len, 0);
}

void *realloc(void *p, size_t size)
{
	assert(p);
	HNODE *curr = (HNODE*)p - 1;
	size_t old_size = curr->allocated;
	void *new_p = malloc(size);
	memcpy(new_p, p, old_size);
	free(p);
	return new_p;
}

void *zalloc(size_t size)
{
	void *p = malloc(size);
	memset(p, 0, size);
	return p;
}

void *calloc(size_t nmemb, size_t size)
{
	return zalloc(nmemb * size);
}
