#pragma once

#include "texture.h"
#include "vhbb.h"

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <shapes.h>

#define INPUT_INIT_REPEAT_DELAY 20 // max 127
#define INPUT_REPEAT_DELAY 3       // max 127
#define INPUT_ANALOG_DEAD_ZONE 64

// Japanese Vita use O to confirm and X to cancel, other regions use the opposite.
extern unsigned int SCE_CTRL_ENTER;
extern unsigned int SCE_CTRL_CANCEL;

class Input
{
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
    int TouchMovementStartedCoordinates(double* touchX, double* touchY) const;
    int TouchMovementStaredInRectangle(const Rectangle& rect) const;
    int TouchAlreadyMovement() const;
    int TouchCoordinates(double* touchX, double* touchY) const;
    int TouchDifference(double* touchDifX, double* touchDifY, unsigned long* timeDif) const;
    int TouchSpeed(double* touchSpeedX, double* touchSpeedY, double* touchSpeed) const;
    int TouchInRectangle(const Rectangle& rect) const;
    int TouchInTexture(const Point& draw_pt, const Texture& tex) const;
};
