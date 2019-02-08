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
	L4_Segment_AllocPage,
	L4_Page_Retype,
	L4_Slab_Allocate,
	L4_Endpoint_Retype,
	L4_TCB_Active,
	L4_TCB_Suspend,
	L4_TCB_SetCap,
	L4_TCB_GetCap,
	L4_TCB_GetExtraBuffer,
	L4_TCB_SetContext,
	L4_TCB_SetPCSP,
	L4_TCB_SetPriority,
	L4_CSpace_SetDestSlot,
	L4_Pgdir_MapPage,
	L4_Pgdir_MapPgtab,
	L4_Pgdir_UnmapPage,
	L4_Pgdir_UnmapPgtab,
	L4_Debug_Halt,
	L4_Debug_Puts,
};
