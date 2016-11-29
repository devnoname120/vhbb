#include "Point.cpp"

#pragma once

//*! Geometrical rectangle
class Rectangle
{
private:
	Point *upperLeftCorner;
	Point *lowerRightCorner;
public:
	/**
	 * Create a rectangle from 2 points
	 * @param upperLeftCorner point of the upper left corner of the rectangle
	 * @param lowerRightCorner point of the lower right corner of the rectangle
	*/
	Rectangle(Point *upperLeftCorner, Point *lowerRightCorner);
	~Rectangle();

	/**
	 * Check if a point is inside the rectangle
	 * @param pt point to check
	*/
	bool isPointInside(Point *pt);
	
};
