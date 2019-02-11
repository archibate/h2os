#pragma once

#include <l4/i/tcb.h>
#include <l4/consts.h>
#include <list.h>

extern byte_t currPriority;
extern struct list_head *runningHeads[L4_MaxPriority];
#define runningHead  (runningHeads[currPriority])
extern tcb_t *currTcb;

void schedTimerCallback(void);
void schedEnter(void);
void schedLeave(void);
void schedInit(tcb_t *x);
void schedActive(tcb_t *x);
void schedSuspend(tcb_t *x);
#define schedGetCurr()	list_entry(runningHead, tcb_t, list)
void schedNext(void);
