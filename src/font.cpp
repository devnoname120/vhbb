#include "font.h"

#include "debug.h"
#include "unordered_map_pair_hash.h"
#include "utils.h"

#include <algorithm>
#include <string>
#include <vita2d.h>

std::unordered_map<std::pair<std::string, unsigned int>, vita2d_font*> Font::fontCache;

Font::Font(const std::string& path, unsigned int fSize)
{
    // log_printf(DBG_DEBUG, "Looking for size %d, path: %s", fSize, path.c_str());
    auto key = std::make_pair(path, fSize);
    if (fontCache[key])
    {
        // log_printf(DBG_DEBUG, "Found it in cache");
        font = fontCache[key];
        size = fSize;
        return;
    }
    // log_printf(DBG_DEBUG, "Storing in cache...");
    font = vita2d_load_font_file(path.c_str());
    fontCache[key] = font;
    size = fSize;
}

int Font::Draw(const Point& pt, const std::string& text, unsigned int color, unsigned int maxWidth, unsigned int maxHeight)
{
    if (maxWidth > 0 && maxHeight > 0)
    {
        return DrawClip(pt, text, Rectangle(pt, Point(pt.x + maxWidth, pt.y + maxHeight)));
    }
    return vita2d_font_draw_text(font, pt.x, pt.y, color, size, text.c_str());
}

int Font::DrawClip(const Point& pt, const std::string& text, const Rectangle& clipRect, unsigned int color)
{
    bool enabledClipping = false;
    if (!vita2d_get_clipping_enabled())
    {
        enabledClipping = true;
        vita2d_enable_clipping();
    }
    vita2d_set_clip_rectangle(clipRect.topLeft.x, clipRect.topLeft.y, clipRect.bottomRight.x, clipRect.bottomRight.y);
    auto ret = vita2d_font_draw_text(font, pt.x, pt.y, color, size, text.c_str());
    if (enabledClipping)
    {
        vita2d_disable_clipping();
    }
    return ret;
}

int Font::DrawCentered(const Rectangle& rect, const std::string& text, unsigned int color, bool clip)
{
    // log_printf(DBG_DEBUG, "DrawCentered: %f,%f:%f,%f", rect.topLeft.x, rect.topLeft.y, rect.bottomRight.x,
    // rect.bottomRight.y);
    int width, height;
    vita2d_font_text_dimensions(font, size, text.c_str(), &width, &height);
    // log_printf(DBG_DEBUG, "Dimensions: %d, %d", width, height);

    double posX = rect.topLeft.x + (rect.Width() - width) / 2.0;
    // +size/3 roughly aligns the font's median line with the middle of rect
    double posY = rect.topLeft.y + rect.Height() / 2 + size / 3.0 - (height - size) / 2.0;

    // log_printf(DBG_DEBUG, "Pos: %d, %d", posX, posY);
    if (!clip)
    {
        return Draw(Point(posX, posY), text, color);
    }
    else
    {
        return DrawClip(Point(posX, posY), text, rect, color);
    }
}

int Font::DrawCenteredVertical(const Rectangle& rect, const std::string& text, unsigned int color, bool clip)
{
    int height = vita2d_font_text_height(font, size, text.c_str());
    double posY = rect.topLeft.y + rect.Height() / 2 + size / 3.0 - (height - size) / 2.0;

    // log_printf(DBG_DEBUG, "Pos: %d, %d", posX, posY);
    if (!clip)
    {
        return Draw(Point(rect.topLeft.x, posY), text, color);
    }
    else
    {
        return DrawClip(Point(rect.topLeft.x, posY), text, rect, color);
    }
}

std::string Font::FitString(const std::string& text, int maxWidth)
{
    log_printf(DBG_DEBUG, "FitString: \"%s\", %i", text.c_str(), maxWidth);
    int numSpaces = std::count(text.begin(), text.end(), ' ');
    if (!numSpaces)
    {
        return std::string(text);
    }
    std::vector<std::string> words = split_string(text);
    if (words.empty())
        return "";

    std::string current_line = words[0];
    std::string res;
    for (auto i = 1; i < words.size(); i++)
    {
        std::string try_line = current_line + " " + words[i];
        int lineWidth = vita2d_font_text_width(font, size, try_line.c_str());
        if (lineWidth <= maxWidth)
        {
            log_printf(DBG_DEBUG, "\"%s\" fits: %i", words[i].c_str(), lineWidth);
            current_line = std::move(try_line);
        }
        else
        {
            log_printf(DBG_DEBUG, "\"%s\" overflows: %i", words[i].c_str(), lineWidth);
            if (res.empty())
                res += current_line;
            else
                res += "\n" + current_line;
            current_line = words[i];
        }
    }
    if (res.empty())
        res += current_line;
    else
        res += "\n" + current_line;

    log_printf(DBG_DEBUG, "Fitted string: \"%s\"", res.c_str());
    return res;
}
