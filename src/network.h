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
 
 
// HTTP ------------------------------------------------
void netInit()
	{
	sceSysmoduleLoadModule( SCE_SYSMODULE_NET );
	SceNetInitParam netInitParam;
	int size 				= 1*1024*1024;
	netInitParam.memory 	= malloc( size );
	netInitParam.size 		= size;
	netInitParam.flags 		= 0;
	sceNetInit( &netInitParam );
	sceNetCtlInit();
	}
void netTerm()
	{
	sceNetCtlTerm();
	sceNetTerm();
	sceSysmoduleUnloadModule( SCE_SYSMODULE_NET );
	}
void httpInit()
	{
	sceSysmoduleLoadModule( SCE_SYSMODULE_HTTP );
	sceHttpInit( 1*1024*1024 );
	}
void httpTerm()
	{
	sceHttpTerm();
	sceSysmoduleUnloadModule( SCE_SYSMODULE_HTTP );
	}

void download( const char *url, const char *dest )
	{
	netInit();
	httpInit();
	// Create template with user agend "PS Vita Sample App"
	int tpl 	= sceHttpCreateTemplate( "Vita HomeBrew Browser", 1, 1 );
	// set url on the template
	int conn 	= sceHttpCreateConnectionWithURL( tpl, url, 0 );
	// create the request with the correct method
	int request = sceHttpCreateRequestWithURL( conn, SCE_HTTP_METHOD_GET, url, 0 );
	// send the actual request. Second parameter would be POST data, third would be length of it.
	int handle 	= sceHttpSendRequest( request, NULL, 0 );
	// open destination file
	int fh 		= sceIoOpen( dest, SCE_O_WRONLY | SCE_O_CREAT, 0777 );
	// create buffer and counter for read bytes.
	unsigned char data[25600] = { 0 }; // test with [16*1024]
	int read 	= 0;
	// read data until finished
	while ( ( read = sceHttpReadData( request, &data, sizeof( data ) ) ) > 0 )
		{
		// writing the count of read bytes from the data buffer to the file
		int write = sceIoWrite( fh, data, read );
		}
	// close file
	sceIoClose( fh );
	httpTerm();
	netTerm();
	}