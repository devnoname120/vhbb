#pragma once

#include "texture.h"
#include "scrollManager.h"

#include <Views/ListView/listView.h>
#include <Views/View.h>
#include <Views/background.h>
#include <Views/statusBar.h>

#define HB_X 0
#define HB_Y 30

class HomebrewView : public View
{
public:
    explicit HomebrewView(Homebrew hb);

    int HandleInput(int focus, const Input& input) override;
    int Display() override;

private:
    Font font_20;
    Font font_25;
    Font font_40;

    Texture img_preview_infobg;
    Texture img_preview_btn_download;
    Texture img_preview_btn_open;
    Texture img_btn_back;
    Texture img_btn_back_pressed;
    bool btn_back_pressed = false;

    Texture img_preview_btn_youtube;

    // FIXME Hack otherwise we see the other views beneath
    Background bg;
    StatusBar sb;

    Homebrew hb_;
    bool installed_ = false;
    Texture img_icon;

    std::vector<Texture> screenshots;
    std::optional<Texture> thumbnail;

    std::string description = "";
    ScrollManager<false, true> scrollManager;
    int description_scroll = 0;

    void checkInstalled();
    void homebrewInstall();
    void startYoutube() const;
    std::string sizeRoundAndString(int);
};
