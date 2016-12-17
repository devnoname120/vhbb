#pragma once

#include <global_include.h>

class View
{
public:
	virtual int HandleInput(int focus, const Input& input);
	virtual int Display();
};

