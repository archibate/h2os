#include <c4/liballoc.h>//

static void call_ctors(void)
{
	typedef void (*ctor_ptr_t)(void);
	extern ctor_ptr_t __CTORS_BEGIN__[], __CTORS_END__[];
	ctor_ptr_t *ctor;
	for (ctor = __CTORS_BEGIN__; ctor < __CTORS_END__; ctor++)
		/*printk("calling ctor %p", *ctor), */(*ctor)();
}

void __crt_init(int argc, char *const argv[], char *const envp[])
{
	static char heap[4096*2];//
	liballoc_set_memory(&heap, sizeof(heap));//
	call_ctors();
}
