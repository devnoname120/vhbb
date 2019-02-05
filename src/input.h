#pragma once

#include <global_include.h>
#include "vhbb.h"
#include "texture.h"
#include <shapes.h>

#define INPUT_INIT_REPEAT_DELAY 20  // max 127
#define INPUT_REPEAT_DELAY 3  // max 127
#define INPUT_ANALOG_DEAD_ZONE 64

class Input {
	private:

		SceCtrlData oldpad;
		SceCtrlData pad;
		
		SceTouchData old2touch;
		SceTouchData oldtouch;
		SceTouchData touch;

		void handleAnalogSticks();
		unsigned int repeatMask;
		void handleRepeatPress();

	public:
		Input();

		int Get();
		int Propagate();

		int KeyPressed(unsigned int buttons) const;
		int KeyNewPressed(unsigned int buttons) const;
		int KeyNewPressedRapidFire(unsigned int buttons) const;

		int TouchPressed() const;
		int TouchAlreadyPressed() const;
		int TouchNewPressed() const;
		int TouchStopPressed() const;
		int TouchNewMovement() const;
		int TouchAlreadyMovement() const;
		int TouchCoordinates(double *touchX, double *touchY) const;
		int TouchDifference(double *touchDifX, double *touchDifY, unsigned long *timeDif) const;
		int TouchSpeed(double *touchSpeedX, double *touchSpeedY, double *touchSpeed) const;
		int TouchInRectangle(const Rectangle &rect) const;
		int TouchInTexture(const Point &draw_pt, const Texture &tex) const;
};
