#pragma once

#include <global_include.h>

class Point {
	public:
		double x;
		double y;
		Point(double _x, double _y);
};

class Rectangle {
	public:
		const Point &topLeft;
		const Point &bottomRight;

		Rectangle(const Point &aTopLeft, const Point &aBottomRight) : topLeft(aTopLeft), bottomRight(aBottomRight) {};

		int Inside(const Point &pt) const;
};
