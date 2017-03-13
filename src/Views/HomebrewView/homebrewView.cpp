#include "homebrewView.h"


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
	img_icon.DrawResize(Point(HB_X + 100, HB_Y + 82), Point(HB_X + 100 + 90, HB_Y + 82 + 90));
	//hb_.screenshot[0].Draw(Point(HB_X + 566, HB_Y + 67));


	return 0;
}

