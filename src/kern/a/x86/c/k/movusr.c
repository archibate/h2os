#include <k/movusr.h>
#include <k/kstack.h>
#include <l4/a/utcb.h>
#include <memory.h>
#include <stassert.h>

void _NORETURN goto_user_entry(void *pc, void *sp)
{
	UTCB_Init(kUTCB, (word_t)pc, (word_t)sp);
	static_assert((void*)&kUTCB->seframe == (void*)kSEFrame);
	static_assert((void*)&kUTCB->iframe == (void*)kIFrame);
	move_to_user(kIFrame); // intrents.asm
}
