#include <stdio.h>
#include <string.h>

#include <global_include.h>

#include <minizip/unzip.h>

#define dir_delimter '/'
#define MAX_FILENAME 512
#define READ_SIZE 8192

static void mkdir_rec(const char *dir) {
        char tmp[256];
        char *p = NULL;
        size_t len;

        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        sceIoMkdir(tmp, 0777);
                        *p = '/';
                }
        sceIoMkdir(tmp, 0777);
}

std::string dirnameOf(const std::string& fname)
{
     size_t pos = fname.find_last_of("\\/");
     return (std::string::npos == pos)
         ? ""
         : fname.substr(0, pos);
}


int unzip(const char *zippath, const char *outpath, uint64_t *zipCur)
{
    unzFile zipfile = unzOpen(zippath);
    if (!zipfile)
    {
        dbg_printf(DBG_DEBUG, "%s: not found", zippath);
        return -1;
    }

    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        dbg_printf(DBG_DEBUG, "could not read file global info");
        unzClose(zipfile);
        return -1;
    }

    char read_buffer[READ_SIZE];

    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        unz_file_info file_info;
        char filename[MAX_FILENAME];
        char fullfilepath[MAX_FILENAME];
        if (unzGetCurrentFileInfo(
            zipfile,
            &file_info,
            filename,
            MAX_FILENAME,
            NULL, 0, NULL, 0) != UNZ_OK)
        {
            dbg_printf(DBG_DEBUG, "could not read file info");
            unzClose(zipfile);
            return -1;
        }

        if (zipCur) *zipCur += file_info.uncompressed_size;
        sprintf(fullfilepath, "%s%s", outpath, filename);

        // Check if this entry is a directory or file.
        const size_t filename_length = strlen(fullfilepath);
        if (fullfilepath[filename_length-1] == dir_delimter)
        {
            dbg_printf(DBG_DEBUG, "dir:%s", fullfilepath);
            //sceIoMkdir(fullfilepath, 0777);
            mkdir_rec(fullfilepath);
        }
        else
        {
            // Create the dir where the file will be placed
            std::string destdir = dirnameOf(std::string(fullfilepath));
            mkdir_rec(destdir.c_str());

            // Entry is a file, so extract it.
            dbg_printf(DBG_DEBUG, "file:%s", fullfilepath);
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                dbg_printf(DBG_DEBUG, "could not open file");
                unzClose(zipfile);
                return -1;
            }

            // Open a file to write out the data.
            FILE *out = fopen(fullfilepath, "wb");
            if (out == NULL)
            {
                dbg_printf(DBG_DEBUG, "could not open destination file");
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return -1;
            }

            int error = UNZ_OK;
            do    
            {
                error = unzReadCurrentFile(zipfile, read_buffer, READ_SIZE);
                if (error < 0)
                {
                    dbg_printf(DBG_DEBUG, "error %d", error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return -1;
                }

                if (error > 0)
                {
                    fwrite(read_buffer, error, 1, out); // TODO check frwrite return
                }
            } while (error > 0);

            fclose(out);
        }

        unzCloseCurrentFile(zipfile);

        // Go the the next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                dbg_printf(DBG_DEBUG, "cound not read next file");
                unzClose(zipfile);
                return -1;
            }
        }
    }

    unzClose(zipfile);

    return 0;
}

int uncompressedSize(const char *zippath, uint64_t *size)
{
    *size = 0;
    unzFile zipfile = unzOpen(zippath);
    if (!zipfile)
    {
        dbg_printf(DBG_DEBUG, "%s: not found", zippath);
        return -1;
    }

    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        dbg_printf(DBG_DEBUG, "could not read file global info");
        unzClose(zipfile);
        return -1;
    }

    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        unz_file_info file_info;
        char filename[MAX_FILENAME];
        if (unzGetCurrentFileInfo(
            zipfile,
            &file_info,
            filename,
            MAX_FILENAME,
            NULL, 0, NULL, 0) != UNZ_OK)
        {
            dbg_printf(DBG_DEBUG, "could not read file info");
            unzClose(zipfile);
            return -1;
        }

        *size += file_info.uncompressed_size;


        // Go the the next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                dbg_printf(DBG_DEBUG, "cound not read next file");
                unzClose(zipfile);
                return -1;
            }
        }
    }        
}
