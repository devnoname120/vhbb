#include "homebrewView.h"

#include <vitaPackage.h>
#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <install_thread.h>

#include <texture.h>
#include <network.h>

extern unsigned char _binary_assets_spr_img_preview_infobg_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_download_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_open_png_start;


std::string wrapDescription(std::string str, size_t width) {
	// TODO: give ability to specify the number of lines to process.
	// eg. wrapDescription(std::string str, size_t width, int totalLines);
    size_t curWidth = width;
    while (curWidth < str.length()) {
        std::string::size_type spacePos = str.rfind(' ', curWidth);
        if (spacePos == std::string::npos)
            spacePos = str.find(' ', curWidth);
        if (spacePos != std::string::npos) {
            str[spacePos] = '\n';
            curWidth = spacePos + width + 1;
		}
	}
    return str.substr(0, str.size() - 1);
}


HomebrewView::HomebrewView(Homebrew hb) :
	font_20(Font(std::string(FONT_DIR "segoeui.ttf"), 20)),
	font_25(Font(std::string(FONT_DIR "segoeui.ttf"), 25)),
	font_40(Font(std::string(FONT_DIR "segoeui.ttf"), 40)),

	img_preview_infobg(Texture(&_binary_assets_spr_img_preview_infobg_png_start)),
	img_preview_btn_download(Texture(&_binary_assets_spr_img_preview_btn_download_png_start)),
	img_preview_btn_open(Texture(&_binary_assets_spr_img_preview_btn_open_png_start)),
	hb_(hb),
	img_icon(Texture(ICONS_FOLDER + "/" + hb.icon))
{
	// FIXME Support more than 1 screenshot
	if (!hb_.screenshots.empty()) {
		std::string path = hb_.screenshots.at(0);
		std::size_t found = path.find_last_of('/');
		std::string filename = path.substr(found+1);

		sceIoMkdir(SCREENSHOTS_FOLDER.c_str(), 0777);

		try {
			Network::get_instance()->Download(SERVER_BASE_URL + path, SCREENSHOTS_FOLDER + "/" + filename);
			// FIXME Should give false to Texture() so as not to cache but for some reason the destructor is called and so the vita2d resource is freed (cf ~Texture())
			screenshots.emplace_back(SCREENSHOTS_FOLDER + "/" + filename, false);
			sceIoRemove((SCREENSHOTS_FOLDER + "/" + filename).c_str());
		} catch (const std::exception &ex) {
            log_printf(DBG_ERROR, "Cannot download screenshot %s", path.c_str());
		}
	}

    log_printf(DBG_DEBUG, "Checking if installed");
	checkInstalled();
	
	std::string descriptionRaw = hb_.long_description;
	std::replace(descriptionRaw.begin(), descriptionRaw.end(), '\n', ' ');
	description = wrapDescription(descriptionRaw, 77);
	
	/*
	std::string long_description_cut_draft = hb_.long_description;
	std::replace(long_description_cut_draft.begin(), long_description_cut_draft.end(), '\n', ' ');

	long_description_cut1 = long_description_cut_draft.substr(0, 77);
	try {
		long_description_cut2 = long_description_cut_draft.substr(77, 77);
		long_description_cut3 = long_description_cut_draft.substr(155, 77);
		long_description_cut4 = long_description_cut_draft.substr(232, 73);
		if (long_description_cut_draft.length() >= 305) long_description_cut4 += "...";

	} catch (const std::out_of_range &ex) {
		// No problem
	} catch (const std::exception &ex) {
		log_printf(DBG_WARNING, "Error when cutting description: %s", ex.what());
	}
*/
}

void HomebrewView::homebrewInstall() {
	try {
		auto *args = new InstallArguments;
		args->hb = hb_;

		int install_thid_ = sceKernelCreateThread("install_thread", (SceKernelThreadEntry)install_thread, 0x40, 0x10000, 0, 0, nullptr);
		sceKernelStartThread(install_thid_, sizeof(InstallArguments), args);
        log_printf(DBG_DEBUG, "OK");
	} catch (const std::exception &ex) {
        log_printf(DBG_ERROR, "%s", ex.what());
	}
}

void HomebrewView::checkInstalled()
{
	try {
		installed_ = hb_.IsInstalled();
	} catch (const std::exception &ex) {
        log_printf(DBG_ERROR, "error checking if installed: %s", ex.what());
	}
    log_printf(DBG_DEBUG, "installed_ = %d", installed_);
}

int HomebrewView::HandleInput(int focus, const Input& input)
{
	if (!focus)
		return 0;

	if (input.TouchNewPressed()) {
		if (input.TouchInRectangle(Rectangle(Point(HB_X + 218, HB_Y + 168), Point(HB_X + 218 + 153, HB_Y + 168 + 46)))) {
            log_printf(DBG_DEBUG, "Touch in rectangle for install");
			homebrewInstall();
		} else if (installed_ && input.TouchInRectangle(Rectangle(Point(HB_X + 218 + 160, HB_Y + 168), Point(HB_X + 218 + 160 + 153, HB_Y + 168 + 46)))) {
			char uri[32];
			snprintf(uri, sizeof(uri), "psgm:play?titleid=%s", hb_.titleid.c_str());

			sceAppMgrLaunchAppByUri(0x20000, uri);
		}
	} else if (input.KeyNewPressedNoRepeat(SCE_CTRL_CIRCLE)) {
		request_destroy = true;
	} else if (input.KeyNewPressedNoRepeat(SCE_CTRL_CROSS)) {
		homebrewInstall();
	}

	return 0;
}

int HomebrewView::Display()
{
	bg.Display();
    sb.Display();

	img_preview_infobg.Draw(Point(HB_X, HB_Y + 300));

	font_40.Draw(Point(HB_X + 225, HB_Y + 88), hb_.name, COLOR_WHITE);
	font_25.Draw(Point(HB_X + 225, HB_Y + 115), hb_.author, COLOR_AQUA);
	font_25.Draw(Point(HB_X + 225, HB_Y + 144), hb_.version, COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 100, HB_Y + 189), std::string("0 Kb"), COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 850, HB_Y + 503), hb_.date.str, COLOR_WHITE);
	/*
	if (!hb_.long_description.empty()) {
		font_25.Draw(Point(HB_X + 40, HB_Y + 362), long_description_cut1);
		if (!long_description_cut2.empty()) font_25.Draw(Point(HB_X + 40, HB_Y + 362 + 40), long_description_cut2);
		if (!long_description_cut3.empty()) font_25.Draw(Point(HB_X + 40, HB_Y + 362 + 80), long_description_cut3);
		if (!long_description_cut4.empty()) font_25.Draw(Point(HB_X + 40, HB_Y + 362 + 120), long_description_cut4);
	}
	*/
	
	font_25.Draw(Point(HB_X + 40, HB_Y + 362), description);

	img_preview_btn_download.Draw(Point(HB_X + 218, HB_Y + 168));


	if (installed_) {
		img_preview_btn_open.Draw(Point(HB_X + 218 + 160, HB_Y + 168));
	}

	img_icon.DrawResize(Point(HB_X + 122, HB_Y + 60), Point(90, 90));

	if (!screenshots.empty()) {
		// FIXME Images aren't all fullscreen-sized
		screenshots.at(0).DrawResize(Point(HB_X + 560, HB_Y + 110), Point(376, 210));
	}
	// else draw a grey rectangle


	return 0;
}
