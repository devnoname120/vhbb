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
	vita2d_font_draw_text( font_40, (originX +203), 140, COLOUR_WHITE, 40, previewName    ); // NAME
	vita2d_font_draw_text( font_25, (originX +203), 166, COLOUR_AQUA,  25, previewAuthor  ); // AUTHOR
	vita2d_font_draw_text( font_25, (originX +203), 196, COLOUR_WHITE, 25, previewVersion ); // VERSION
	vita2d_font_draw_text( font_20, (originX +100), 219, COLOUR_WHITE, 20, previewDlSize  ); // DOWNLOAD SIZE
	vita2d_font_draw_text( font_20, (originX +850), 533, COLOUR_WHITE, 20, previewRelease ); // RELEASE DATE
	vita2d_font_draw_textf(font_25, (originX +40), (349 +previewDescriptionHeight), COLOUR_WHITE, 25, "%s\n%s\n%s\n%s\n%s", previewDescription, previewDesLine2, previewDesLine3, previewDesLine4, previewDesLine5 );
	
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
							vita2d_draw_texture( img_item_panel_new, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListNew[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListNew[i].release 	);
							vita2d_draw_texture( iconListNew[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 1:			// APPS
						ground = 145 +(itemCount_apps *90);
						for ( i = 0; i < itemCount_apps; i++ )
							{
							vita2d_draw_texture( img_item_panel_apps, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListApps[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListApps[i].release 	);
							vita2d_draw_texture( iconListApps[i],  (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 2:			// GAMES
						ground = 145 +(itemCount_games *90);
						for ( i = 0; i < itemCount_games; i++ )
							{
							vita2d_draw_texture( img_item_panel_games, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListGames[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListGames[i].release 	);
							vita2d_draw_texture( iconListGames[i], (originX +62 +3), 	(originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 3:			// EMULOATORS
						ground = 145 +(itemCount_emu *90);
						for ( i = 0; i < itemCount_emu; i++ )
							{
							vita2d_draw_texture( img_item_panel_emu, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListEmulators[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListEmulators[i].release 	);
							vita2d_draw_texture( iconListEmulators[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 4:			// UTILITIES
						ground = 145 +(itemCount_util *90);
						for ( i = 0; i < itemCount_util; i++ )
							{
							vita2d_draw_texture( img_item_panel_util, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListUtilities[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListUtilities[i].release 	);
							vita2d_draw_texture( iconListUtilities[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 5:			// THEMES
						ground = 145 +(itemCount_themes *90);
						for ( i = 0; i < itemCount_themes; i++ )
							{
							vita2d_draw_texture( img_item_panel_themes, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListThemes[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListThemes[i].release 	);
							vita2d_draw_texture( iconListThemes[i], (originX +62 +3), (originY +115 +3 +(i *itemPanelHeight)) );
							}
						break;
		case 6:			// DEMOS
						ground = 145 +(itemCount_demos *90);
						for ( i = 0; i < itemCount_demos; i++ )
							{
							vita2d_draw_texture( img_item_panel_demos, (originX +62), (originY +115 +(i *90)) );
							if ( i == itemPressed ) { vita2d_draw_rectangle( (originX +152), (originY +115 +(i *90)), 808, 90, RGBA8( 255, 255, 255, 30 ) ); }
							if ( i == itemPressed ) { vita2d_draw_texture_tint( img_item_panel_highlight, (originX +152), (originY +115 +(i *90)), RGBA8(255, 255, 255, itemHighlight) ); }
							vita2d_font_draw_text( font_35, (originX +62 +120), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 35, catListDemos[i].name    	);
							vita2d_font_draw_text( font_25, (originX +62 +120), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].author  	);
							vita2d_font_draw_text( font_25, (originX +62 +800), (originY +115 +33 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].version 	);
							vita2d_font_draw_text( font_25, (originX +62 +746), (originY +115 +76 +(i *itemPanelHeight)), COLOUR_WHITE, 25, catListDemos[i].release 	);
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
	vita2d_font_draw_text( font_25, 12, 22, COLOUR_WHITE, 25, "Vita HomeBrew Browser" );

	// BATTERY
	float battery_x = ALIGN_LEFT( 949, vita2d_texture_get_width(img_statsbar_battery) );
	vita2d_draw_texture( img_statsbar_battery, battery_x, 2 );
	float percent 	= scePowerGetBatteryLifePercent();
	float width 	= ((29.0f *percent) /100.0f);
	if ( scePowerIsLowBattery() )
		{
		vita2d_draw_rectangle( (947.0f -width), 4, width, 16, RGBA8( 255, 48, 48, 255 ) );
		}
	else
		{
		vita2d_draw_rectangle( (947.0f -width), 4, width, 16, RGBA8( 91, 223, 38, 255 ) );
		}
	/* 
	if ( scePowerIsBatteryCharging() )
		{
		// TODO: add charging Icon
		}
	*/
	
	// DATE & TIME
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime( &time );
	char date_string[16];
	getDateString( date_string, date_format, &time );
	char time_string[24];
	getTimeString( time_string, time_format, &time );
	char string[64];
	sprintf( string, "%s  %s", date_string, time_string );
	vita2d_font_draw_text( font_25, 650, 22, COLOUR_WHITE, 25, string );
	

// INSTALL ANIMATION
if ( install_animate ){
	install_animate_timer++;
	if ( install_animate_timer > 100 ){
		install_animate_scale -= 0.02;
		if ( install_animate_vs  < 0 ){
			install_animate_vs = 0;
		}
		else{
			install_animate_vs -= 1;
		}
		install_animate_x += 10;
		install_animate_y = install_animate_y -install_animate_vs +15;
	}
	if ( install_animate_alpha < 235 ) { install_animate_alpha += 15; }
	else							   { install_animate_alpha = 255; }
	vita2d_draw_rectangle( install_animate_x -3, install_animate_y -3, (84 *install_animate_scale) +6, (84 *install_animate_scale) +6, RGBA8( 255, 255, 255, install_animate_alpha ) );
	
	switch ( screen ){
		case 0:		vita2d_draw_texture_tint_scale( iconListNew[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 1:		vita2d_draw_texture_tint_scale( iconListApps[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 2:		vita2d_draw_texture_tint_scale( iconListGames[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 3:		vita2d_draw_texture_tint_scale( iconListEmulators[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 4:		vita2d_draw_texture_tint_scale( iconListUtilities[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 5:		vita2d_draw_texture_tint_scale( iconListThemes[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
		case 6:		vita2d_draw_texture_tint_scale( iconListDemos[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
	}
	if ( install_animate_x > 960 || install_animate_y > 544 ) { install_animate = 0; install_animate_timer = 0; install_animate_x = 438; install_animate_y = 230; install_animate_alpha = 0; install_animate_scale = 1; install_animate_vs = 30; }
}

// INSTALL QUE
if ( install_que_count ){
	if ( install_que1_alpha < 235 ){
		install_que1_alpha += 15;
	}
	else{
		install_que1_alpha = 255;
		install_que2_alpha_timer++;
		if ( install_que2_alpha_timer > 120 ){
			install_que2_alpha_timer = 121;
			if ( install_que2_alpha < 235 ){
				install_que2_alpha += 50;
			}
			else{
				install_que2_alpha = 255;
				// PROGRESS ANIMATION
				install_que_progress_timer++;
				if ( install_que_progress_timer > 50 ){
					install_que_progress_timer = 0;
					install_que_progress++;
					if ( install_que_progress > 2 ){
						install_que_progress = 0;
					}
				}
			}
		}
	}
}
else {
	if ( install_que2_alpha > 0 ){
		install_que2_alpha -= 15;
	}
	else{
		install_que2_alpha			= 0;
		install_que2_alpha_timer	= 0;
	}
}
vita2d_draw_texture_tint( img_inst_que1, 873, 469, RGBA8( 255, 255, 255, install_que1_alpha ) );
vita2d_draw_texture_tint( img_inst_que2, 873, 469, RGBA8( 255, 255, 255, install_que2_alpha ) );
vita2d_font_draw_textf( font_25, 922, 508, RGBA8( 255, 255, 255, install_que2_alpha ), 25, "%d", install_que_count );
switch ( install_que_progress ){
	case 0:		vita2d_draw_fill_circle( 912, 527, 4, RGBA8( 30, 255, 0, install_que2_alpha ) ); break;
	case 1:		vita2d_draw_fill_circle( 927, 527, 4, RGBA8( 30, 255, 0, install_que2_alpha ) ); break;
	case 2:		vita2d_draw_fill_circle( 942, 527, 4, RGBA8( 30, 255, 0, install_que2_alpha ) ); break;
}


// INSTALL DIALOG
if ( install_dialog_show ){
	install_dialog_timer++;
	if ( install_dialog_alpha < 235 ){
		install_dialog_alpha += 20;
	}
	else{
		install_dialog_alpha = 255;
	}
	if ( install_dialog_timer > 250 ){
		install_dialog_show = 0;
	}
}
else{
	if ( install_dialog_alpha > 35 ) { install_dialog_alpha -= 20; }
	else{
		install_dialog_alpha  =  0;
	}
}
vita2d_draw_texture_tint( img_inst_dialog, 344, 407, RGBA8( 255, 255, 255, install_dialog_alpha ) );
if ( install_que_count <= 0 ){
	vita2d_draw_texture_tint( img_inst_que1, 873, 469, RGBA8( 255, 255, 255, install_dialog_alpha ) );
}
switch ( install_dialog_screen ) {
	case 0:		vita2d_draw_texture_tint_scale( iconListNew[install_dialog_pos], 		371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListNew[install_dialog_pos].name ); break;
	case 1:		vita2d_draw_texture_tint_scale( iconListApps[install_dialog_pos], 		371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListApps[install_dialog_pos].name ); break;
	case 2:		vita2d_draw_texture_tint_scale( iconListGames[install_dialog_pos], 		371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListGames[install_dialog_pos].name ); break;
	case 3:		vita2d_draw_texture_tint_scale( iconListEmulators[install_dialog_pos], 	371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListEmulators[install_dialog_pos].name ); break;
	case 4:		vita2d_draw_texture_tint_scale( iconListUtilities[install_dialog_pos], 	371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListUtilities[install_dialog_pos].name ); break;
	case 5:		vita2d_draw_texture_tint_scale( iconListThemes[install_dialog_pos], 	371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListThemes[install_dialog_pos].name ); break;
	case 6:		vita2d_draw_texture_tint_scale( iconListDemos[install_dialog_pos], 		371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListDemos[install_dialog_pos].name ); break;
}

// INSTALL QUE OPEN
if (install_que_open) {
	if (install_que_posy > 40)	{ install_que_posy -= 40; }
	else						{ install_que_posy  = 32; }
	// BACKGROUND
	vita2d_draw_rectangle( 0, install_que_posy, 960, 512, RGBA8( 37, 40, 45, 255 ) );
	
	int p;
	// QUEUED ITEMS
	ground = 109 +((install_que_count +install_que_complete_count) *90);
	for (i = 0; i < install_que_count; i++) {
		vita2d_draw_texture( img_inst_panel, 62, (install_que_posy +109 +(i *90)) );
		vita2d_font_draw_text( font_25, 808, (install_que_posy +185 +(i *itemPanelHeight)), COLOUR_WHITE, 25, downloadQue[i].status );
		p = downloadQue[i].pos;
		switch (downloadQue[i].screen) {
			case 0: vita2d_draw_texture( iconListNew[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListNew[p].name ); break;
			case 1: vita2d_draw_texture( iconListApps[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListApps[p].name ); break;
			case 2: vita2d_draw_texture( iconListGames[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListGames[p].name ); break;
			case 3: vita2d_draw_texture( iconListEmulators[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListEmulators[p].name ); break;
			case 4: vita2d_draw_texture( iconListUtilities[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListUtilities[p].name ); break;
			case 5: vita2d_draw_texture( iconListThemes[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListThemes[p].name ); break;
			case 6: vita2d_draw_texture( iconListDemos[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListDemos[p].name ); break;
		}
	}
	for ( i = 0; i < install_que_complete_count; i++) {
		vita2d_draw_texture( img_inst_panel, 62, (install_que_posy +109 +(i *90) +(install_que_count *90)) );
		vita2d_font_draw_text( font_25, 808, (install_que_posy +185 +(i *itemPanelHeight) +(install_que_count *90)), COLOUR_WHITE, 25, "completed" );
		p = downloadCompleted[i].pos;
		switch (downloadCompleted[i].screen) {
			case 0: vita2d_draw_texture( iconListNew[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListNew[p].name ); break;
			case 1: vita2d_draw_texture( iconListApps[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListApps[p].name ); break;
			case 2: vita2d_draw_texture( iconListGames[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListGames[p].name ); break;
			case 3: vita2d_draw_texture( iconListEmulators[p], 	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListEmulators[p].name ); break;
			case 4: vita2d_draw_texture( iconListUtilities[p], 	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListUtilities[p].name ); break;
			case 5: vita2d_draw_texture( iconListThemes[p],  	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListThemes[p].name ); break;
			case 6: vita2d_draw_texture( iconListDemos[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListDemos[p].name ); break;
		}
	}
	vita2d_draw_rectangle( 0, install_que_posy, 960, 64, RGBA8( 28, 30, 33, 255 ) );
	vita2d_font_draw_text( font_25, 438, (install_que_posy +40), COLOUR_WHITE, 25, "Install Que" );
	vita2d_draw_texture( img_inst_btn_close, 6, (install_que_posy +5) );
	if (btnState_installBtn) {
		vita2d_draw_rectangle( 10, 41, 44, 46, RGBA8( 255, 255, 255, 200 ) );
	}
}
else {
	if (install_que_posy < 550) {
		install_que_posy += 40;
		vita2d_draw_rectangle( 0, install_que_posy, 960, 512, RGBA8( 37, 40, 45, 255 ) );
		int p;
		for (i = 0; i < install_que_count; i++) {
			vita2d_draw_texture( img_inst_panel, 62, (install_que_posy +109 +(i *90)) );
			vita2d_font_draw_text( font_25, 808, (install_que_posy +185 +(i *itemPanelHeight)), COLOUR_WHITE, 25, downloadQue[i].status );
			p = downloadQue[i].pos;
			switch (downloadQue[i].screen) {
				case 0: vita2d_draw_texture( iconListNew[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListNew[p].name ); break;
				case 1: vita2d_draw_texture( iconListApps[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListApps[p].name ); break;
				case 2: vita2d_draw_texture( iconListGames[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListGames[p].name ); break;
				case 3: vita2d_draw_texture( iconListEmulators[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListEmulators[p].name ); break;
				case 4: vita2d_draw_texture( iconListUtilities[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListUtilities[p].name ); break;
				case 5: vita2d_draw_texture( iconListThemes[p], 	65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListThemes[p].name ); break;
				case 6: vita2d_draw_texture( iconListDemos[p], 		65, (install_que_posy +112 +(i *itemPanelHeight)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90)), COLOUR_WHITE, 35, catListDemos[p].name ); break;
			}
		}
		for ( i = 0; i < install_que_complete_count; i++) {
			vita2d_draw_texture( img_inst_panel, 62, (install_que_posy +109 +(i *90) +(install_que_count *90)) );
			vita2d_font_draw_text( font_25, 808, (install_que_posy +185 +(i *itemPanelHeight) +(install_que_count *90)), COLOUR_WHITE, 25, "completed" 	);
			p = downloadCompleted[i].pos;
			switch (downloadCompleted[i].screen) {
				case 0: vita2d_draw_texture( iconListNew[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListNew[p].name ); break;
				case 1: vita2d_draw_texture( iconListApps[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListApps[p].name ); break;
				case 2: vita2d_draw_texture( iconListGames[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListGames[p].name ); break;
				case 3: vita2d_draw_texture( iconListEmulators[p], 	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListEmulators[p].name ); break;
				case 4: vita2d_draw_texture( iconListUtilities[p], 	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListUtilities[p].name ); break;
				case 5: vita2d_draw_texture( iconListThemes[p], 	65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListThemes[p].name ); break;
				case 6: vita2d_draw_texture( iconListDemos[p], 		65, (install_que_posy +112 +(i *itemPanelHeight) +(install_que_count *90)) ); vita2d_font_draw_text( font_35, 182, (install_que_posy +142 +(i *90) +(install_que_count *90)), COLOUR_WHITE, 35, catListDemos[p].name ); break;
			}
		}
		vita2d_draw_rectangle( 0, install_que_posy, 960, 64, RGBA8( 37, 40, 45, 255 ) );
		vita2d_font_draw_text( font_25, 438, (install_que_posy +40), COLOUR_WHITE, 25, "Install Que" );
		vita2d_draw_texture( img_inst_btn_close, 6, (install_que_posy +5) );
	}
	else {
		install_que_posy  = 550;
	}
}


// KEYBOARD
if ( show_input == 1 ) {
	// UPDATE TEXT
	//vita2d_clear_screen();
	SceCommonDialogStatus status = sceImeDialogGetStatus();
	if ( status == IME_DIALOG_RESULT_FINISHED ) {
		SceImeDialogResult result;
		memset( &result, 0, sizeof(SceImeDialogResult) );
		sceImeDialogGetResult(&result);
		if ( result.button == SCE_IME_DIALOG_BUTTON_CLOSE ) {
			// USER CANCELED
			status = IME_DIALOG_RESULT_CANCELED;
		}
		else {
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

// HIGHLIGHT CALCULATION
if ( itemHighlightDir ) {
	if ( itemHighlight < 245 )  { itemHighlight += 8; }
	else						{ itemHighlightDir = !itemHighlightDir; }
}
else {
	if ( itemHighlight > 0 )  	{ itemHighlight -= 8; }
	else						{ itemHighlightDir = !itemHighlightDir; }
}
	
	


	
vita2d_end_drawing();
vita2d_common_dialog_update();
vita2d_swap_buffers();
sceDisplayWaitVblankStart();