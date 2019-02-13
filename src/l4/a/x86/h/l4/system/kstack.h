#pragma once

#include <l4/system/kbase.h>
#include <l4/object/utcb.h>

#define kUTCB        ((struct utcb*)KernUTCBAddr)
#define kErnelExiter (kUTCB->exiter)
#define kIFrame      (kUTCB->iframe)
#define kSEFrame     (kUTCB->seframe)

#define KernSEStackTop  ((word_t)&kSEFrame + sizeof(kSEFrame))
#define KernIStackTop   ((word_t)&kIFrame  + sizeof(kIFrame))

static void __stassertF12350950(void)
{
	static_assert(KernSEStackTop == KernUTCBAddr + sizeof(kErnelExiter) + sizeof(kSEFrame));
	static_assert(KernIStackTop == KernUTCBAddr + sizeof(kErnelExiter) + sizeof(kSEFrame) + sizeof(kIFrame));
	static_assert((word_t)&kSEFrame == KernUTCBAddr + sizeof(kErnelExiter));
	static_assert((word_t)&kIFrame == KernUTCBAddr + sizeof(kErnelExiter) + sizeof(kSEFrame));
}
