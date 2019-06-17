#include <utility>

#include "progressView.h"

#include <debug.h>
#include <psp2/kernel/processmgr.h>

extern unsigned char _binary_assets_spr_img_dialog_progress_bg_png_start;
extern unsigned char _binary_assets_spr_img_dialog_progress_bar_png_start;
extern unsigned char _binary_assets_spr_img_dialog_progress_bar_glow_png_start;
extern unsigned char _binary_assets_spr_img_dialog_btn_png_start;


ProgressView::ProgressView(InfoProgress progress, Homebrew hb) :
	ProgressView(std::move(progress), hb.name, std::string(ICONS_FOLDER "/") + hb.icon)
{
}

ProgressView::ProgressView(InfoProgress progress, std::string hb_name, std::string icon_path) :
	ProgressView(std::move(progress), std::move(hb_name), Texture(icon_path))
{
}
ProgressView::ProgressView(InfoProgress progress, std::string hb_name, Texture icon_texture) :
	hb_name(std::move(hb_name)),
	progress_(std::move(progress)),
	font_24(Font(std::string(FONT_DIR "segoeui.ttf"), 24)),
	//thid_(thid),
	img_icon(icon_texture),
	img_dialog_progress_bg(Texture(&_binary_assets_spr_img_dialog_progress_bg_png_start)),
	img_dialog_progress_bar(Texture(&_binary_assets_spr_img_dialog_progress_bar_png_start)),
	img_dialog_progress_bar_glow(Texture(&_binary_assets_spr_img_dialog_progress_bar_glow_png_start)),
	img_dialog_btn(Texture(&_binary_assets_spr_img_dialog_btn_png_start))
{
}

int ProgressView::HandleInput(int focus, const Input& input)
{
	if (input.TouchNewPressed() && input.TouchInTexture(Point(PROGRESS_VIEW_X + 148, PROGRESS_VIEW_Y + 178), img_dialog_btn)) {
        log_printf(DBG_WARNING, "Cancel not implemented");
		//progress_.Finish(3000);
	}
    return 0;
}

int ProgressView::Display()
{
	float progress_x = ((float)PROGRESS_BAR_X_WIDTH) * (progress_.percent() / (float)100);
	// Icon
	img_icon.Draw(Point(PROGRESS_VIEW_X + 45, PROGRESS_VIEW_Y + 7));
	// Background
	img_dialog_progress_bg.Draw(Point(PROGRESS_VIEW_X, PROGRESS_VIEW_Y));
	// Name
	font_24.Draw(Point(PROGRESS_VIEW_X + 197, PROGRESS_VIEW_Y + 53), hb_name);
	// Message
	font_24.Draw(Point(PROGRESS_VIEW_X + 197, PROGRESS_VIEW_Y + 117), progress_.message());
    // Progress bar
    img_dialog_progress_bar.DrawResize(Point(PROGRESS_VIEW_X + 55, PROGRESS_VIEW_Y + 148), Point(progress_x, 11));
	// if ((float)progress_.percent() > progressPrevious)
	img_dialog_progress_bar_glow.Draw(Point(PROGRESS_VIEW_X + progress_x + 5, PROGRESS_VIEW_Y + 104));
	// Cancel button
	//img_dialog_btn.Draw(Point(PROGRESS_VIEW_X + 148, PROGRESS_VIEW_Y + 178));
	// Percent (if required)
	//font_24.Draw(Point(PROGRESS_VIEW_X + 197, PROGRESS_VIEW_Y + 117), progress_.percent());

    if (finish_tick != 0 && sceKernelGetProcessTimeLow() > finish_tick) request_destroy = true;
	//progressPrevious = (float)progress_.percent();
	return 0;
}

void ProgressView::Finish(unsigned int wait)
{
    if (wait == 0) {request_destroy = true; return;}

    finish_tick = sceKernelGetProcessTimeLow() + wait * 1000;
}
