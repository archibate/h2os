#pragma once

#define EntrySize   4		// Size per (PDE or PTE) Entry

#define PageBits    12		// Page Offset Width
#define BigPageBits 22		// Big Page Offset Width
#define PgtabBits   10		// Page Table Index Width
#define PgdirBits   10		// Page Directory Index Width

#define SizeOfBits(n)    (1 << (n))
#define LomaskOfBits(n)  (SizeOfBits(n) - 1)
#define HimaskOfBits(n)  (-SizeOfBits(n))
#define RoundDown(n,x)   (((x) / (n)) * (n))
#define RoundUp(n,x)     RoundDown(n, (x) + (n) - 1)

#define PageSize      SizeOfBits(PageBits)
#define BigPageSize   SizeOfBits(BigPageBits)
#define PgtabSize     SizeOfBits(PgtabBits)
#define PgdirSize     SizeOfBits(PgdirBits)
#define PageLomask    LomaskOfBits(PageBits)
#define BigPageLomask LomaskOfBits(BigPageBits)
#define PgtabLomask   LomaskOfBits(PgtabBits)
#define PgdirLomask   LomaskOfBits(PgdirBits)
#define PageHimask    HimaskOfBits(PageBits)
#define BigPageHimask HimaskOfBits(BigPageBits)
#define PgtabHimask   HimaskOfBits(PgtabBits)
#define PgdirHimask   HimaskOfBits(PgdirBits)

#define PageDown(x)    RoundDown(PageSize, x)
#define BigPageDown(x) RoundDown(BigPageSize, x)
#define PgtabDown(x)   RoundDown(PgtabSize, x)
#define PgdirDown(x)   RoundDown(PgdirSize, x)
#define PageUp(x)      RoundUp(PageSize, x)
#define BigPageUp(x)   RoundUp(BigPageSize, x)
#define PgtabUp(x)     RoundUp(PgtabSize, x)
#define PgdirUp(x)     RoundUp(PgdirSize, x)

#define PageNum(x)     ((x) >> PageBits)
#define BigPageNum(x)  ((x) >> BigPageBits)
#define PgtabNum(x)    ((x) >> PgtabBits)
#define PgdirNum(x)    ((x) >> PgdirBits)

#define PteIndex(x)    (PageNum(x) & PgtabLomask)
#define PdeIndex(x)    (BigPageNum(x))

#ifndef _GPCPP_
#include <inttypes.h>
#include <ptrdeco.h>
typedef ulong va_t, __phys pa_t;
typedef pa_t pde_t, pte_t;
#endif
