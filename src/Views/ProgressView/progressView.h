#pragma once

#include <global_include.h>

#include <Views/View.h>

#include <texture.h>
#include <font.h>

#define PROGRESS_VIEW_X 228
#define PROGRESS_VIEW_Y 155

#define PROGRESS_BAR_X 82
#define PROGRESS_BAR_X_WIDTH 344
#define PROGRESS_BAR_Y 145
#define PROGRESS_BAR_Y_HEIGHT 34

class ProgressView: public View {
public:
	 ProgressView();

	int HandleInput(int focus, const Input& input) override;
	int Display() override;

    void SetProgress(int percent, std::string message);
private:
	Texture img_dialog_progress_bg;

	Font font_25;

    int percent_;
    std::string message_;
};
