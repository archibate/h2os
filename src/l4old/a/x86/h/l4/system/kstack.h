#pragma once

#include <l4/system/kbase.h>
#include <l4/system/asm/iframe.h>
#include <l4/system/asm/seframe.h>
#include <l4/machine/mmu/page.h>

#define KernSEFrame     (KernUTCBAddr)
#define KernSEStackTop  (KernSEFrame + SEFrameWords * 4)
#define KernIFrame      (KernSEStackTop)
#define KernIStackTop   (KernIFrame + IFrameWords * 4)

#ifndef _GPCPP_
#include <inttypes.h>
#define kPgdir    ((pde_t*)KernPgdirAddr)
#define kUTCB    ((struct UTCB*)KernUTCBAddr)
#define kIFrame  ((ulong*)KernIFrame)
#define kSEFrame ((ulong*)KernSEFrame)
#endif
