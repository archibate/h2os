#pragma once

enum L4_InvokeNumber
{
	L4_NullInvocation = 0,
	L4_Untyped_Retype,
	L4_CNode_Delete,
	L4_CNode_Move,
	L4_CNode_Swap,
	L4_IPCBuffer_setRegister,
	L4_IPCBuffer_getRegister,
};
