#include <c4/libcrt.h>
#include <c4/liballoc.h>
#include <h4/mm/init.h>
#include <h4/sys/types.h>
#include <h4/sys/spawn.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

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

static int crt_parse_once(char **arr, size_t max, char **psp)
{
	char *sp = *psp;
	int i;
	for (i = 0; i < max; i++) {
		//printk("sp=[%s]", sp);
		arr[i] = sp;
		size_t size = strlen(sp);
		sp += size + 1;
		if (size == 0)
			break;
	}
	//printk("----");
	arr[i] = NULL;
	*psp = sp;
	return i;
}

void _NORETURN __crt_start(void *ebss, int (*main)(int, char *const *, char *const *), void *sp)
{
	__mm_init(ebss);
	liballoc_init();

	char *argv[MAX_ARGV+1];
	char *envp[MAX_ENVP+1];
	__crt_argc = crt_parse_once(argv, MAX_ARGV, (char**)&sp);
	crt_parse_once(envp, MAX_ENVP, (char**)&sp);
	__crt_argv = argv;
	__crt_envp = envp;

	call_ctors();
	exit(main(__crt_argc, __crt_argv, __crt_envp));
}
