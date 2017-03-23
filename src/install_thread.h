#pragma once

#include <global_include.h>

#include "infoProgress.h"

struct InstallArguments {
	std::string url;
};

void install_thread(SceSize args_size, InstallArguments *args);
