#pragma once

// `Can' Permissions:
#define CanRead  1
#define CanWrite 2
#define CanGrant 4
#define CanRW    (CanRead | CanWrite)
#define CanRO    (CanRead)
