#include <l4/invoke.h>
#include <l4/cnode.h>
#include <l4/sched.h>

int doSystemCall(cptr_t target, word_t *tag)
	// does l4Send(), l4Call(), l4Recv(), l4Reply(), l4Poll() seperated necessary?
{
	return 0;
	/*tcb_t *sender = schedGetCurrent();
	cap_t *targetCap = cget(sender->root, target);
	return sysInvoke(targetCap, sender, tag);*/
}
