


const char *__ntNameTableOfEnum_L4_ErrorNumber[] =
{
	[0] = "L4_NoError",
	"L4_EService",
	"L4_EPerm",
	"L4_ELimit",
	"L4_ELength",
	"L4_EOffset",
	"L4_ECapType",
	"L4_EObjType",
	"L4_ERetype",
	"L4_EWater",
	"L4_ErrorMax",
};


const char *__ntNameTableOfEnum_L4_InitCapPtr[] =
{
	[0] = "L4_InitCapNull",
	"L4_InitCapConsole",
	"L4_InitCapIOPort",
	"L4_InitCapDebug",
	"L4_InitCapExtra",
	"L4_InitCapSigma0",
	"L4_InitCapDestSlot0",
	"L4_InitCaps",
};

const char *__ntNameTableOfEnum_L4_ArgumentsIndex[] =
{
	[0] = "L4_Arg_Service",
	"L4_RWFragArg_DataBegin",
	[1] = "L4_RWArg_Length",
	"L4_RWArg_DataBegin",
	[1] = "L4_PRWArg_Length",
	"L4_PRWArg_Offset",
	"L4_PRWArg_DataBegin",
	[1] = "L4_TCB_SetContext_Arg_ContextBegin",
	[1] = "L4_TCB_SetPriority_Arg_Priority",
	[1] = "L4_Segment_Split_Arg_Point",
	[1] = "L4_Segment_AllocSlab_Arg_Count",
	[1] = "L4_Slab_Allocate_Arg_Count",
	[1] = "L4_Slab_Retype_Arg_ObjType",
};

const char *__ntNameTableOfEnum_L4_CapType[] =
{
	[0] = "L4_NullCap",
	"L4_UntypedCap",
	"L4_ConsoleCap",
	"L4_IOPortCap",
	"L4_DebugCap",
	"L4_SlabCap",
	"L4_TCBCap",
	"L4_ExtraCap",
	"L4_SegmentCap",
	"L4_TestObjCap",
	"L4_CapTypesNR",
};

const char *__ntNameTableOfEnum_L4_ServiceNumber[] =
{
	"L4_NullService",
	"L4_Write",
	"L4_Read",
	"L4_PWrite",
	"L4_PRead",
	"L4_WriteFrag",
	"L4_RewindWriteFrag",
	"L4_Segment_Split",
	"L4_Segment_AllocSlab",
	"L4_Slab_Retype",
	"L4_Slab_Allocate",
	"L4_TCB_SetContext",
	"L4_TCB_SetPriority",
	"L4_Debug_Halt",
	"L4_Debug_Puts",
};

