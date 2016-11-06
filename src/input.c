#include <global_include.h>

#include "input.h"

int initInput()
{
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
	return 0;	
}

int getInput(Input *input)
{
	// FIXME: use pointer instead to avoid unnecessary copy
	memcpy(&(input->oldpad), &(input->pad), sizeof(input->pad));
	memcpy(&(input->oldtouch), &(input->touch), sizeof(input->touch));
	
	sceCtrlPeekBufferPositive(0, &(input->pad), 1);
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &(input->touch), 1);
	return 0;
}

int handleInput(GUIViews view, Input *input)
{
	switch (view) {

	case LIST_VIEW:
		handleCategoryViewInput(1, input);
		handleListViewInput(1, input);
		//handleQueueViewInput(0, input);
		break;

	case QUEUE_VIEW:
		handleCategoryViewInput(0, input);
		handleListViewInput(0, input);
		//handleQueueViewInput(1, input);
		break;
	}

	return 0;

}

// Check if a combination of buttons is pressed
int keyPressed(Input *input, int buttons)
{
	return (input->pad.buttons & buttons) == buttons;
}

// Check if a combination of buttons is pressed and was not pressed before
int keyNewPressed(Input *input, int buttons)
{
	return keyPressed(input, buttons) && !((input->oldpad.buttons & buttons) == buttons);
}

int touchPressed(Input *input)
{
	return input->touch.reportNum;
}

int touchAlreadyPressed(Input *input)
{
	return touchPressed(input) && input->oldtouch.reportNum;
}

int touchNewPressed(Input *input)
{
	return touchPressed(input) && !input->oldtouch.reportNum;
}

int touchCoordinates(Input *input, int *touchX, int *touchY)
{
	if(touchX)
		*touchX = lerp(input->touch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH);
	if(touchY)
		*touchY = lerp(input->touch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT);
	return 0;
}

int touchDifference(Input *input, int *touchDifX, int *touchDifY, unsigned long *timeDif)
{
	if(touchDifX)
		*touchDifX = lerp(input->touch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH) - lerp(input->oldtouch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH);
	if(touchDifY)
		*touchDifY = lerp(input->touch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT) - lerp(input->oldtouch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT);
	if(timeDif)
		*timeDif = input->touch.timeStamp - input->oldtouch.timeStamp;
	return 0;
}