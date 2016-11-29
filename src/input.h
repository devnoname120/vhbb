#pragma once

#include <global_include.h>

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

		int KeyPressed(unsigned int buttons);
		int KeyNewPressed(unsigned int buttons);

		int TouchPressed();
		int TouchAlreadyPressed();
		int TouchNewPressed();
		int TouchNewMovement();
		int TouchCoordinates(double *touchX, double *touchY);
		int TouchDifference(double *touchDifX, double *touchDifY, unsigned long *timeDif);
		int TouchSpeed(double *touchSpeedX, double *touchSpeedY, double *touchSpeed);	
};
