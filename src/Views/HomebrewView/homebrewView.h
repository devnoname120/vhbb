#pragma once

#include "texture.h"

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

    CachedTexture img_preview_infobg;
    CachedTexture img_preview_btn_download;
    CachedTexture img_preview_btn_open;
    CachedTexture img_btn_back;
    CachedTexture img_btn_back_pressed;
    bool btn_back_pressed = false;

    CachedTexture img_preview_btn_youtube;

    // FIXME Hack otherwise we see the other views beneath
    Background bg;
    StatusBar sb;

    Homebrew hb_;
    bool installed_ = false;
    CachedTexture img_icon;

    std::vector<UncachedTexture> screenshots;
    std::optional<UncachedTexture> thumbnail;

    std::string description = "";

    void checkInstalled();
    void homebrewInstall();
    void startYoutube() const;
};
