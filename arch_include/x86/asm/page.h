#pragma once

#define PGWID   12		// Page Offset Width
#define PTWID   10		// Page Table Index Width
#define PDWID   10		// Page Directory Index Width
#define VAWID   32		// Virtual Address Width (Sum of above)

#define PGSIZE  (1 << PGWID)
#define PTXSIZE (1 << PTWID)
#define PDXSIZE (1 << PDWID)
#define PGOFFMSK  (PGSIZE - 1)
#define PGMASK    (-PGSIZE)

#define PGOFF(va)   ((va) & (PGSIZE - 1))
#define PGDN(va)    ((va) & PGMASK)
#define PGUP(va)    (((va) + PGSIZE - 1) & PGMASK)
#define PGNUM(va)   ((va) >> PGWID)
#define PTX(va)     (PGNUM(va) & (PTXSIZE - 1))
#define PDX(va)     ((PGNUM(va) >> PTWID) & (PDXSIZE - 1))
