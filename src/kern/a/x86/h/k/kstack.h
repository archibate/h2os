#pragma once

#include <k/kbase.h>
#include <k/asm/iframe.h>
#include <k/asm/seframe.h>
#include <mmu/page.h>

#define KernSEStackTop  (KernPhysEnd)
#define KernIStackTop   (KernPhysEnd - 2 * PageSize)
#define KernIFrame      (KernIStackTop - IFrameWords * 4)
#define KernSEFrame     (KernSEStackTop - SEFrameWords * 4)

#ifndef _GPCPP_
#include <inttypes.h>
#define kIFrame  ((ulong*)KernIFrame)
#define kSEFrame ((ulong*)KernSEFrame)
#endif
