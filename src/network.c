/*
 * 	Copyright (c) 2016 Arkanite
 *
 *	 	-A Native HomeBrew Browser for the Playstation Vita-
 *
 *	This program is free software:
 *		you can redistribute it and/or modify it under the terms of the
 *		GNU General Public License as published by the Free Software Foundation,
 *		either version 3 of the License, or (at your option) any later version.
 *		
 *		This program comes WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		You should have received a copy of the GNU General Public License
 *		along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include <psp2/sysmodule.h>

#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>

#include <stdlib.h>

#include "network.h"

int net_init;
int tpl;

void network_init()
{
	if (net_init) return 0;

	netInit();
	httpInit();

	tpl = sceHttpCreateTemplate("Vita HomeBrew Browser", 1, 1);

	net_init = 1;
}

void network_term()
{
	if (!net_init) return 0;

	sceHttpDeleteTemplate(tpl);

	netTerm();
	httpTerm();

	net_init = 0;
}


// HTTP ------------------------------------------------
void netInit()
{
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	SceNetInitParam netInitParam;
	int size 				= 1*1024*1024;
	netInitParam.memory 	= malloc(size);
	netInitParam.size 		= size;
	netInitParam.flags 		= 0;
	sceNetInit(&netInitParam);
	sceNetCtlInit();
}

void netTerm()
{
	sceNetCtlTerm();
	sceNetTerm();
	sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
}

void httpInit()
{
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);
	sceHttpInit(1*1024*1024);
}

void httpTerm()
{
	sceHttpTerm();
	sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
}


void network_download(const char *url, const char *dest)
{
	network_init();

	int conn 	= sceHttpCreateConnectionWithURL(tpl, url, 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url, 0);
	int handle 	= sceHttpSendRequest(request, NULL, 0);

	int fh 		= sceIoOpen(dest, SCE_O_WRONLY | SCE_O_CREAT, 0777);

	unsigned char data[25600] = {0}; // test with [16*1024]
	int read = 0;

	while ((read = sceHttpReadData(request, &data, sizeof( data ) ) ) > 0)
	{
		int write = sceIoWrite(fh, data, read);
	}

	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(conn);

	sceIoClose(fh);
}


char *network_get(const char *url)
{
	network_init();

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

	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(conn);

	// Shrink the buffer
	buffer = realloc(buffer, pos);

	return buffer;
}
