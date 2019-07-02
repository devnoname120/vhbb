#pragma once



#include <Views/View.h>
#include <homebrew.h>

#include "infoProgress.h"

#include <texture.h>
#include <font.h>


#define PROGRESS_VIEW_X 171
#define PROGRESS_VIEW_Y 152

#define PROGRESS_BAR_X_WIDTH 508

class ProgressView: public View {
public:
	ProgressView(InfoProgress progress, Homebrew hb);
	ProgressView(InfoProgress progress, std::string hb_name, std::string icon_path);

	template <typename TCachingPolicy>
	ProgressView(InfoProgress progress, std::string hb_name, Texture <TCachingPolicy> icon_texture);

	int HandleInput(int focus, const Input& input) override;
	int Display() override;
	// Wait in millisecond
	void Finish(unsigned int wait = 300);

	std::string hb_name;
private:
	uint32_t finish_tick = 0;

	InfoProgress progress_;

	Font font_24;
	CachedTexture img_icon;
	CachedTexture img_dialog_progress_bg;
	CachedTexture img_dialog_progress_bar;
	CachedTexture img_dialog_progress_bar_glow;
	CachedTexture img_dialog_btn;
};
