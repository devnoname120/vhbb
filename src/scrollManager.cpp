#include "scrollManager.h"

#include "debug.h"

template <bool xScroll, bool yScroll>
void ScrollManager<xScroll, yScroll>::stopScroll()
{
    if (xScroll)
        scrollSpeedX = 0;
    if (yScroll)
        scrollSpeedY = 0;
}


template <bool xScroll, bool yScroll>
void ScrollManager<xScroll, yScroll>::_update(int focus, const Input& input, int* posX, int* posY)
{
    if (disabled)
        return;
    if (!input.TouchPressed())
    {
        movementStaredInBounds = false;
        movementStared = false;
        updatePosFreeScroll(input, posX, posY);
    }
    else if (focus)
    {
        updatePosTouchScroll(input, posX, posY);
    }

    updateScrollSpeed(input);
}

template <bool xScroll, bool yScroll>
void ScrollManager<xScroll, yScroll>::updateScrollSpeed(const Input& input)
{
    unsigned long timeDif;
    input.TouchDifference(nullptr, nullptr, &timeDif);
    if (yScroll)
    {
        if (std::abs(scrollSpeedY) <= acceleration * (double)timeDif)
        {
            scrollSpeedY = 0;
        }
        else
        {
            double sign = scrollSpeedY > 0 ? -1 : 1;
            scrollSpeedY = scrollSpeedY + sign * acceleration * (double)timeDif;
        }
    }
    if (xScroll)
    {
        if (std::abs(scrollSpeedX) <= acceleration * (double)timeDif)
        {
            scrollSpeedX = 0;
        }
        else
        {
            double sign = scrollSpeedX > 0 ? -1 : 1;
            scrollSpeedX = scrollSpeedX + sign * acceleration * (double)timeDif;
        }
    }
}

template <bool xScroll, bool yScroll>
void ScrollManager<xScroll, yScroll>::updatePosFreeScroll(const Input& input, int* posX, int* posY)
{
    if (input.TouchPressed())
        return;
    unsigned long timeDif;
    input.TouchDifference(nullptr, nullptr, &timeDif);
    if (xScroll && posX) {
        double newXPos = std::min<double>(posXMax, std::max<double>(*posX + (scrollSpeedX * timeDif), posXMin));
        *posX = (int)std::min<double>(INT_MAX, std::max<double>(INT_MIN, newXPos));
    }
    if (yScroll && posY) {
        double newYPos = std::min<double>(posYMax, std::max<double>(*posY + (scrollSpeedY * timeDif), posYMin));
        *posY = (int)std::min<double>(INT_MAX, std::max<double>(INT_MIN, newYPos));
    }
}

template <bool xScroll, bool yScroll>
void ScrollManager<xScroll, yScroll>::updatePosTouchScroll(const Input& input, int* posX, int* posY)
{
    if (input.TouchAlreadyMovement())
    {
        if (!movementStared)
        {
            movementStared = true;
            movementStaredInBounds = input.TouchMovementStaredInRectangle(
                { { static_cast<double>(touchXMin), static_cast<double>(touchYMin) },
                  { static_cast<double>(touchXMax), static_cast<double>(touchYMax) } });
            log_printf(DBG_DEBUG, "scroll inbounds %i", movementStaredInBounds);
        }
        if (!movementStaredInBounds)
            return;

        unsigned long timeDif;
        double touchSpeedX, touchSpeedY, touchDifX, touchDifY;
        input.TouchSpeed(xScroll ? &touchSpeedX : nullptr , yScroll ? &touchSpeedY : nullptr, nullptr);
        input.TouchDifference(xScroll ? &touchDifX : nullptr, yScroll ? &touchDifY : nullptr, &timeDif);
        if (xScroll && posX)
        {
            double newXPos = std::min<double>(posXMax, std::max<double>(posXMin, *posX - touchDifX));
            *posX = (int)std::min<double>(INT_MAX, std::max<double>(INT_MIN, newXPos));
            scrollSpeedX = -touchSpeedX;
        }
        if (yScroll && posY)
        {
            double newYPos = std::min<double>(posYMax, std::max<double>(posYMin, *posY - touchDifY));
            *posY = (int)std::min<double>(INT_MAX, std::max<double>(INT_MIN, newYPos));
            scrollSpeedY = -touchSpeedY;
        }
    }
}

template<> void ScrollManager<false, false>::update(int focus, const Input& input, int* posX, int* posY) {}
template<> void ScrollManager<false, false>::updateScrollSpeed(const Input& input) {}
template<> void ScrollManager<false, false>::updatePosFreeScroll(const Input& input, int* posX, int* posY) {}
template<> void ScrollManager<false, false>::updatePosTouchScroll(const Input& input, int* posX, int* posY) {}

template class ScrollManager<true, false>;
template class ScrollManager<false, true>;
template class ScrollManager<true, true>;
