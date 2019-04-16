#pragma once

#define SWAP(x, y) do { typeof(y) t = y; y = x; x = t; } while (0)
