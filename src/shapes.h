#pragma once



class Point {
public:
	double x;
	double y;
	Point(double _x, double _y);
};

struct Dimensions {
    int width;
    int height;
};

class Rectangle {
public:
	const Point &topLeft;
	const Point &bottomRight;

	Rectangle(const Point &aTopLeft, const Point &aBottomRight) : topLeft(aTopLeft), bottomRight(aBottomRight) {};

	int Inside(const Point &pt) const;

	inline double Width() const { return bottomRight.x - topLeft.x; }
	inline double Height() const { return bottomRight.y - topLeft.y; }
};
