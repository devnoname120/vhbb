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

int touchPressed(Input *input)
{
	return input->touch.reportNum;
}

int touchCoordinates(Input *input, int *touchX, int *touchY)
{
	*touchX = lerp(input->touch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH);
	*touchY = lerp(input->touch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT);
	return 0;
}