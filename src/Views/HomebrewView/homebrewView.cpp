#include "homebrewView.h"

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

		Network::get_instance()->Download(SERVER_BASE_URL + path, SCREENSHOTS_FOLDER + "/" + filename);
		screenshots.push_back(Texture(SCREENSHOTS_FOLDER + "/" + filename));
	}

}

int HomebrewView::HandleInput(int focus, const Input& input)
{
	// TODO Implement
	return 0;
}

int HomebrewView::Display()
{
	img_preview_infobg.Draw(Point(HB_X, HB_Y + 246));

	font_40.Draw(Point(HB_X + 203, HB_Y + 110), hb_.name, COLOR_WHITE);
	font_25.Draw(Point(HB_X + 203, HB_Y + 136), hb_.author, COLOR_AQUA);
	font_25.Draw(Point(HB_X + 203, HB_Y + 169), hb_.version, COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 100, HB_Y + 189), std::string("0 Kb"), COLOR_WHITE);
	//font_20.Draw(Point(HB_X + 850, HB_Y + 503), hb_.date.str, COLOR_WHITE);
	// FIXME: Display real description
	font_25.Draw(Point(HB_X + 40, HB_Y + 319), std::string("this is a long description"));

	img_preview_btn_download.Draw(Point(HB_X + 201, HB_Y + 187));
	img_icon.DrawResize(Point(HB_X + 100, HB_Y + 82), Point(90, 90));

	if (!screenshots.empty()) {
		// FIXME Images aren't all fullscreen-sized
		screenshots.at(0).DrawResize(Point(HB_X + 466, HB_Y + 67), Point(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	}
	// else draw a grey rectangle


	return 0;
}

