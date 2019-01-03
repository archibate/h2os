#pragma once

#define KPBEG 0x100000
#define KBASE 0xc0000000
#define PHYSTOP 0x10000000

#ifdef _GPCPP_
#define KP2V(pa) ((typeof(pa) __unphys)((ulong)(pa) + KBASE))
#define KV2P(va) ((typeof(va) __phys)((ulong)(va) - KBASE))
#else
#define KP2V(pa) ((pa) + KBASE)
#define KV2P(va) ((va) - KBASE)
#endif
