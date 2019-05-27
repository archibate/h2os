#pragma once

sl4id_t sys_create_mm(l4id_t mmc);
int sys_mm_btw_fdup2(l4id_t msrc, l4fd_t fsrc, l4id_t mdst, l4fd_t fdst);
sl4id_t sys_mm_new_thread(l4id_t mmc, uintptr_t pc, uintptr_t sp);
int sys_mm_destroy(l4id_t mmc);
