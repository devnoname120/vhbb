#pragma once

#include "input.h"
#include "shapes.h"

#include <cmath>

template <bool xScroll=false, bool yScroll=false>
class ScrollManager
{
public:
    explicit ScrollManager(
        int touchXMin = INT_MIN, int touchYMin = INT_MIN, int touchXMax = INT_MAX, int touchYMax = INT_MAX,
        int posXMin = INT_MIN, int posYMin = INT_MIN, int posXMax = INT_MAX, int posYMax = INT_MAX,
        double acceleration = 0.0000000010)
        : touchXMin(touchXMin)
        , touchYMin(touchYMin)
        , touchXMax(touchXMax)
        , touchYMax(touchYMax)
        , posXMin(posXMin)
        , posYMin(posYMin)
        , posXMax(posXMax)
        , posYMax(posYMax)
        , acceleration(acceleration){};
    void stopScroll();
    double getScrollSpeedX() const { return scrollSpeedX; } ;
    double getScrollSpeedY() const { return scrollSpeedY; } ;
    void getScrollSpeed(double* speedX, double* speedY) const
    {
        if (speedX)
            *speedX = scrollSpeedX;
        if (speedY)
            *speedY = scrollSpeedY;
    }

    bool disabled = false;
    inline void update(int focus, const Input& input, int* posX = nullptr, int* posY = nullptr) {
        if (!disabled)
            _update(focus, input, posX, posY);
    };

    int touchXMin;
    int touchYMin;
    int touchXMax;
    int touchYMax;
    int posXMin;
    int posYMin;
    int posXMax;
    int posYMax;

private:
    double acceleration;
    double scrollSpeedX = 0;
    double scrollSpeedY = 0;
    bool movementStared = false;
    bool movementStaredInBounds = false;

    void _update(int focus, const Input& input, int* posX = nullptr, int* posY = nullptr);
    void updateScrollSpeed(const Input& input);
    void updatePosFreeScroll(const Input& input, int* posX = nullptr, int* posY = nullptr);
    void updatePosTouchScroll(const Input& input, int* posX = nullptr, int* posY = nullptr);
};
