#pragma once

#define kern_pd         ((pde_t*) 0x400000)
#define kern_ptes       ((pte_t*) 0x401000)
#define kern_ptpool_beg   ((pa_t) 0x801000)
