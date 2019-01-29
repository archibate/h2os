#pragma once

#define kern_pd         ((pde_t*) 0x400000)
#define kern_ptes       ((pte_t*) 0x401000)
#define myWindowPgtab   ((pte_t*) 0x3ff000)
#define myWindowAddr    ((va_t)   0x3fe000)
