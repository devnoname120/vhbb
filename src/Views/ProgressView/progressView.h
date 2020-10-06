#pragma once



#include <Views/View.h>
#include <homebrew.h>

#include "infoProgress.h"
#include "concurrency.h"

#include <texture.h>
#include <font.h>


#define PROGRESS_VIEW_X 171
#define PROGRESS_VIEW_Y 152

#define PROGRESS_BAR_X_WIDTH 508

class ProgressView: public View {
public:
	ProgressView(InfoProgress progress, Homebrew hb);
	ProgressView(InfoProgress progress, std::string hb_name, std::string icon_path);
	ProgressView(InfoProgress progress, std::string hb_name, Texture icon_texture);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;
	// Wait in millisecond
	void Finish(unsigned int wait = 300);

	std::string hb_name;
private:
	std::atomic<uint32_t> finish_tick = 0;

	InfoProgress progress_;

	Font font_24;
	Texture img_icon;
	Texture img_dialog_progress_bg;
	Texture img_dialog_progress_bar;
	Texture img_dialog_progress_bar_glow;
	Texture img_dialog_btn;
};
