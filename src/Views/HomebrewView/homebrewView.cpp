#include "homebrewView.h"

#include <activity.h>
#include <algorithm>
#include <install_thread.h>
#include <network.h>
#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <vitaPackage.h>

extern unsigned char _binary_assets_spr_img_preview_infobg_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_download_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_open_png_start;
extern unsigned char _binary_assets_spr_img_btn_back_png_start;
extern unsigned char _binary_assets_spr_img_btn_back_pressed_png_start;
extern unsigned char _binary_assets_spr_img_preview_btn_youtube_png_start;

std::string wrapDescription(std::string str, size_t width)
{
    // TODO: give ability to specify the number of lines to process.
    // eg. wrapDescription(std::string str, size_t width, int totalLines);
    size_t curWidth = width;
    while (curWidth < str.length())
    {
        std::string::size_type spacePos = str.rfind(' ', curWidth);
        if (spacePos == std::string::npos)
            spacePos = str.find(' ', curWidth);
        if (spacePos != std::string::npos)
        {
            str[spacePos] = '\n';
            curWidth = spacePos + width + 1;
        }
    }
    return str.substr(0, str.size() - 1);
}

HomebrewView::HomebrewView(Homebrew hb)
    : font_20(Font(std::string(FONT_DIR "segoeui.ttf"), 20))
    , font_25(Font(std::string(FONT_DIR "segoeui.ttf"), 25))
    , font_40(Font(std::string(FONT_DIR "segoeui.ttf"), 40))

    , img_preview_infobg(Texture(&_binary_assets_spr_img_preview_infobg_png_start))
    , img_preview_btn_download(Texture(&_binary_assets_spr_img_preview_btn_download_png_start))
    , img_preview_btn_open(Texture(&_binary_assets_spr_img_preview_btn_open_png_start))
    , img_btn_back(Texture(&_binary_assets_spr_img_btn_back_png_start))
    , img_btn_back_pressed(Texture(&_binary_assets_spr_img_btn_back_pressed_png_start))
    , img_preview_btn_youtube(Texture(&_binary_assets_spr_img_preview_btn_youtube_png_start))
    , hb_(hb)
    , img_icon(Texture(std::string(ICONS_FOLDER "/") + hb.icon))
{
    if (!hb_.trailer.empty())
    {
        // https://stackoverflow.com/a/2068371/3634271
        std::string thumbnail_url = std::string("https://img.youtube.com/vi/") + hb_.trailer + "/mqdefault.jpg";

        try
        {
            std::string thumbnail_path = std::string(SCREENSHOTS_FOLDER "/") + hb_.trailer + ".jpg";

            Network::get_instance()->Download(thumbnail_url, thumbnail_path);
            // FIXME Should give false to Texture() so as not to cache but for some reason the destructor is called and so the
            // vita2d resource is freed (cf ~Texture())
            thumbnail = Texture(thumbnail_path);
            sceIoRemove(thumbnail_path.c_str());
        }
        catch (const std::exception& ex)
        {
            log_printf(DBG_ERROR, "Cannot download thumbnail %s", thumbnail_url.c_str());
        }
    }

    // FIXME Support more than 1 screenshot
    if (!hb_.screenshots.empty())
    {
        std::string path = hb_.screenshots.at(0);
        std::size_t found = path.find_last_of('/');
        std::string filename = path.substr(found + 1);

        sceIoMkdir(SCREENSHOTS_FOLDER, 0777);

        try
        {
            Network::get_instance()->Download(
                std::string(SERVER_BASE_URL) + path, std::string(SCREENSHOTS_FOLDER "/") + filename);
            // FIXME Should give false to Texture() so as not to cache but for some reason the destructor is called and so the
            // vita2d resource is freed (cf ~Texture())
            screenshots.emplace_back(std::string(SCREENSHOTS_FOLDER "/") + filename, false);
            sceIoRemove((std::string(SCREENSHOTS_FOLDER "/") + filename).c_str());
        }
        catch (const std::exception& ex)
        {
            log_printf(DBG_ERROR, "Cannot download screenshot %s", path.c_str());
        }
    }

    log_printf(DBG_DEBUG, "Checking if installed");
    checkInstalled();

    std::string descriptionRaw = hb_.long_description;
    std::replace(descriptionRaw.begin(), descriptionRaw.end(), '\n', ' ');
    description = wrapDescription(descriptionRaw, 77);
}

void HomebrewView::homebrewInstall()
{
    try
    {
        auto* args = new InstallArguments;
        args->hb = hb_;

        int install_thid_ = sceKernelCreateThread(
            "install_thread", (SceKernelThreadEntry)install_thread, 0x40, 0x10000, 0, 0, nullptr);
        sceKernelStartThread(install_thid_, sizeof(InstallArguments), args);
        log_printf(DBG_DEBUG, "OK");
    }
    catch (const std::exception& ex)
    {
        log_printf(DBG_ERROR, "%s", ex.what());
    }
}

void HomebrewView::checkInstalled()
{
    try
    {
        installed_ = hb_.IsInstalled();
    }
    catch (const std::exception& ex)
    {
        log_printf(DBG_ERROR, "error checking if installed: %s", ex.what());
    }
    log_printf(DBG_DEBUG, "installed_ = %d", installed_);
}

int HomebrewView::HandleInput(int focus, const Input& input)
{
    if (!focus)
        return 0;

    if (input.TouchNewPressed())
    {
        if (input.TouchInRectangle(Rectangle(Point(HB_X + 218, HB_Y + 168), Point(HB_X + 218 + 153, HB_Y + 168 + 46))))
        {
            log_printf(DBG_DEBUG, "Touch in rectangle for install");
            homebrewInstall();
        }
        else if (
            installed_
            && input.TouchInRectangle(
                Rectangle(Point(HB_X + 218 + 160, HB_Y + 168), Point(HB_X + 218 + 160 + 153, HB_Y + 168 + 46))))
        {
            char uri[32];
            snprintf(uri, sizeof(uri), "psgm:play?titleid=%s", hb_.titleid.c_str());

            sceAppMgrLaunchAppByUri(0x20000, uri);
        }
        else if (input.TouchInTexture(Point(HB_X, SCREEN_HEIGHT - img_btn_back_pressed.Height()), img_btn_back))
        {
            btn_back_pressed = true;
        }
        else if (thumbnail.has_value())
        {
            // FIXME Hack because thumbnail is displayed centered
            float width = vita2d_texture_get_width(thumbnail->texture.get());
            float height = vita2d_texture_get_height(thumbnail->texture.get());
            auto pt = Point(HB_X + 560 + 376.f / 2, HB_Y + 110 + 210.f / 2);
            Point top_left = Point(pt.x - width / 2, pt.y - height / 2);
            Point bottom_right = Point(top_left.x + width, top_left.y + height);

            if (input.TouchInRectangle(Rectangle(Point(HB_X + 560, HB_Y + 110), Point(HB_X + 560 + 376, HB_Y + 110 + 210))))
            {
                startYoutube();
            }
        }
    }
    else if (
        input.TouchPressed() && !input.TouchInTexture(Point(HB_X, SCREEN_HEIGHT - img_btn_back_pressed.Height()), img_btn_back))
    {
        btn_back_pressed = false;
    }
    else if (!input.TouchPressed() && btn_back_pressed)
    {
        request_destroy = true;
    }
    else if (input.KeyNewPressed(SCE_CTRL_CANCEL))
    {
        request_destroy = true;
    }
    else if (input.KeyNewPressed(SCE_CTRL_ENTER))
    {
        homebrewInstall();
    }
    else if (input.KeyNewPressed(SCE_CTRL_SQUARE))
    {
        if (!hb_.trailer.empty())
        {
            startYoutube();
        }
    }

    return 0;
}
void HomebrewView::startYoutube() const
{
    sceAppMgrLaunchAppByUri(
        0x20000, (std::string("webmodal: https://www.youtube.com/embed/") + hb_.trailer + "?autoplay=1").c_str());
}

int HomebrewView::Display()
{
    bg.Display();
    sb.Display();

    img_preview_infobg.Draw(Point(HB_X, HB_Y + 300));

    font_40.Draw(Point(HB_X + 225, HB_Y + 88), hb_.name, COLOR_WHITE);
    font_25.Draw(Point(HB_X + 225, HB_Y + 115), hb_.author, COLOR_AQUA);
    font_25.Draw(Point(HB_X + 225, HB_Y + 144), hb_.version, COLOR_WHITE);
    // font_20.Draw(Point(HB_X + 100, HB_Y + 189), std::string("0 Kb"), COLOR_WHITE);
    // font_20.Draw(Point(HB_X + 850, HB_Y + 503), hb_.date.str, COLOR_WHITE);

    font_25.Draw(Point(HB_X + 40, HB_Y + 362), description);

    img_preview_btn_download.Draw(Point(HB_X + 218, HB_Y + 168));

    if (installed_)
    {
        img_preview_btn_open.Draw(Point(HB_X + 218 + 160, HB_Y + 168));
    }

    img_icon.DrawResize(Point(HB_X + 122, HB_Y + 60), Point(90, 90));

    if (thumbnail.has_value())
    {
        thumbnail->DrawResize(Point(HB_X + 560, HB_Y + 110), Point(376, 210));
    }
    else if (!screenshots.empty())
    {
        // FIXME Images aren't all fullscreen-sized
        screenshots.at(0).DrawResize(Point(HB_X + 560, HB_Y + 110), Point(376, 210));
    }
    // else draw a grey rectangle

    if (!hb_.trailer.empty())
    {
        img_preview_btn_youtube.DrawCentered(Point(HB_X + 560 + 376.f / 2, HB_Y + 110 + 210.f / 2));
    }

    if (btn_back_pressed)
        img_btn_back_pressed.Draw(Point(HB_X, SCREEN_HEIGHT - img_btn_back_pressed.Height()));
    else
        img_btn_back.Draw(Point(HB_X, SCREEN_HEIGHT - img_btn_back.Height()));

    return 0;
}
