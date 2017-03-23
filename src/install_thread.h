#pragma once

#include <global_include.h>

#include "infoProgress.h"

struct InstallArguments {
	std::string url;
};

struct DownloadArguments {
	std::string url;
    std::string dest;
    InfoProgress *progress;
};

struct UnzipArguments {
    int *res;
    InfoProgress *progress;
};

void install_thread(SceSize args_size, InstallArguments *args);
void download_thread(SceSize args_size, DownloadArguments *args);
void unzip_thread(SceSize args_size, UnzipArguments *args);
