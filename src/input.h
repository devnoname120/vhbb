#pragma once

#include <global_include.h>

typedef struct {
	SceDateTime timer;

	SceCtrlData oldpad;
	SceTouchData oldtouch;

	SceCtrlData pad;
	SceTouchData touch;
} Input;

int initInput();
int getInput(Input *input);
int handleInput(GUIViews view, Input *input);

int keyPressed(Input *input, int buttons);
int keyNewPressed(Input *input, int buttons);

int touchPressed(Input *input);
int touchAlreadyPressed(Input *input);
int touchNewPressed(Input *input);
int touchCoordinates(Input *input, double *touchX, double *touchY);
int touchDifference(Input *input, double *touchDifX, double *touchDifY, unsigned long *timeDif);
int touchSpeed(Input *input, double *touchSpeedX, double *touchSpeedY, double *touchSpeed);
