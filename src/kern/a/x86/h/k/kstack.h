#pragma once

#include <k/kbase.h>
#include <k/asm/iframe.h>

#define KernStackTop    (KernPhysEnd)
#define KernTCB         (KernPoolBegin - 0x1000)
#define KernIFrame      (KernStackTop - IFrameWords * 4)

#ifndef _GPCPP_
#include <inttypes.h>
#define kTCB     ((struct TCB *)KernTCB)
#define kIFrame  ((ulong*)KernIFrame)
#endif
