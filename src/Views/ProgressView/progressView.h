#pragma once

#include <global_include.h>

#include <Views/View.h>
#include <homebrew.h>

#include "infoProgress.h"
#include "../../CancelHandler.h"
#include "../../../../../../../usr/local/vitasdk/arm-vita-eabi/include/c++/7.3.0/optional"

#include <texture.h>
#include <font.h>
#include <CancelHandler.h>


#define PROGRESS_VIEW_X 171
#define PROGRESS_VIEW_Y 152

#define PROGRESS_BAR_X 55
#define PROGRESS_BAR_X_WIDTH 508
#define PROGRESS_BAR_Y 148
#define PROGRESS_BAR_Y_HEIGHT 11

class ProgressView: public View {
public:
	ProgressView(InfoProgress progress, Homebrew hb, CancelHandler *cancelHandler = nullptr);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;
	// Wait in millisecond
	void Finish(unsigned int wait = 300);
private:
	uint32_t finish_tick = 0;

    InfoProgress progress_;
    CancelHandler *m_cancelHandler = nullptr;

	Homebrew hb_;

	Font font_24;
	Texture img_icon;
	Texture img_dialog_progress_bg;
	Texture img_dialog_progress_bar;
	Texture img_dialog_progress_bar_glow;
	Texture img_dialog_btn;
};
