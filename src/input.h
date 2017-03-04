#pragma once

#include <global_include.h>
#include "vhbb.h"
#include <shapes.h>

class Input {
	private:
		static int isInit;

    	SceDateTime timer;

		SceCtrlData oldpad;
		SceCtrlData pad;
		
		SceTouchData old2touch;	
		SceTouchData oldtouch;
		SceTouchData touch;
	public:
    	Input();
    	
    	int Get();
		int Propagate(GUIViews curView);

		int KeyPressed(unsigned int buttons) const;
		int KeyNewPressed(unsigned int buttons) const;

		int TouchPressed() const;
		int TouchAlreadyPressed() const;
		int TouchNewPressed() const;
		int TouchNewMovement() const;
		int TouchAlreadyMovement() const;
		int TouchCoordinates(double *touchX, double *touchY) const;
		int TouchDifference(double *touchDifX, double *touchDifY, unsigned long *timeDif) const;
		int TouchSpeed(double *touchSpeedX, double *touchSpeedY, double *touchSpeed) const;
		int TouchInRectangle(const Rectangle &rect) const;
};
