#include "listItem.h"

#include <font.h>
#include <texture.h>

extern unsigned char _binary_assets_spr_img_itm_panel_png_start;
extern unsigned char _binary_assets_spr_img_itm_panel_highlight_png_start;
extern unsigned char _binary_assets_spr_img_itm_label_game_png_start;
extern unsigned char _binary_assets_spr_img_itm_label_port_png_start;
extern unsigned char _binary_assets_spr_img_itm_label_emu_png_start;
extern unsigned char _binary_assets_spr_img_itm_label_util_png_start;
extern unsigned char _binary_assets_spr_img_update_available_sticker_png_start;

ListItem::ListItem(Homebrew hb)
    : homebrew(hb)
    , isUpdateAvailable(hb.IsUpdateAvailable())
    , font_22(Font(std::string(FONT_DIR "segoeui.ttf"), 22))
    , font_32(Font(std::string(FONT_DIR "segoeui.ttf"), 32))
    , img_itm_panel(Texture(&_binary_assets_spr_img_itm_panel_png_start))
    , img_itm_panel_highlight(Texture(&_binary_assets_spr_img_itm_panel_highlight_png_start))
    , img_icon_(Texture(std::string(ICONS_FOLDER "/") + hb.icon))
    , img_itm_label_game(Texture(&_binary_assets_spr_img_itm_label_game_png_start))
    , img_itm_label_port(Texture(&_binary_assets_spr_img_itm_label_port_png_start))
    , img_itm_label_emu(Texture(&_binary_assets_spr_img_itm_label_emu_png_start))
    , img_itm_label_util(Texture(&_binary_assets_spr_img_itm_label_util_png_start))
    , img_update_available_sticker(Texture(&_binary_assets_spr_img_update_available_sticker_png_start))

{
}

int ListItem::Display(int posY, int highlight, int highlightAlpha)
{
    display(posY);
    if (highlight)
        displayHighlight(posY, highlightAlpha);
    return 0;
}

int ListItem::display(int posY)
{
    img_itm_panel.Draw(Point(ITEM_POSX, posY));
    // log_printf(DBG_DEBUG, "Drawing title: %s", homebrew.title.c_str());
    font_32.Draw(Point(ITEM_POSX + 120, posY + 30), homebrew.name);
    font_22.Draw(Point(ITEM_POSX + 122, posY + 76), homebrew.author);
    font_22.Draw(Point(ITEM_POSX + 776, posY + 76), homebrew.date.str);
    // font_35.Draw(Point(ITEM_POSX + 120, posY + 33), homebrew.version);
    // font_35.Draw(Point(ITEM_POSX + 120, posY + 33), homebrew.release);

    img_icon_.DrawResize(Point(ITEM_POSX, posY), Point(90, 90));

    if (isUpdateAvailable)
        img_update_available_sticker.Draw(Point(ITEM_POSX, posY));


    // TODO: Is there a better way of handling this? Yes i'm talking to You good sir :-)
    if (homebrew.category == "1")
        img_itm_label_game.Draw(Point(ITEM_POSX + 732, posY));
    else if (homebrew.category == "2")
        img_itm_label_port.Draw(Point(ITEM_POSX + 732, posY));
    else if (homebrew.category == "5")
        img_itm_label_emu.Draw(Point(ITEM_POSX + 732, posY));
    else if (homebrew.category == "4")
        img_itm_label_util.Draw(Point(ITEM_POSX + 732, posY));

    return 0;
}

int ListItem::displayHighlight(int posY, int alpha)
{
    img_itm_panel_highlight.DrawExt(Point(ITEM_POSX, posY), alpha);
    return 0;
}
