#include <global_include.h>

Point::Point(double _x, double _y)
{
	x = _x;
	y = _y;
}


int Rectangle::Inside(const Point &pt) const
{
	dbg_printf(DBG_DEBUG, "Inside point: %f,%f", pt.x, pt.y);
	dbg_printf(DBG_DEBUG, "Cond is %d", (int)(pt.x >= topLeft.x && pt.x <= bottomRight.x
		&& pt.y >=topLeft.y && pt.y <= bottomRight.y));
	return pt.x >= topLeft.x && pt.x <= bottomRight.x
		&& pt.y >=topLeft.y && pt.y <= bottomRight.y;
}
