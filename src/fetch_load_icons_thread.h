#pragma once

#include <atomic>

void StartFetchLoadIconsThread();
void FetchLoadIcons(unsigned int arglen, std::atomic<bool> ** db_done);