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


#include "vpk_install/vpk_install.h"
#include "vhbb.h"
#include "macros.h"
#include "network.h"
#include "tools.h"
/// SYSTEM VARIABLES
int screen = 0;
char databaseTS[15];
int i;
int dPadTimer = 0;
int loadAnim = 0;
int loadTimer = 0;
int date_format, time_format;
int dialogOpen = 0;
int install_que_running = 0;
int install_que_count = 0;
int install_que_complete_count = 0;
int install_que_open = 0;
int install_que_posy = 550;
int install_completed = 0;
int install_que1_alpha = 0;
int install_que2_alpha = 0;
int install_que2_alpha_timer = 0;
int install_que_progress = 0;
int install_que_progress_timer = 0;
int install_animate = 0;
int install_animate_timer = 0;
int install_animate_x = 438;
int install_animate_y = 230;
float install_animate_scale = 1;
int install_animate_alpha = 0;
int install_animate_vs = 30;
int install_dialog_show = 0;
int install_dialog_alpha = 0;
int install_dialog_timer = 0;
int install_dialog_screen = 0;
int install_dialog_pos = 0;
float install_dialog_icon_scale = 0.60;
int itemCount_new = 0;
int itemCount_apps = 0;
int itemCount_games = 0;
int itemCount_emu = 0;
int itemCount_util = 0;
int itemCount_themes = 0;
int itemCount_demos = 0;
/// BUTTON STATES
int btnState_search = 0;
int btnState_previewAction = 0;
int btnState_previewBack = 0;
int btnState_settingsSound = 0;
int btnState_settingsCtrls = 0;
int btnState_settingsContact = 0;
int btnState_settingsAbout = 0;
int btnState_dialogBtn1 = 0;
int btnState_dialogBtn2 = 0;
int btnState_installBtn = 0;
/// POSITION VARIABLES
int originX = 0;
int originY = 0;
int topbarHeight = 86;
int itemPanelHeight = 90;
/// SCROLLING VARIABLES
int scroll = 0;
int prevy = 0;
double momentum = 0;
int scrolldir = 0;
int usemomentum = 1;
int ground = 0;
int clickable = 1;
int newy = 0;
// LOAD VITA PAD, TOUCH FRONT
SceCtrlData pad;
SceTouchData touch;
int touch_x;
int touch_y;
int touch_check_pressed = 0;
int touch_check_pressed_prev = 0;
int touch_check = 0;
int touch_check_released = 0;
int touch_check_released_prev = 0;
// ITEM PRESSES
int itemPressed = -1;
int itemHighlight =  0;
int itemHighlightDir =  1;
// PREVIEW VARIABLES
int  previewActive = 0;
int  previewCategory = 0;
char previewName[30];
char previewVersion[30];
char previewAuthor[30];
char previewDlSize[30];
char previewRelease[30];
char previewDescription[500];
char previewLink[500];
char previewDir[200];
int  previewEbootSize = 0;
int  previewDescriptionHeight = 0;
int  previewListNumber = 0;
int  previewScreenNumber = 0;
vita2d_texture *previewIcon;
char *previewDesLine2;
char *previewDesLine3;
char *previewDesLine4;
char *previewDesLine5;
int preview_isInstalled;
int preview_isDownloaded;
int preview_isCurrent;
// SETTINGS VARIABLES
int settingsOpen = 0;
int settingsDraw = 0;
int settingsPosY = 0;
int settingsGround = 679;
// COLOURS
enum {
	COLOUR_WHITE = RGBA8(255, 255, 255, 255),
	COLOUR_BLACK = RGBA8(0, 0, 0, 255),
	COLOUR_AQUA = RGBA8(168, 244, 253, 255),
	COLOUR_BLUEGREY = RGBA8(92, 108, 124, 255),
	COLOUR_RED = RGBA8(255, 0, 0, 255),
	COLOUR_YELLOW = RGBA8(240, 255, 0, 255)
};
struct homeBrewItem {
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
struct homeBrewItem catListNew[9];
struct homeBrewItem catListApps[MAX_ITEMS];
struct homeBrewItem catListGames[MAX_ITEMS];
struct homeBrewItem catListEmulators[MAX_ITEMS];
struct homeBrewItem catListUtilities[MAX_ITEMS];
struct homeBrewItem catListThemes[MAX_ITEMS];
struct homeBrewItem catListDemos[MAX_ITEMS];
/// ICON LISTS
struct vita2d_texture *iconListNew[9];
struct vita2d_texture *iconListApps[MAX_ITEMS];
struct vita2d_texture *iconListGames[MAX_ITEMS];
struct vita2d_texture *iconListEmulators[MAX_ITEMS];
struct vita2d_texture *iconListUtilities[MAX_ITEMS];
struct vita2d_texture *iconListThemes[MAX_ITEMS];
struct vita2d_texture *iconListDemos[MAX_ITEMS];
/// PREVIEW LISTS
struct vita2d_texture *previewListNew[9];
struct vita2d_texture *previewListApps[MAX_ITEMS];
struct vita2d_texture *previewListGames[MAX_ITEMS];
struct vita2d_texture *previewListEmulators[MAX_ITEMS];
struct vita2d_texture *previewListUtilities[MAX_ITEMS];
struct vita2d_texture *previewListThemes[MAX_ITEMS];
struct vita2d_texture *previewListDemos[MAX_ITEMS];
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
extern unsigned char _binary_assets_img_item_panel_highlight_png_start;
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
extern unsigned char _binary_assets_img_inst_que1_png_start;
extern unsigned char _binary_assets_img_inst_que2_png_start;
extern unsigned char _binary_assets_img_inst_dialog_png_start;
extern unsigned char _binary_assets_img_inst_btn_close_png_start;
extern unsigned char _binary_assets_img_inst_panel_png_start;
#include "input.h"
#include "vitaAudio/vitaAudio.h"

#include "api/api.h"

 // Init power tick thread
initPowerTickThread();

// LOAD AUDIO HANDLERS
vaudio snd_ui;
vaudio snd_install;

int main() {
	// CREATE DIRECTORIES
	dir_create("ux0:/data/VitaHbBrowser/sys/");
	dir_create("ux0:/data/VitaHbBrowser/sys/vers/");
	dir_create("ux0:/data/VitaHbBrowser/dta/icons/");
	dir_create("ux0:/data/VitaHbBrowser/dta/prevs/");
	dir_create("ux0:/data/VitaHbBrowser/dta/files/");
	// CLEAR LOG
	if (access(VHBB_APP_FILE_LOGCAT, F_OK) != -1) {
		remove(VHBB_APP_FILE_LOGCAT);
	}

	// INITIALIZE VITA2D
	vita2d_init();
	vita2d_set_clear_color(COLOUR_BLACK);

	// LOAD DEFAULT FONT
	vita2d_font *font_16 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	vita2d_font *font_20 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	vita2d_font *font_25 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	vita2d_font *font_30 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	vita2d_font *font_35 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	vita2d_font *font_40 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");



	// INPUT
	sceAppUtilInit(&(SceAppUtilInitParam) {}, &(SceAppUtilBootParam){});
	sceCommonDialogSetConfigParam(&(SceCommonDialogConfigParam) {});

	// SET TOUCH SAMPLING
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);

	// INITIALIZE SPRITE POINTERS
	vita2d_texture *img_splash = vita2d_load_PNG_buffer(&_binary_assets_img_splash_png_start);
	vita2d_texture *img_bg = vita2d_load_PNG_buffer(&_binary_assets_img_bg_png_start);
	vita2d_texture *img_topbar0 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_new_png_start);
	vita2d_texture *img_topbar1 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_apps_png_start);
	vita2d_texture *img_topbar2 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_games_png_start);
	vita2d_texture *img_topbar3 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_emu_png_start);
	vita2d_texture *img_topbar4 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_util_png_start);
	vita2d_texture *img_topbar5 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_themes_png_start);
	vita2d_texture *img_topbar6 = vita2d_load_PNG_buffer(&_binary_assets_img_topbar_demos_png_start);
	vita2d_texture *img_btn_search = vita2d_load_PNG_buffer(&_binary_assets_img_btn_search_png_start);
	vita2d_texture *img_item_panel_new = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_new_png_start);
	vita2d_texture *img_item_panel_apps = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_apps_png_start);
	vita2d_texture *img_item_panel_games = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_games_png_start);
	vita2d_texture *img_item_panel_emu = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_emu_png_start);
	vita2d_texture *img_item_panel_util = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_util_png_start);
	vita2d_texture *img_item_panel_themes = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_themes_png_start);
	vita2d_texture *img_item_panel_demos = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_demos_png_start);
	vita2d_texture *img_item_panel_highlight = vita2d_load_PNG_buffer(&_binary_assets_img_item_panel_highlight_png_start);
	vita2d_texture *img_preview_btn_download = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_download_png_start);
	vita2d_texture *img_preview_btn_download_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_download_pressed_png_start);
	vita2d_texture *img_preview_btn_install = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_install_png_start);
	vita2d_texture *img_preview_btn_install_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_install_pressed_png_start);
	vita2d_texture *img_preview_btn_update = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_update_png_start);
	vita2d_texture *img_preview_btn_update_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_update_pressed_png_start);
	vita2d_texture *img_preview_infobg = vita2d_load_PNG_buffer(&_binary_assets_img_preview_infobg_png_start);
	vita2d_texture *img_preview_btn_back = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_back_png_start);
	vita2d_texture *img_preview_btn_back_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_preview_btn_back_pressed_png_start);
	vita2d_texture *img_settings_about = vita2d_load_PNG_buffer(&_binary_assets_img_settings_about_png_start);
	vita2d_texture *img_settings_about_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_settings_about_pressed_png_start);
	vita2d_texture *img_settings_bg	= vita2d_load_PNG_buffer(&_binary_assets_img_settings_bg_png_start);
	vita2d_texture *img_settings_cont = vita2d_load_PNG_buffer(&_binary_assets_img_settings_cont_png_start);
	vita2d_texture *img_settings_cont_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_settings_cont_pressed_png_start);
	vita2d_texture *img_settings_ctrl = vita2d_load_PNG_buffer(&_binary_assets_img_settings_ctrl_png_start);
	vita2d_texture *img_settings_ctrl_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_settings_ctrl_pressed_png_start);
	vita2d_texture *img_settings_snd = vita2d_load_PNG_buffer(&_binary_assets_img_settings_snd_png_start);
	vita2d_texture *img_settings_snd_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_settings_snd_pressed_png_start);
	vita2d_texture *img_settings_topbar = vita2d_load_PNG_buffer(&_binary_assets_img_settings_topbar_png_start);
	vita2d_texture *img_dialog_progress_bg = vita2d_load_PNG_buffer(&_binary_assets_img_dialog_progress_bg_png_start);
	vita2d_texture *img_dialog_btn_cancel = vita2d_load_PNG_buffer(&_binary_assets_img_dialog_btn_cancel_png_start);
	vita2d_texture *img_dialog_btn_cancel_pressed = vita2d_load_PNG_buffer(&_binary_assets_img_dialog_btn_cancel_pressed_png_start);
	vita2d_texture *img_statsbar_battery = vita2d_load_PNG_buffer(&_binary_assets_img_statsbar_battery_png_start);
	vita2d_texture *img_inst_que1 = vita2d_load_PNG_buffer(&_binary_assets_img_inst_que1_png_start);
	vita2d_texture *img_inst_que2 = vita2d_load_PNG_buffer(&_binary_assets_img_inst_que2_png_start);
	vita2d_texture *img_inst_dialog = vita2d_load_PNG_buffer(&_binary_assets_img_inst_dialog_png_start);
	vita2d_texture *img_inst_btn_close = vita2d_load_PNG_buffer(&_binary_assets_img_inst_btn_close_png_start);
	vita2d_texture *img_inst_panel = vita2d_load_PNG_buffer(&_binary_assets_img_inst_panel_png_start);

	memset(&pad, 0, sizeof(pad));

	// SHOW SPLASH BACKGROUND
	vita2d_start_drawing();
	vita2d_clear_screen();
	vita2d_draw_texture(img_splash, 0, 0);
	vita2d_end_drawing();
	vita2d_swap_buffers();

	// BOOT -------------------------------------------------------
	logcat_add("booting...", "", "\n");
	if (access(VHBB_APP_FILE_SYSTEMFILE, F_OK) == -1) {
		// FIRST BOOT (ADD DEFAULT CONFIG HERE)
		logcat_add("-FIRST LOAD-\n", "creating system config file", "\n");
		FILE *fp = fopen(VHBB_APP_FILE_SYSTEMFILE, "ab+");
		fprintf(fp, "***********************************\n-Vita HomeBrew Browser-\n ver. ");
		fprintf(fp, VHBB_VERSION);
		fprintf(fp, "\n Created By\n      Arkanite\n***********************************\n\n");
		fprintf(fp, VHBB_VERSION);
		fclose(fp);
		logcat_add("default system config created", "", "\n");
	}
	else {
		// LOAD USER SETTINGS

	}
	//-------------------------------------------------------------

	// RETRIEVE AND LOAD DATABASE ---------------------------------
	#include "database_load.h"
	//-------------------------------------------------------------

	/// MAIN LOOP -------------------------------------------------

	while (1) {
		sceKernelPowerTick(0);
		
		// CONTROL CHECKING -------------------------------------	
		sceCtrlPeekBufferPositive(0, &pad, 1);
		dPadTimer = max(0, (dPadTimer -1));
		if (pad.buttons) {
			// DPAD - UP
			if (pad.buttons & SCE_CTRL_UP) {
				if (previewActive == 0) {
					if (dPadTimer == 0) {
						dPadTimer = 9;
						itemPressed--;
						if (itemPressed < 0){
							itemPressed = 0;
						}
						int dif	= (originY +115 +(itemPressed *90));
						if (dif < 115) {
							originY += 90;
						}
						if (originY > 0) {
							originY = 0;
						}
					}
				}
			}
			// DPAD - DOWN
			if (pad.buttons & SCE_CTRL_DOWN) {
				if (previewActive == 0) {
					if (dPadTimer == 0) {
						dPadTimer = 9;
						itemPressed++;
						int adj = 0;
						switch (screen) {
							case 0:		if (itemPressed > (itemCount_new -1)) { itemPressed = (itemCount_new -1); adj = 1; } break;
							case 1:		if (itemPressed > (itemCount_apps -1)) { itemPressed = (itemCount_apps -1); adj = 1; } break;
							case 2:		if (itemPressed > (itemCount_games -1)) { itemPressed = (itemCount_games -1); adj = 1; } break;
							case 3:		if (itemPressed > (itemCount_emu -1)) { itemPressed = (itemCount_emu -1); adj = 1; } break;
							case 4:		if (itemPressed > (itemCount_util -1)) { itemPressed = (itemCount_util -1); adj = 1; } break;
							case 5:		if (itemPressed > (itemCount_themes -1)) { itemPressed = (itemCount_themes -1); adj = 1; } break;
							case 6:		if (itemPressed > (itemCount_demos -1)) { itemPressed = (itemCount_demos -1); adj = 1; } break;
						}
						if (!adj) {
							int dif		= (originY +115 +(itemPressed *90));
							if (dif > 454) {
								originY -= 90;
							}
						}
					}
				}
			}
			// DPAD - LEFT
			if (pad.buttons & SCE_CTRL_LEFT) {
				
			}
			// DPAD - RIGHT
			if (pad.buttons & SCE_CTRL_RIGHT) {
				
			}
			// BUTTON - CROSS
			if (pad.buttons & SCE_CTRL_CROSS) {
				if (previewActive == 0) {
					if (itemPressed >= 0 && clickable == 1) {
						int previewReady = 1;
						switch (screen) {
							case 0:			// NEW
											if (itemPressed < 0 || itemPressed > itemCount_new) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListNew[itemPressed].cat;
											strcpy( previewName, catListNew[itemPressed].name);
											strcpy( previewAuthor, catListNew[itemPressed].author);
											strcpy( previewVersion, catListNew[itemPressed].version);
											strcpy( previewDlSize, catListNew[itemPressed].dlSize);
											strcpy( previewRelease, catListNew[itemPressed].release);
											strcpy( previewDescription, catListNew[itemPressed].description);
											strcpy( previewLink, catListNew[itemPressed].link);
											strcpy( previewDir, catListNew[itemPressed].dir);
											previewEbootSize = catListNew[itemPressed].ebootSize;
											break;
							case 1:			// APPS
											if (itemPressed < 0 || itemPressed > itemCount_apps) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListApps[itemPressed].cat;
											strcpy( previewName, catListApps[itemPressed].name);
											strcpy( previewAuthor, catListApps[itemPressed].author);
											strcpy( previewVersion, catListApps[itemPressed].version);
											strcpy( previewDlSize, catListApps[itemPressed].dlSize);
											strcpy( previewRelease, catListApps[itemPressed].release);
											strcpy( previewDescription, catListApps[itemPressed].description);
											strcpy( previewLink, catListApps[itemPressed].link);
											strcpy( previewDir, catListApps[itemPressed].dir);
											previewEbootSize = catListApps[itemPressed].ebootSize;
											break;
							case 2:			// GAMES
											if (itemPressed < 0 || itemPressed > itemCount_games) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListGames[itemPressed].cat;
											strcpy( previewName, catListGames[itemPressed].name);
											strcpy( previewAuthor, catListGames[itemPressed].author);
											strcpy( previewVersion, catListGames[itemPressed].version);
											strcpy( previewDlSize, catListGames[itemPressed].dlSize);
											strcpy( previewRelease, catListGames[itemPressed].release);
											strcpy( previewDescription, catListGames[itemPressed].description);
											strcpy( previewLink, catListGames[itemPressed].link);
											strcpy( previewDir, catListGames[itemPressed].dir);
											previewEbootSize = catListGames[itemPressed].ebootSize;
											break;
							case 3:			// EMULATORS
											if (itemPressed < 0 || itemPressed > itemCount_emu) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListEmulators[itemPressed].cat;
											strcpy( previewName, catListEmulators[itemPressed].name);
											strcpy( previewAuthor, catListEmulators[itemPressed].author);
											strcpy( previewVersion, catListEmulators[itemPressed].version);
											strcpy( previewDlSize, catListEmulators[itemPressed].dlSize);
											strcpy( previewRelease, catListEmulators[itemPressed].release);
											strcpy( previewDescription, catListEmulators[itemPressed].description);
											strcpy( previewLink, catListEmulators[itemPressed].link);
											strcpy( previewDir, catListEmulators[itemPressed].dir);
											previewEbootSize = catListEmulators[itemPressed].ebootSize;
											break;
							case 4:			// UTILITIES
											if (itemPressed < 0 || itemPressed > itemCount_util) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListUtilities[itemPressed].cat;
											strcpy( previewName, catListUtilities[itemPressed].name);
											strcpy( previewAuthor, catListUtilities[itemPressed].author);
											strcpy( previewVersion, catListUtilities[itemPressed].version);
											strcpy( previewDlSize, catListUtilities[itemPressed].dlSize);
											strcpy( previewRelease, catListUtilities[itemPressed].release);
											strcpy( previewDescription, catListUtilities[itemPressed].description);
											strcpy( previewLink, catListUtilities[itemPressed].link);
											strcpy( previewDir, catListUtilities[itemPressed].dir);
											previewEbootSize = catListUtilities[itemPressed].ebootSize;
											break;
							case 5:			// THEMES
											if (itemPressed < 0 || itemPressed > itemCount_themes) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListThemes[itemPressed].cat;
											strcpy( previewName, catListThemes[itemPressed].name);
											strcpy( previewAuthor, catListThemes[itemPressed].author);
											strcpy( previewVersion, catListThemes[itemPressed].version);
											strcpy( previewDlSize, catListThemes[itemPressed].dlSize);
											strcpy( previewRelease, catListThemes[itemPressed].release);
											strcpy( previewDescription, catListThemes[itemPressed].description);
											strcpy( previewLink, catListThemes[itemPressed].link);
											strcpy( previewDir, catListThemes[itemPressed].dir);
											previewEbootSize = catListThemes[itemPressed].ebootSize;
											break;
							case 6:			// DEMOS
											if (itemPressed < 0 || itemPressed > itemCount_demos) {
												previewReady = 0; break;
											}
											vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
											previewCategory	= catListDemos[itemPressed].cat;
											strcpy( previewName, catListDemos[itemPressed].name);
											strcpy( previewAuthor, catListDemos[itemPressed].author);
											strcpy( previewVersion, catListDemos[itemPressed].version);
											strcpy( previewDlSize, catListDemos[itemPressed].dlSize);
											strcpy( previewRelease, catListDemos[itemPressed].release);
											strcpy( previewDescription, catListDemos[itemPressed].description);
											strcpy( previewLink, catListDemos[itemPressed].link);
											strcpy( previewDir, catListDemos[itemPressed].dir);
											previewEbootSize = catListDemos[itemPressed].ebootSize;
											break;
						}
						if (previewReady == 1) {
							strtok_r(previewDescription, "|", &previewDesLine2);
							strtok_r(previewDesLine2, "|", &previewDesLine3);
							strtok_r(previewDesLine3, "|", &previewDesLine4);
							strtok_r(previewDesLine4, "|", &previewDesLine5);
							previewActive =  1;
							previewListNumber = itemPressed;
							// CHECK DOWNLOADED, INSTALLED, CURRENT VERSION
							if (access(previewDir, F_OK) == -1) {
								preview_isInstalled = 0;
							}
							else {
								preview_isInstalled = 1;
								unsigned long localV = file_size(string_join(2, previewDir, "eboot.bin"));
								if (localV == (unsigned long)previewEbootSize) {
									preview_isCurrent = 1;
								}
								else {
									preview_isCurrent = 0;
								}
							}
							if (access(string_join(3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk"), F_OK) == -1) {
								preview_isDownloaded = 0;
							}
							else {
								preview_isDownloaded = 1;
							}
						}
					}
				}
			}
			// BUTTON - CIRCLE
			if (pad.buttons & SCE_CTRL_CIRCLE) {
				if (settingsOpen == 0) {
					if ( previewActive == 1 ) {
						previewActive = 0;
						itemPressed = previewListNumber;
					}
				}
				else {
					settingsOpen == 0;
				}
			}
			// BUTTON - SQUARE
			if (pad.buttons & SCE_CTRL_SQUARE) {
				
			}
			// BUTTON - TRIANGLE
			if (pad.buttons & SCE_CTRL_TRIANGLE)
				{
				if (dPadTimer == 0) {
					dPadTimer = 9;
					/* SETTINGS REMOVED
					if (settingsOpen != 1) {
						settingsOpen = 1;
					}
					else {
						settingsOpen = 0;
					}
					*/
				}
			}
			// BUTTON - SELECT
			if (pad.buttons & SCE_CTRL_SELECT) {
				break;
			}
			// BUTTON - START
			if ( pad.buttons & SCE_CTRL_START ) {
				
			}
			// BUTTON - LEFT TRIGGER
			if (pad.buttons & SCE_CTRL_LTRIGGER) {
				if (previewActive == 0) {
					if (dPadTimer == 0) {
						dPadTimer = 9;
						switch (screen) {
							case 1:		screen = 0; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 2:		screen = 1; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 3:		screen = 2; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 4:		screen = 3; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 5:		screen = 4; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 6:		screen = 5; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
						}
					}
				}
			}
			// BUTTON - RIGHT TRIGGER
			if (pad.buttons & SCE_CTRL_RTRIGGER) {
				if (previewActive == 0) {
					if (dPadTimer == 0) {
						dPadTimer = 9;
						switch (screen) {
							case 0:		screen = 1; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 1:		screen = 2; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 2:		screen = 3; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 3:		screen = 4; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 4:		screen = 5; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
							case 5:		screen = 6; originY = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); break;
						}
					}
				}
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		/// TOUCH CHECKS -----------------------------------
		sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
		if (touch.reportNum > 0) {
			touch_check = 1;
			touch_x = (lerp(touch.report[0].x, 1919, 960));
			touch_y = (lerp(touch.report[0].y, 1087, 544));
			touch_check_released_prev = 0;
			touch_check_released = 0;
			if (touch_check_pressed_prev == 0) {
				touch_check_pressed_prev = 1; touch_check_pressed = 1;
			}
			else {
				touch_check_pressed = 0;
			}
		}
		else {
			touch_check = 0;
			touch_check_pressed = 0;
			touch_check_pressed_prev = 0;
			if (touch_check_released_prev == 0) {
				touch_check_released_prev = 1;
				touch_check_released = 1;
			}
			else {
				touch_check_released = 0;
			}
		}
		///-------------------------------------------------

		// SETTINGS ANIMATION -----------------------------------------
		if (settingsOpen) {
			settingsDraw = 1;
			if (originX < 192) {
				originX += 15;
			}
			else {
				originX = 207;
			}
		}
		else {
			if (originX > 15) {
				originX -= 15;
			}
			else {
				originX = 0;
				settingsDraw = 0;
			}
		}
		//-------------------------------------------------------------

		// SCROLLING --------------------------------------------------
		// MAIN SCROLL
		if (momentum < -1 || momentum > 1) {
			clickable = 0;
		}
		if (!previewActive && !settingsOpen && !install_que_open) {
			if (scroll && ground > 544 && touch_y > 90) {
				if (touch_check) {
					newy= 0;
					newy = originY + (touch_y - prevy);
					if (usemomentum == 1) {
						momentum = abs(round((touch_y - prevy) /2));
						if (touch_y - prevy > 0) {
							scrolldir = 1;
						}
						else {
							scrolldir = 0;
						}
					}
					if (newy > 0) {
						newy = 0;
					}
					if (newy -544 < (-ground)) {
						newy = (-ground) +544;
					}
					originY = newy;
					prevy = touch_y;          
				}
				if (touch_check_released) {
					prevy = 0; scroll = 0;
				}
			}
			else {
				if (touch_check_pressed) {
					scroll = 1;
					prevy = touch_y;
				}
				if (usemomentum && momentum > 0) {
					newy = 0;
					if (scrolldir == 1) {
						newy = originY +momentum;
						if (newy > 0) {
							newy = 0;
							momentum = 0;
						}
					}
					else {
						newy = originY -momentum;
						if (newy - 544 <= (-ground)) {
							newy = (-ground) +544;
							momentum = 0;
						}
					}
					originY = newy;
					if (momentum > 0) {
						momentum -= 0.2;
					}
				}    
			}
		}
		// SETTINGS SCROLL
		if (settingsOpen) {
			if (scroll && settingsGround > 544 && touch_y > 90) {
				if (touch_check && touch_x < 207) {
					newy = 0;
					newy = settingsPosY + (touch_y - prevy);
					if (usemomentum) {
						momentum = abs(round((touch_y - prevy) /2));
						if (touch_y - prevy > 0) {
							scrolldir = 1;
						}
						else {
							scrolldir = 0;
						}
					}
					if (newy > 0) {
						newy = 0;
					}
					if (newy -544 < (-settingsGround)) {
						newy = (-settingsGround) +544;
					}
					settingsPosY = newy;
					prevy = touch_y;          
				}
				if (touch_check_released) {
					prevy = 0;
					scroll = 0;
				}
			}
			else {
				if (touch_check_pressed && touch_x < 207) {
					scroll = 1;
					prevy = touch_y;
				}
				if (usemomentum && momentum > 0) {
					newy = 0;
					if (scrolldir == 1) {
						newy = settingsPosY +momentum;
						if (newy > 0) {
							newy = 0;
							momentum = 0;
						}
					}
					else {
						newy = settingsPosY -momentum;
						if (newy - 544 <= (-settingsGround)) {
							newy = (-settingsGround) +544;
							momentum = 0;
						}
					}
					settingsPosY = newy;
					if (momentum > 0) {
						momentum -= 0.2;
					}
				}    
			}
		}
			
		// INSTALL QUE OPEN
		if (install_que_open) {
			if (scroll && settingsGround > 544 && touch_y > 96) {
				if (touch_check) {
					newy = 0;
					newy = install_que_posy + (touch_y - prevy);
					if ( usemomentum ) {
						momentum = abs(round((touch_y - prevy) /2));
						if (touch_y - prevy > 0) {
							scrolldir = 1;
						}
						else {
							scrolldir = 0;
						}
					}
					if (newy > 0) {
						newy = 0;
					}
					if (newy -544 < (-settingsGround)) {
						newy = (-settingsGround) +544;
					}
					install_que_posy = newy;
					prevy = touch_y;          
				}
				if (touch_check_released) {
					prevy = 0;
					scroll = 0;
				}
			}
			else {
				if (touch_check_pressed) {
					scroll = 1;
					prevy = touch_y;
				}
				if (usemomentum && momentum > 0) {
					newy = 0;
					if (scrolldir == 1) {
						newy = install_que_posy +momentum;
						if (newy > 0) {
							newy = 0;
							momentum = 0;
						}
					}
					else {
						newy = install_que_posy -momentum;
						if (newy - 544 <= (-settingsGround)) {
							newy = (-settingsGround) +544;
							momentum = 0;
						}
					}
					install_que_posy = newy;
					if (momentum > 0) {
						momentum -= 0.2;
					}
				}    
			}
		}
		//-------------------------------------------------------------


		// TOUCH CHECK PRESSED ----------------------------------------
		if (touch_check_pressed) {
			if (!dialogOpen) {
				if (!settingsOpen) {
					if (!install_que_open) {
						if (touch_y < itemPanelHeight) {
							// TOPBAR BUTTONS
							if (point_in_rectangle(touch_x, touch_y, 1, 1, 98, 90) && screen != 0) { screen = 0; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 99, 1, 204, 90) && screen != 1) { screen = 1; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 205, 1, 326, 90) && screen != 2) { screen = 2; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 327, 1, 427, 90) && screen != 3) { screen = 3; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 428, 1, 552, 90) && screen != 4) { screen = 4; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 553, 1, 674, 90) && screen != 5) { screen = 5; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 675, 1, 799, 90) && screen != 6) { screen = 6; originY = 0; previewActive = 0; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
							if (point_in_rectangle(touch_x, touch_y, 800, 1, 960, 90)) { btnState_search = 1; vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg"); }
						}
						else {
							if (previewActive) {
								// INSTALL PRESSED
								if (point_in_rectangle(touch_x, touch_y, 197, 213, 357, 266)) {
									vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
									btnState_previewAction = 1;
								}
							}
						}
					}
					else {
						if (point_in_rectangle(touch_x, touch_y, 7, 39, 58, 90)) {
							btnState_installBtn = 1;
						}
					}
				}
				else {
					if (settingsPosY >= 207) { // wait for animation to complete
						if (point_in_rectangle(touch_x, touch_y, 40, (settingsPosY + 110), 60, (settingsPosY + 222))) { btnState_settingsSound = 1; }
						if (point_in_rectangle(touch_x, touch_y, 40, (settingsPosY + 245), 60, (settingsPosY + 357))) { btnState_settingsCtrls = 1; }
						if (point_in_rectangle(touch_x, touch_y, 40, (settingsPosY + 380), 60, (settingsPosY + 492))) { btnState_settingsContact = 1; }
						if (point_in_rectangle(touch_x, touch_y, 40, (settingsPosY + 515), 60, (settingsPosY + 627))) { btnState_settingsAbout = 1; }
					}
				}
			}
			else {
				if (point_in_rectangle(touch_x, touch_y, 287, 403, 682, 448)) {
					btnState_dialogBtn2 = 1;
					vaudio_play_sound_ogg(&snd_ui, VHBB_RES_DIR_SOUND "snd_tap.ogg");
				}
			}
		}
		//-------------------------------------------------------------

		// TOUCH CHECK HOLDING ----------------------------------------
		if (touch_check && !previewActive && !settingsDraw) {
			if (touch_y > 90 && clickable) {
				itemPressed = floor((touch_y + abs(originY)) / itemPanelHeight) - 1;
			}
			else {
				itemPressed = -1;
			}
		}
		//-------------------------------------------------------------

		// TOUCH CHECK RELEASE ----------------------------------------
		if (touch_check_released) {
			if (!dialogOpen) {
				if (!settingsOpen) {
					if (!install_que_open) {
						if (!previewActive) {
							if (itemPressed >= 0 && clickable) {
								int previewReady = 1;
								// PREP FOR ITEM PREVIEW
								switch (screen) {
								case 0:			// NEW
									if (itemPressed < 0 || itemPressed >(itemCount_new - 1)) { previewReady = 0; break; }
									previewCategory = catListNew[itemPressed].cat;
									strcpy(previewName, catListNew[itemPressed].name);
									strcpy(previewAuthor, catListNew[itemPressed].author);
									strcpy(previewVersion, catListNew[itemPressed].version);
									strcpy(previewDlSize, catListNew[itemPressed].dlSize);
									strcpy(previewRelease, catListNew[itemPressed].release);
									strcpy(previewDescription, catListNew[itemPressed].description);
									strcpy(previewLink, catListNew[itemPressed].link);
									strcpy(previewDir, catListNew[itemPressed].dir);
									previewEbootSize = catListNew[itemPressed].ebootSize;
									break;
								case 1:			// APPS
									if (itemPressed < 0 || itemPressed >(itemCount_apps - 1)) { previewReady = 0; break; }
									previewCategory = catListApps[itemPressed].cat;
									strcpy(previewName, catListApps[itemPressed].name);
									strcpy(previewAuthor, catListApps[itemPressed].author);
									strcpy(previewVersion, catListApps[itemPressed].version);
									strcpy(previewDlSize, catListApps[itemPressed].dlSize);
									strcpy(previewRelease, catListApps[itemPressed].release);
									strcpy(previewDescription, catListApps[itemPressed].description);
									strcpy(previewLink, catListApps[itemPressed].link);
									strcpy(previewDir, catListApps[itemPressed].dir);
									previewEbootSize = catListApps[itemPressed].ebootSize;
									break;
								case 2:			// GAMES
									if (itemPressed < 0 || itemPressed >(itemCount_games - 1)) { previewReady = 0; break; }
									previewCategory = catListGames[itemPressed].cat;
									strcpy(previewName, catListGames[itemPressed].name);
									strcpy(previewAuthor, catListGames[itemPressed].author);
									strcpy(previewVersion, catListGames[itemPressed].version);
									strcpy(previewDlSize, catListGames[itemPressed].dlSize);
									strcpy(previewRelease, catListGames[itemPressed].release);
									strcpy(previewDescription, catListGames[itemPressed].description);
									strcpy(previewLink, catListGames[itemPressed].link);
									strcpy(previewDir, catListGames[itemPressed].dir);
									previewEbootSize = catListGames[itemPressed].ebootSize;
									break;
								case 3:			// EMULATORS
									if (itemPressed < 0 || itemPressed >(itemCount_emu - 1)) { previewReady = 0; break; }
									previewCategory = catListEmulators[itemPressed].cat;
									strcpy(previewName, catListEmulators[itemPressed].name);
									strcpy(previewAuthor, catListEmulators[itemPressed].author);
									strcpy(previewVersion, catListEmulators[itemPressed].version);
									strcpy(previewDlSize, catListEmulators[itemPressed].dlSize);
									strcpy(previewRelease, catListEmulators[itemPressed].release);
									strcpy(previewDescription, catListEmulators[itemPressed].description);
									strcpy(previewLink, catListEmulators[itemPressed].link);
									strcpy(previewDir, catListEmulators[itemPressed].dir);
									previewEbootSize = catListEmulators[itemPressed].ebootSize;
									break;
								case 4:			// UTILITIES
									if (itemPressed < 0 || itemPressed >(itemCount_util - 1)) { previewReady = 0; break; }
									previewCategory = catListUtilities[itemPressed].cat;
									strcpy(previewName, catListUtilities[itemPressed].name);
									strcpy(previewAuthor, catListUtilities[itemPressed].author);
									strcpy(previewVersion, catListUtilities[itemPressed].version);
									strcpy(previewDlSize, catListUtilities[itemPressed].dlSize);
									strcpy(previewRelease, catListUtilities[itemPressed].release);
									strcpy(previewDescription, catListUtilities[itemPressed].description);
									strcpy(previewLink, catListUtilities[itemPressed].link);
									strcpy(previewDir, catListUtilities[itemPressed].dir);
									previewEbootSize = catListUtilities[itemPressed].ebootSize;
									break;
								case 5:			// THEMES
									if (itemPressed < 0 || itemPressed >(itemCount_themes - 1)) { previewReady = 0; break; }
									previewCategory = catListThemes[itemPressed].cat;
									strcpy(previewName, catListThemes[itemPressed].name);
									strcpy(previewAuthor, catListThemes[itemPressed].author);
									strcpy(previewVersion, catListThemes[itemPressed].version);
									strcpy(previewDlSize, catListThemes[itemPressed].dlSize);
									strcpy(previewRelease, catListThemes[itemPressed].release);
									strcpy(previewDescription, catListThemes[itemPressed].description);
									strcpy(previewLink, catListThemes[itemPressed].link);
									strcpy(previewDir, catListThemes[itemPressed].dir);
									previewEbootSize = catListThemes[itemPressed].ebootSize;
									break;
								case 6:			// DEMOS
									if (itemPressed < 0 || itemPressed >(itemCount_demos - 1)) { previewReady = 0; break; }
									previewCategory = catListDemos[itemPressed].cat;
									strcpy(previewName, catListDemos[itemPressed].name);
									strcpy(previewAuthor, catListDemos[itemPressed].author);
									strcpy(previewVersion, catListDemos[itemPressed].version);
									strcpy(previewDlSize, catListDemos[itemPressed].dlSize);
									strcpy(previewRelease, catListDemos[itemPressed].release);
									strcpy(previewDescription, catListDemos[itemPressed].description);
									strcpy(previewLink, catListDemos[itemPressed].link);
									strcpy(previewDir, catListDemos[itemPressed].dir);
									previewEbootSize = catListDemos[itemPressed].ebootSize;
									break;
								}
								if (previewReady) {
									strtok_r(previewDescription, "|", &previewDesLine2);
									strtok_r(previewDesLine2, "|", &previewDesLine3);
									strtok_r(previewDesLine3, "|", &previewDesLine4);
									strtok_r(previewDesLine4, "|", &previewDesLine5);
									previewActive = 1;
									previewListNumber = itemPressed;
									previewScreenNumber = screen;
									// CHECK DOWNLOADED, INSTALLED, CURRENT VERSION
									if (access(previewDir, F_OK) == -1) { preview_isInstalled = 0; }
									else {
										preview_isInstalled = 1;
										unsigned long localV = file_size(string_join(2, previewDir, "eboot.bin"));
										if (localV == (unsigned long)previewEbootSize) { preview_isCurrent = 1; }
										else { preview_isCurrent = 0; }
									}
									if (access(string_join(3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk"), F_OK) == -1) { preview_isDownloaded = 0; }
									else { preview_isDownloaded = 1; }
								}
								itemPressed = -1;
							}
							if (point_in_rectangle(touch_x, touch_y, 800, 1, 960, 90)) {
								// USER SEARCH
								input_get("Vita HomeBrew Search", "", SCE_IME_DIALOG_MAX_TEXT_LENGTH);
							}
						}
						else {
							// INSTALL/UPDATE PRESSED
							if (point_in_rectangle(touch_x, touch_y, 197, 213, 357, 266)) {
								// INSTALL
								char *fCloud;
								char *fLocal;
								fCloud = string_join(3, VHBB_CLOUD_ADDRESS_FILES, previewName, ".vpk");
								fLocal = string_join(3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk");
								if (!download_que_add(fCloud, fLocal, previewScreenNumber, previewListNumber)) {
									// QUE FULL (show que full dialog? if needed)

								}
								install_animate = 1;
							}
							if (point_in_rectangle(touch_x, touch_y, 0, 460, 90, 544) && !settingsDraw) {
								previewActive = 0;
								itemPressed = previewListNumber;
							}
						}
					}
					else {
						if (point_in_rectangle(touch_x, touch_y, 7, 39, 58, 90)) {
							install_que_open = 0;
						}
					}
					
				}
				else {
					// TOUCH RELEASE FOR SETTINGS HERE (REDO)


				}
			}
			else {
				if (point_in_rectangle(touch_x, touch_y, 287, 403, 682, 448)) {
					dialogOpen = 0;
				}


			}

			// RESET BUTTON STATES
			btnState_previewAction = 0;
			btnState_settingsSound = 0;
			btnState_settingsCtrls = 0;
			btnState_settingsContact = 0;
			btnState_settingsAbout = 0;
			btnState_dialogBtn2 = 0;
			btnState_installBtn = 0;
			btnState_search = 0;
			clickable = 1;
			
			// INSTALL QUE TOUCHED
			if (install_que_count > 0 || install_que_complete_count > 0) {
				if (point_in_rectangle(touch_x, touch_y, 873, 466, 960, 544)) {
					// INTALL QUE TAPPED
					install_que_open = 1;
				}
			}
			
		}
		//-------------------------------------------------------------


		// MAIN DRAW --------------------------------------------------
		vita2d_start_drawing();
		vita2d_clear_screen();

		// BACKGROUND
		vita2d_draw_texture(img_bg, originX, 0);
			
		// SETTINGS
		if (settingsDraw) {
			vita2d_draw_texture(img_settings_bg, (originX -207), 81);
			if ( btnState_settingsSound == 0) {
				vita2d_draw_texture(img_settings_snd, (originX -167), (settingsPosY +110));
			}
			else {
				vita2d_draw_texture(img_settings_snd_pressed, (originX -167), (settingsPosY +110));
			}
			if (btnState_settingsCtrls == 0) {
				vita2d_draw_texture(img_settings_ctrl, (originX -167), (settingsPosY +245));
			}
			else {
				vita2d_draw_texture(img_settings_ctrl_pressed, (originX -167), (settingsPosY +245));
			}
			if (btnState_settingsContact == 0) {
				vita2d_draw_texture( img_settings_cont, (originX -167), (settingsPosY +380));
			}
			else {
				vita2d_draw_texture(img_settings_cont_pressed, (originX -167), (settingsPosY +380));
			}
			if (btnState_settingsAbout == 0) {
				vita2d_draw_texture( img_settings_about, (originX -167), (settingsPosY +515));
			}
			else {
				vita2d_draw_texture(img_settings_about_pressed,	(originX -167), (settingsPosY +515));
			}
			vita2d_draw_texture(img_settings_topbar, (originX -207), 0);
		}



		if (previewActive) {
			// INFORMATION BACKGROUND
			vita2d_draw_texture(img_preview_infobg, originX, 276 );
			vita2d_font_draw_text(font_40, (originX +203), 140, COLOUR_WHITE, 40, previewName); // NAME
			vita2d_font_draw_text(font_25, (originX +203), 166, COLOUR_AQUA,  25, previewAuthor); // AUTHOR
			vita2d_font_draw_text(font_25, (originX +203), 196, COLOUR_WHITE, 25, previewVersion); // VERSION
			vita2d_font_draw_text(font_20, (originX +100), 219, COLOUR_WHITE, 20, previewDlSize); // DOWNLOAD SIZE
			vita2d_font_draw_text(font_20, (originX +850), 533, COLOUR_WHITE, 20, previewRelease); // RELEASE DATE
			vita2d_font_draw_textf(font_25, (originX +40), (349 +previewDescriptionHeight), COLOUR_WHITE, 25, "%s\n%s\n%s\n%s\n%s", previewDescription, previewDesLine2, previewDesLine3, previewDesLine4, previewDesLine5);
			
			if (preview_isDownloaded == 1) {
				if (btnState_previewAction == 0) {
					vita2d_draw_texture(img_preview_btn_install, (originX +201), 217);
				}
				else {
					vita2d_draw_texture(img_preview_btn_install_pressed, (originX +201), 217);
				}
			}
			else {
				if (btnState_previewAction == 0) {
					vita2d_draw_texture(img_preview_btn_download, (originX +201), 217);
				}
				else {
					vita2d_draw_texture(img_preview_btn_download_pressed, (originX +201), 217);
				}
			}
			// BACK BUTTON
			if (btnState_previewBack == 0) {
				vita2d_draw_texture(img_preview_btn_back, originX, 470);
			}
			else {
				vita2d_draw_texture(img_preview_btn_back_pressed, originX, 470);
			}
			switch (screen) {
				case 0:		vita2d_draw_texture( iconListNew[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListNew[previewListNumber], (originX +566), 97 ); break;
				case 1:		vita2d_draw_texture( iconListApps[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListApps[previewListNumber], (originX +566), 97 ); break;
				case 2:		vita2d_draw_texture( iconListGames[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListGames[previewListNumber], (originX +566), 97 ); break;
				case 3:		vita2d_draw_texture( iconListEmulators[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListEmulators[previewListNumber], (originX +566), 97 ); break;
				case 4:		vita2d_draw_texture( iconListUtilities[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListUtilities[previewListNumber], (originX +566), 97 ); break;
				case 5:		vita2d_draw_texture( iconListThemes[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListThemes[previewListNumber], (originX +566), 97 ); break;
				case 6:		vita2d_draw_texture( iconListDemos[previewListNumber], (originX +100), 112 ); vita2d_draw_texture( previewListDemos[previewListNumber], (originX +566), 97 ); break;
			}
		}
		else {
			/// ITEMS
			switch (screen) {
				case 0:			// NEW
								ground = 145 +(itemCount_new *90);
								for (i = 0; i < itemCount_new; i++) {
									vita2d_draw_texture(img_item_panel_new, (originX +62), (originY +115 +(i *90)));
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
								for (i = 0; i < itemCount_apps; i++) {
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
								for (i = 0; i < itemCount_games; i++) {
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
								for (i = 0; i < itemCount_emu; i++) {
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
								for (i = 0; i < itemCount_util; i++) {
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
								for (i = 0; i < itemCount_themes; i++) {
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
								for (i = 0; i < itemCount_demos; i++) {
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
		switch (screen) {
			case 0:		vita2d_draw_texture( img_topbar0, originX, 0 ); break;
			case 1:		vita2d_draw_texture( img_topbar1, originX, 0 ); break;
			case 2:		vita2d_draw_texture( img_topbar2, originX, 0 ); break;
			case 3:		vita2d_draw_texture( img_topbar3, originX, 0 ); break;
			case 4:		vita2d_draw_texture( img_topbar4, originX, 0 ); break;
			case 5:		vita2d_draw_texture( img_topbar5, originX, 0 ); break;
			case 6:		vita2d_draw_texture( img_topbar6, originX, 0 ); break;
		}


		// SEARCH BUTTON
		vita2d_draw_texture(img_btn_search, (originX +821), 37);
		if (btnState_search == 1) {
			vita2d_draw_rectangle((originX +823), 38, 117, 37, RGBA8( 255, 255, 255, 120 ));
		}
		
		// STATUS BAR
			// Title
			vita2d_font_draw_text(font_25, 12, 22, COLOUR_WHITE, 25, "Vita HomeBrew Browser");

			// BATTERY
			float battery_x = ALIGN_LEFT(949, vita2d_texture_get_width(img_statsbar_battery));
			vita2d_draw_texture(img_statsbar_battery, battery_x, 2);
			float percent = scePowerGetBatteryLifePercent();
			float width = ((29.0f *percent) /100.0f);
			if (scePowerIsLowBattery()) {
				vita2d_draw_rectangle((947.0f -width), 4, width, 16, RGBA8( 255, 48, 48, 255 ));
			}
			else {
				vita2d_draw_rectangle((947.0f -width), 4, width, 16, RGBA8( 91, 223, 38, 255 ));
			}
			/* 
			if (scePowerIsBatteryCharging()) {
				// TODO: add charging Icon
			}
			*/
			
			// DATE & TIME
			SceDateTime time;
			sceRtcGetCurrentClockLocalTime(&time);
			char date_string[16];
			getDateString(date_string, date_format, &time);
			char time_string[24];
			getTimeString(time_string, time_format, &time);
			char string[64];
			sprintf(string, "%s  %s", date_string, time_string);
			vita2d_font_draw_text(font_25, 650, 22, COLOUR_WHITE, 25, string);
			

		// INSTALL ANIMATION
		if (install_animate) {
			install_animate_timer++;
			if (install_animate_timer > 100) {
				install_animate_scale -= 0.02;
				if (install_animate_vs  < 0) {
					install_animate_vs = 0;
				}
				else {
					install_animate_vs -= 1;
				}
				install_animate_x += 10;
				install_animate_y = install_animate_y -install_animate_vs +15;
			}
			if (install_animate_alpha < 235) {
				install_animate_alpha += 15;
			}
			else {
				install_animate_alpha = 255;
			}
			vita2d_draw_rectangle(install_animate_x -3, install_animate_y -3, (84 *install_animate_scale) +6, (84 *install_animate_scale) +6, RGBA8(255, 255, 255, install_animate_alpha));
			switch ( screen ) {
				case 0:		vita2d_draw_texture_tint_scale( iconListNew[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 1:		vita2d_draw_texture_tint_scale( iconListApps[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 2:		vita2d_draw_texture_tint_scale( iconListGames[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 3:		vita2d_draw_texture_tint_scale( iconListEmulators[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 4:		vita2d_draw_texture_tint_scale( iconListUtilities[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 5:		vita2d_draw_texture_tint_scale( iconListThemes[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
				case 6:		vita2d_draw_texture_tint_scale( iconListDemos[previewListNumber], install_animate_x, install_animate_y, install_animate_scale, install_animate_scale, RGBA8( 255, 255, 255, install_animate_alpha ) ); break;
			}
			if (install_animate_x > 960 || install_animate_y > 544) {
				install_animate = 0;
				install_animate_timer = 0;
				install_animate_x = 438;
				install_animate_y = 230;
				install_animate_alpha = 0;
				install_animate_scale = 1;
				install_animate_vs = 30;
			}
		}

		// INSTALL QUE
		if (install_que_count) {
			if (install_que1_alpha < 235) {
				install_que1_alpha += 15;
			}
			else {
				install_que1_alpha = 255;
				install_que2_alpha_timer++;
				if (install_que2_alpha_timer > 120) {
					install_que2_alpha_timer = 121;
					if (install_que2_alpha < 235) {
						install_que2_alpha += 50;
					}
					else {
						install_que2_alpha = 255;
						// PROGRESS ANIMATION
						install_que_progress_timer++;
						if (install_que_progress_timer > 50) {
							install_que_progress_timer = 0;
							install_que_progress++;
							if (install_que_progress > 2) {
								install_que_progress = 0;
							}
						}
					}
				}
			}
		}
		else {
			if (install_que2_alpha > 0) {
				install_que2_alpha -= 15;
			}
			else {
				install_que2_alpha = 0;
				install_que2_alpha_timer = 0;
			}
		}
		vita2d_draw_texture_tint(img_inst_que1, 873, 469, RGBA8(255, 255, 255, install_que1_alpha));
		vita2d_draw_texture_tint(img_inst_que2, 873, 469, RGBA8(255, 255, 255, install_que2_alpha));
		vita2d_font_draw_textf(font_25, 922, 508, RGBA8(255, 255, 255, install_que2_alpha), 25, "%d", install_que_count);
		switch (install_que_progress) {
			case 0:		vita2d_draw_fill_circle(912, 527, 4, RGBA8(30, 255, 0, install_que2_alpha)); break;
			case 1:		vita2d_draw_fill_circle(927, 527, 4, RGBA8(30, 255, 0, install_que2_alpha)); break;
			case 2:		vita2d_draw_fill_circle(942, 527, 4, RGBA8(30, 255, 0, install_que2_alpha)); break;
		}


		// INSTALL DIALOG
		if (install_dialog_show) {
			install_dialog_timer++;
			if (install_dialog_alpha < 235) {
				install_dialog_alpha += 20;
			}
			else {
				install_dialog_alpha = 255;
			}
			if (install_dialog_timer > 250) {
				install_dialog_show = 0;
			}
		}
		else {
			if (install_dialog_alpha > 35 ) {
				install_dialog_alpha -= 20;
			}
			else {
				install_dialog_alpha  =  0;
			}
		}
		vita2d_draw_texture_tint(img_inst_dialog, 344, 407, RGBA8(255, 255, 255, install_dialog_alpha));
		if (install_que_count <= 0) {
			vita2d_draw_texture_tint(img_inst_que1, 873, 469, RGBA8(255, 255, 255, install_dialog_alpha));
		}
		switch (install_dialog_screen) {
			case 0:		vita2d_draw_texture_tint_scale( iconListNew[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListNew[install_dialog_pos].name ); break;
			case 1:		vita2d_draw_texture_tint_scale( iconListApps[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListApps[install_dialog_pos].name ); break;
			case 2:		vita2d_draw_texture_tint_scale( iconListGames[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListGames[install_dialog_pos].name ); break;
			case 3:		vita2d_draw_texture_tint_scale( iconListEmulators[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListEmulators[install_dialog_pos].name ); break;
			case 4:		vita2d_draw_texture_tint_scale( iconListUtilities[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListUtilities[install_dialog_pos].name ); break;
			case 5:		vita2d_draw_texture_tint_scale( iconListThemes[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListThemes[install_dialog_pos].name ); break;
			case 6:		vita2d_draw_texture_tint_scale( iconListDemos[install_dialog_pos], 371, 426, install_dialog_icon_scale, install_dialog_icon_scale, RGBA8( 255, 255, 255, install_dialog_alpha ) ); vita2d_font_draw_textf( font_25, 442, 445, RGBA8( 255, 255, 255, install_dialog_alpha ), 25, "%s\n%Intalled successfully!", catListDemos[install_dialog_pos].name ); break;
		}

		// INSTALL QUE OPEN
		if (install_que_open) {
			if (install_que_posy > 40) {
				install_que_posy -= 40;
			}
			else {
				install_que_posy  = 32;
			}
			// BACKGROUND
			vita2d_draw_rectangle(0, install_que_posy, 960, 512, RGBA8(37, 40, 45, 255));
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
			vita2d_draw_rectangle(0, install_que_posy, 960, 64, RGBA8(28, 30, 33, 255));
			vita2d_font_draw_text(font_25, 438, (install_que_posy +40), COLOUR_WHITE, 25, "Install Que");
			vita2d_draw_texture(img_inst_btn_close, 6, (install_que_posy +5));
			if (btnState_installBtn) {
				vita2d_draw_rectangle( 10, 41, 44, 46, RGBA8(255, 255, 255, 200));
			}
		}
		else {
			if (install_que_posy < 550) {
				install_que_posy += 40;
				vita2d_draw_rectangle(0, install_que_posy, 960, 512, RGBA8(37, 40, 45, 255));
				int p;
				for (i = 0; i < install_que_count; i++) {
					vita2d_draw_texture(img_inst_panel, 62, (install_que_posy +109 +(i *90)));
					vita2d_font_draw_text(font_25, 808, (install_que_posy +185 +(i *itemPanelHeight)), COLOUR_WHITE, 25, downloadQue[i].status);
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
					vita2d_draw_texture(img_inst_panel, 62, (install_que_posy +109 +(i *90) +(install_que_count *90)));
					vita2d_font_draw_text(font_25, 808, (install_que_posy +185 +(i *itemPanelHeight) +(install_que_count *90)), COLOUR_WHITE, 25, "completed");
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
				vita2d_draw_rectangle(0, install_que_posy, 960, 64, RGBA8(37, 40, 45, 255));
				vita2d_font_draw_text(font_25, 438, (install_que_posy +40), COLOUR_WHITE, 25, "Install Que" );
				vita2d_draw_texture(img_inst_btn_close, 6, (install_que_posy +5));
			}
			else {
				install_que_posy  = 550;
			}
		}


		// KEYBOARD
		if (show_input == 1) {
			// UPDATE TEXT
			//vita2d_clear_screen();
			SceCommonDialogStatus status = sceImeDialogGetStatus();
			if (status == IME_DIALOG_RESULT_FINISHED) {
				SceImeDialogResult result;
				memset(&result, 0, sizeof(SceImeDialogResult));
				sceImeDialogGetResult(&result);
				if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE) {
					// USER CANCELED
					status = IME_DIALOG_RESULT_CANCELED;
				}
				else {
					// SUCCESSFULL, SET NAME
					oslOskGetText(userSearch);
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
		if (itemHighlightDir) {
			if (itemHighlight < 245) {
				itemHighlight += 8;
			}
			else {
				itemHighlightDir = !itemHighlightDir;
			}
		}
		else {
			if ( itemHighlight > 0 ) {
				itemHighlight -= 8;
			}
			else {
				itemHighlightDir = !itemHighlightDir;
			}
		}
		
		vita2d_end_drawing();
		vita2d_common_dialog_update();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();
		//-------------------------------------------------------------


		// SIMPLE CHECK FOR INSTALLATION COMPLETE SOUND EFFECT
		if (install_completed) { install_completed = 0; vaudio_play_sound_wav(&snd_install, VHBB_RES_DIR_SOUND "snd_installed.wav"); }
	}


	// CLEANUP/SHUTDOWN -------------------------------------------
	// RELEASE VITA2D
	vita2d_fini();
	// RELEASE SPRITE ARRAYS
	vita2d_free_texture(img_splash);
	vita2d_free_texture(img_bg);
	vita2d_free_texture(img_topbar0);
	vita2d_free_texture(img_topbar1);
	vita2d_free_texture(img_topbar2);
	vita2d_free_texture(img_topbar3);
	vita2d_free_texture(img_topbar4);
	vita2d_free_texture(img_topbar5);
	vita2d_free_texture(img_topbar6);

	vita2d_free_texture(img_btn_search);

	vita2d_free_texture(img_item_panel_new);
	vita2d_free_texture(img_item_panel_apps);
	vita2d_free_texture(img_item_panel_games);
	vita2d_free_texture(img_item_panel_emu);
	vita2d_free_texture(img_item_panel_util);
	vita2d_free_texture(img_item_panel_themes);
	vita2d_free_texture(img_item_panel_demos);

	vita2d_free_texture(img_preview_btn_download);
	vita2d_free_texture(img_preview_btn_download_pressed);
	vita2d_free_texture(img_preview_btn_install);
	vita2d_free_texture(img_preview_btn_install_pressed);
	vita2d_free_texture(img_preview_btn_update);
	vita2d_free_texture(img_preview_btn_update_pressed);

	vita2d_free_texture(img_preview_infobg);
	vita2d_free_texture(img_preview_btn_back);
	vita2d_free_texture(img_preview_btn_back_pressed);

	for (i = 0; i < itemCount_new; i++) { vita2d_free_texture(iconListNew[i]); vita2d_free_texture(previewListNew[i]); }
	for (i = 0; i < itemCount_apps; i++) { vita2d_free_texture(iconListApps[i]); vita2d_free_texture(previewListApps[i]); }
	for (i = 0; i < itemCount_games; i++) { vita2d_free_texture(iconListGames[i]); vita2d_free_texture(previewListGames[i]); }
	for (i = 0; i < itemCount_emu; i++) { vita2d_free_texture(iconListEmulators[i]); vita2d_free_texture(previewListEmulators[i]); }
	for (i = 0; i < itemCount_util; i++) { vita2d_free_texture(iconListUtilities[i]); vita2d_free_texture(previewListUtilities[i]); }
	for (i = 0; i < itemCount_themes; i++) { vita2d_free_texture(iconListThemes[i]); vita2d_free_texture(previewListThemes[i]); }
	for (i = 0; i < itemCount_demos; i++) { vita2d_free_texture(iconListDemos[i]); vita2d_free_texture(previewListDemos[i]); }

	vita2d_free_texture(img_settings_about);
	vita2d_free_texture(img_settings_about_pressed);
	vita2d_free_texture(img_settings_bg);
	vita2d_free_texture(img_settings_cont);
	vita2d_free_texture(img_settings_cont_pressed);
	vita2d_free_texture(img_settings_ctrl);
	vita2d_free_texture(img_settings_ctrl_pressed);
	vita2d_free_texture(img_settings_snd);
	vita2d_free_texture(img_settings_snd_pressed);
	vita2d_free_texture(img_settings_topbar);
	vita2d_free_texture(img_dialog_progress_bg);
	vita2d_free_texture(img_dialog_btn_cancel);
	vita2d_free_texture(img_dialog_btn_cancel_pressed);
	vita2d_free_texture(img_statsbar_battery);
	vita2d_free_texture(img_inst_que1);
	vita2d_free_texture(img_inst_que2);
	vita2d_free_texture(img_inst_dialog);
	vita2d_free_texture(img_inst_btn_close);
	vita2d_free_texture(img_inst_panel);
	
	// RELEASE FONT
	vita2d_free_font(font_16);
	vita2d_free_font(font_20);
	vita2d_free_font(font_25);
	vita2d_free_font(font_30);
	vita2d_free_font(font_35);
	vita2d_free_font(font_40);
	// SHUTDOWN NETWORK
	//?????
	sceKernelExitProcess(0);
	//-------------------------------------------------------------
	return 0;
}