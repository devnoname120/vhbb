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
#include "vhbb.h"
#include "boot.h"
#include "tools.h"

int boot()
	{
	// BOOT -------------------------------------------------
	logcat_add( "booting...", "\n", "" );
	
	// CHECK FOR FIRST LOAD
	if ( access( VHBB_APP_FILE_SYSTEMFILE, F_OK ) == -1 )
		{
		// FIRST BOOT
		logcat_add( "-FIRST LOAD-\n", "creating system config file", "\n" );
		FILE *fp 	= fopen( VHBB_APP_FILE_SYSTEMFILE, "ab+" );
		fprintf( fp, "***********************************\n" );
		fprintf( fp, "-Vita HomeBrew Browser-\n" );
		fprintf( fp, " ver. " );
		fprintf( fp, VHBB_VERSION );
		fprintf( fp, "\n Created By\n" );
		fprintf( fp, "      Arkanite\n" );
		fprintf( fp, "***********************************\n\n" );
		fprintf( fp, VHBB_VERSION );
		fprintf( fp, "1" );
		fprintf( fp, "" );
		fprintf( fp, "" );
		fprintf( fp, "" );
		fprintf( fp, "" );
		fprintf( fp, "" );
		fclose( fp );
		logcat_add( "config file created", "\n", "" );
		}
	else
		{
		// LOAD USER SETTINGS
		
		}
	
	// PULL DATABASE
	download( VHBB_CLOUD_FILE_DATABASE, VHBB_APP_FILE_DATABASE );
	
	// BUILD DATABASE
	if ( access( VHBB_APP_FILE_DATABASE, F_OK ) == -1 )
		{
		/// ERROR
		exit(1);
		}
	logcat_add( "Database successfuly downloaded", "", "\n" );
	FILE *fp 	= fopen( VHBB_APP_FILE_DATABASE, "r" );
	char line[1028];
	// CHECK FIRST LINE
	fgets ( line, 1028, fp );
	string_remove_newline( line );
	if ( strstr( line, "<VHBB DATABASE>" ) == NULL )
		{
		// CORRUPT DATABASE FILE
		logcat_add( "database file corrupt!!!", "\n", "" );
		fclose( fp );
		exit(1);
		}
	else
		{
		// GET DATABASE TIMESTAMP
		int c 	= 0;
		fgets ( line, 1028, fp ); string_remove_newline( line ); strcpy( databaseTS, line );
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_apps 	= c; // APPS
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_games 	= c; // GAMES
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_emu 	= c; // EMULATORS
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_util 	= c; // UTILITIES
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_themes	= c; // THEMES
		logcat_add( line, "\n", "" );
		fgets ( line, 1028, fp ); string_remove_newline( line ); c = atoi( line ); itemCount_demos 	= c; // DEMOS
		logcat_add( line, "\n", "" );
		
		// BEGIN ITEM SCANNING
		int posNew, posApp, posGames, posEmu, posUtil, posTheme, posDemo, catIndex, ebootSize;
		posNew 				= posApp = posGames = posEmu = posUtil = posTheme = posDemo = 0;
		char dataCat[10]; 		char dataName[30]; 	char dataVer[30]; 	char dataAuth[30]; 		char dataDls[30];
		char dataRel[30]; 		char dataDes[500]; 	char dataLink[500]; char dataInstall[200];  char dataEboot[30];
		char *targetFileCloud; 	char *targetFileStroage;
		while ( fgets ( line, 1028, fp ) != NULL )
			{
			string_remove_newline( line ); strcpy( dataCat, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataName, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataVer, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataAuth, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataDls, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataRel, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataDes, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataLink, 	line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataInstall, line  ); 	fgets ( line, 1028, fp );
			string_remove_newline( line ); strcpy( dataEboot,   line  ); 	catIndex = atoi( dataCat );
			ebootSize	= atoi( dataEboot );
			// ADD TO CATEGORY DATA LIST
			switch ( catIndex )
				{
				case 1:		// APPS
							catListApps[posApp].cat 		= catIndex;
							strcpy( catListApps[posApp].name, 			dataName   	);
							strcpy( catListApps[posApp].version, 		dataVer 	);
							strcpy( catListApps[posApp].author, 		dataAuth 	);
							strcpy( catListApps[posApp].dlSize, 		dataDls  	);
							strcpy( catListApps[posApp].release, 		dataRel 	);
							strcpy( catListApps[posApp].description, 	dataDes 	);
							strcpy( catListApps[posApp].link, 			dataLink 	);
							strcpy( catListApps[posApp].dir, 			dataEboot 	);
							catListApps[posApp].eboot 		= ebootSize;
							targetFileStroage		= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListApps[posApp] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListApps[posApp] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage		= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListApps[posApp] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListApps[posApp] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posApp++;
							break;
				case 2:		// GAMES
							catListGames[posGames].cat	 	= catIndex;
							strcpy( catListGames[posGames].name, 		dataName   	);
							strcpy( catListGames[posGames].version, 	dataVer 	);
							strcpy( catListGames[posGames].author, 		dataAuth 	);
							strcpy( catListGames[posGames].dlSize, 		dataDls  	);
							strcpy( catListGames[posGames].release, 	dataRel 	);
							strcpy( catListGames[posGames].description, dataDes 	);
							strcpy( catListGames[posGames].link, 		dataLink 	);
							strcpy( catListGames[posGames].dir, 		dataEboot 	);
							catListGames[posGames].eboot 	= ebootSize;
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListGames[posGames] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListGames[posGames] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListGames[posGames] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListGames[posGames] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posGames++;
							break;
				case 3:		// EMULATORS
							catListEmulators[posEmu].cat	= catIndex;
							strcpy( catListEmulators[posEmu].name, 			dataName   	);
							strcpy( catListEmulators[posEmu].version, 		dataVer 	);
							strcpy( catListEmulators[posEmu].author, 	 	dataAuth 	);
							strcpy( catListEmulators[posEmu].dlSize, 		dataDls  	);
							strcpy( catListEmulators[posEmu].release, 		dataRel 	);
							strcpy( catListEmulators[posEmu].description, 	dataDes 	);
							strcpy( catListEmulators[posEmu].link, 			dataLink 	);
							strcpy( catListEmulators[posEmu].dir, 			dataEboot 	);
							catListEmulators[posEmu].eboot 	= ebootSize;
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListEmulators[posEmu] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListEmulators[posEmu] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListEmulators[posEmu] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListEmulators[posEmu] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posEmu++;
							break;
				case 4:		// UTILITIES
							catListUtilities[posUtil].cat 	= catIndex;
							strcpy( catListUtilities[posUtil].name, 		dataName   	);
							strcpy( catListUtilities[posUtil].version, 		dataVer 	);
							strcpy( catListUtilities[posUtil].author, 		dataAuth 	);
							strcpy( catListUtilities[posUtil].dlSize, 		dataDls  	);
							strcpy( catListUtilities[posUtil].release, 		dataRel 	);
							strcpy( catListUtilities[posUtil].description, 	dataDes 	);
							strcpy( catListUtilities[posUtil].link, 		dataLink 	);
							strcpy( catListUtilities[posUtil].dir, 			dataEboot 	);
							catListUtilities[posUtil].eboot = ebootSize;
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListUtilities[posUtil] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListUtilities[posUtil] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListUtilities[posUtil] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListUtilities[posUtil] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posUtil++;
							break;
				case 5:		// THEMES
							catListThemes[posTheme].cat 	= catIndex;
							strcpy( catListThemes[posTheme].name, 			dataName   	);
							strcpy( catListThemes[posTheme].version, 		dataVer 	);
							strcpy( catListThemes[posTheme].author, 		dataAuth 	);
							strcpy( catListThemes[posTheme].dlSize, 		dataDls  	);
							strcpy( catListThemes[posTheme].release, 		dataRel 	);
							strcpy( catListThemes[posTheme].description, 	dataDes 	);
							strcpy( catListThemes[posTheme].link, 			dataLink 	);
							strcpy( catListThemes[posTheme].dir, 			dataEboot 	);
							catListThemes[posTheme].eboot 	= ebootSize;
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListThemes[posTheme] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListThemes[posTheme] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListThemes[posTheme] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListThemes[posTheme] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posTheme++;
							break;
				case 6:		// DEMOS
							catListDemos[posDemo].cat	 	= catIndex;
							strcpy( catListDemos[posDemo].name, 		dataName   	);
							strcpy( catListDemos[posDemo].version, 		dataVer 	);
							strcpy( catListDemos[posDemo].author, 		dataAuth 	);
							strcpy( catListDemos[posDemo].dlSize, 		dataDls  	);
							strcpy( catListDemos[posDemo].release, 		dataRel 	);
							strcpy( catListDemos[posDemo].description, 	dataDes 	);
							strcpy( catListDemos[posDemo].link, 		dataLink 	);
							strcpy( catListDemos[posDemo].dir, 			dataEboot 	);
							catListDemos[posDemo].eboot 	= ebootSize;
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								iconListDemos[posDemo] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								iconListDemos[posDemo] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
								}
							targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
							if ( access( targetFileStroage, F_OK ) == -1 )
								{
								targetFileCloud			= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_prev.png" );
								download( targetFileCloud, targetFileStroage );
								if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
								}
							if ( access( targetFileStroage, F_OK ) != -1 )
								{
								previewListDemos[posDemo] 	= vita2d_load_PNG_file( targetFileStroage );
								}
							else
								{
								previewListDemos[posDemo] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
								}
							posDemo++;
							break;
				}
			// NEW CATEGORY CHECK
			if ( posNew < 10 )
				{
				catListNew[posNew].cat 			= catIndex;
				strcpy( catListNew[posNew].name, 		dataName   	);
				strcpy( catListNew[posNew].version, 	dataVer 	);
				strcpy( catListNew[posNew].author, 		dataAuth 	);
				strcpy( catListNew[posNew].dlSize, 		dataDls  	);
				strcpy( catListNew[posNew].release, 	dataRel 	);
				strcpy( catListNew[posNew].description, dataDes 	);
				strcpy( catListNew[posNew].link, 		dataLink 	);
				strcpy( catListNew[posNew].dir, 	 	dataEboot 	);
				catListNew[posNew].eboot 		= ebootSize;
				targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_ICONS, dataName, "_icon.png" );
				if ( access( targetFileStroage, F_OK ) == -1 )
					{
					targetFileCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_ASSETS, dataName, "_icon.png" );
					download( targetFileCloud, targetFileStroage );
					if ( file_is_png( targetFileStroage ) == 0 )	{ remove( targetFileStroage ); }
					}
				if ( access( targetFileStroage, F_OK ) != -1 )
					{
					iconListNew[posNew] 	= vita2d_load_PNG_file( targetFileStroage );
					}
				else
					{
					iconListNew[posNew] 	= vita2d_load_PNG_buffer( &_binary_assets_img_icon_default_png_start );
					}
				targetFileStroage	= string_join( 3, VHBB_APP_ADDRESS_STORAGE_PREVS, dataName, "_prev.png" );
				if ( access( targetFileStroage, F_OK ) != -1 )
					{
					previewListNew[posNew] 	= vita2d_load_PNG_file( targetFileStroage );
					}
				else
					{
					previewListNew[posNew] 	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_default_png_start );
					}
				posNew++;
				itemCount_new++;
				}
			}
		}
	fclose( fp );
	//-------------------------------------------------------
	//-------------------------------------------------------
	logcat_add( "Starting main loop\n", "=======================================", "\n" );
	}