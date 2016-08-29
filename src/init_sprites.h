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


// LOAD TEXTURE SPRITES
vita2d_texture *img_splash 							= vita2d_load_PNG_buffer( &_binary_assets_img_splash_png_start  						);
vita2d_texture *img_bg 				 				= vita2d_load_PNG_buffer( &_binary_assets_img_bg_png_start  							);
vita2d_texture *img_topbar0 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_new_png_start  					);
vita2d_texture *img_topbar1 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_apps_png_start 					);
vita2d_texture *img_topbar2 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_games_png_start 					);
vita2d_texture *img_topbar3 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_emu_png_start 						);
vita2d_texture *img_topbar4 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_util_png_start 					);
vita2d_texture *img_topbar5 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_themes_png_start 					);
vita2d_texture *img_topbar6 						= vita2d_load_PNG_buffer( &_binary_assets_img_topbar_demos_png_start					);

vita2d_texture *img_btn_search 						= vita2d_load_PNG_buffer( &_binary_assets_img_btn_search_png_start						);

vita2d_texture *img_item_panel_new 					= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_new_png_start					);
vita2d_texture *img_item_panel_apps 				= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_apps_png_start					);
vita2d_texture *img_item_panel_games 				= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_games_png_start 				);
vita2d_texture *img_item_panel_emu 					= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_emu_png_start					);
vita2d_texture *img_item_panel_util 				= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_util_png_start					);
vita2d_texture *img_item_panel_themes 				= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_themes_png_start 				);
vita2d_texture *img_item_panel_demos				= vita2d_load_PNG_buffer( &_binary_assets_img_item_panel_demos_png_start 				);

vita2d_texture *img_preview_btn_download			= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_download_png_start 			);
vita2d_texture *img_preview_btn_download_pressed	= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_download_pressed_png_start 	);
vita2d_texture *img_preview_btn_install				= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_install_png_start 			);
vita2d_texture *img_preview_btn_install_pressed		= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_install_pressed_png_start 	);
vita2d_texture *img_preview_btn_update				= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_update_png_start 				);
vita2d_texture *img_preview_btn_update_pressed		= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_update_pressed_png_start 		);

vita2d_texture *img_preview_infobg					= vita2d_load_PNG_buffer( &_binary_assets_img_preview_infobg_png_start 					);
vita2d_texture *img_preview_btn_back				= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_back_png_start 			 	);
vita2d_texture *img_preview_btn_back_pressed		= vita2d_load_PNG_buffer( &_binary_assets_img_preview_btn_back_pressed_png_start  		);

vita2d_texture *img_settings_about					= vita2d_load_PNG_buffer( &_binary_assets_img_settings_about_png_start  		);
vita2d_texture *img_settings_about_pressed			= vita2d_load_PNG_buffer( &_binary_assets_img_settings_about_pressed_png_start 	);
vita2d_texture *img_settings_bg						= vita2d_load_PNG_buffer( &_binary_assets_img_settings_bg_png_start  			);
vita2d_texture *img_settings_cont					= vita2d_load_PNG_buffer( &_binary_assets_img_settings_cont_png_start  			);
vita2d_texture *img_settings_cont_pressed			= vita2d_load_PNG_buffer( &_binary_assets_img_settings_cont_pressed_png_start 	);
vita2d_texture *img_settings_ctrl					= vita2d_load_PNG_buffer( &_binary_assets_img_settings_ctrl_png_start  			);
vita2d_texture *img_settings_ctrl_pressed			= vita2d_load_PNG_buffer( &_binary_assets_img_settings_ctrl_pressed_png_start	);
vita2d_texture *img_settings_snd					= vita2d_load_PNG_buffer( &_binary_assets_img_settings_snd_png_start  			);
vita2d_texture *img_settings_snd_pressed			= vita2d_load_PNG_buffer( &_binary_assets_img_settings_snd_pressed_png_start 	);
vita2d_texture *img_settings_topbar					= vita2d_load_PNG_buffer( &_binary_assets_img_settings_topbar_png_start  		);

vita2d_texture *img_dialog_progress_bg			 	= vita2d_load_PNG_buffer( &_binary_assets_img_dialog_progress_bg_png_start   		);
vita2d_texture *img_dialog_btn_cancel			 	= vita2d_load_PNG_buffer( &_binary_assets_img_dialog_btn_cancel_png_start   		);
vita2d_texture *img_dialog_btn_cancel_pressed	 	= vita2d_load_PNG_buffer( &_binary_assets_img_dialog_btn_cancel_pressed_png_start 	);

vita2d_texture *img_statsbar_battery	 			= vita2d_load_PNG_buffer( &_binary_assets_img_statsbar_battery_png_start 	);
