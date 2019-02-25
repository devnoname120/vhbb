#pragma once

#include <regex>

#include "global_include.h"
#include "Views/ProgressView/progressView.h"

class Updater {
public:
	static void startUpdateThread();

private:
	static void updateThread(unsigned int arglen, void *args);
	static bool updateExists();
	static std::shared_ptr<ProgressView> startProgressView(InfoProgress progress, std::string title);
	static void installUpdater(InfoProgress progress);
	static void prepareUpdateFiles(InfoProgress progress);
	static void runUpdater();
};
