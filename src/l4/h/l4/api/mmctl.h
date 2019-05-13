#pragma once

int sys_mm_new_thread(l4id_t mmc, uintptr_t pc, uintptr_t sp);
int sys_mm_destroy(l4id_t mmc);
