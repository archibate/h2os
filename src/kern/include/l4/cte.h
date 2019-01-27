#pragma once

#include <l4/cap.h>

#define CteWords   (CapWords + 1)

typedef union cte
{
	CapType_t type;
	cap_t cap;
}
cte_t;
