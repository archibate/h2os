#pragma once

int sys_mmctl_setpcsp(l4id_t mmc, uintptr_t pc, uintptr_t sp);
int sys_mmctl_destroy(l4id_t mmc);
