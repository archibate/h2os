#include <l4/generic/ipc.h>
#include <l4/generic/softfault.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/msginfo.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/lib/swapval.h>

int endp_recv(struct endpoint *ep, bool block)
{
	struct ktcb *target = endpoint_wait(ep, current, block);
	if (target != NULL) {
		current->prplmip = target->psndmip;
		msginfo_copy(&current->msginfo, target->psndmip);
		SWAP(current->ipcbuf, target->ipcbuf);
	} else {
		if (!block)
			return -EAGAIN;
	}
	return 0;
}

int endp_call(struct fd_entry *fde, bool block, bool recv, int phase)
{
	struct endpoint *ep = fde->ep;
	struct msginfo *mip = &fde->msginfo;
	struct ktcb *target = endpoint_call(ep, current, block, recv);
	//current->ipcphase = phase ? phase : fd_entry(fd)->phase;
	//fd_entry(fd)->msginfo.phase = phase;
	if (target != NULL) {
		//target->msginfo.phase = fd_entry(fd)->phase;
		target->prplmip = mip;
		msginfo_copy(&target->msginfo, mip);
		SWAP(current->ipcbuf, target->ipcbuf);
	} else {
		current->psndmip = mip;
		//current->msginfo = *current->replymsi;
	}
	return 0;
}

int endp_reply(uintptr_t badge, uintptr_t offset, uintptr_t type)
{
	if (current->prplmip != NULL) {
		current->prplmip->badge = badge;
		current->prplmip->offset = offset;
		current->prplmip->type = type;
	}
	current->prplmip = NULL;
	struct ktcb *target = endpoint_reply(current); // T,ep
	if (target != NULL) {
		if (target->sfipc_type)
			softfault_onreply(target);
		SWAP(current->ipcbuf, target->ipcbuf);
	}
	return target == NULL ? 0 : 1;
}
