#pragma once

#include <global_include.h>

enum {
	COLOR_WHITE = RGBA8(255, 255, 255, 255),
	COLOR_BLACK = RGBA8(0, 0, 0, 255),
	COLOR_AQUA = RGBA8(168, 244, 253, 255),
	COLOR_BLUEGREY = RGBA8(92, 108, 124, 255),
	COLOR_RED = RGBA8(255, 0, 0, 255),
	COLOR_YELLOW = RGBA8(240, 255, 0, 255)
};

#define MAX(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })

#define MIN(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a < _b ? _a : _b; })

#define countof(lst) (sizeof(lst)/sizeof(*(lst)))
#define lerp(value, from_max, to_max) ((((value *10) *(to_max *10)) /(from_max *10)) /10)

#define ALIGN_CENTER(a, b) ((a - b) / 2)
#define ALIGN_LEFT(x, w) (x - w)
