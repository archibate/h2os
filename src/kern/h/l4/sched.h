#pragma once

#include <l4/thread.h>
#include <list.h>

extern struct list_head *runningHead;
extern tcb_t *currTcb;

void schedTimer(void);
void schedEnter(void);
void schedLeave(void);
void schedInit(tcb_t *x);
void schedSetActive(tcb_t *x);
void schedSetInactive(tcb_t *x);
#define schedGetCurr()	list_entry(runningHead, tcb_t, list)
void schedNext(void);
