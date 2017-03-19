#include "progressView.h"

#include <font.h>

extern unsigned char _binary_assets_spr_img_dialog_progress_bg_png_start;


ProgressView::ProgressView() :
	font_25(Font(std::string(FONT_DIR "segoeui.ttf"), 25)),
    img_dialog_progress_bg(Texture(&_binary_assets_spr_img_dialog_progress_bg_png_start))
{
}

int ProgressView::HandleInput(int focus, const Input& input)
{
    return 0;
}

int ProgressView::Display()
{
	img_dialog_progress_bg.Draw(Point(PROGRESS_VIEW_X, PROGRESS_VIEW_Y));
    float progress_x = ((float)PROGRESS_BAR_X_WIDTH - (float)PROGRESS_BAR_X) / (float)100 * percent_;
    vita2d_draw_rectangle(PROGRESS_VIEW_X + PROGRESS_BAR_X, PROGRESS_VIEW_Y + PROGRESS_BAR_Y, progress_x, PROGRESS_BAR_Y_HEIGHT, COLOR_BLUEGREY);
    
    font_25.Draw(Point(PROGRESS_VIEW_X + PROGRESS_BAR_X + 10, PROGRESS_VIEW_Y + PROGRESS_BAR_Y + PROGRESS_BAR_Y_HEIGHT + 40), message_);

    if (percent_ == 100) {
        request_destroy = true;
    }
	return 0;
}

void ProgressView::SetProgress(int percent, std::string message)
{
    dbg_printf(DBG_DEBUG, "New progress: %d, %s", percent, message.c_str());
    percent_ = percent;
    message_ = message;  
}

