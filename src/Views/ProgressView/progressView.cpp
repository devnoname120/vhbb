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

    if (99.9 <= progress_.percent() && progress_.percent() <= 100.01) {
        // FIXME hacky
        vita2d_end_drawing();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();
        sceKernelDelayThread(3 * 1000 * 1000);
        vita2d_start_drawing();
        request_destroy = true;
    }
	return 0;
}
