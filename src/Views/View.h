#pragma once

#include <input.h>

class View
{
public:
	virtual ~View();

	virtual int HandleInput(int focus, const Input& input);
	virtual int Display();

	// FIXME Not a good way to signal it needs to be destroyed
	bool request_destroy = false;
	unsigned int priority = 100;
};
