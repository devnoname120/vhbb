#pragma once

#include <regex>

#include "global_include.h"
#include "network.h"
#include "filesystem.h"
#include "Views/dialogView.h"

class Updater {
public:
	static void startUpdateCheckThread();

private:
	static void updateCheckThread(unsigned int arglen, void *args);
	static bool updateExists();
};
