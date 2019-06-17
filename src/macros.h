#pragma once

#include <vita2d.h>

#define VHBB_DATA "ux0:/data/VitaHbBrowser"
#define VHBB_RESOURCES "ux0:/app/" VITA_TITLEID "/resources"

#define API_ENDPOINT "https://rinnegatamante.it/vitadb/list_hbs_yaml.php"
#define API_LOCAL "ux0:/data/VitaHbBrowser/homebrews.yaml"

#define ICONS_FOLDER "ux0:/data/VitaHbBrowser/icons"
#define ICON_URL_PREFIX "https://rinnegatamante.it/vitadb/icons/"

#define SERVER_BASE_URL "https://rinnegatamante.it/vitadb/"
#define SCREENSHOTS_FOLDER "ux0:/data/VitaHbBrowser/screenshots"

enum {
	COLOR_WHITE = RGBA8(255, 255, 255, 255),
	COLOR_BLACK = RGBA8(0, 0, 0, 255),
	COLOR_AQUA = RGBA8(168, 244, 253, 255),
	COLOR_BLUEGREY = RGBA8(92, 108, 124, 255),
	COLOR_RED = RGBA8(255, 0, 0, 255),
	COLOR_YELLOW = RGBA8(240, 255, 0, 255)
};
