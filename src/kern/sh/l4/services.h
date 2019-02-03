#pragma once

enum L4_ServiceNumber
{
	L4_NullService,
	L4_Write,
	L4_Read,
	L4_PWrite,
	L4_PRead,
	L4_WriteFrag,
	L4_ReadFrag,
	L4_RewindWriteFrag,
	L4_RewindReadFrag,
	L4_Segment_Split,
	L4_Segment_AllocSlab,
	L4_Slab_Retype,
	L4_Slab_Allocate,
	L4_Endpoint_Retype,
	L4_TCB_SetContext,
	L4_TCB_SetPriority,
	L4_TCB_GetExtraBuffer,
	L4_CSpace_SetDestSlot,
	L4_Debug_Halt,
	L4_Debug_Puts,
};
