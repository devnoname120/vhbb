#include <stddef.h>
#include <stdlib.h>
#include <cJSON.h>

#include <psp2/sysmodule.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include "../../src/network.h"

#include "homebrew.h"

int json_open(char *json_str, cJSON **root);
int json_close(cJSON *root);
int check_status(cJSON *root, int *code, char **message);
int get_data(cJSON *root, cJSON **data);
int get_homebrew_entries(cJSON *root, Homebrew **homebrews);
int homebrew_entry(cJSON *hb_entry, Homebrew *homebrew);
