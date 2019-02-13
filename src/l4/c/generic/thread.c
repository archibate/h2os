#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/task-switch.h>
#include <l4/misc/printk.h>
#include <l4/misc/assert.h>
#include <l4/misc/bug.h>

static void _sched_lower_priority(void);

bool curr_idle = true;
unsigned char curr_priority;
struct list_head *running_heads[MAX_PRIORITY];
struct ktcb *current;

void thread_set_priority(struct ktcb *tcb, unsigned char priority)
{
	bool need = tcb->state == THREAD_RUNNING;
	if (need) thread_suspend(tcb);
	tcb->priority = priority;
	if (need) thread_active(tcb);
}

void thread_active(struct ktcb *x)
{
	BUG_ON(x->state != THREAD_RUNNING);

	curr_idle = false;
	if (!running_heads[x->priority])
	list_init(running_heads[x->priority] = &x->list);
	else
		list_add_tail(&x->list, running_heads[x->priority]);
	if (curr_idle || x->priority > curr_priority)
		curr_priority = x->priority;
}

void thread_suspend(struct ktcb *x)
{
	BUG_ON(x->state == THREAD_RUNNING);

	if (&x->list == running_head) {
		if (running_head == running_head->next) {
			running_head = 0;
			_sched_lower_priority();
		} else {
			sched_next();
			list_del(&x->list);
		}
	} else {
		list_del(&x->list);
	}
}

void sched_enter(void)
{
	current = sched_get_curr();
	assert(&current->list != NULL);
	assert(&current->list != INVALID_PTR);
}

void sched_leave(void)
{
	struct ktcb *next = sched_get_curr();
	assert(&next->list != NULL);
	assert(&next->list != INVALID_PTR);
	if (next != current) {
		task_switch(current, next);
	}
}

void _sched_lower_priority(void)
{
	int i;
	for (i = 0; i < curr_priority; i++) {
		if (running_heads[i]) {
			curr_priority = i;
			//dprintk("%d!", i);
			return;
		}
	}
	curr_idle = 1;
}

void sched_next(void)
{
	running_head = running_head->next;
}

void sched_timer_callback(void)
{
#ifdef CONFIG_LOG_SCHED_TIMER
	printk("sched_timer (%s)...", curr_idle ? "idle" : "busy");
#endif
	if (!curr_idle) {
		sched_enter();
		sched_next();
		sched_leave();
	}
}
