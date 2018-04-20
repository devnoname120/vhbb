#pragma once

#include <global_include.h>

#include "View.h"
#include <texture.h>

typedef enum {
	STEP_FADING_IN,
	STEP_STATIC,
	STEP_FADING_OUT,
} SplashStep;

class Splash : public View
{
public:
	Splash();

	int Display() override;
private:
	unsigned int splash_index = 0;
	SplashStep step = STEP_FADING_IN;
	unsigned int alpha = 0;
	unsigned int frame_count = 0;

	Texture vhbb_splash;
	Texture gekihen_splash;
};
