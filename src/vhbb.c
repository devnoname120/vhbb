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
 

// LIBRARIES
#include "vhbb.h"
// DEFINE MACROS
#include "macros.h"
// INCLUDE NETWORK
#include "network.h"
// INCLUDE TOOLS
#include "tools.h"
// INITIALIZE VARIABLES
#include "init_var.h"


// MAIN -------------------------------------------------
//-------------------------------------------------------

int main()
	{
	// CREATE DIRECTORIES
	dir_create( "ux0:/data/VitaHbBrowser/sys/" 		 );
	dir_create( "ux0:/data/VitaHbBrowser/sys/vers/"  );
	dir_create( "ux0:/data/VitaHbBrowser/dta/icons/" );
	dir_create( "ux0:/data/VitaHbBrowser/dta/prevs/" );
	dir_create( "ux0:/data/VitaHbBrowser/dta/files/" );
	
	// INITIALIZE VITA2D
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x40, 0x40, 0x40, 0xFF ) );
	
	// LOAD DEFAULT FONT
	vita2d_font *font_default 						= vita2d_load_font_mem( fontDefault, fontDefault_size );
	
	// SET TOUCH SAMPLING
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, 1 );
	sceTouchSetSamplingState( SCE_TOUCH_PORT_BACK,  1 );
	
	// INITIALIZE SPRITE POINTERS
	#include "init_sprites.h"
	
	memset( &pad, 0, sizeof( pad ) );
	
	// SHOW SPLASH BACKGROUND
	vita2d_start_drawing();
	vita2d_clear_screen();
	vita2d_draw_texture( img_splash, 0, 0 );
	vita2d_end_drawing();
	vita2d_swap_buffers();
	
	// BOOT -------------------------------------------------
	logcat_add( "booting...", "\n", "" );
	
	// CHECK FOR FIRST LOAD
	if ( access( VHBB_APP_FILE_SYSTEMFILE, F_OK ) == -1 )
		{
		// FIRST BOOT
		logcat_add( "-FIRST LOAD-\n", "creating system config file", "\n" );
		FILE *fp 	= fopen( VHBB_APP_FILE_SYSTEMFILE, "ab+" );
		fprintf( fp, "***********************************\n" );
		fprintf( fp, "-Vita HomeBrew Browser-" );
		fprintf( fp, " ver. " );
		fprintf( fp, VHBB_VERSION );
		fprintf( fp, "\n Created By\n" );
		fprintf( fp, "      Arkanite\n" );
		fprintf( fp, "***********************************\n\n" );
		fprintf( fp, VHBB_VERSION );
		fclose( fp );
		logcat_add( "config file created", "\n", "" );
		}
	else
		{
		// LOAD USER SETTINGS
		
		}
	#include "database_load.h"
	
//////// MAIN LOOP
	int i;
	int dPadTimer	= 0;
	#include "vpkDownload.h"
	while ( 1 )
		{
		sceKernelPowerTick(0);
		////////////////////////////////// controls
		#include "ctrls.h"
		// STEP EVENT
		/// touch CHECKS -----------------------------------
		sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch, 1 );
		if ( touch.reportNum > 0 )
			{
			touch_check 				= 1;
			touch_x = (lerp(touch.report[0].x, 1919, 960));
			touch_y = (lerp(touch.report[0].y, 1087, 544));
			touch_check_released_prev 	= 0;
			touch_check_released 		= 0;
			if ( touch_check_pressed_prev == 0 ) { touch_check_pressed_prev = 1; touch_check_pressed = 1; }
			else 								 { touch_check_pressed = 0; }
			}
		else
			{
			touch_check 				= 0;
			touch_check_pressed 		= 0;
			touch_check_pressed_prev 	= 0;
			if ( touch_check_released_prev == 0 ) { touch_check_released_prev = 1; touch_check_released = 1; }
			else 								  { touch_check_released = 0; }
			btnState_search = 0;
			}
		///-------------------------------------------------
		
		// SETTINGS
		if ( settingsOpen == 1 )
			{
			settingsDraw		= 1;
			if ( originX < 192 ) { originX += 15; }
			else 				  { originX = 207; }
			}
		else
			{
			if ( originX > 15 ) { originX -= 15; }
			else 				 { originX	= 0; settingsDraw = 0; }
			}
		
		
		
		///-------------------------------------------------
		
		// SCROLL
		if ( momentum < -1 || momentum > 1 )    { clickable = 0; }
		if ( previewActive == 0 && settingsOpen == 0 )
			{
			if ( scroll == 1 && ground > 544 && touch_y > 90 )
				{
				// CHECK FOR TOUCH
				if ( touch_check == 1 )
					{
					newy	= 0; newy = originY + ( touch_y - prevy );
					if ( usemomentum == 1 )
						{
						momentum = abs( round(( touch_y - prevy ) /2 ));
						if ( touch_y - prevy > 0 ) 	{ scrolldir = 1; }
						else 						{ scrolldir = 0; }
						}
					if ( newy > 0 ) { newy = 0; }
					if ( newy -544 < (-ground) )  { newy = (-ground) +544; }
					originY = newy; prevy = touch_y;          
					}
				if ( touch_check_released == 1 ) { prevy = 0; scroll = 0; }
				}
			else
				{
				if ( touch_check_pressed == 1 ) { scroll = 1; prevy = touch_y; }
				if ( usemomentum && momentum > 0 )
					{
					newy	= 0;
					if ( scrolldir == 1 )
						{
						newy = originY +momentum;
						if ( newy > 0 ) { newy = 0; momentum = 0; }
						}
					else
						{
						newy    = originY -momentum;
						if ( newy - 544 <= (-ground) ) { newy = (-ground) +544; momentum = 0; }
						}
					originY = newy;
					if ( momentum > 0 ) { momentum -= 0.2; }
					}    
				}
			}
		if ( settingsOpen == 1 )
			{
			if ( scroll == 1 && ground > settingsGround && touch_y > 90 )
				{
				// CHECK FOR TOUCH
				if ( touch_check == 1 && touch_x < 207 )
					{
					newy	= 0; newy = settingsPosY + ( touch_y - prevy );
					if ( usemomentum == 1 )
						{
						momentum = abs( round(( touch_y - prevy ) /2 ));
						if ( touch_y - prevy > 0 ) 	{ scrolldir = 1; }
						else 						{ scrolldir = 0; }
						}
					if ( newy > 0 ) { newy = 0; }
					if ( newy -544 < (-settingsGround) )  { newy = (-settingsGround) +544; }
					settingsPosY = newy; prevy = touch_y;          
					}
				if ( touch_check_released == 1 ) { prevy = 0; scroll = 0; }
				}
			else
				{
				if ( touch_check_pressed == 1 && touch_x < 207 ) { scroll = 1; prevy = touch_y; }
				if ( usemomentum && momentum > 0 )
					{
					newy	= 0;
					if ( scrolldir == 1 )
						{
						newy = settingsPosY +momentum;
						if ( newy > 0 ) { newy = 0; momentum = 0; }
						}
					else
						{
						newy    = settingsPosY -momentum;
						if ( newy - 544 <= (-settingsGround) ) { newy = (-settingsGround) +544; momentum = 0; }
						}
					settingsPosY = newy;
					if ( momentum > 0 ) { momentum -= 0.2; }
					}    
				}
			}
		/// BUTTON CHECK PRESSED
		if ( touch_check_pressed == 1 )
			{
			if ( dialogOpen == 0 )
				{
				if ( settingsOpen == 0 )
					{
					if ( touch_y < 90 )
						{
						// TOPBAR BUTTONS
						if ( point_in_rectangle( touch_x, touch_y,   1, 1,  98, 90 ) == 1 && screen != 0 ) { screen = 0; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y,  99, 1, 204, 90 ) == 1 && screen != 1 ) { screen = 1; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 205, 1, 326, 90 ) == 1 && screen != 2 ) { screen = 2; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 327, 1, 427, 90 ) == 1 && screen != 3 ) { screen = 3; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 428, 1, 552, 90 ) == 1 && screen != 4 ) { screen = 4; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 553, 1, 674, 90 ) == 1 && screen != 5 ) { screen = 5; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 675, 1, 799, 90 ) == 1 && screen != 6 ) { screen = 6; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 800, 1, 960, 90 ) == 1 ) { btnState_search = 1; }
						}
					else
						{
						if ( previewActive == 1 )
							{
							// DOWNLOAD/INSTALL/UPDATE PRESSED
							if ( point_in_rectangle( touch_x, touch_y, 197, 213, 357, 266 ) == 1 )
								{
								btnState_previewAction		= 1;
								}
							}
						}
					}
				else
					{
					if ( settingsPosY >= 207 )
						{
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +110), 60, (settingsPosY +222) ) == 1 ) { btnState_settingsSound   = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +245), 60, (settingsPosY +357) ) == 1 ) { btnState_settingsCtrls  = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +380), 60, (settingsPosY +492) ) == 1 ) { btnState_settingsContact  = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +515), 60, (settingsPosY +627) ) == 1 ) { btnState_settingsAbout = 1; }
						}
					
					
					}
				}
			else
				{
				if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) == 1 )
					{
					btnState_dialogBtn2		= 1;
					}
				}
			}
		
		/// BUTTON CHECK
		if ( touch_check == 1 && previewActive == 0 && settingsDraw == 0 )
			{
			if ( touch_y > 90  && clickable == 1 ) 	{ itemPressed = floor((touch_y +abs(originY)) /itemPanelHeight) -1; }
			else									{ itemPressed = -1; }
			}
		else { /*itemPressed = -1; */ }
			
		/// BUTTON CHECK RELEASE
		if ( touch_check_released == 1 )
			{
			if ( dialogOpen == 0 )
				{
				if ( settingsOpen == 0 )
					{
					if ( previewActive == 0 )
						{
						if ( itemPressed >= 0 && clickable == 1 )
							{
							int previewReady	= 1;
							switch ( screen )
								{
								case 0:			// NEW
												if ( itemPressed < 0 || itemPressed > itemCount_new ) { previewReady = 0; break; }
												previewCategory		= catListNew[itemPressed].cat;
												strcpy( previewName, 		catListNew[itemPressed].name 		);
												strcpy( previewAuthor, 		catListNew[itemPressed].author 		);
												strcpy( previewVersion, 	catListNew[itemPressed].version 	);
												strcpy( previewDlSize, 		catListNew[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListNew[itemPressed].release 	);
												strcpy( previewDescription, catListNew[itemPressed].description );
												strcpy( previewLink, 		catListNew[itemPressed].link 		);
												strcpy( previewDir, 		catListNew[itemPressed].dir 		);
												previewEbootSize		= catListNew[itemPressed].ebootSize;
												break;
								case 1:			// APPS
												if ( itemPressed < 0 || itemPressed > itemCount_apps ) { previewReady = 0; break; }
												previewCategory		= catListApps[itemPressed].cat;
												strcpy( previewName, 		catListApps[itemPressed].name 			);
												strcpy( previewAuthor, 		catListApps[itemPressed].author 		);
												strcpy( previewVersion, 	catListApps[itemPressed].version 		);
												strcpy( previewDlSize, 		catListApps[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListApps[itemPressed].release 		);
												strcpy( previewDescription, catListApps[itemPressed].description 	);
												strcpy( previewLink, 		catListApps[itemPressed].link 			);
												strcpy( previewDir, 		catListApps[itemPressed].dir 			);
												previewEbootSize		= catListApps[itemPressed].ebootSize;
												break;
								case 2:			// GAMES
												if ( itemPressed < 0 || itemPressed > itemCount_games ) { previewReady = 0; break; }
												previewCategory		= catListGames[itemPressed].cat;
												strcpy( previewName, 		catListGames[itemPressed].name 			);
												strcpy( previewAuthor, 		catListGames[itemPressed].author 		);
												strcpy( previewVersion, 	catListGames[itemPressed].version 		);
												strcpy( previewDlSize, 		catListGames[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListGames[itemPressed].release 		);
												strcpy( previewDescription, catListGames[itemPressed].description 	);
												strcpy( previewLink, 		catListGames[itemPressed].link 			);
												strcpy( previewDir, 		catListGames[itemPressed].dir 			);
												previewEbootSize		= catListGames[itemPressed].ebootSize;
												break;
								case 3:			// EMULATORS
												if ( itemPressed < 0 || itemPressed > itemCount_emu ) { previewReady = 0; break; }
												previewCategory		= catListEmulators[itemPressed].cat;
												strcpy( previewName, 		catListEmulators[itemPressed].name 			);
												strcpy( previewAuthor, 		catListEmulators[itemPressed].author 		);
												strcpy( previewVersion, 	catListEmulators[itemPressed].version 		);
												strcpy( previewDlSize, 		catListEmulators[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListEmulators[itemPressed].release 		);
												strcpy( previewDescription, catListEmulators[itemPressed].description 	);
												strcpy( previewLink, 		catListEmulators[itemPressed].link 			);
												strcpy( previewDir, 		catListEmulators[itemPressed].dir 			);
												previewEbootSize		= catListEmulators[itemPressed].ebootSize;
												break;
								case 4:			// UTILITIES
												if ( itemPressed < 0 || itemPressed > itemCount_util ) { previewReady = 0; break; }
												previewCategory		= catListUtilities[itemPressed].cat;
												strcpy( previewName, 		catListUtilities[itemPressed].name 			);
												strcpy( previewAuthor, 		catListUtilities[itemPressed].author 		);
												strcpy( previewVersion, 	catListUtilities[itemPressed].version 		);
												strcpy( previewDlSize, 		catListUtilities[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListUtilities[itemPressed].release 		);
												strcpy( previewDescription, catListUtilities[itemPressed].description 	);
												strcpy( previewLink, 		catListUtilities[itemPressed].link 			);
												strcpy( previewDir, 		catListUtilities[itemPressed].dir 			);
												previewEbootSize		= catListUtilities[itemPressed].ebootSize;
												break;
								case 5:			// THEMES
												if ( itemPressed < 0 || itemPressed > itemCount_themes ) { previewReady = 0; break; }
												previewCategory		= catListThemes[itemPressed].cat;
												strcpy( previewName, 		catListThemes[itemPressed].name 		);
												strcpy( previewAuthor, 		catListThemes[itemPressed].author 		);
												strcpy( previewVersion, 	catListThemes[itemPressed].version 		);
												strcpy( previewDlSize, 		catListThemes[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListThemes[itemPressed].release 		);
												strcpy( previewDescription, catListThemes[itemPressed].description 	);
												strcpy( previewLink, 		catListThemes[itemPressed].link 		);
												strcpy( previewDir, 		catListThemes[itemPressed].dir 			);
												previewEbootSize		= catListThemes[itemPressed].ebootSize;
												break;
								case 6:			// DEMOS
												if ( itemPressed < 0 || itemPressed > itemCount_demos ) { previewReady = 0; break; }
												previewCategory		= catListDemos[itemPressed].cat;
												strcpy( previewName, 		catListDemos[itemPressed].name 			);
												strcpy( previewAuthor, 		catListDemos[itemPressed].author 		);
												strcpy( previewVersion, 	catListDemos[itemPressed].version 		);
												strcpy( previewDlSize, 		catListDemos[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListDemos[itemPressed].release 		);
												strcpy( previewDescription, catListDemos[itemPressed].description 	);
												strcpy( previewLink, 		catListDemos[itemPressed].link 			);
												strcpy( previewDir, 		catListDemos[itemPressed].dir 			);
												previewEbootSize		= catListDemos[itemPressed].ebootSize;
												break;
								}
							if ( previewReady == 1 )
								{
								strtok_r( previewDescription, "|", &previewDesLine2 );
								strtok_r( previewDesLine2, "|", &previewDesLine3 );
								strtok_r( previewDesLine3, "|", &previewDesLine4 );
								strtok_r( previewDesLine4, "|", &previewDesLine5 );
								previewActive		=  1;
								previewListNumber	= itemPressed;
								// CHECK IF INSTALLED
								if ( access( previewDir, F_OK ) == -1 ) { preview_isInstalled = 0; }
								else  									{ preview_isInstalled = 1; }
								// CHECK IF DOWNLOADED
								if ( access( string_join( 3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk" ), F_OK ) == -1 ) { preview_isDownloaded = 0; }
								else 																							   { preview_isDownloaded = 1; }
								// CHECK IF LATEST VERSION
								//preview_isCurrent
								}
							itemPressed 		= -1;
							}
						}
					else
						{
						// DOWNLOAD/INSTALL/UPDATE PRESSED
						if ( point_in_rectangle( touch_x, touch_y, 197, 213, 357, 266 ) == 1 )
							{
							/// CHECK WHICH BUTTON IS SHOWING
								
								// DOWNLOAD
								strcpy( dialogMessage, string_join( 4, "Downloading\n", previewName, "\n", previewRelease ) );
								char *fileVpkCloud;
								char *fileVpkLocal;
								fileVpkCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_FILES, previewName, ".vpk" );
								fileVpkLocal		= string_join( 3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk" );
								
								if ( download_file( fileVpkCloud, fileVpkLocal ) < 0 )
									{
									// DOWNLOAD FAILED
									
									}
								else
									{
									
									}
								
								
								
							}
						if ( point_in_rectangle( touch_x, touch_y, 0, 460, 90, 544 ) == 1 && settingsDraw == 0 )
							{
							previewActive	= 0;
							itemPressed		= previewListNumber;
							}
						}
					}
				else
					{
					/// touch RELEASE FOR SETTINGS HERE
					}
				}
			else
				{
				if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) == 1 )
					{
					dialogOpen		= 0;
					}
				
				
				}
				
			// RESET BUTTON STATES
			btnState_previewAction				= 0;
			btnState_settingsSound			= 0;
			btnState_settingsCtrls			= 0;
			btnState_settingsContact			= 0;
			btnState_settingsAbout			= 0;
			btnState_dialogBtn2				= 0;
			
			clickable = 1;
			}
		//-------------------------------------------------------
		//-------------------------------------------------------
		
		// DRAW EVENT -------------------------------------------
		vita2d_start_drawing();
		vita2d_clear_screen();

			// BACKGROUND
			vita2d_draw_texture( img_bg, originX, 0 );
			
			
			// SETTINGS
			if ( settingsDraw == 1 )
				{
				vita2d_draw_texture( img_settings_bg, 		(originX -207), 81 );
				
				if ( btnState_settingsSound   == 0 ) { vita2d_draw_texture( img_settings_snd,   			(originX -167), (settingsPosY +110) ); }
				else						   { vita2d_draw_texture( img_settings_snd_pressed, 	(originX -167), (settingsPosY +110) ); }
				if ( btnState_settingsCtrls	== 0 ) { vita2d_draw_texture( img_settings_ctrl,  			(originX -167), (settingsPosY +245) ); }
				else						   { vita2d_draw_texture( img_settings_ctrl_pressed,	(originX -167), (settingsPosY +245) ); }
				if ( btnState_settingsContact  == 0 ) { vita2d_draw_texture( img_settings_cont,  			(originX -167), (settingsPosY +380) ); }
				else						   { vita2d_draw_texture( img_settings_cont_pressed,  	(originX -167), (settingsPosY +380) ); }
				if ( btnState_settingsAbout == 0 ) { vita2d_draw_texture( img_settings_about,  			(originX -167), (settingsPosY +515) ); }
				else						   { vita2d_draw_texture( img_settings_about_pressed,   (originX -167), (settingsPosY +515) ); }
				
				vita2d_draw_texture( img_settings_topbar, 	(originX -207),  0 );
				}
			
			
			
			if ( previewActive == 1 )
				{
				// INFORMATION BACKGROUND
				vita2d_draw_texture( img_preview_infobg, originX, 276 );
				vita2d_font_draw_text( font_default, (originX +203), 140, C_WHITE, 40, previewName 	 ); // NAME
				vita2d_font_draw_text( font_default, (originX +203), 166, C_AQUA,  26, previewAuthor  ); // AUTHOR
				vita2d_font_draw_text( font_default, (originX +203), 196, C_WHITE, 26, previewVersion ); // VERSION
				vita2d_font_draw_text( font_default, (originX +100), 219, C_WHITE, 20, previewDlSize  ); // DOWNLOAD SIZE
				vita2d_font_draw_text( font_default, (originX +850), 533, C_WHITE, 20, previewRelease ); // RELEASE DATE
				vita2d_font_draw_textf(font_default, (originX +40), (349 +previewDescriptionHeight), C_WHITE, 26, "%s\n%s\n%s\n%s\n%s", previewDescription, previewDesLine2, previewDesLine3, previewDesLine4, previewDesLine5 );
				
				// DOWNLOAD/INSTALL/UPDATE BUTTON
				
						/**** 	STILL NEED TO CONFIRM A WAY TO CHECK LATEST VERSION
								so far the option is to have files eg name +version +.ver
								to check which version has been downloaded.
								This would only work with vpks installed via this app,
								and it would also need install directory checking on booting
								to determine if apps have been uninstalled since last use.
								******/
				
				
				
				
				
				
				if ( preview_isDownloaded == 1 )
					{
					if ( btnState_previewAction == 0 )
						{
						vita2d_draw_texture( img_preview_btn_install, (originX +201), 217 );
						}
					else
						{
						vita2d_draw_texture( img_preview_btn_install_pressed, (originX +201), 217 );
						}
					}
				else
					{
					if ( btnState_previewAction == 0 )
						{
						vita2d_draw_texture( img_preview_btn_download, (originX +201), 217 );
						}
					else
						{
						vita2d_draw_texture( img_preview_btn_download_pressed, (originX +201), 217 );
						}
					}
				
				// BACK BUTTON
				if ( btnState_previewBack == 0 ) 	{ vita2d_draw_texture( img_preview_btn_back, originX, 470 ); }
				else 								{ vita2d_draw_texture( img_preview_btn_back_pressed, originX, 470 ); }
				switch ( screen )
					{
					case 0:		vita2d_draw_texture( iconListNew[previewListNumber], 		(originX +100), 112 ); vita2d_draw_texture( previewListNew[previewListNumber], 		(originX +566), 97 ); break;
					case 1:		vita2d_draw_texture( iconListApps[previewListNumber], 		(originX +100), 112 ); vita2d_draw_texture( previewListApps[previewListNumber], 		(originX +566), 97 ); break;
					case 2:		vita2d_draw_texture( iconListGames[previewListNumber], 		(originX +100), 112 ); vita2d_draw_texture( previewListGames[previewListNumber], 	 	(originX +566), 97 ); break;
					case 3:		vita2d_draw_texture( iconListEmulators[previewListNumber], 	(originX +100), 112 ); vita2d_draw_texture( previewListEmulators[previewListNumber], 	(originX +566), 97 ); break;
					case 4:		vita2d_draw_texture( iconListUtilities[previewListNumber], 	(originX +100), 112 ); vita2d_draw_texture( previewListUtilities[previewListNumber], 	(originX +566), 97 ); break;
					case 5:		vita2d_draw_texture( iconListThemes[previewListNumber], 	(originX +100), 112 ); vita2d_draw_texture( previewListThemes[previewListNumber], 		(originX +566), 97 ); break;
					case 6:		vita2d_draw_texture( iconListDemos[previewListNumber], 		(originX +100), 112 ); vita2d_draw_texture( previewListDemos[previewListNumber], 		(originX +566), 97 ); break;
					}
				}
			else
				{
				/// ITEMS
				switch ( screen )
					{
					case 0:			// NEW
									ground = 145 +(itemCount_new *90);
									for ( i = 0; i < itemCount_new; i++ )
										{
										vita2d_draw_texture( img_item_panel_new, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListNew[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListNew[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListNew[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListNew[i].release 	);
										vita2d_draw_texture( iconListNew[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 1:			// APPS
									ground = 145 +(itemCount_apps *90);
									for ( i = 0; i < itemCount_apps; i++ )
										{
										vita2d_draw_texture( img_item_panel_apps, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListApps[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListApps[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListApps[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListApps[i].release 	);
										vita2d_draw_texture( iconListApps[i],  (originX +62 +3), 	(originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 2:			// GAMES
									ground = 145 +(itemCount_games *90);
									for ( i = 0; i < itemCount_games; i++ )
										{
										vita2d_draw_texture( img_item_panel_games, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListGames[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListGames[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListGames[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListGames[i].release 	);
										vita2d_draw_texture( iconListGames[i], (originX +62 +3), 	(originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 3:			// EMULOATORS
									ground = 145 +(itemCount_emu *90);
									for ( i = 0; i < itemCount_emu; i++ )
										{
										vita2d_draw_texture( img_item_panel_emu, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListEmulators[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListEmulators[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListEmulators[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListEmulators[i].release 	);
										vita2d_draw_texture( iconListEmulators[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 4:			// UTILITIES
									ground = 145 +(itemCount_util *90);
									for ( i = 0; i < itemCount_util; i++ )
										{
										vita2d_draw_texture( img_item_panel_util, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListUtilities[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListUtilities[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListUtilities[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListUtilities[i].release 	);
										vita2d_draw_texture( iconListUtilities[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 5:			// THEMES
									ground = 145 +(itemCount_themes *90);
									for ( i = 0; i < itemCount_themes; i++ )
										{
										vita2d_draw_texture( img_item_panel_themes, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListThemes[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListThemes[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListThemes[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListThemes[i].release 	);
										vita2d_draw_texture( iconListThemes[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					case 6:			// DEMOS
									ground = 145 +(itemCount_demos *90);
									for ( i = 0; i < itemCount_demos; i++ )
										{
										vita2d_draw_texture( img_item_panel_demos, (originX +62), (originY +115 +(i *90)) );
										if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 35, catListDemos[i].name    	);
										vita2d_font_draw_text( font_default, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListDemos[i].author  	);
										vita2d_font_draw_text( font_default, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), C_WHITE, 25, catListDemos[i].version 	);
										vita2d_font_draw_text( font_default, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), C_WHITE, 25, catListDemos[i].release 	);
										vita2d_draw_texture( iconListDemos[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
										}
									break;
					}
				}
			/// TOPBAR
			switch ( screen )
				{
				case 0:		vita2d_draw_texture( img_topbar0, originX, 0 ); break;
				case 1:		vita2d_draw_texture( img_topbar1, originX, 0 ); break;
				case 2:		vita2d_draw_texture( img_topbar2, originX, 0 ); break;
				case 3:		vita2d_draw_texture( img_topbar3, originX, 0 ); break;
				case 4:		vita2d_draw_texture( img_topbar4, originX, 0 ); break;
				case 5:		vita2d_draw_texture( img_topbar5, originX, 0 ); break;
				case 6:		vita2d_draw_texture( img_topbar6, originX, 0 ); break;
				}
			// SEARCH BUTTON
			vita2d_draw_texture( img_btn_search, (originX +821), 37 );
			if ( btnState_search == 1 ) { vita2d_draw_rectangle( (originX +823), 38, 117, 37, RGBA8( 255, 255, 255, 120 ) ); }
			
			
			
			
		// DEBUG --------------------------------------------------------------------
		/*char debug_info[30];
		
		sprintf( debug_info, "Item Pressed abs = %d", abs(touch_y -originY -115) );
		vita2d_font_draw_text( font_default, 10, 130, C_WHITE, 30, debug_info );
		sprintf( debug_info, "Item Pressed = %d", itemPressed);
		vita2d_font_draw_text( font_default, 10, 160, C_WHITE, 30, debug_info );
		
		
		sprintf( debug_info, "touch_x = %d", touch_x);
		vita2d_font_draw_text( font_default, 10, 200, C_WHITE, 30, debug_info );
		sprintf( debug_info, "touch_y = %d", touch_y);
		vita2d_font_draw_text( font_default, 10, 230, C_WHITE, 30, debug_info );
		sprintf( debug_info, "clickable = %d", clickable);
		vita2d_font_draw_text( font_default, 10, 290, C_WHITE, 30, debug_info );
		
		sprintf( debug_info, "touch_check = %d", touch_check);
		vita2d_font_draw_text( font_default, 10, 320, C_WHITE, 30, debug_info );
		sprintf( debug_info, "touch_check_pressed = %d", touch_check_pressed);
		vita2d_font_draw_text( font_default, 10, 350, C_WHITE, 30, debug_info );
		sprintf( debug_info, "touch_check_released = %d", touch_check_released);
		vita2d_font_draw_text( font_default, 10, 380, C_WHITE, 30, debug_info );
		
		sprintf( debug_info, "originY = %d", originY);
		vita2d_font_draw_text( font_default, 10, 440, C_WHITE, 30, debug_info );
		*/
			
			
		vita2d_end_drawing();
		vita2d_swap_buffers();
		//-------------------------------------------------------
		//-------------------------------------------------------
		
		
		}
		
		
	// RELEASE VITA2D
	vita2d_fini();
	// RELEASE SPRITE ARRAYS
	vita2d_free_texture( img_splash   );
	vita2d_free_texture( img_bg  	  );
	vita2d_free_texture( img_topbar0  );
	vita2d_free_texture( img_topbar1  );
	vita2d_free_texture( img_topbar2  );
	vita2d_free_texture( img_topbar3  );
	vita2d_free_texture( img_topbar4  );
	vita2d_free_texture( img_topbar5  );
	vita2d_free_texture( img_topbar6  );
	vita2d_free_texture( img_btn_search  	  				);
	vita2d_free_texture( img_item_panel_new  	  			);
	vita2d_free_texture( img_item_panel_apps  	  			);
	vita2d_free_texture( img_item_panel_games  	  			);
	vita2d_free_texture( img_item_panel_emu  	  			);
	vita2d_free_texture( img_item_panel_util  	  			);
	vita2d_free_texture( img_item_panel_themes  	 	 	);
	vita2d_free_texture( img_item_panel_demos  	  			);
	vita2d_free_texture( img_preview_btn_download  	  		);
	vita2d_free_texture( img_preview_btn_download_pressed  	);
	vita2d_free_texture( img_preview_infobg  	  			);
	vita2d_free_texture( img_preview_btn_back  	  			);
	vita2d_free_texture( img_preview_btn_back_pressed  	  	);
	for ( i = 0; i < itemCount_new; 	i++ ) { vita2d_free_texture( iconListNew[i] );		 vita2d_free_texture( previewListNew[i] ); 		 }
	for ( i = 0; i < itemCount_apps; 	i++ ) { vita2d_free_texture( iconListApps[i] ); 	 vita2d_free_texture( previewListApps[i] ); 	 }
	for ( i = 0; i < itemCount_games; 	i++ ) { vita2d_free_texture( iconListGames[i] ); 	 vita2d_free_texture( previewListGames[i] ); 	 }
	for ( i = 0; i < itemCount_emu; 	i++ ) { vita2d_free_texture( iconListEmulators[i] ); vita2d_free_texture( previewListEmulators[i] ); }
	for ( i = 0; i < itemCount_util; 	i++ ) { vita2d_free_texture( iconListUtilities[i] ); vita2d_free_texture( previewListUtilities[i] ); }
	for ( i = 0; i < itemCount_themes; 	i++ ) { vita2d_free_texture( iconListThemes[i] ); 	 vita2d_free_texture( previewListThemes[i] ); 	 }
	for ( i = 0; i < itemCount_demos; 	i++ ) { vita2d_free_texture( iconListDemos[i] ); 	 vita2d_free_texture( previewListDemos[i] ); 	 }
	vita2d_free_texture( img_settings_about  	  		);
	vita2d_free_texture( img_settings_about_pressed  	);
	vita2d_free_texture( img_settings_bg  	  			);
	vita2d_free_texture( img_settings_cont  	  		);
	vita2d_free_texture( img_settings_cont_pressed  	);
	vita2d_free_texture( img_settings_ctrl  	  		);
	vita2d_free_texture( img_settings_ctrl_pressed  	);
	vita2d_free_texture( img_settings_snd  	  			);
	vita2d_free_texture( img_settings_snd_pressed  	  	);
	vita2d_free_texture( img_settings_topbar  	  		);
	vita2d_free_texture( img_dialog_progress_bg  	  	);
	vita2d_free_texture( img_dialog_btn_cancel  	  	);
	vita2d_free_texture( img_dialog_btn_cancel_pressed	);
	
	
	// RELEASE FONT
	vita2d_free_font( font_default );
	// SHUTDOWN NETWORK
	
	sceKernelExitProcess(0);
	return 0;
	}