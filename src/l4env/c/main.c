#include <l4/types.h>
#include <l4/enum/rtype.h>
#include <l4/api/hello.h>
#include <l4/api/rtalloc.h>

void main(void)
{
	sys_hello();

	l4id_t id = sys_rt_new(RTYPE_THREAD);

	sys_halt();
}
