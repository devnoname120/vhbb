#include "homebrewView.h"
#include <vitaPackage.h>
#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <install_thread.h>

#include <texture.h>
#include <network.h>

extern unsigned char _binary_assets_spr_img_preview_infobg_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_download_png_start;


HomebrewView::HomebrewView(Homebrew hb) :
	font_20(Font(std::string(FONT_DIR "segoeui.ttf"), 20)),
	font_25(Font(std::string(FONT_DIR "segoeui.ttf"), 25)),
	font_40(Font(std::string(FONT_DIR "segoeui.ttf"), 40)),

	img_preview_infobg(Texture(&_binary_assets_spr_img_preview_infobg_png_start)),
	img_preview_btn_download(Texture(&_binary_assets_spr_img_preview_btn_download_png_start)),
	hb_(hb),
	img_icon(Texture(ICONS_FOLDER + "/" + hb.icon))
{
	// FIXME Support more than 1 screenshot
	if (!hb_.screenshots.empty()) {
		std::string path = hb_.screenshots.at(0);
		std::size_t found = path.find_last_of("/");
		std::string filename = path.substr(found+1);

		sceIoMkdir(SCREENSHOTS_FOLDER.c_str(), 0777);

		sceIoRemove(SCREENSHOTS_FOLDER.c_str());

		Network::get_instance()->Download(SERVER_BASE_URL + path, SCREENSHOTS_FOLDER + "/" + filename);
		// FIXME Should give false to Texture() so as not to cache but for some reason the destructor is called and so the vita2d resource is freed (cf ~Texture())
		screenshots.push_back(Texture(SCREENSHOTS_FOLDER + "/" + filename, false));
	}

}

void HomebrewView::homebrewInstall() {
	try {
		InstallArguments *args = new InstallArguments;
		args->url = hb_.url;
		SceUID thid2 = sceKernelCreateThread("install_thread", (SceKernelThreadEntry)install_thread, 0x40, 0x10000, 0, 0, NULL);
		sceKernelStartThread(thid2, sizeof(InstallArguments), args);
		dbg_printf(DBG_DEBUG, "OK");
	} catch (const std::exception &ex) {
		dbg_printf(DBG_ERROR, "%s", ex.what());
	}
}

int HomebrewView::HandleInput(int focus, const Input& input)
{
	if (!focus)
		return 0;
	
	if (input.TouchNewPressed()) {
		if (input.TouchInRectangle(Rectangle(Point(151, 43), Point(151 + 202, 43 + 217)))) {
			dbg_printf(DBG_DEBUG, "Touch in rectangle for install");
			homebrewInstall();
		}
	} else if (input.KeyNewPressed(SCE_CTRL_CIRCLE)) {
		request_destroy = true;
	} else if (input.KeyNewPressed(SCE_CTRL_CROSS)) {
		homebrewInstall();
	}
	// TODO Implement
	return 0;
}

int HomebrewView::Display()
{
	img_preview_infobg.Draw(Point(HB_X, HB_Y + 300));

	font_40.Draw(Point(HB_X + 225, HB_Y + 88), hb_.name, COLOR_WHITE);
	font_25.Draw(Point(HB_X + 225, HB_Y + 115), hb_.author, COLOR_AQUA);
	font_25.Draw(Point(HB_X + 225, HB_Y + 144), hb_.version, COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 100, HB_Y + 189), std::string("0 Kb"), COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 850, HB_Y + 503), hb_.date.str, COLOR_WHITE);
	// FIXME: Display real description
	if (!hb_.long_description.empty()) {
		std::string cut = hb_.long_description.substr(0, 70) + "...";
		font_25.Draw(Point(HB_X + 40, HB_Y + 319 + 80), cut);
	}
	

	img_preview_btn_download.Draw(Point(HB_X + 218, HB_Y + 168));
	img_icon.DrawResize(Point(HB_X + 122, HB_Y + 60), Point(90, 90));

	if (!screenshots.empty()) {
		// FIXME Images aren't all fullscreen-sized
		screenshots.at(0).DrawResize(Point(HB_X + 560, HB_Y + 110), Point(376, 210));
	}
	// else draw a grey rectangle


	return 0;
}

