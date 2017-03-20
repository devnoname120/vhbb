#pragma once

#include <global_include.h>

struct InstallArguments {
	std::string url;
};

struct DownloadArguments {
	std::string url;
    std::string dest;
    uint64_t *cur;
    bool *finished;
};

struct UnzipArguments {
	uint64_t *cur;
    bool *finished;
    int *res;
    int *step;
};

int install_thread(SceSize args_size, InstallArguments *args);
int download_thread(SceSize args_size, DownloadArguments *args);
int unzip_thread(SceSize args_size, UnzipArguments *args);
