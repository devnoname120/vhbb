#include "progressView.h"

#include <font.h>

extern unsigned char _binary_assets_spr_img_dialog_progress_bg_png_start;


ProgressView::ProgressView(InfoProgress progress) :
    progress_(progress),
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
    float progress_x = ((float)PROGRESS_BAR_X_WIDTH) * ((float)progress_.percent() / (float)100);
    vita2d_draw_rectangle(PROGRESS_VIEW_X + PROGRESS_BAR_X, PROGRESS_VIEW_Y + PROGRESS_BAR_Y, progress_x, PROGRESS_BAR_Y_HEIGHT, COLOR_BLUEGREY);
    
    font_25.Draw(Point(PROGRESS_VIEW_X + PROGRESS_BAR_X, PROGRESS_VIEW_Y + 72), progress_.message());

    if (finish_tick != 0 && sceKernelGetProcessTimeLow() > finish_tick) request_destroy = true;
	return 0;
}

void ProgressView::Finish(uint wait)
{
    if (wait == 0) {request_destroy = true; return;}

    finish_tick = sceKernelGetProcessTimeLow() + wait * 1000;
}
