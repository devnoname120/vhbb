#ifndef __PACKAGE_INSTALLER_H__
#define __PACKAGE_INSTALLER_H__

#include <psp2/types.h>


typedef struct SfoHeader {
	uint32_t magic;
	uint32_t version;
	uint32_t keyofs;
	uint32_t valofs;
	uint32_t count;
} __attribute__((packed)) SfoHeader;

typedef struct SfoEntry {
	uint16_t nameofs;
	uint8_t  alignment;
	uint8_t  type;
	uint32_t valsize;
	uint32_t totalsize;
	uint32_t dataofs;
} __attribute__((packed)) SfoEntry;

typedef struct {
	char *file;
} InstallArguments;

int install_thread(SceSize args_size, InstallArguments *args);

int addEndSlash(char *path);

#endif