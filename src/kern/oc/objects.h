#pragma once

enum L4_ObjectType
{
	L4_NullObject = 0,
	L4_UntypedObject, // from Retype
	L4_CNodeObject, // from Retype
	L4_EndpointObject,
	L4_IPCBufferObject, // from Retype
	L4_ObjectTypesNR,
};

#define CASE_PTRED_OBJECTS \
	case L4_IPCBufferObject: \
	;
#define CASE_RAWED_OBJECTS \
	case L4_UntypedObject: \
	case L4_CNodeObject: \
	case L4_EndpointObject: \
	;

#define Cap_Untyped(cap)   ((struct Untyped   *) getCapPtr_RAWED(cap))
#define Cap_CNode(cap)     ((struct CNode     *) getCapPtr_RAWED(cap))
#define Cap_Endpoint(cap)  ((struct Endpoint  *) getCapPtr_RAWED(cap))
#define Cap_IPCBuffer(cap) ((struct IPCBuffer *) getCapPtr_PTRED(cap))
