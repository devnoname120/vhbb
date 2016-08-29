#ifndef __FILE_H__
#define __FILE_H__

#include <psp2/types.h>

#define MAX_PATH_LENGTH 1024
#define MAX_NAME_LENGTH 256
#define MAX_SHORT_NAME_LENGTH 64
#define MAX_MOUNT_POINT_LENGTH 16

#define TRANSFER_SIZE 64 * 1024

#define HOME_PATH "home"
#define DIR_UP ".."

enum FileTypes {
	FILE_TYPE_UNKNOWN,
	FILE_TYPE_BMP,
	FILE_TYPE_JPEG,
	FILE_TYPE_PNG,
	FILE_TYPE_MP3,
	FILE_TYPE_VPK,
	FILE_TYPE_ZIP,
};

enum SortFlags {
	SORT_NONE,
	SORT_BY_NAME_AND_FOLDER,
};

typedef struct FileListEntry {
	struct FileListEntry *next;
	struct FileListEntry *previous;
	char name[MAX_NAME_LENGTH];
	int name_length;
	int is_folder;
	int type;
	SceOff size;
	SceDateTime time;
	int reserved[16];
} FileListEntry;

typedef struct {
	FileListEntry *head;
	FileListEntry *tail;
	int length;
	char path[MAX_PATH_LENGTH];
	int files;
	int folders;
} FileList;

int ReadFile(char *file, void *buf, int size);
int WriteFile(char *file, void *buf, int size);

int getFileSize(char *pInputFileName);
int getPathInfo(char *path, uint64_t *size, uint32_t *folders, uint32_t *files);
int removePath(char *path, uint64_t *value, uint64_t max, void (* SetProgress)(uint64_t value, uint64_t max), int (* cancelHandler)());
int copyFile(char *src_path, char *dst_path, uint64_t *value, uint64_t max, void (* SetProgress)(uint64_t value, uint64_t max), int (* cancelHandler)());
int copyPath(char *src_path, char *dst_path, uint64_t *value, uint64_t max, void (* SetProgress)(uint64_t value, uint64_t max), int (* cancelHandler)());

int getFileType(char *file);

int getNumberMountPoints();
char **getMountPoints();

FileListEntry *fileListFindEntry(FileList *list, char *name);
FileListEntry *fileListGetNthEntry(FileList *list, int n);
void fileListAddEntry(FileList *list, FileListEntry *entry, int sort);
int fileListRemoveEntry(FileList *list, FileListEntry *entry);
int fileListRemoveEntryByName(FileList *list, char *name);
void fileListEmpty(FileList *list);
int fileListGetEntries(FileList *list, char *path);

#endif
