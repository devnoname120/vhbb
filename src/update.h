#pragma once

#include "global_include.h"

#include "Views/background.h"
#include "Views/ProgressView/progressView.h"

class Update {
public:
	static void startUpdateThread();
	static void tick();  // needs to be called frequently from main thread

private:
	static void updateThread(unsigned int arglen, void* argv[]);
	static bool updateExists();
	static std::shared_ptr<Background> startBackgroundView();
	static std::shared_ptr<ProgressView> startProgressView(InfoProgress progress, std::string title);
	static void installUpdater(InfoProgress progress);
	static void prepareUpdateFiles(InfoProgress progress);
	static void startUpdaterApp();
};
