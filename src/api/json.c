#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <cJSON.h>

#include <psp2/sysmodule.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include "../../src/network.h"

#include "json.h"

#include "../tools.h"

// Open a JSON string
int json_open(char *json_str, cJSON **root)
{
	if (json_str == NULL || root == NULL) return -1;

	*root = cJSON_Parse(json_str);
	return 0;
}

// Close a JSON string
int json_close(cJSON *root)
{
	cJSON_Delete(root);
	return 0;
}



/*
{
	"status": {
		"code": "200",
		"message": "OK"
	},
	"data": []
}
*/

// Retrieve code and message from the JSON
int json_status(cJSON *root, int *code, char **message)
{
	cJSON *status = cJSON_GetObjectItem(root, "status");
	if (status == NULL) return -1;

	*message = cJSON_GetObjectItem(status, "message")->valuestring;

	// FIXME: the API returns a string when it should be a number
	*code = atoi(cJSON_GetObjectItem(status, "code")->valuestring);

	return 0;
}

// Get data node
int json_data(cJSON *root, cJSON **data)
{
	*data = cJSON_GetObjectItem(root, "data");
	if (data == NULL) return -1;

	return 0;
}

// Create a list of Homebrew structs, fill them, and put the list pointer into *homebrews
int json_homebrew_list(cJSON *root, Homebrew **homebrews)
{
	cJSON *data = NULL;
	json_data(root, &data);

	int hb_s = cJSON_GetArraySize(data);

	*homebrews = malloc(hb_s * sizeof(Homebrew));

	int i = 0;
	for (i = 0; i < hb_s; i++) {
		cJSON *hb_entry = cJSON_GetArrayItem(data, i);
		json_homebrew_entry(hb_entry, &(*homebrews)[i]);
	}

	return hb_s;
}

Homebrew *json_homebrew(cJSON *root)
{
	Homebrew *homebrew = malloc(sizeof(Homebrew));
	cJSON *hb_entry = NULL;
	json_data(root, &hb_entry);

	json_homebrew_entry(hb_entry, homebrew);

	return homebrew;
}

// Fill the structure *homebrew from a hb_entry node
int json_homebrew_entry(cJSON *hb_entry, Homebrew *homebrew)
{
	logcat_add("1: ", cJSON_GetObjectItem(hb_entry, "UUID")->valuestring, "\n");
	homebrew->UUID = cJSON_GetObjectItem(hb_entry, "UUID")->valuestring;
	logcat_add("1: ", homebrew->UUID, "\n");
	homebrew->name = cJSON_GetObjectItem(hb_entry, "name")->valuestring;
	homebrew->developer = cJSON_GetObjectItem(hb_entry, "developer")->valuestring;
	homebrew->type = cJSON_GetObjectItem(hb_entry, "type")->valuestring;
	homebrew->first_release = cJSON_GetObjectItem(hb_entry, "first_release")->valuestring;
	homebrew->description = cJSON_GetObjectItem(hb_entry, "description")->valuestring;

	return 0;
}

Icon *json_icon(cJSON *root)
{
	cJSON *icon_entry;
	json_data(root, &icon_entry);

	Icon *icon = malloc(sizeof(Icon));
	icon->url = cJSON_GetObjectItem(icon_entry, "icon")->valuestring;

	return icon;
}

int json_screenshot_list(cJSON *root, Screenshot **screenshots)
{
	cJSON *data = NULL;
	json_data(root, &data);

	int sc_s = cJSON_GetArraySize(data);

	*screenshots = malloc(sc_s * sizeof(Screenshot));

	int i = 0;
	for (i = 0; i < sc_s; i++) {
		cJSON *sc_entry = cJSON_GetArrayItem(data, i);
		json_screenshot_entry(sc_entry, &(*screenshots)[i]);
	}

	return sc_s;
}

Screenshot *json_screenshot(cJSON *root)
{
	Screenshot *screenshot = malloc(sizeof(Screenshot));
	cJSON *sc_entry = NULL;
	json_data(root, &sc_entry);

	json_screenshot_entry(sc_entry, screenshot);

	return screenshot;
}

int json_screenshot_entry(cJSON *sc_entry, Screenshot *screenshot)
{
	screenshot->UUID = cJSON_GetObjectItem(sc_entry, "UUID")->valuestring;
	screenshot->description = cJSON_GetObjectItem(sc_entry, "description")->valuestring;
	screenshot->url = cJSON_GetObjectItem(sc_entry, "download")->valuestring;

	return 0;
}
