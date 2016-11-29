#pragma once

class View
{
public:
	View();
	~View();

	virtual int HandleInput();
	virtual int Display();
};
