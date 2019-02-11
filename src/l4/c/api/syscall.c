#include <l4/api/syscall.h>
#include <l4/api/capability.h>
#include <l4/generic/space.h>
#include <l4/generic/thread.h>

int sys_action(l4id_t capid, unsigned int action, unsigned long argument)
{
	struct capability *cap;
	cap = space_cap_lookup(&current->space, capid);

	return cap_action(cap, action, argument);
}
