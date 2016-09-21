#include "json.h"

#define API_BASE "http://api.playlight.com.au/v1/"

char *json_get(const char *url);
int get_homebrew_list(Homebrew **homebrews);
