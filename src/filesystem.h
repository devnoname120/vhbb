#pragma once


#include <string>
#include <psp2/types.h>

int removePath(std::string path);

int readFile(const std::string &path, void *buffer, SceSize size);
int readFile(const std::string &path, std::string &content);

int copyFile(const std::string &path_source, const std::string &path_dest);
