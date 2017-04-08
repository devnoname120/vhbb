#pragma once

#include <global_include.h>

#include "homebrew.h"
#include "infoProgress.h"

struct InstallArguments {
	Homebrew hb;
};

void install_thread(SceSize args_size, InstallArguments *installArgs);
