#pragma once

enum L4_ServiceType
{
	L4_Write,
	L4_Read,
	L4_Segment_Split,
	L4_Segment_AllocSlab,
	L4_Slab_Retype,
	L4_Slab_Allocate,
	L4_Debug_Halt,
	L4_Debug_Puts,
};
