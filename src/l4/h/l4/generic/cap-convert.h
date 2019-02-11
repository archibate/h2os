#pragma once

#include <l4/object/capability.h>

#define cap_get_thread(cap)    ((struct ktcb     *)(cap)->pointer)
#define cap_get_endpoint(cap)  ((struct endpoint *)(cap)->pointer)
