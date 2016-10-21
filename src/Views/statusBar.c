extern unsigned char _binary_assets_spr_img_statsbar_battery_png_start;

vita2d_font *font_25;
vita2d_texture *img_statsbar_battery;

int initStatusBar()
{
	font25 = vita2d_load_font_file(VHBB_RES_DIR_FONTS "segoeui.ttf");
	img_statsbar_battery = vita2d_load_PNG_buffer(&_binary_assets_spr_img_statsbar_battery_png_start);
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
	getDateString(date_string, date_format, &time);

	char time_string[24];
	getTimeString(time_string, time_format, &time);

	char string[64];
	sprintf(string, "%s  %s", date_string, time_string);

	vita2d_font_draw_text(font_25, 650, 22, COLOUR_WHITE, 25, string);

	return 0;
}

int displayStatusBar()
{
	vita2d_font_draw_text(font_25, 12, 22, COLOUR_WHITE, 25, "Vita HomeBrew Browser");

	displayBattery();
	displayDate();

	return 0;
}
