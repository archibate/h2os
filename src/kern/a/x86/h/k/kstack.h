#pragma once

#include <k/kbase.h>
#include <k/asm/iframe.h>
#include <k/asm/seframe.h>
#include <mmu/page.h>

#define KernSEFrame     (KernUTCBAddr)
#define KernSEStackTop  (KernSEFrame + SEFrameWords * 4)
#define KernIFrame      (KernSEStackTop)
#define KernIStackTop   (KernIFrame + IFrameWords * 4)

#ifndef _GPCPP_
#include <inttypes.h>
#define kUTCB    ((struct UTCB*)KernUTCBAddr)
#define kIFrame  ((ulong*)KernIFrame)
#define kSEFrame ((ulong*)KernSEFrame)
#endif
