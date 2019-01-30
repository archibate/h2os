#pragma once

#define static_assert(x) \
	switch (0) { case 0: case (x): break; }
