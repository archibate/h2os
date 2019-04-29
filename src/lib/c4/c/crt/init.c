#include <c4/libcrt.h>
#include <h4/mm/init.h>
#include <c4/liballoc.h>

static void call_ctors(void)
{
	typedef void (*ctor_ptr_t)(void);
	extern ctor_ptr_t __CTORS_BEGIN__[], __CTORS_END__[];
	ctor_ptr_t *ctor;
	for (ctor = __CTORS_BEGIN__; ctor < __CTORS_END__; ctor++)
		/*printk("calling ctor %p", *ctor), */(*ctor)();
}

int __crt_argc;
char *const *__crt_argv;
char *const *__crt_envp;

void __crt_init(void *ebss, int argc, char *const argv[], char *const envp[])
{
	__crt_argc = argc;
	__crt_argv = argv;
	__crt_envp = envp;
	__mm_init(ebss);
	liballoc_init();
	call_ctors();
}
