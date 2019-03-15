// vim: ts=4 sts=4 sw=4 fdm=marker
// reference: http://wiki.0xffffff.org/posts/hurlex-11.html
#include <malloc.h> // impelementation
#include <liballoc.h> // impelementation
#include <memory.h> // memcpy in realloc
#define PGSIZE 4096 // machi
#define PGMASK (-PGSIZE)
#include <inttypes.h> // size_t
typedef size_t addr_t;

#include <assert.h> // assert

typedef struct HNODE {
	struct HNODE *next, *prev;
	size_t allocated;
} HNODE;

static HNODE *heap_head;

#if 0
#include <unistd.h> // brk
static void *get_break(void)
{
	return sbrk(0);
}

static void set_break(void *p)
{
	static void *currbrk = NULL;
	p = p + PGSIZE;
	if (currbrk != p)
		brk(currbrk = p);
}
#else
static void *currbrk, *maxbrk;

static void *get_break(void)
{
	return currbrk;
}

static void set_break(void *p)
{
	currbrk = p + PGSIZE;
	assert_info(currbrk < maxbrk, "malloc: out of memory");
}

void liballoc_set_memory(void *begin, size_t size)
{
	currbrk = begin;
	maxbrk = begin + size;
	liballoc_init();
}
#endif

void liballoc_init(void)
{
	heap_head = (HNODE*)get_break();
	set_break(heap_head);
	heap_head->allocated = 0;
	heap_head->next = 0;
	heap_head->prev = heap_head;
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

void *malloc(size_t len)
{
	len = (len + sizeof(int) - 1) & (~0UL ^ (sizeof(int) - 1));
	len += sizeof(HNODE);

	HNODE *curr;
	for (curr = heap_head; curr->next; curr = curr->next)
	{
		if (!curr->allocated) {
			ssize_t rest_len = (void*)curr->next - (void*)curr - len;
			if (rest_len > 0) {
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
	}

	HNODE *next = (HNODE*)((void*)curr + len);
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
