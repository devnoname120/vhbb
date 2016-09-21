#include "api.h"

// Get a string from a URL using GET
char *json_get(const char *url)
{
	// FIXME: don't init / deinit on each call
	netInit();
	httpInit();

	int tpl 	= sceHttpCreateTemplate("Vita HomeBrew Browser", 1, 1);

	int conn 	= sceHttpCreateConnectionWithURL(tpl, url, 0 );
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url, 0 );
	int handle 	= sceHttpSendRequest(request, NULL, 0 );

	int buf_size = 1000 * sizeof(char);
	char *buffer = malloc(buf_size);

	int read = 0;
	int pos = 0;
	// read data until finished
	while ((read = sceHttpReadData(request, &buffer[pos], buf_size - pos)) > 0 )
	{
		pos += read;
		buf_size += 1000 * sizeof(char);
		buffer = realloc(buffer, buf_size);
	}

	httpTerm();
	netTerm();

	// Shrink the buffer
	buffer = realloc(buffer, pos);

	return buffer;
}

// Get the basic list of homebrews and put it in a list of structs Homebrew and put the pointer into *homebrews
int get_homebrew_list(Homebrew **homebrews)
{
	char *json = json_get(API_BASE "homebrews.json");

	logcat_add("Buffer: ", json, "\n");

	cJSON *root;
	int ret = json_open(json, &root);

	int code;
	char *message;
	ret = check_status(root, &code, &message);

	if (code == 200) {
		logcat_add("Status 200: ", message, "\n");
	}

	return get_homebrew_entries(root, homebrews);
}