#include <stdio.h>

#include <cJSON.h>

#include "../network.h"
#include "json.h"

#include "api.h"

// Get the basic list of homebrews and put it in a list of structs Homebrew and put the pointer into *homebrews
int api_homebrew_list(Homebrew **homebrews, char *search, char *sort, char* filter)
{
	char *api_url = malloc(100 * sizeof(char));
	sprintf(api_url, "%s.json?search=%s&sort=%s&filter=", API_HOMEBREWS, search, sort, filter);
	logcat_add(api_url, "", "\n");

	char *json = network_get(api_url);
	free(api_url);

	logcat_add("Buffer: ", json, "\n");

	cJSON *root;
	int ret = json_open(json, &root);

	int code;
	char *message;
	ret = json_status(root, &code, &message);

	if (code == 200) {
		logcat_add("Status 200: ", message, "\n");
	}

	ret = json_homebrew_list(root, homebrews);
	logcat_add("api UUID: ", homebrews[0]->UUID, "\n");
	// FIXME: Closing the root invalidates our strings
	//json_close(root);
	logcat_add("api UUID: ", homebrews[0]->UUID, "\n");
	return ret;
}

Homebrew *api_homebrew(char *id)
{
	char *api_url = malloc(100 * sizeof(char));
	sprintf(api_url, "%s/%s.json", API_HOMEBREWS, id);

	char *json = network_get(api_url);
	free(api_url);

	cJSON *root;
	int ret = json_open(json, &root);

	int code;
	char *message;
	ret = json_status(root, &code, &message);

	Homebrew *homebrew = json_homebrew(root);
	// FIXME: Closing the root invalidates our strings
	//json_close(root);

	return homebrew;
}

Icon *api_icon(char *hb_id)
{
	char *api_url = malloc(100 * sizeof(char));
	sprintf(api_url, API_ICON ".json", hb_id);

	char *json = network_get(api_url);
	free(api_url);

	cJSON *root;
	int ret = json_open(json, &root);

	int code;
	char *message;
	ret = json_status(root, &code, &message);

	Icon *icon = json_icon(root);
	// FIXME: Closing the root invalidates our strings
	//json_close(root);

	return icon;
}

int api_screenshot_list(Screenshot **screenshots, char *hb_id)
{
	char *api_url = malloc(100 * sizeof(char));
	sprintf(api_url, API_SCREENSHOTS ".json", hb_id);

	char *json = network_get(api_url);
	free(api_url);

	cJSON *root;
	int ret = json_open(json, &root);

	int code;
	char *message;
	ret = json_status(root, &code, &message);

	ret = json_screenshot_list(root, screenshots);

	// FIXME: Closing the root invalidates our strings
	//json_close(root);

	return ret;
}
