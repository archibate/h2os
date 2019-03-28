#pragma once

#include <errno.h>

#define error(x) ({ errno = (x); NULL; })
