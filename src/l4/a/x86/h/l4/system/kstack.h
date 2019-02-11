#pragma once

#include <l4/system/kbase.h>
#include <l4/object/utcb.h>

#define kPd       ((pde_t*)KernPgdirAddr)
#define kUTCB     ((struct utcb*)KernUTCBAddr)
#define kIFrame   (kUTCB->iframe)
#define kSEFrame  (kUTCB->seframe)

#define KernSEStackTop  ((unsigned long)&kUTCB->seframe + sizeof(kUTCB->seframe))
#define KernIStackTop   ((unsigned long)&kUTCB->iframe + sizeof(kUTCB->iframe))
