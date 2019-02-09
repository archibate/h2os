#pragma once

#include <bittools.h>

#define PdeDomainBits       4
#define PdeDomainShift      5
#define PdeDomainMask       LomaskOfBits(PdeDomainBits)
#define MakePdeDomain(d)    (((d) & PdeDomainMask) << PdeDomainShift)
#define PdeDomain(x)        (((x) >> PdeDomainShift) & PdeDomainMask)
#define setPdeDomain(x,t)   (((x) &= (PdeDomainMask << PdeDomainShift)) |= MakePdeDomain(t))
#define PdeDomain_Kernel    0x2
#define PdeDomain_User      0x1
#define PdeDomain_IO        0x0

#define VMAttributeMask             (3<<2)
#define VMAttribute_Cached          (3<<2)
#define VMAttribute_WriteBackOnly   (2<<2)
#define VMAttribute_WriteThrough    (1<<2)
#define VMAttribute_CacheDisabled   (0<<2)
#define PdeVMAttribute(x)           ((x) & VMAttributeMask)
#define setPdeVMAttribute(x,a)      (((x) &= VMAttributeMask) |= VMAttribute(a))
#define PteVMAttribute(x)           ((x) & VMAttributeMask)
#define setPteVMAttribute(x,a)      (((x) &= VMAttributeMask) |= VMAttribute(a))

#define PtePerm_KernRO      (0<<4)
#define PtePerm_KernRW      (1<<4)
#define PtePerm_UserRO      (2<<4)
#define PtePerm_UserRW      (3<<4)
#define PtePermMask         (3<<4)
#define PtePerm(x)          (((x) & PtePermMask)

#define PdeType_Pgtab       0x1
#define PdeType_Section     0x2
#define PdeTypeMask         0x3
#define PdeType(x)          ((x) & PdeTypeMask)
#define setPdeType(x,t)     (((x) &= PdeTypeMask) |= PdeType(t))

#define PdeIsValid(x)       (PdeType(x) != 0)

#define PteValidBit         0x2
#define PteIsValid(x)       ((x) & PteValidBit)

#define PteAddr(x)          ((x) & PageHimask)

#define Pte(x,a)            (PteValidBit | PteAddr(x) | PtePerm(a))

