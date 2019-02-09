#include <l4/i/tcbsch.h>
#include <l4/captypes.h>
#include <l4/errors.h>
#include <l4/sched.h>

int do_TCB_SetPriority(tcb_t *tcb, byte_t prio)
{
	bool running = tcb->state == TCB_Running;
	if (running) schedSuspend(tcb);
	tcb->priority = prio;
	if (running) schedActive(tcb);
	return 0;
}

int do_TCB_Active(tcb_t *tcb)
{
	if (tcb->state != TCB_NullState)
		return -L4_EActived;
	if (tcb->t_cspace.ctype != L4_CSpaceCap)
		return -L4_ERetype;
	if (tcb->t_pgdir.ctype != L4_PgdirCap)
		return -L4_ERetype;
	if (tcb->t_utcb.ctype != L4_PageCap)
		return -L4_ERetype;
	tcb->state = TCB_Running;
	schedActive(tcb);
	return 0;
}

int do_TCB_Suspend(tcb_t *tcb)
{
	if (tcb->state != TCB_Running)
		return -L4_EBlocked;
	if (tcb->t_cspace.ctype != L4_CSpaceCap)
		return -L4_ERetype;
	if (tcb->t_pgdir.ctype != L4_PgdirCap)
		return -L4_ERetype;
	if (tcb->t_utcb.ctype != L4_PageCap)
		return -L4_ERetype;
	tcb->state = TCB_Suspend;
	schedSuspend(tcb);
	return 0;
}
