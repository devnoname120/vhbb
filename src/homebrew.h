#pragma once

#include <global_include.h>

#include "homebrewRelease.h"

class Homebrew {
public:
	// Those should be const but then it's mandatory to use an initialization list
	std::string title;
	std::string icon; // FIXME Use special class instead
	std::string author;
	std::string category;
	std::string description;
	//std::string home; // We don't care about the home
	
	HomebrewRelease releases;
};
