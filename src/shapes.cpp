#include "shapes.h"
#include "debug.h"

Point::Point(double _x, double _y)
{
	x = _x;
	y = _y;
}


int Rectangle::Inside(const Point &pt) const
{
    log_printf(DBG_DEBUG, "Inside point: %f,%f", pt.x, pt.y);

	return pt.x >= topLeft.x && pt.x <= bottomRight.x
		&& pt.y >=topLeft.y && pt.y <= bottomRight.y;
}
