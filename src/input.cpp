#include <global_include.h>

#include "input.h"




Input::Input()
{
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
}

int Input::Get()
{
	// FIXME: use pointer instead to avoid unnecessary copy
	memcpy(&(oldpad), &(pad), sizeof(pad));
	memcpy(&(old2touch), &(oldtouch), sizeof(touch));
	memcpy(&(oldtouch), &(touch), sizeof(touch));
	
	sceCtrlPeekBufferPositive(0, &(pad), 1);
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &(touch), 1);
	return 0;
}

int Input::Propagate(GUIViews curView)
{
	// FIXME: rework function
	/*switch (curView) {

	case LIST_VIEW:
		handleCategoryViewInput(1, this);
		handleListViewInput(1, this);
		//handleQueueViewInput(0, input);
		break;

	case QUEUE_VIEW:
		handleCategoryViewInput(0, this);
		handleListViewInput(0, this);
		//handleQueueViewInput(1, input);
		break;
	}
	*/
	return 0;

}

// Check if a combination of buttons is pressed
int Input::KeyPressed(unsigned int buttons) const
{
	return (pad.buttons & buttons) == buttons;
}

// Check if a combination of buttons is pressed and was not pressed before
int Input::KeyNewPressed(unsigned int buttons) const
{
	return KeyPressed(buttons) && !((oldpad.buttons & buttons) == buttons);
}

int Input::TouchPressed() const
{
	return touch.reportNum;
}

int Input::TouchAlreadyPressed() const
{
	return TouchPressed() && oldtouch.reportNum;
}

int Input::TouchNewPressed() const
{
	return TouchPressed() && !oldtouch.reportNum;
}

int Input::TouchNewMovement() const
{
	return TouchAlreadyPressed() && !old2touch.reportNum;
}

int Input::TouchAlreadyMovement() const
{
	return TouchAlreadyPressed() && old2touch.reportNum;
}

int Input::TouchCoordinates(double *touchX, double *touchY) const
{
	if(touchX)
		*touchX = lerpd(touch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH);
	if(touchY)
		*touchY = lerpd(touch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT);
	return 0;
}

int Input::TouchDifference(double *touchDifX, double *touchDifY, unsigned long *timeDif) const
{
	if(touchDifX)
		*touchDifX = lerpd((double)touch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH) - lerpd(oldtouch.report[0].x, TOUCH_WIDTH, SCREEN_WIDTH);
	if(touchDifY)
		*touchDifY = lerpd((double)touch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT) - lerpd(oldtouch.report[0].y, TOUCH_HEIGHT, SCREEN_HEIGHT);
	if(timeDif)
		*timeDif = touch.timeStamp - oldtouch.timeStamp;
	return 0;
}

// Return the speed of the touch movement
int Input::TouchSpeed(double *touchSpeedX, double *touchSpeedY, double *touchSpeed) const
{
	double difX;
	double difY;
	unsigned long timeDif;

	TouchDifference(&difX, &difY, &timeDif);

	double speedX = (double)difX/timeDif;
	if (touchSpeedX)
		*touchSpeedX = speedX;

	double speedY = (double)difY/timeDif;
	if (touchSpeedY)
		*touchSpeedY = speedY;

	if (!touchSpeed)
		return 0;

	double speed = sqrt(pow(speedX, 2) + pow(speedY, 2));
	*touchSpeed = speed;

	return 0;
}
