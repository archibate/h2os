#pragma once

#define EntrySize   4		// Size per (PDE or PTE) Entry

#define PageBits    12		// Page Offset Width
#define SectionBits 22		// Big Page Offset Width
#define PgtabBits   8		// Page Table Index Width
#define PgdirBits   12		// Page Directory Index Width

#include <bittools.h>
#include <roundtools.h>

#define PageSize      SizeOfBits(PageBits)
#define SectionSize   SizeOfBits(SectionBits)
#define PgtabSize     SizeOfBits(PgtabBits)
#define PgdirSize     SizeOfBits(PgdirBits)
#define PageLomask    LomaskOfBits(PageBits)
#define SectionLomask LomaskOfBits(SectionBits)
#define PgtabLomask   LomaskOfBits(PgtabBits)
#define PgdirLomask   LomaskOfBits(PgdirBits)
#define PageHimask    HimaskOfBits(PageBits)
#define SectionHimask HimaskOfBits(SectionBits)
#define PgtabHimask   HimaskOfBits(PgtabBits)
#define PgdirHimask   HimaskOfBits(PgdirBits)

#define PageDown(x)    RoundDown(PageSize, x)
#define SectionDown(x) RoundDown(SectionSize, x)
#define PgtabDown(x)   RoundDown(PgtabSize, x)
#define PgdirDown(x)   RoundDown(PgdirSize, x)
#define PageUp(x)      RoundUp(PageSize, x)
#define SectionUp(x)   RoundUp(SectionSize, x)
#define PgtabUp(x)     RoundUp(PgtabSize, x)
#define PgdirUp(x)     RoundUp(PgdirSize, x)

#define PageNum(x)     ((x) >> PageBits)
#define SectionNum(x)  ((x) >> SectionBits)
#define PgtabNum(x)    ((x) >> PgtabBits)
#define PgdirNum(x)    ((x) >> PgdirBits)

#define PageOffset(x)     ((x) % PageSize)
#define SectionOffset(x)  ((x) % SectionSize)
#define PgtabOffset(x)    ((x) % PgtabSize)
#define PgdirOffset(x)    ((x) % PgdirSize)

#define PteIndex(x)    (PageNum(x) & PgtabLomask)
#define PdeIndex(x)    (SectionNum(x))
