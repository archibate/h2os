#pragma once

enum L4_ArgumentsIndex {
	L4_Arg_Service = 0,
	L4_RWFragArg_DataBegin,
	L4_RWArg_Length = 1,
	L4_RWArg_DataBegin,
	L4_PRWArg_Length = 1,
	L4_PRWArg_Offset,
	L4_PRWArg_DataBegin,
	L4_TCB_SetContext_Arg_ContextBegin = 1,
	L4_TCB_SetPriority_Arg_Priority = 1,
	L4_CSpace_SetDestSlot_Arg_SlotCPtr = 1,
	L4_Segment_Split_Arg_Point = 1,
	L4_Segment_AllocSlab_Arg_Count = 1,
	L4_Slab_Allocate_Arg_Count = 1,
	L4_Slab_Retype_Arg_ObjType = 1,
};

enum L4_ReturnsIndex {
	L4_ReadRet_DataBegin = 0,
};
