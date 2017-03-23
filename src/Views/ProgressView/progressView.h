#pragma once

#include <global_include.h>

#include <Views/View.h>

#include "infoProgress.h"

#include <texture.h>
#include <font.h>

#define PROGRESS_VIEW_X 227
#define PROGRESS_VIEW_Y 221

#define PROGRESS_BAR_X 83
#define PROGRESS_BAR_X_WIDTH 342
#define PROGRESS_BAR_Y 101
#define PROGRESS_BAR_Y_HEIGHT 20

class ProgressView: public View {
public:
	 ProgressView(InfoProgress progress);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;
private:
	InfoProgress progress_;

	Font font_25;
	Texture img_dialog_progress_bg;
};
