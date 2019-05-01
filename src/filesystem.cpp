#include "filesystem.h"
#include <psp2/io/dirent.h>

// Path must end with '/'
int removePath(std::string path) {
	// sceIoDopen doesn't work if there is a '/' at the end
	if (path.back() == '/')
		path.pop_back();

	SceUID dfd = sceIoDopen(path.c_str());
	if (dfd >= 0) {
		path += "/";
		int res = 0;

		do {
			SceIoDirent dir;
			memset(&dir, 0, sizeof(SceIoDirent));

			res = sceIoDread(dfd, &dir);
			if (res > 0) {
                auto new_path = path + dir.d_name;

				if (SCE_S_ISDIR(dir.d_stat.st_mode)) {
					int ret = removePath(new_path);
					if (ret <= 0) {
						sceIoDclose(dfd);
						return ret;
					}
				} else {
					int ret = sceIoRemove(new_path.c_str());
					if (ret < 0) {
						sceIoDclose(dfd);
						return ret;
					}
				}
			}
		} while (res > 0);

		sceIoDclose(dfd);

		int ret = sceIoRmdir(path.c_str());
		if (ret < 0)
			return ret;
	} else {
		int ret = sceIoRemove(path.c_str());
		if (ret < 0)
			return ret;
	}

	return 1;
}

int readFile(const std::string fn, void *buffer, SceSize size) {
	SceUID fd = sceIoOpen(fn.c_str(), SCE_O_RDONLY, 0);
	if (fd < 0)
		return fd;

	int read = sceIoRead(fd, buffer, size);

	sceIoClose(fd);
	return read;
}
