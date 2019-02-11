#pragma once

#define SizeOfBits(n)    (1 << (n))
#define LomaskOfBits(n)  (SizeOfBits(n) - 1)
#define HimaskOfBits(n)  (-SizeOfBits(n))
