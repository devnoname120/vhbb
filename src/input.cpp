#include "input.h"

#include "screen.h"
#include "touch.h"


Input::Input()
{
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
}

int Input::Get()
{
	// FIXME: use pointer instead to avoid unnecessary copy
	memcpy(&(oldpad), &(pad), sizeof(pad));
	memcpy(&(old2touch), &(oldtouch), sizeof(touch));
	memcpy(&(oldtouch), &(touch), sizeof(touch));

	int ret = sceCtrlPeekBufferPositive(0, &(pad), 1);
	if (ret < 0)
        log_printf(DBG_ERROR, "sceCtrlPeekBufferPositive() = 0x%08X", ret);

	ret = sceTouchPeek(SCE_TOUCH_PORT_FRONT, &(touch), 1);
	if (ret < 0)
		log_printf(DBG_ERROR, "sceTouchPeek() = 0x%08X", ret);

	handleAnalogSticks();
	handleRepeatPress();

	return 0;
}

void Input::handleAnalogSticks() {
	if (pad.ly < 127 - INPUT_ANALOG_DEAD_ZONE)
		pad.buttons |= SCE_CTRL_UP;
	if (pad.ly > 127 + INPUT_ANALOG_DEAD_ZONE)
		pad.buttons |= SCE_CTRL_DOWN;
	if (pad.lx < 127 - INPUT_ANALOG_DEAD_ZONE)
		pad.buttons |= SCE_CTRL_LEFT;
	if (pad.lx > 127 + INPUT_ANALOG_DEAD_ZONE)
		pad.buttons |= SCE_CTRL_RIGHT;
}

void Input::handleRepeatPress() {
	repeatMask = 0;
	std::tuple<unsigned int, uint8_t*, uint8_t>  buttons[] = {
		std::make_tuple(SCE_CTRL_UP, &pad.up, oldpad.up),
		std::make_tuple(SCE_CTRL_RIGHT, &pad.right, oldpad.right),
		std::make_tuple(SCE_CTRL_DOWN, &pad.down, oldpad.down),
		std::make_tuple(SCE_CTRL_LEFT, &pad.left, oldpad.left),
		std::make_tuple(SCE_CTRL_LEFT, &pad.left, oldpad.left),
		std::make_tuple(SCE_CTRL_LTRIGGER, &pad.lt, oldpad.lt),
		std::make_tuple(SCE_CTRL_RTRIGGER, &pad.rt, oldpad.rt),
		std::make_tuple(SCE_CTRL_TRIANGLE, &pad.triangle, oldpad.triangle),
		std::make_tuple(SCE_CTRL_CIRCLE, &pad.circle, oldpad.circle),
		std::make_tuple(SCE_CTRL_CROSS, &pad.cross, oldpad.cross),
		std::make_tuple(SCE_CTRL_SQUARE, &pad.square, oldpad.square)
	};
	for (const auto &b : buttons) {
		unsigned int SCE_CTRL = std::get<0>(b);
		if(!KeyPressed(SCE_CTRL)) continue;
		uint8_t *pad_mem = std::get<1>(b);
		uint8_t oldpad_mem = std::get<2>(b);
		// log_printf(DBG_DEBUG, "oldmem 0x%02X    mem 0x%02X    btn 0x%08X", oldpad_mem, *pad_mem, SCE_CTRL);
		if ((oldpad_mem & 1) == 1) {
			*pad_mem = (oldpad_mem + 2) % (INPUT_REPEAT_DELAY << 2);
			if (*pad_mem < oldpad_mem) {
				oldpad.buttons &= ~SCE_CTRL;
				repeatMask |= SCE_CTRL;
			}
		} else {
			*pad_mem = (oldpad_mem + 2) % (INPUT_INIT_REPEAT_DELAY << 2);
			if (*pad_mem < oldpad_mem) {
				oldpad.buttons &= ~SCE_CTRL;
				repeatMask |= SCE_CTRL;
				*pad_mem |= 1;
			}
		}
		// log_printf(DBG_DEBUG, "oldmem 0x%02X    mem 0x%02X    btn 0x%08X\n", oldpad_mem, *pad_mem, SCE_CTRL);
	}
}

int Input::Propagate()
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
	return KeyPressed(buttons) && (oldpad.buttons & buttons) != buttons;
}

int Input::KeyNewPressedNoRepeat(unsigned int buttons) const {
	// unsigned int repeatMask =
	// 	(pad.up & 1) << __builtin_ctz(SCE_CTRL_UP) |
	// 	(pad.right & 1) << __builtin_ctz(SCE_CTRL_RIGHT) |
	// 	(pad.down & 1) << __builtin_ctz(SCE_CTRL_DOWN) |
	// 	(pad.left & 1) << __builtin_ctz(SCE_CTRL_LEFT) |
	// 	(pad.lt & 1) << __builtin_ctz(SCE_CTRL_LTRIGGER) |
	// 	(pad.rt & 1) << __builtin_ctz(SCE_CTRL_RTRIGGER) |
	// 	(pad.triangle & 1) << __builtin_ctz(SCE_CTRL_TRIANGLE) |
	// 	(pad.circle & 1) << __builtin_ctz(SCE_CTRL_CIRCLE) |
	// 	(pad.cross & 1) << __builtin_ctz(SCE_CTRL_CROSS) |
	// 	(pad.square & 1) << __builtin_ctz(SCE_CTRL_SQUARE);
	// log_printf(DBG_DEBUG, "pad.lt=0x%02X    repeatMast=0x%08X", pad.lt, repeatMask);
	// log_printf(DBG_DEBUG, "pad.lt & 1=0x%02X", pad.lt & 1);
	// log_printf(DBG_DEBUG, __builtin_ctz(SCE_CTRL_LTRIGGER=%i",__builtin_ctz(SCE_CTRL_LTRIGGER);
	// log_printf(DBG_DEBUG, "1 <<__builtin_ctz(SCE_CTRL_LTRIGGER=0x%08X", 1 << __builtin_ctz(SCE_CTRL_LTRIGGER));
	// log_printf(DBG_DEBUG, "(pad.lt & 1) << __builtin_ctz(SCE_CTRL_LTRIGGER)=0x%08X\n", (pad.lt & 1) << __builtin_ctz(SCE_CTRL_LTRIGGER));
	// log_printf(DBG_DEBUG, "buttons=0x%08X\n", buttons);
	// log_printf(DBG_DEBUG, "buttons & ~repeatMask=0x%08X\n", buttons & ~repeatMask);
	return KeyNewPressed(buttons & ~repeatMask);
}


int Input::TouchPressed() const
{
	return touch.reportNum != 0;
}

int Input::TouchAlreadyPressed() const
{
	return TouchPressed() && oldtouch.reportNum;
}

int Input::TouchNewPressed() const
{
	return TouchPressed() && !oldtouch.reportNum;
}

int Input::TouchStopPressed() const
{
	return !TouchPressed() && oldtouch.reportNum;
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

	double speedX = difX / timeDif;
	if (touchSpeedX)
		*touchSpeedX = speedX;

	double speedY = difY / timeDif;
	if (touchSpeedY)
		*touchSpeedY = speedY;

	if (!touchSpeed)
		return 0;

	double speed = sqrt(pow(speedX, 2) + pow(speedY, 2));
	*touchSpeed = speed;

	return 0;
}

int Input::TouchInRectangle(const Rectangle &rect) const
{
	double touchX, touchY;
	TouchCoordinates(&touchX, &touchY);

    log_printf(DBG_DEBUG, "TouchInRectangle rectangle: %f,%f:%f,%f", rect.topLeft.x, rect.topLeft.y, rect.bottomRight.x,
               rect.bottomRight.y);
    log_printf(DBG_DEBUG, "TouchInRectangle touch: %f,%f", touchX, touchY);

	return rect.Inside(Point(touchX, touchY));
}

int Input::TouchInTexture(const Point &draw_pt, const Texture &tex) const
{
	return TouchInRectangle(Rectangle(draw_pt, Point(draw_pt.x + vita2d_texture_get_width(tex.texture.get()), draw_pt.y + vita2d_texture_get_height(tex.texture.get()))));
}
