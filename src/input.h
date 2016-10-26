#pragma once

#include <global_include.h>

typedef struct {
	SceCtrlData pad;
	SceTouchData touch;
} Input;

int initInput();
int getInput(Input *input);
int handleInput(GUIViews view, Input *input);
int touchPressed(Input *input);
int touchCoordinates(Input *input, int *touchX, int *touchY);
