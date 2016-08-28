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
vita2d_free_texture( img_settings_about  	  			);
vita2d_free_texture( img_settings_about_pressed  		);
vita2d_free_texture( img_settings_bg  	  				);
vita2d_free_texture( img_settings_cont  	  			);
vita2d_free_texture( img_settings_cont_pressed  		);
vita2d_free_texture( img_settings_ctrl  	  			);
vita2d_free_texture( img_settings_ctrl_pressed  		);
vita2d_free_texture( img_settings_snd  	  				);
vita2d_free_texture( img_settings_snd_pressed  	  		);
vita2d_free_texture( img_settings_topbar  	  			);
vita2d_free_texture( img_dialog_progress_bg  	  		);
vita2d_free_texture( img_dialog_btn_cancel  	  		);
vita2d_free_texture( img_dialog_btn_cancel_pressed		);


// RELEASE FONT
vita2d_free_font( font_segoeui );
// SHUTDOWN NETWORK
//?????
sceKernelExitProcess(0);