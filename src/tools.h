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

// TOOLS ------------------------------------------------

/* |||| STRINGS ||| */
char* string_join(int count, ...)
    {
    va_list ap; int i; int len = 1;
    va_start( ap, count );
    for ( i = 0 ; i < count; i++ ) { len += strlen( va_arg( ap, char* ) ); }
    va_end( ap );
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;
    va_start( ap, count );
    for( i = 0 ; i < count; i++ )
		{
        char *s = va_arg( ap, char* );
        strcpy( merged +null_pos, s );
        null_pos += strlen(s);
		}
    va_end(ap);
    return merged;
    }
void string_remove_newline( char *str ) { str[strcspn(str, "\r\n")] = 0; }


/* ||| DIRECTORIES ||| */
void dir_create( const char *dir )
	{
	char dir_copy[0x400] = {0};
	snprintf( dir_copy, sizeof( dir_copy ) - 2, "%s", dir );
	dir_copy[strlen( dir_copy )] = '/';
	char *c;
	for ( c = dir_copy; *c; ++c )
		{
		if (*c == '/') { *c = '\0'; sceIoMkdir( dir_copy, 0777 ); *c = '/'; }
		}
	}
int dir_exists( char *path )
	{
	DIR  *dip;
	if ( (dip = opendir(path) ) == NULL ) { return 0; }
	else { closedir(dip); return 1;
	}

	
/* ||| FILES ||| */
void file_delete( const char *path )
	{
	int ret = sceIoRemove( path );
	if ( ret < 0 ) { /* error could not find file. */ }
	}
int file_is_png( char *file )
	{
	unsigned char pngHead[8];
	unsigned char pngSignature[8] 	= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	FILE *dat = fopen ( file, "rt" );
    if ( dat == NULL ) { return 1; } // Just return 1 as the file will be downloaded next load
	fread( pngHead, sizeof(pngHead), 1, dat );
	if ( memcmp( pngSignature, pngHead, 8 ) == 0 ) 	{ fclose( dat ); return 1; }
	else 									 		{ fclose( dat ); return 0; }
	}
	
	
/* ||| LOGCAT ||| */
void logcat_add( char *str1, char *str2, char *str3 )
	{
	FILE *lf = fopen( VHBB_APP_FILE_LOGCAT, "a+" );
	if ( lf != NULL )
		{
		fprintf( lf, "%s%s%s", str1, str2, str3 );
		}
	else
		{
		FILE *lf 	= fopen( VHBB_APP_FILE_LOGCAT, "ab+" );
		fprintf( lf, "%s%s%s", str1, str2, str3 );
		}
	fclose( lf );
	}
	

/* ||| POSITION CHECKING ||| */
int point_in_rectangle( int px, int py, int x1, int y1, int x2, int y2 )
	{
	if ( px > x1 && px < x2 && py > y1 && py < y2 ) { return 1; }
	else { return 0; }
	}
	
//-------------------------------------------------------
