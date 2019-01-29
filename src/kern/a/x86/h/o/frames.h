#pragma once

#include <mmu/types.h>
#include <mmu/pte.h>

enum FramesType
{
	Frames_NullType = 0,
	Frames_PhysRegion,
};
typedef unsigned FramesType_t;

typedef struct Frames
{
	uint ptePerm : PtePermMaxBits;
	FramesType_t fsType : 32 - 1 - PtePermMaxBits;
	unsigned fs_needZero : 1;
	pa_t fs_pa0, fs_pa1;
}
Frames_t;

pte_t _Frames_GetFramePte(Frames_t *fs);
