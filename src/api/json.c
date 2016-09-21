#include "json.h"

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
int check_status(cJSON *root, int *code, char **message)
{
    cJSON *status = cJSON_GetObjectItem(root, "status");
    if (status == NULL) return -1;

    *message = cJSON_GetObjectItem(status, "message")->valuestring;

    // FIXME: the API returns a string when it should be a number
    *code = atoi(cJSON_GetObjectItem(status, "code")->valuestring);

    return 0;
}

// Get data node
int get_data(cJSON *root, cJSON **data)
{
    *data = cJSON_GetObjectItem(root, "data");
    if (data == NULL) return -1;

    return 0;
}

// Create a list of Homebrew structs, fill them, and put the list pointer into *homebrews
int get_homebrew_entries(cJSON *root, Homebrew **homebrews)
{
    cJSON *data = NULL;
    get_data(root, &data);

    int hb_s = cJSON_GetArraySize(data);

    *homebrews = malloc(hb_s * sizeof(Homebrew));

    int i = 0;
    for (i=0; i < hb_s; i++) {
        cJSON *hb_entry = cJSON_GetArrayItem(data, i);
        homebrew_entry(hb_entry, &(*homebrews)[i]);
    }

    return hb_s;
}

// Fill the structure *homebrew from a hb_entry node
int homebrew_entry(cJSON *hb_entry, Homebrew *homebrew)
{
    homebrew->UUID = cJSON_GetObjectItem(hb_entry, "UUID")->valuestring;
    homebrew->name = cJSON_GetObjectItem(hb_entry, "name")->valuestring;
    homebrew->developer = cJSON_GetObjectItem(hb_entry, "developer")->valuestring;
    homebrew->type = cJSON_GetObjectItem(hb_entry, "type")->valuestring;
    homebrew->first_release = cJSON_GetObjectItem(hb_entry, "first_release")->valuestring;
    homebrew->description = cJSON_GetObjectItem(hb_entry, "description")->valuestring;

    return 0;
}