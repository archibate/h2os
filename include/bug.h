#pragma once

#include <assert.h>

#define BUG_ON(x)  assert_info(!(x), "BUG_ON("#x")")
#define BUG()      assert_info(0,    "BUG()")
