#pragma once

#include <global_include.h>

#define VHBB_DATA std::string("ux0:/data/VitaHbBrowser")

#define API_ENDPOINT std::string("http://rinnegatamante.it/vitadb/list_hbs_yaml.php")
#define API_LOCAL std::string("ux0:/data/VitaHbBrowser/homebrews.yaml")

#define ICONS_FOLDER std::string("ux0:/data/VitaHbBrowser/icons")
#define ICON_URL_PREFIX std::string("http://rinnegatamante.it/vitadb/icons/")

#define SERVER_BASE_URL std::string("http://rinnegatamante.it/vitadb/")
#define SCREENSHOTS_FOLDER std::string("ux0:/data/VitaHbBrowser/screenshots")

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
#define lerpd(value, from_max, to_max) (((((double)value *10.) *((double)to_max *10.)) /((double)from_max *10.)) /10.)

#define ALIGN_CENTER(a, b) ((a - b) / 2)
#define ALIGN_LEFT(x, w) (x - w)
