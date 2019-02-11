#pragma once

#include <l4/misc/assert.h>

#define BUG_ON(x)  assert_info(!(x), "BUG: "#x)
#define BUG()      assert_info(0,    "BUG!")
