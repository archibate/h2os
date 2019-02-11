#pragma once

#include <l4/object/pgdir.h>

struct utcb;

void pgdir_init(struct pgdir *pgdir);
void pgdir_switch(struct pgdir *pgdir, struct utcb *utcb_ptr);
