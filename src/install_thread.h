#pragma once


#include <psp2/types.h>
#include "homebrew.h"
#include "infoProgress.h"

struct InstallArguments {
	Homebrew hb;
};

int install_thread(SceSize args_size, InstallArguments *installArgs);
