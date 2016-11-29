#include <global_include.h>

extern unsigned char _binary_assets_spr_img_statsbar_battery_png_start;


void getDateString(char *string, int date_format, SceDateTime *time);
void getTimeString(char *string, int time_format, SceDateTime *time);
int displayBattery();
int displayDate();

StatusBar::StatusBar()
{
	#ifdef PSP2SHELL

	SceNetCtlInfo info;
	if(sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_ADDRESS, &info) < 0) {
		dbg_printf(DBG_ERROR, "Failed to obtain Vita IP address");
	} else {
		strncpy(vitaip, (char * restrict)&(info.ip_address), 16);
	}

	#endif

	font_25 = vita2d_load_font_file(FONT_DIR "segoeui.ttf");
	img_statsbar_battery = vita2d_load_PNG_buffer(&_binary_assets_spr_img_statsbar_battery_png_start);
}

StatusBar::~StatusBar()
{
}

int StatusBar::Display()
{
	// Background
	vita2d_draw_rectangle(0, 0, SCREEN_WIDTH, STATUSBAR_HEIGHT, COLOR_BLACK);

	vita2d_font_draw_text(font_25, 12, 22, COLOR_WHITE, 25, "Vita HomeBrew Browser");

	displayBattery();
	displayDate();

	#ifdef PSP2SHELL
	vita2d_font_draw_text(font_25, 400, 22, COLOR_WHITE, 25, vitaip);
	#endif

	return 0;
}


void getDateString(char *string, int date_format, SceDateTime *time)
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
	}
}

void getTimeString(char *string, int time_format, SceDateTime *time)
{
	switch (time_format) {
	case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
		sprintf(string, "%02d:%02d %s", (time->hour > 12) ? (time->hour - 12) : ((time->hour == 0) ? 12 : time->hour), time->minute, time->hour >= 12 ? "PM" : "AM");
		break;

	case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
		sprintf(string, "%02d:%02d", time->hour, time->minute);
		break;
	}
}

int displayBattery()
{
	float battery_x = ALIGN_LEFT(949, vita2d_texture_get_width(img_statsbar_battery));
	vita2d_draw_texture(img_statsbar_battery, battery_x, 2);

	float percent = scePowerGetBatteryLifePercent();
	float width = ((29.0f * percent) / 100.0f);

	unsigned int battery_color = RGBA8(91, 223, 38, 255);
	if (scePowerIsLowBattery())
		battery_color = RGBA8(255, 48, 48, 255);

	/* 
	if (scePowerIsBatteryCharging()) {
		// TODO: add charging Icon
	}
	*/

	vita2d_draw_rectangle((947.0f - width), 4, width, 16, battery_color);

	return 0;
}

int displayDate()
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

	vita2d_font_draw_text(font_25, 650, 22, COLOR_WHITE, 25, string);

	return 0;
}


