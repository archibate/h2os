#pragma once

#include <l4/object/pgdir.h>

struct utcb;
struct ipc_buf;

void pgdir_init(struct pgdir *pgdir);
void pgdir_switch(struct pgdir *pgdir, struct utcb *utcb_ptr,
		struct ipc_buf *ipcbuf_ptr);
