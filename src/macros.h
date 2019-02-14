#pragma once

#include <global_include.h>

#define VHBB_DATA std::string("ux0:/data/VitaHbBrowser")

#define API_ENDPOINT std::string("https://rinnegatamante.it/vitadb/list_hbs_yaml.php")
#define API_LOCAL std::string("ux0:/data/VitaHbBrowser/homebrews.yaml")

#define ICONS_FOLDER std::string("ux0:/data/VitaHbBrowser/icons")
#define ICON_URL_PREFIX std::string("https://rinnegatamante.it/vitadb/icons/")

#define UPDATE_FOLDER "ux0:/temp/vhbb/update"

#define SERVER_BASE_URL std::string("https://rinnegatamante.it/vitadb/")
#define SCREENSHOTS_FOLDER std::string("ux0:/data/VitaHbBrowser/screenshots")

enum {
	COLOR_WHITE = RGBA8(255, 255, 255, 255),
	COLOR_BLACK = RGBA8(0, 0, 0, 255),
	COLOR_AQUA = RGBA8(168, 244, 253, 255),
	COLOR_BLUEGREY = RGBA8(92, 108, 124, 255),
	COLOR_RED = RGBA8(255, 0, 0, 255),
	COLOR_YELLOW = RGBA8(240, 255, 0, 255)
};
