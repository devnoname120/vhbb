#pragma once

int unzip(const char *zippath, const char *outpath, uint64_t *zipCur = nullptr);
int uncompressedSize(const char *zippath, uint64_t *size);
