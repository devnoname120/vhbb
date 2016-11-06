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
int touchCoordinates(Input *input, int *touchX, int *touchY);
int touchDifference(Input *input, int *touchDifX, int *touchDifY, unsigned long *timeDif);
