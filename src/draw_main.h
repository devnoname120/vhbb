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


// DRAW EVENT -------------------------------------------
vita2d_start_drawing();
vita2d_clear_screen();

// BACKGROUND
vita2d_draw_texture( img_bg, originX, 0 );
	
// SETTINGS
if ( settingsDraw )
	{
	vita2d_draw_texture( img_settings_bg, (originX -207), 81 );
	
	if ( btnState_settingsSound 	== 0 ) 	{ vita2d_draw_texture( img_settings_snd,    		(originX -167), (settingsPosY +110) ); }
	else						   			{ vita2d_draw_texture( img_settings_snd_pressed, 	(originX -167), (settingsPosY +110) ); }
	if ( btnState_settingsCtrls		== 0 ) 	{ vita2d_draw_texture( img_settings_ctrl,  			(originX -167), (settingsPosY +245) ); }
	else						   			{ vita2d_draw_texture( img_settings_ctrl_pressed,	(originX -167), (settingsPosY +245) ); }
	if ( btnState_settingsContact  	== 0 ) 	{ vita2d_draw_texture( img_settings_cont,  			(originX -167), (settingsPosY +380) ); }
	else						   			{ vita2d_draw_texture( img_settings_cont_pressed,  	(originX -167), (settingsPosY +380) ); }
	if ( btnState_settingsAbout 	== 0 ) 	{ vita2d_draw_texture( img_settings_about,  		(originX -167), (settingsPosY +515) ); }
	else						   			{ vita2d_draw_texture( img_settings_about_pressed,	(originX -167), (settingsPosY +515) ); }
	
	vita2d_draw_texture( img_settings_topbar, 	(originX -207),  0 );
	}



if ( previewActive )
	{
	// INFORMATION BACKGROUND
	vita2d_draw_texture( img_preview_infobg, originX, 276 );
	vita2d_font_draw_text( font_myriadpro, (originX +203), 140, COLOUR_WHITE, 40, previewName    ); // NAME
	vita2d_font_draw_text( font_segoeui, (originX +203), 166, COLOUR_AQUA,  26, previewAuthor  ); // AUTHOR
	vita2d_font_draw_text( font_segoeui, (originX +203), 196, COLOUR_WHITE, 26, previewVersion ); // VERSION
	vita2d_font_draw_text( font_segoeui, (originX +100), 219, COLOUR_WHITE, 20, previewDlSize  ); // DOWNLOAD SIZE
	vita2d_font_draw_text( font_segoeui, (originX +850), 533, COLOUR_WHITE, 20, previewRelease ); // RELEASE DATE
	vita2d_font_draw_textf(font_myriadpro, (originX +40), (349 +previewDescriptionHeight), COLOUR_WHITE, 26, "%s\n%s\n%s\n%s\n%s", previewDescription, previewDesLine2, previewDesLine3, previewDesLine4, previewDesLine5 );
	
	// DOWNLOAD/INSTALL/UPDATE BUTTON
	
			/**** 	
			
			
			
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
		case 0:		vita2d_draw_texture( iconListNew[previewListNumber], 		(originX +100), 112 ); vita2d_draw_texture( previewListNew[previewListNumber], 		    (originX +566), 97 ); break;
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
							//logcat_add_real( " POSA", i, "\n" );
							vita2d_draw_texture( img_item_panel_new, (originX +62), (originY +115 +(i *90)) );
							//logcat_add_real( " POSB", i, "\n" );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							//logcat_add_real( " POSC", i, "\n" );
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListNew[i].name    	);
							//logcat_add_real( " POSD", i, "\n" );
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].author  	);
							//logcat_add_real( " POSE", i, "\n" );
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].version 	);
							//logcat_add_real( " POSF", i, "\n" );
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].release 	);
							//logcat_add_real( " POSG", i, "\n" );
							
							vita2d_draw_texture( iconListNew[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							//logcat_add_real( " POSH", i, "\n" );
							/*
							 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
							 * 	application crash on icon draw after returning from download.
							 *	see
							 *
							 *
							 */
							}
						break;
		case 1:			// APPS
						ground = 145 +(itemCount_apps *90);
						for ( i = 0; i < itemCount_apps; i++ )
							{
							vita2d_draw_texture( img_item_panel_apps, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListApps[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].release 	);
							vita2d_draw_texture( iconListApps[i],  (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 2:			// GAMES
						ground = 145 +(itemCount_games *90);
						for ( i = 0; i < itemCount_games; i++ )
							{
							vita2d_draw_texture( img_item_panel_games, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListGames[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].release 	);
							vita2d_draw_texture( iconListGames[i], (originX +62 +3), 	(originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 3:			// EMULOATORS
						ground = 145 +(itemCount_emu *90);
						for ( i = 0; i < itemCount_emu; i++ )
							{
							vita2d_draw_texture( img_item_panel_emu, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListEmulators[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].release 	);
							vita2d_draw_texture( iconListEmulators[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 4:			// UTILITIES
						ground = 145 +(itemCount_util *90);
						for ( i = 0; i < itemCount_util; i++ )
							{
							vita2d_draw_texture( img_item_panel_util, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListUtilities[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].release 	);
							vita2d_draw_texture( iconListUtilities[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 5:			// THEMES
						ground = 145 +(itemCount_themes *90);
						for ( i = 0; i < itemCount_themes; i++ )
							{
							vita2d_draw_texture( img_item_panel_themes, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListThemes[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].release 	);
							vita2d_draw_texture( iconListThemes[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 6:			// DEMOS
						ground = 145 +(itemCount_demos *90);
						for ( i = 0; i < itemCount_demos; i++ )
							{
							vita2d_draw_texture( img_item_panel_demos, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 120 ) ); }
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListDemos[i].name    	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].author  	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].version 	);
							vita2d_font_draw_text( font_segoeui, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].release 	);
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


// STATUS BAR
	// Title
	vita2d_font_draw_text( font_segoeui, 15, 25, COLOUR_WHITE, 25, "Vita HomeBrew Browser" );

	// BATTERY
	float battery_x = ALIGN_LEFT( 949, vita2d_texture_get_width(img_statsbar_battery) );
	vita2d_draw_texture( img_statsbar_battery, battery_x, 5 );
	float percent 	= scePowerGetBatteryLifePercent() / 100.0f;
	float width 	= ((29 * percent) / 100);
	if ( scePowerIsLowBattery() )
		{
		vita2d_draw_rectangle( 938, 9, (width *-1), 14, RGBA8( 255, 48, 48, 255 ) );
		}
	else
		{
		vita2d_draw_rectangle( 938, 9, (width *-1), 14, RGBA8( 91, 223, 38, 255 ) );
		}

	// DATE & TIME
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime( &time );
	char date_string[16];
	getDateString( date_string, date_format, &time );
	char time_string[24];
	getTimeString( time_string, time_format, &time );
	char string[64];
	sprintf( string, "%s  %s", date_string, time_string );
	vita2d_font_draw_text( font_myriadpro, 670, 25, COLOUR_WHITE, 25, string );
	
// INSTALL DIALOG
if ( dialogInstall )
	{
	loadTimer++;
	if ( loadTimer > 30 )  { loadAnim++; loadTimer = 0; }
	if ( loadAnim >   4 )  { loadAnim = 0;}
	if ( dialogInstallAlpha < 210 ) { dialogInstallAlpha += 15;  }
	else 							{ dialogInstallAlpha  = 210; }
	vita2d_draw_rectangle( 0, 0, 960, 544, RGBA8( 0, 0, 0, dialogInstallAlpha ) );
	switch ( loadAnim )
		{
		case 0:		vita2d_font_draw_textf( font_myriadpro, 434, 285, RGBA8( 255, 255, 255, dialogInstallAlpha ), 25, "%s%s", previewName, "\nInstalling" ); break;
		case 1:		vita2d_font_draw_textf( font_myriadpro, 434, 285, RGBA8( 255, 255, 255, dialogInstallAlpha ), 25, "%s%s", previewName, "\nInstalling." ); break;
		case 2: 	vita2d_font_draw_textf( font_myriadpro, 434, 285, RGBA8( 255, 255, 255, dialogInstallAlpha ), 25, "%s%s", previewName, "\nInstalling.." ); break;
		case 3:		vita2d_font_draw_textf( font_myriadpro, 434, 285, RGBA8( 255, 255, 255, dialogInstallAlpha ), 25, "%s%s", previewName, "\nInstalling..." ); break;
		case 4: 	vita2d_font_draw_textf( font_myriadpro, 434, 285, RGBA8( 255, 255, 255, dialogInstallAlpha ), 25, "%s%s", previewName, "\nInstalling...." ); break;
		}
	}


// KEYBOARD
if ( show_input == 1 )
	{
	// UPDATE TEXT
	//vita2d_clear_screen();
	SceCommonDialogStatus status = sceImeDialogGetStatus();
	if ( status == IME_DIALOG_RESULT_FINISHED )
		{
		SceImeDialogResult result;
		memset( &result, 0, sizeof(SceImeDialogResult) );
		sceImeDialogGetResult(&result);
		if ( result.button == SCE_IME_DIALOG_BUTTON_CLOSE )
			{
			// USER CANCELED
			status = IME_DIALOG_RESULT_CANCELED;
			}
		else
			{
			// SUCCESSFULL, SET NAME
			oslOskGetText( userSearch );
			// PERFORM SEARCH PROCEDURE
			
			
			
			
			
			}
		sceImeDialogTerm();
		show_input		= 0;
		}
	}

// DEBUG --------------------------------------------------------------------
/*char debug_info[30];

sprintf( debug_info, "abs(originY) = %d", abs(originY) );
vita2d_font_draw_text( font_segoeui, 10, 130, COLOUR_YELLOW, 30, debug_info );
sprintf( debug_info, "(itemPressed *90) = %d", (itemPressed *90));
vita2d_font_draw_text( font_segoeui, 10, 160, COLOUR_YELLOW, 30, debug_info );
sprintf( debug_info, "length = %d", length);
vita2d_font_draw_text( font_segoeui, 10, 200, COLOUR_YELLOW, 30, debug_info );
sprintf( debug_info, "length - abs(originY) = %d", (length - abs(originY)));
vita2d_font_draw_text( font_segoeui, 10, 230, COLOUR_YELLOW, 30, debug_info );
sprintf( debug_info, "dif = %d", dif);
vita2d_font_draw_text( font_segoeui, 10, 290, COLOUR_YELLOW, 30, debug_info );


sprintf( debug_info, "touch_x = %d", touch_x);
vita2d_font_draw_text( font_segoeui, 10, 200, COLOUR_WHITE, 30, debug_info );
sprintf( debug_info, "touch_y = %d", touch_y);
vita2d_font_draw_text( font_segoeui, 10, 230, COLOUR_WHITE, 30, debug_info );
sprintf( debug_info, "clickable = %d", clickable);
vita2d_font_draw_text( font_segoeui, 10, 290, COLOUR_WHITE, 30, debug_info );

sprintf( debug_info, "touch_check = %d", touch_check);
vita2d_font_draw_text( font_segoeui, 10, 320, COLOUR_WHITE, 30, debug_info );
sprintf( debug_info, "touch_check_pressed = %d", touch_check_pressed);
vita2d_font_draw_text( font_segoeui, 10, 350, COLOUR_WHITE, 30, debug_info );
sprintf( debug_info, "touch_check_released = %d", touch_check_released);
vita2d_font_draw_text( font_segoeui, 10, 380, COLOUR_WHITE, 30, debug_info );

sprintf( debug_info, "originY = %d", originY);
vita2d_font_draw_text( font_segoeui, 10, 440, COLOUR_WHITE, 30, debug_info );

	*/

vita2d_end_drawing();
vita2d_common_dialog_update();
vita2d_swap_buffers();
sceDisplayWaitVblankStart();