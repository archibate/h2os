#pragma once

#include <l4/lib/assert.h>

#define BUG_ON(x)  assert(!(x))
#define BUG()      assert(0)
