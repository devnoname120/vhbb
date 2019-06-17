#pragma once



#include <font.h>
#include <screen.h>
#include "View.h"
#include <texture.h>
#include <psp2/types.h>


#define STATUSBAR_HEIGHT 30

class StatusBar: public View {
public:
	StatusBar();

	int Display() override;

	
private:
	Font font_22;
	
	Texture img_statsbar_battery;
	Texture img_statsbar_battery_charge;

	#ifdef _DEBUG
	char vitaip[16] = {0};
	#endif

	void getDateString(char *string, int date_format, SceDateTime *time);
	void getTimeString(char *string, int time_format, SceDateTime *time);
	int displayBattery();
	int displayDate();
};
