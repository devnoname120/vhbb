#pragma once

#include "homebrew.h"

#define API_BASE "http://api.playlight.com.au/v1/"

#define API_HOMEBREWS API_BASE "homebrews"
#define API_ICON API_HOMEBREWS "/%s/icon"
#define API_SCREENSHOTS API_HOMEBREWS "/%s/screenshots"


int api_homebrew_list(Homebrew **homebrews, char *search, char *sort, char* filter);
Homebrew *api_homebrew(char *id);

Icon *api_icon(char *hb_id);
int api_screenshot_list(Screenshot **screenshots, char *hb_id);