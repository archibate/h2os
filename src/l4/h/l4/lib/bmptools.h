#pragma once

#define bmp_getbit(bmp, i) (((bmp)[(i)/8] >> ((i)%8)) & 1)
#define bmp_setbit(bmp, i) ((bmp)[(i)/8] |=  (1 << ((i)%8)))
#define bmp_clrbit(bmp, i) ((bmp)[(i)/8] &= ~(1 << ((i)%8)))

