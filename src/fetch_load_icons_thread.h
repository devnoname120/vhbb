#pragma once

#include <atomic>

void StartFetchLoadIconsThread();
int FetchLoadIcons(unsigned int arglen, std::atomic<bool> *db_done);
