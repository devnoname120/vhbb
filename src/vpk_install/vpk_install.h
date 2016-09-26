#ifndef __PACKAGE_INSTALLER_H__
#define __PACKAGE_INSTALLER_H__

#include <psp2/types.h>

#define MAX_QUE_SIZE		50

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

typedef struct {
	char *fileCloud;
	char *fileLocal;
} DownloadArguments;

typedef struct
	{
	char *fileCloud;
	char *fileLocal;
	int  percent;
	int  screen;
	int  pos;
	char *status;
	} queItem;
	
typedef struct
	{
	int  screen;
	int  pos;
	} completeItem;
	
queItem downloadQue[MAX_QUE_SIZE];
completeItem downloadCompleted[MAX_QUE_SIZE];


int install_thread(SceSize args_size, InstallArguments *args);
int download_thread( SceSize args_size, DownloadArguments *arguments );
int download_que_add( char *fileCloud, char *fileLocal, int screen, int pos );
void download_que_remove();

int addEndSlash(char *path);

#endif