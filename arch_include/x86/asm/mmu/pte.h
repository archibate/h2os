#pragma once

#include "pte-bits.h"

#define PteInitialValue (PG_P)
#define PteIsValid(x)   ((x) & PG_P)

#define PtePerm_KernRO  (0)
#define PtePerm_KernRW  (PG_W)
#define PtePerm_RO      (PG_U)
#define PtePerm_RW      (PG_W | PG_U)
#define PtePermMask     (PG_W | PG_U)
#define PtePerm(x)      ((x) & PtePermMask)
#define setPtePerm(x,a) (((x) &= PtePermMask) |= PtePerm(a))

#define PteAddrMask     (PGMASK)
#define PteAddr(x)      ((x) & PteAddrMask)

#define Pte(x,a)        (PteInitialValue | PteAddr(x) | PtePerm(a))


#define PdeInitialValue (PG_P | PG_W | PG_U)
#define PdeIsValid(x)   ((x) & PG_P)

#define PdeType_Pgtab   (PG_P)
#define PdeType_Section (PG_P | PG_PSE)
#define PdeTypeMask     (PG_P | PG_PSE)
#define PdeType(x)      ((x) & PdeTypeMask)
#define setPdeType(x,t) (((x) &= PdeTypeMask) |= PdeType(t))

#define PdeAddrMask     (PGMASK)
#define PdeAddr(x)      ((x) & PdeAddrMask)

#define PdePgtab(x)     (PdeInitialValue | PdeAddr(x) | PdeType_Pgtab)
#define PdeBigPage(x)   (PdeInitialValue | PdeAddr(x) | PdeType_BigPage)
