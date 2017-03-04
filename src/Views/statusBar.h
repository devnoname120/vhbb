#pragma once

#include <global_include.h>

#include <font.h>
#include <screen.h>
#include "View.h"


#define STATUSBAR_HEIGHT 30

class StatusBar: public View {
public:
	StatusBar();

	int Display() override;

	
private:
	vita2d_font *font_25;
	vita2d_texture *img_statsbar_battery;

	#ifdef PSP2SHELL
	char vitaip[16] = {0};
	#endif

	void getDateString(char *string, int date_format, SceDateTime *time);
	void getTimeString(char *string, int time_format, SceDateTime *time);
	int displayBattery();
	int displayDate();
};
