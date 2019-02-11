#pragma once

#include "pte-bits.h"
#include "page.h"

#define PteInitialValue (PG_P)
#define PteIsValid(x)   ((x) & PG_P)

#define PtePerm_KernRO  (0)
#define PtePerm_KernRW  (PG_W)
#define PtePerm_UserRO  (PG_U)
#define PtePerm_UserRW  (PG_W | PG_U)
#define PtePermMask     (PG_W | PG_U)
#define PtePerm(x)      ((x) & PtePermMask)
#define setPtePerm(x,a) (((x) &= PtePermMask) |= PtePerm(a))
//#define PtePermMaxBits  3

#define VMAttributeMask             (PG_PCD | PG_PWT)
#define VMAttribute_CacheDisabled   (PG_PCD | PG_PWT)
#define VMAttribute_WriteThrough    (PG_PWT)
#define VMAttribute_WriteBackOnly   (PG_PCD)
#define VMAttribute_Cached          (0)
#define PteVMAttribute(x)           ((x) & VMAttributeMask)
#define setPteVMAttribute(x,a)      (((x) &= VMAttributeMask) |= VMAttribute(a))

//#define PteAddrMask     (PGMASK)
#define PteAddr(x)      ((x) & PageHimask)

#define Pte(x,a)        (PteInitialValue | PteAddr(x) | PtePerm(a))


#define PdeInitialValue (PG_P | PG_W | PG_U)
#define PdeIsValid(x)   ((x) & PG_P)

#define PdeType_Pgtab   (PG_P)
#define PdeType_Section (PG_P | PG_PSE)
#define PdeTypeMask     (PG_P | PG_PSE)
#define PdeType(x)      ((x) & PdeTypeMask)
#define setPdeType(x,t) (((x) &= PdeTypeMask) |= PdeType(t))

#define PdePgtabAddr(x)   ((x) & PgtabHimask)
#define PdeSectionAddr(x) ((x) & SectionHimask)

#define PdePgtab(x)     (PdeInitialValue | PdePgtabAddr(x) | PdeType_Pgtab)
#define PdeSection(x)   (PdeInitialValue | PdeSectionAddr(x) | PdeType_Section)
