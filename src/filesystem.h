#pragma once

#include <global_include.h>

int removePath(std::string path);

int readFile(const std::string &path, void *buffer, SceSize size);
int readFile(const std::string &path, std::string &content);

int copyFile(const std::string &fn_src, const std::string &fn_dst);
