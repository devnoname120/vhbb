#pragma once

#include <global_include.h>

#include "texture.h"
#include <Views/View.h>
#include <Views/ListView/listView.h>

#define HB_X 0
#define HB_Y 30


class HomebrewView: public View {
public:
	HomebrewView(Homebrew hb);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;


private:
	Texture img_preview_infobg;
	Texture img_preview_btn_download;

	Font font_20;
	Font font_25;
	Font font_40;

	Homebrew hb_;
	Texture img_icon;

	std::vector<Texture> screenshots;

	void homebrewInstall();

};

