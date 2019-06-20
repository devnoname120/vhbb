#include "filesystem.h"

#include "debug.h"
#include "utils.h"

#include <cstring>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>

// TODO: Encapsulate these functions into a Filesystem class

// Path must end with '/'
int removePath(std::string path)
{
    // sceIoDopen doesn't work if there is a '/' at the end
    if (path.back() == '/')
        path.pop_back();

    SceUID dfd = sceIoDopen(path.c_str());
    if (dfd >= 0)
    {
        path += "/";
        int res = 0;

        do
        {
            SceIoDirent dir;
            memset(&dir, 0, sizeof(SceIoDirent));

            res = sceIoDread(dfd, &dir);
            if (res > 0)
            {
                auto new_path = path + dir.d_name;

                if (SCE_S_ISDIR(dir.d_stat.st_mode))
                {
                    int ret = removePath(new_path);
                    if (ret <= 0)
                    {
                        sceIoDclose(dfd);
                        return ret;
                    }
                }
                else
                {
                    int ret = sceIoRemove(new_path.c_str());
                    if (ret < 0)
                    {
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
    }
    else
    {
        int ret = sceIoRemove(path.c_str());
        if (ret < 0)
            return ret;
    }

    return 1;
}

int readFile(const std::string& path, void* buffer, SceSize size)
{
    SceUID fd = sceIoOpen(path.c_str(), SCE_O_RDONLY, 0);
    if (fd < 0)
        return fd;

    int read = sceIoRead(fd, buffer, size);

    sceIoClose(fd);
    return read;
}

int readFile(const std::string& path, std::string& content)
{
    SceUID fd = sceIoOpen(path.c_str(), SCE_O_RDONLY, 0);
    if (fd < 0)
        return fd;

    int size = sceIoLseek32(fd, 0, SCE_SEEK_END);
    sceIoLseek32(fd, 0, SCE_SEEK_SET);

    if (size < 0)
        return size;

    char buf[size + 1];

    int read = sceIoRead(fd, buf, size);

    if (read < 0)
        return read;

    sceIoClose(fd);

    buf[read] = 0; // add null char terminator

    content.reserve((uint)size);
    content.assign(buf);

    return read;
}

#define CP_BUF_SIZE (128 * 1024)

int copyFile(const std::string& path_source, const std::string& path_dest)
{
    // The source and destination paths are identical
    if (std_string_iequals(path_source, path_dest))
    {
        log_printf(DBG_ERROR, "source equals destination: %s", path_source.c_str());
        return -1;
    }

    // The destination is a subfolder of the source folder
    unsigned int len = path_source.length();
    if (std_string_iequals(path_source.substr(0, len), path_dest.substr(0, len))
        && (path_dest[len] == '/' || path_dest[len - 1] == '/'))
    {
        log_printf(DBG_ERROR, "source (%s) is sub-dir of dst (%s)", path_source.c_str(), path_dest.c_str());
        return -2;
    }

    SceUID fdsrc = sceIoOpen(path_source.c_str(), SCE_O_RDONLY, 0);
    if (fdsrc < 0)
    {
        log_printf(DBG_ERROR, "couldn't open source %s: 0x%08x", path_source.c_str(), fdsrc);
        return fdsrc;
    }

    SceUID fddst = sceIoOpen(path_dest.c_str(), SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
    if (fddst < 0)
    {
        sceIoClose(fdsrc);
        log_printf(DBG_ERROR, "couldn't read destination %s: 0x%08x", path_dest.c_str(), fddst);
        return fddst;
    }

    std::vector<char> buf(CP_BUF_SIZE);

    while (true)
    {
        int read = sceIoRead(fdsrc, buf.data(), CP_BUF_SIZE);

        if (read < 0)
        {
            log_printf(DBG_ERROR, "source read error %s: 0x%08x", path_source.c_str(), read);
            sceIoClose(fddst);
            sceIoClose(fdsrc);
            sceIoRemove(path_dest.c_str());
            return read;
        }

        if (read == 0)
            break;

        int written = sceIoWrite(fddst, buf.data(), (SceSize)read);

        if (written < 0)
        {
            log_printf(DBG_ERROR, "destination write error %s: 0x%08x", path_dest.c_str(), written);
            sceIoClose(fddst);
            sceIoClose(fdsrc);
            sceIoRemove(path_dest.c_str());
            return written;
        }
    }

    // Inherit file stat
    SceIoStat stat;
    memset(&stat, 0, sizeof(SceIoStat));
    sceIoGetstatByFd(fdsrc, &stat);
    sceIoChstatByFd(fddst, &stat, 0x3B);

    sceIoClose(fddst);
    sceIoClose(fdsrc);

    return 0;
}
