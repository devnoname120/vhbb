#pragma once

#include <atomic>
#include <input.h>

class View
{
public:
	virtual ~View();

	virtual int HandleInput(int focus, const Input& input);
	virtual int Display();

	// FIXME Not a good way to signal it needs to be destroyed
	std::atomic_bool request_destroy = false;
	std::atomic_uint priority = 100;
};
