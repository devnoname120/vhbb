#pragma once

#include <global_include.h>

#define STATUSBAR_HEIGHT 30

int initStatusBar();
void getDateString(char *string, int date_format, SceDateTime *time);
void getTimeString(char *string, int time_format, SceDateTime *time);
int displayBattery();
int displayDate();
int displayStatusBar();
