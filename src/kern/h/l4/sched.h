#pragma once

#include <l4/thread.h>
#include <list.h>

extern byte_t currPriority;
#define L4_MaxPriority 256
extern struct list_head *runningHeads[L4_MaxPriority];
#define runningHead  (runningHeads[currPriority])
extern tcb_t *currTcb;

void schedTimer(void);
void schedEnter(void);
void schedLeave(void);
void schedInit(tcb_t *x);
void schedActive(tcb_t *x);
void schedSuspend(tcb_t *x);
#define schedGetCurr()	list_entry(runningHead, tcb_t, list)
void schedNext(void);
