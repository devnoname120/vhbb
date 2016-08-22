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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include <psp2/ctrl.h>
#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#include <psp2/touch.h>

#include <vita2d.h>


// DEFINE MACROS
	#define VHBB_VERSION						"0.7.5"
	#define MAX_ITEMS							500
	
	// APPLICATION ADDRESSES
	#define VHBB_APP_ADDRESS_ROOT 				"ux0:/data/VitaHbBrowser/"
	#define VHBB_APP_ADDRESS_STORAGE 			"ux0:/data/VitaHbBrowser/dta/"
	#define VHBB_APP_ADDRESS_STORAGE_ICONS  	"ux0:/data/VitaHbBrowser/dta/icons/"
	#define VHBB_APP_ADDRESS_STORAGE_PREVS  	"ux0:/data/VitaHbBrowser/dta/prevs/"
	#define VHBB_APP_ADDRESS_STORAGE_FILES  	"ux0:/data/VitaHbBrowser/dta/files/"

	// APPLICATION FILES
	#define VHBB_APP_FILE_DATABASE				"ux0:/data/VitaHbBrowser/dta/vhbb.db"
	#define VHBB_APP_FILE_SYSTEMFILE 		 	"ux0:/data/VitaHbBrowser/sys/sys.config"
	#define VHBB_APP_FILE_LOGCAT 		 		"ux0:/data/VitaHbBrowser/sys/vhbb.log"
	#define VHBB_APP_FILE_DLque				    "ux0:/data/VitaHbBrowser/sys/que.assets"

	// COLOURS
	#define C_WHITE   							RGBA8( 255, 255, 255, 255 )
	#define C_AQUA   							RGBA8( 168, 244, 253, 255 )
	#define C_BLUEGREY							RGBA8(  92, 108, 124, 255 )

	// DIALOGS
	#define PROGRESS_BAR_WIDTH					342
	#define PROGRESS_BAR_HEIGHT					35

	// MATH
	#define lerp(value, from_max, to_max) ((((value *10) *(to_max *10)) /(from_max *10)) /10)
	#define max(x, y) (((x) > (y)) ? (x) : (y))
	#define min(x, y) (((x) < (y)) ? (x) : (y))

/// FONTS
extern unsigned int  fontDefault_size;
extern unsigned char fontDefault[];

// VITA PAD, TOUCH FRONT
SceCtrlData 	pad;
SceTouchData    touch;

// SCREEN STATES
enum screen
	{
	screenSettings,
	screenNew,
	screenApps,
	screenGames,
	screenEmu,
	screenUtilities,
	screenThemes,
	screenDemos,
	screenPreview
	}

// STRUCTS
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
struct vitaItem catListNew		[9];
struct vitaItem catListApps		[MAX_ITEMS];
struct vitaItem catListGames	[MAX_ITEMS];
struct vitaItem catListEmulators[MAX_ITEMS];
struct vitaItem catListUtilities[MAX_ITEMS];
struct vitaItem catListThemes	[MAX_ITEMS];
struct vitaItem catListDemos	[MAX_ITEMS];

/// ICON LISTS
struct vita2d_texture *iconListNew		[9];
struct vita2d_texture *iconListApps		[MAX_ITEMS];
struct vita2d_texture *iconListGames	[MAX_ITEMS];
struct vita2d_texture *iconListEmulators[MAX_ITEMS];
struct vita2d_texture *iconListUtilities[MAX_ITEMS];
struct vita2d_texture *iconListThemes	[MAX_ITEMS];
struct vita2d_texture *iconListDemos	[MAX_ITEMS];

/// PREVIEW LISTS
struct vita2d_texture *previewListNew		[9];
struct vita2d_texture *previewListApps		[MAX_ITEMS];
struct vita2d_texture *previewListGames		[MAX_ITEMS];
struct vita2d_texture *previewListEmulators	[MAX_ITEMS];
struct vita2d_texture *previewListUtilities	[MAX_ITEMS];
struct vita2d_texture *previewListThemes	[MAX_ITEMS];
struct vita2d_texture *previewListDemos		[MAX_ITEMS];




// VARIABLES
char databaseTS[15];

int itemCount_new				= 0;
int itemCount_apps				= 0;
int itemCount_games				= 0;
int itemCount_emu				= 0;
int itemCount_util	 			= 0;
int itemCount_themes			= 0;
int itemCount_demos				= 0;
int itemPressed					= -1;

int btnState_search	 			= 0;
int btnState_previewAction		= 0;
int btnState_previewBack		= 0;
int btnState_settingsSound		= 0;
int btnState_settingsCtrl		= 0;
int btnState_settingsContact	= 0;
int btnState_settingsAbout		= 0;
int btnState_dialogBtn1			= 0;
int btnState_dialogBtn2			= 0;

int originX						= 0;
int originY						= 0;
int topbarHeight				= 86;
int itemPanelHeight			 	= 90;

int scroll                  	= 0;
int prevy                   	= 0;
double momentum                	= 0;
int scrolldir               	= 0;
int usemomentum             	= 1;
int ground                  	= 0;
int clickable               	= 1;
int newy						= 0;

int touch_x;
int touch_y;
int touch_check_pressed  		= 0;
int touch_check_pressed_prev  	= 0;
int touch_check			 		= 0;
int touch_check_released		= 0;
int touch_check_released_prev	= 0;


int  previewCategory			= 0;
char previewName[30];
char previewVersion[30];
char previewAuthor[30];
char previewDlSize[30];
char previewRelease[30];
char previewDescription[500];
char previewLink[500];
char previewDir[200];
int  previewEbootSize			= 0;
int  previewDescriptionHeight 	= 0;
int  previewListPos				= 0;
char *previewDesLine2;
char *previewDesLine3;
char *previewDesLine4;
char *previewDesLine5;

int itemState_isInstalled;
int itemState_isDownloaded;
int itemState_isCurrent;

//int settingsOpen				= 0;
int settingsDraw				= 0;
int settingsPosY				= 0;
int settingsGround				= 679;


int  dialogOpen					= 0;
char dialogMessage[200];

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








