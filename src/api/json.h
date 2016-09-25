#pragma once

#include "homebrew.h"

int json_open(char *json_str, cJSON **root);
int json_close(cJSON *root);
int json_status(cJSON *root, int *code, char **message);
int json_data(cJSON *root, cJSON **data);

int json_homebrew_list(cJSON *root, Homebrew **homebrews);
Homebrew *json_homebrew(cJSON *root);
int json_homebrew_entry(cJSON *hb_entry, Homebrew *homebrew);

Icon *json_icon(cJSON *root);

int json_screenshot_list(cJSON *root, Screenshot **screenshots);
Screenshot *json_screenshot(cJSON *root);
int json_screenshot_entry(cJSON *sc_entry, Screenshot *screenshot);
