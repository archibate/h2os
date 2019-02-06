#pragma once

enum L4_ArgumentsIndex
{
	L4_Arg_Service = 0,
	L4_RWFragArg_DataBegin,
	L4_RWArg_Length = 1,
	L4_RWArg_DataBegin,
	L4_PRWArg_Length = 1,
	L4_PRWArg_Offset,
	L4_PRWArg_DataBegin,
	L4_TCB_SetCap_Arg_CapIdx = 1,
	L4_TCB_SetCap_Arg_CPtr,
	L4_TCB_GetCap_Arg_CapIdx = 1,
	L4_TCB_SetContext_Arg_ContextBegin = 1,
	L4_TCB_SetPriority_Arg_Priority = 1,
	L4_Endpoint_Retype_Arg_EPType = 1,
	L4_CSpace_SetDestSlot_Arg_SlotCPtr = 1,
	L4_Segment_Split_Arg_Point = 1,
	L4_Segment_AllocPage_Arg_Count = 1,
	L4_Slab_Allocate_Arg_Count = 1,
	L4_Page_Retype_Arg_ToType = 1,
	L4_Page_Retype_Arg_ObjType,
	L4_Pgdir_MapPage_Arg_VAddr = 1,
	L4_Pgdir_MapPage_Arg_PageCPtr,
	L4_Pgdir_MapPgtab_Arg_VAddr = 1,
	L4_Pgdir_MapPgtab_Arg_PgtabCPtr,
	L4_Pgdir_UnmapPage_Arg_VAddr = 1,
	L4_Pgdir_UnmapPgtab_Arg_VAddr = 1,
};

enum L4_ReturnsIndex
{
	L4_ReadRet_DataBegin = 0,
};
