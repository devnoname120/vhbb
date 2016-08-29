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


/// SYSTEM VARIABLES
int screen						= 0;
char databaseTS[15];
int i;
int dPadTimer					= 0;
int date_format, time_format;

int itemCount_new				= 0;
int itemCount_apps				= 0;
int itemCount_games				= 0;
int itemCount_emu				= 0;
int itemCount_util	 			= 0;
int itemCount_themes			= 0;
int itemCount_demos				= 0;


/// BUTTON STATES
int btnState_search	 			= 0;
int btnState_previewAction	 	= 0;
int btnState_previewBack		= 0;
int btnState_settingsSound		= 0;
int btnState_settingsCtrls		= 0;
int btnState_settingsContact	= 0;
int btnState_settingsAbout		= 0;
int btnState_dialogBtn1			= 0;
int btnState_dialogBtn2			= 0;

/// POSITION VARIABLES
int originX						= 0;
int originY						= 0;
int topbarHeight				= 86;
int itemPanelHeight				= 90;

/// SCROLLING VARIABLES
int scroll                  	= 0;
int prevy                   	= 0;
double momentum                	= 0;
int scrolldir               	= 0;
int usemomentum             	= 1;
int ground                  	= 0;
int clickable               	= 1;
int newy						= 0;

// LOAD VITA PAD, TOUCH FRONT
SceCtrlData 	pad;
SceTouchData    touch;

int touch_x;
int touch_y;
int touch_check_pressed  		= 0;
int touch_check_pressed_prev  	= 0;
int touch_check			 		= 0;
int touch_check_released		= 0;
int touch_check_released_prev	= 0;

// ITEM PRESSES
int itemPressed					= -1;

// PREVIEW VARIABLES
int  previewActive				= 0;
int  previewCategory			= 0;
char previewName[30];
char previewVersion[30];
char previewAuthor[30];
char previewDlSize[30];
char previewRelease[30];
char previewDescription[500];
char previewLink[500];
char previewDir[200];
int  previewEbootSize		 	= 0;
int  previewDescriptionHeight 	= 0;
int  previewListNumber			= 0;
char *previewDesLine2;
char *previewDesLine3;
char *previewDesLine4;
char *previewDesLine5;
int preview_isInstalled;
int preview_isDownloaded;
int preview_isCurrent;

// SETTINGS VARIABLES
int settingsOpen				= 0;
int settingsDraw				= 0;
int settingsPosY				= 0;
int settingsGround				= 679;

// DIALOG VARIABLES
int  dialogOpen					= 0;


// COLOURS
enum
	{
	COLOUR_WHITE 		= RGBA8( 255, 255, 255, 255 ),
	COLOUR_BLACK 		= RGBA8(   0,   0,   0, 255 ),
	COLOUR_AQUA			= RGBA8( 168, 244, 253, 255 ),
	COLOUR_BLUEGREY 	= RGBA8(  92, 108, 124, 255 ),
	COLOUR_RED 			= RGBA8( 255,   0,   0, 255 ),
	COLOUR_YELLOW 		= RGBA8( 240, 255,   0, 255 )
	
	};



/*

	-SCREENS-
   -1	- 	Settings (left)
	0	-	New ???
	1	-	Apps
	2	-	Games
	3	-	Emulators
	4	-	Utilities
	5	-	Themes
	6	-	Demos
	
*/

/// DATA STRUCTURES
// item example: Category Number|Item Name|Item Version|Item Author|Download Size|Release Date|Item Description|Item Icon|Item Preview|Item HTTP Link
struct homeBrewItem
	{
	int  cat;
	char name[30];
	char version[30];
	char author[30];
	char dlSize[30];
	char release[30];
	char description[500];
	char link[500];
	char dir[200];
	int  ebootSize;
	};
	
/// CATEGORY LISTS (CURRENTLY SUPPORTS UP TO 501 ITEMS PER CATEGORY)
struct homeBrewItem catListNew				[9];
struct homeBrewItem catListApps				[MAX_ITEMS];
struct homeBrewItem catListGames			[MAX_ITEMS];
struct homeBrewItem catListEmulators		[MAX_ITEMS];
struct homeBrewItem catListUtilities		[MAX_ITEMS];
struct homeBrewItem catListThemes			[MAX_ITEMS];
struct homeBrewItem catListDemos			[MAX_ITEMS];

/// ICON LISTS
struct vita2d_texture *iconListNew			[9];
struct vita2d_texture *iconListApps			[MAX_ITEMS];
struct vita2d_texture *iconListGames		[MAX_ITEMS];
struct vita2d_texture *iconListEmulators	[MAX_ITEMS];
struct vita2d_texture *iconListUtilities	[MAX_ITEMS];
struct vita2d_texture *iconListThemes		[MAX_ITEMS];
struct vita2d_texture *iconListDemos		[MAX_ITEMS];

/// PREVIEW LISTS
struct vita2d_texture *previewListNew		[9];
struct vita2d_texture *previewListApps		[MAX_ITEMS];
struct vita2d_texture *previewListGames		[MAX_ITEMS];
struct vita2d_texture *previewListEmulators	[MAX_ITEMS];
struct vita2d_texture *previewListUtilities	[MAX_ITEMS];
struct vita2d_texture *previewListThemes	[MAX_ITEMS];
struct vita2d_texture *previewListDemos		[MAX_ITEMS];

// SPRITES
extern unsigned char _binary_assets_img_splash_png_start;
extern unsigned char _binary_assets_img_bg_png_start;
extern unsigned char _binary_assets_img_topbar_new_png_start;
extern unsigned char _binary_assets_img_topbar_apps_png_start;
extern unsigned char _binary_assets_img_topbar_games_png_start;
extern unsigned char _binary_assets_img_topbar_emu_png_start;
extern unsigned char _binary_assets_img_topbar_util_png_start;
extern unsigned char _binary_assets_img_topbar_themes_png_start;
extern unsigned char _binary_assets_img_topbar_demos_png_start;

extern unsigned char _binary_assets_img_btn_search_png_start;

extern unsigned char _binary_assets_img_item_panel_new_png_start;
extern unsigned char _binary_assets_img_item_panel_apps_png_start;
extern unsigned char _binary_assets_img_item_panel_games_png_start;
extern unsigned char _binary_assets_img_item_panel_emu_png_start;
extern unsigned char _binary_assets_img_item_panel_util_png_start;
extern unsigned char _binary_assets_img_item_panel_themes_png_start;
extern unsigned char _binary_assets_img_item_panel_demos_png_start;
extern unsigned char _binary_assets_img_preview_btn_download_png_start;
extern unsigned char _binary_assets_img_preview_btn_download_pressed_png_start;
extern unsigned char _binary_assets_img_preview_btn_install_png_start;
extern unsigned char _binary_assets_img_preview_btn_install_pressed_png_start;
extern unsigned char _binary_assets_img_preview_btn_update_png_start;
extern unsigned char _binary_assets_img_preview_btn_update_pressed_png_start;

extern unsigned char _binary_assets_img_preview_infobg_png_start;
extern unsigned char _binary_assets_img_preview_btn_back_png_start;
extern unsigned char _binary_assets_img_preview_btn_back_pressed_png_start;

extern unsigned char _binary_assets_img_icon_default_png_start;
extern unsigned char _binary_assets_img_preview_default_png_start;


extern unsigned char _binary_assets_img_settings_about_png_start;
extern unsigned char _binary_assets_img_settings_about_pressed_png_start;
extern unsigned char _binary_assets_img_settings_bg_png_start;
extern unsigned char _binary_assets_img_settings_cont_png_start;
extern unsigned char _binary_assets_img_settings_cont_pressed_png_start;
extern unsigned char _binary_assets_img_settings_ctrl_png_start;
extern unsigned char _binary_assets_img_settings_ctrl_pressed_png_start;
extern unsigned char _binary_assets_img_settings_snd_png_start;
extern unsigned char _binary_assets_img_settings_snd_pressed_png_start;
extern unsigned char _binary_assets_img_settings_topbar_png_start;

extern unsigned char _binary_assets_img_dialog_progress_bg_png_start;
extern unsigned char _binary_assets_img_dialog_btn_cancel_png_start;
extern unsigned char _binary_assets_img_dialog_btn_cancel_pressed_png_start;

extern unsigned char _binary_assets_img_statsbar_battery_png_start;

