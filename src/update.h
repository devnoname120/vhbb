#pragma once

#include "global_include.h"

#include "Views/background.h"
#include "Views/ProgressView/progressView.h"

class Update {
public:
	static void startUpdateThread();
	static void tick();  // needs to be called frequently from main thread
	static bool checkIsDone();

private:
	static void updateThread(unsigned int arglen, void* argv[]);
	static int getVersionInfo(bool &available, std::string &url);
	static std::shared_ptr<Background> startBackgroundView();
	static std::shared_ptr<ProgressView> startProgressView(InfoProgress progress, std::string title);
	static void installUpdater(InfoProgress progress);
	static void prepareUpdateFiles(const std::string &updateURL, InfoProgress progress);
	static void startUpdaterApp();
};
