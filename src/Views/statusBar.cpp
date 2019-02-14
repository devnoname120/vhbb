#include "statusBar.h"
#include <global_include.h>

#include <texture.h>

extern unsigned char _binary_assets_spr_img_statsbar_battery_png_start;
extern unsigned char _binary_assets_spr_img_statsbar_battery_charge_png_start;

StatusBar::StatusBar() :
	font_22(Font(std::string(FONT_DIR "segoeui.ttf"), 22)),
	img_statsbar_battery(Texture(&_binary_assets_spr_img_statsbar_battery_png_start)),
	img_statsbar_battery_charge(Texture(&_binary_assets_spr_img_statsbar_battery_charge_png_start))
{
	#ifdef _DEBUG

	SceNetCtlInfo info;
	if(sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_ADDRESS, &info) < 0) {
        log_printf(DBG_ERROR, "Failed to obtain Vita IP address");
	} else {
		strncpy(vitaip, (char *)&(info.ip_address), 16);
	}

	#endif
}


int StatusBar::Display()
{
	// Background
	vita2d_draw_rectangle(0, 0, SCREEN_WIDTH, STATUSBAR_HEIGHT, COLOR_BLACK);

	font_22.DrawFromBaseline(Point(15, 22), "Vita HomeBrew Browser", COLOR_WHITE);

	displayBattery();
	displayDate();

	#ifdef _DEBUG
	font_22.DrawFromBaseline(Point(400, 22), vitaip, COLOR_WHITE);
	#endif
	return 0;
}


void StatusBar::getDateString(char *string, int date_format, SceDateTime *time)
{
	switch (date_format) {
		case SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD:
			sprintf(string, "%04d/%02d/%02d", time->year, time->month, time->day);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY:
			sprintf(string, "%02d/%02d/%04d", time->day, time->month, time->year);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY:
			sprintf(string, "%02d/%02d/%04d", time->month, time->day, time->year);
			break;

		default:
			log_printf(DBG_WARNING, "Unexpected system date format: %u", date_format);
			sprintf(string, "%04d/%02d/%02d", time->year, time->month, time->day);
			break;
	}
}

void StatusBar::getTimeString(char *string, int time_format, SceDateTime *time)
{
	switch (time_format) {
	case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
		sprintf(string, "%02d:%02d %s", (time->hour > 12) ? (time->hour - 12) : ((time->hour == 0) ? 12 : time->hour), time->minute, time->hour >= 12 ? "PM" : "AM");
		break;

	case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
		sprintf(string, "%02d:%02d", time->hour, time->minute);
		break;

	default:
		log_printf(DBG_WARNING, "Unexpected system time format: %u", time_format);
		sprintf(string, "%02d:%02d", time->hour, time->minute);
		break;
	}
}

int StatusBar::displayBattery()
{
	float battery_x = align_left(949, vita2d_texture_get_width(img_statsbar_battery.texture.get()));

	float percent = scePowerGetBatteryLifePercent();
	float width = ((29.0f * percent) / 100.0f);

	unsigned int battery_color = RGBA8(91, 223, 38, 255);

	// FIXME Not sure if it's good performance-wise to do that 60 times per second
	if (scePowerIsLowBattery())
		battery_color = RGBA8(255, 48, 48, 255);
	vita2d_draw_rectangle((947.0f - width), 4, width, 16, battery_color);
	
	if (scePowerIsBatteryCharging())
		img_statsbar_battery_charge.Draw(Point(battery_x, 2));
	else
		img_statsbar_battery.Draw(Point(battery_x, 2));

	return 0;
}

int StatusBar::displayDate()
{
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	char date_string[16];
	// FIXME: Get the date format from the system
	getDateString(date_string, SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY, &time);

	char time_string[24];
	// FIXME: Get the time format from the system
	getTimeString(time_string, SCE_SYSTEM_PARAM_TIME_FORMAT_24HR, &time);

	char string[64];
	sprintf(string, "%s  %s", date_string, time_string);

	font_22.DrawFromBaseline(Point(700, 22), string, COLOR_WHITE);

	return 0;
}


