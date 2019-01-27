#pragma once

#include <mmu/types.h>

typedef struct Frames
{
	uint ptePerm;
}
Frames_t;

pte_t _Frames_GetFramePte(Frames_t *fg);
