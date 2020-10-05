#include "vitaPackage.h"

#include "debug.h"
#include "filesystem.h"
#include "macros.h"
#include "sha1.h"
#include "zip.h"

#include <stdexcept>
#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/promoterutil.h>
#include <psp2/sysmodule.h>

#define ntohl __builtin_bswap32

#define UPDATER_SRC_EBOOT_PATH VHBB_RESOURCES "/updater/eboot.bin"
#define UPDATER_SRC_SFO_PATH VHBB_RESOURCES "/updater/param.sfo"

#define UPDATER_DST_EBOOT_PATH PACKAGE_TEMP_FOLDER "eboot.bin"
#define UPDATER_DST_SFO_DIR PACKAGE_TEMP_FOLDER "sce_sys/"
#define UPDATER_DST_SFO_PATH PACKAGE_TEMP_FOLDER "sce_sys/param.sfo"

extern unsigned char _binary_assets_head_bin_start;
extern unsigned char _binary_assets_head_bin_size;

#define SFO_MAGIC 0x46535000

static void fpkg_hmac(const uint8_t* data, unsigned int len, uint8_t hmac[16])
{
    SHA1_CTX ctx;
    char sha1[20];
    char buf[64];

    sha1_init(&ctx);
    sha1_update(&ctx, (BYTE*)data, len);
    sha1_final(&ctx, (BYTE*)sha1);

    memset(buf, 0, 64);
    memcpy(&buf[0], &sha1[4], 8);
    memcpy(&buf[8], &sha1[4], 8);
    memcpy(&buf[16], &sha1[12], 4);
    buf[20] = sha1[16];
    buf[21] = sha1[1];
    buf[22] = sha1[2];
    buf[23] = sha1[3];
    memcpy(&buf[24], &buf[16], 8);

    sha1_init(&ctx);
    sha1_update(&ctx, (BYTE*)buf, 64);
    sha1_final(&ctx, (BYTE*)sha1);
    memcpy(hmac, sha1, 16);
}

typedef struct SfoHeader
{
    uint32_t magic;
    uint32_t version;
    uint32_t keyofs;
    uint32_t valofs;
    uint32_t count;
} __attribute__((packed)) SfoHeader;

typedef struct SfoEntry
{
    uint16_t nameofs;
    uint8_t alignment;
    uint8_t type;
    uint32_t valsize;
    uint32_t totalsize;
    uint32_t dataofs;
} __attribute__((packed)) SfoEntry;

int getSfoString(char* buffer, const char* name, char* string, unsigned int length)
{
    auto* header = (SfoHeader*)buffer;
    auto* entries = (SfoEntry*)((uint32_t)buffer + sizeof(SfoHeader));

    if (header->magic != SFO_MAGIC)
        return -1;

    int i;
    for (i = 0; i < header->count; i++)
    {
        if (strcmp(buffer + header->keyofs + entries[i].nameofs, name) == 0)
        {
            memset(string, 0, length);
            strncpy(string, buffer + header->valofs + entries[i].dataofs, length);
            string[length - 1] = '\0';
            return 0;
        }
    }

    return -2;
}

int WriteFile(const char* file, const void* buf, unsigned int size)
{
    log_printf(DBG_DEBUG, "Writing file %s @%p+%u", file, buf, size);
    SceUID fd = sceIoOpen(file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
    if (fd < 0)
        return fd;

    int written = sceIoWrite(fd, buf, size);

    sceIoClose(fd);
    return written;
}

int checkFileExist(const char* file)
{
    SceIoStat stat;
    memset(&stat, 0, sizeof(SceIoStat));

    return sceIoGetstat(file, &stat) >= 0;
}

int allocateReadFile(const char* file, char** buffer)
{
    SceUID fd = sceIoOpen(file, SCE_O_RDONLY, 0);
    if (fd < 0)
        return fd;

    // FIXME Check if < 0
    int size = sceIoLseek32(fd, 0, SCE_SEEK_END);
    sceIoLseek32(fd, 0, SCE_SEEK_SET);

    *buffer = (char*)malloc(size);
    if (!*buffer)
    {
        sceIoClose(fd);
        return -1;
    }

    int read = sceIoRead(fd, *buffer, size);
    sceIoClose(fd);

    return read;
}

int makeHeadBin()
{
    uint8_t hmac[16];
    uint32_t off;
    uint32_t len;
    uint32_t out;

    if (checkFileExist(PACKAGE_TEMP_FOLDER "sce_sys/package/head.bin"))
        return 0;

    // Read param.sfo
    char* sfo_buffer = nullptr;
    int res = allocateReadFile(PACKAGE_TEMP_FOLDER "sce_sys/param.sfo", &sfo_buffer);
    if (res < 0)
        return res;

    // Get title id
    char titleid[12];
    memset(titleid, 0, sizeof(titleid));
    getSfoString(sfo_buffer, "TITLE_ID", titleid, sizeof(titleid));

    // Enforce TITLE_ID format
    if (strlen(titleid) != 9)
        return -1;

    // Get content id
    char contentid[48];
    memset(contentid, 0, sizeof(contentid));
    getSfoString(sfo_buffer, "CONTENT_ID", contentid, sizeof(contentid));

    // Free sfo buffer
    free(sfo_buffer);

    // Allocate head.bin buffer
    uint8_t* head_bin = (uint8_t*)malloc((size_t)&_binary_assets_head_bin_size);
    memcpy(head_bin, (void*)&_binary_assets_head_bin_start, (size_t)&_binary_assets_head_bin_size);

    // Write full title id
    char full_title_id[48];
    snprintf(full_title_id, sizeof(full_title_id), "EP9000-%s_00-0000000000000000", titleid);
    strncpy((char*)&head_bin[0x30], strlen(contentid) > 0 ? contentid : full_title_id, 48);

    // hmac of pkg header
    len = ntohl(*(uint32_t*)&head_bin[0xD0]);
    fpkg_hmac(&head_bin[0], len, hmac);
    memcpy(&head_bin[len], hmac, 16);

    // hmac of pkg info
    off = ntohl(*(uint32_t*)&head_bin[0x8]);
    len = ntohl(*(uint32_t*)&head_bin[0x10]);
    out = ntohl(*(uint32_t*)&head_bin[0xD4]);
    fpkg_hmac(&head_bin[off], len - 64, hmac);
    memcpy(&head_bin[out], hmac, 16);

    // hmac of everything
    len = ntohl(*(uint32_t*)&head_bin[0xE8]);
    fpkg_hmac(&head_bin[0], len, hmac);
    memcpy(&head_bin[len], hmac, 16);

    // Make dir
    sceIoMkdir(PACKAGE_TEMP_FOLDER "sce_sys/package", 0777);

    // Write head.bin
    WriteFile(PACKAGE_TEMP_FOLDER "sce_sys/package/head.bin", head_bin, (unsigned int)&_binary_assets_head_bin_size);

    free(head_bin);

    log_printf(DBG_DEBUG, "Created head.bin for %s", titleid);
    return 0;
}

VitaPackage::VitaPackage(const std::string vpk)
    : vpk_(vpk)
{
    log_printf(DBG_INFO, "Loading PAF");
    // ScePaf is required for PromoterUtil
    uint32_t ptr[0x100] = { 0 };
    ptr[0] = 0;
    ptr[1] = (uint32_t)&ptr[0];
    uint32_t scepaf_argp[] = { 0x400000, 0xEA60, 0x40000, 0, 0 };
    sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(scepaf_argp), scepaf_argp, ptr);

    sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
    scePromoterUtilityInit();
}

VitaPackage::~VitaPackage()
{
    log_printf(DBG_INFO, "Unloading PAF");
    scePromoterUtilityExit();
    sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
}

void VitaPackage::Extract(InfoProgress* progress)
{
    int ret = removePath(std::string(PACKAGE_TEMP_FOLDER));

    if (ret < 0)
    {
        log_printf(DBG_ERROR, "removePath(%s) = 0x%08X", PACKAGE_TEMP_FOLDER, ret);
    }

    sceIoMkdir(PACKAGE_TEMP_FOLDER, 0777);

    Zipfile vpk_file = Zipfile(vpk_);

    vpk_file.Unzip(std::string(PACKAGE_TEMP_FOLDER), progress);
    sceIoRemove(vpk_.c_str());
}

int VitaPackage::InstallExtracted(InfoProgress* progress)
{
    log_printf(DBG_DEBUG, "Installing extracted");
    progress->message("Installing...");
    int ret = makeHeadBin();
    if (ret < 0)
    {
        log_printf(DBG_ERROR, "Can't make head.bin for %s: 0x%08X", vpk_.c_str(), ret);
        throw std::runtime_error("Error faking app signature");
    }
    log_printf(DBG_DEBUG, "head.bin created");

    ret = scePromoterUtilityPromotePkg(PACKAGE_TEMP_FOLDER, 0);
    if (ret < 0)
    {
        log_printf(
            DBG_ERROR, "Can't Promote %s: scePromoterUtilityPromotePkg(%s, 0) = 0x%08X", vpk_.c_str(), PACKAGE_TEMP_FOLDER,
            ret);
        throw std::runtime_error("Error installing app");
    }
    log_printf(DBG_DEBUG, "Package promotion started");

    int state = 0;
    unsigned int i = 0;
    do
    {
        ret = scePromoterUtilityGetState(&state);
        if (ret < 0)
        {
            log_printf(DBG_ERROR, "Can't Promote %s: scePromoterUtilityGetState() = 0x%08X", vpk_.c_str(), ret);
            throw std::runtime_error("Error while instaling");
        }

        i += 1;
        if (i < 50 && progress)
            progress->percent(i * 2);
        sceKernelDelayThread(150 * 1000);
    } while (state);

    int result = 0;
    ret = scePromoterUtilityGetResult(&result);
    if (ret < 0)
    {
        log_printf(DBG_DEBUG, "Package promotion ended: failure");
        log_printf(DBG_ERROR, "Can't Promote %s: scePromoterUtilityGetResult() = 0x%08X", vpk_.c_str(), ret);
        throw std::runtime_error("Installation failed");
    }
    log_printf(DBG_DEBUG, "Package promotion ended: success");

    removePath(std::string(PACKAGE_TEMP_FOLDER));

    if (progress)
        progress->percent(100);
    return 0;
}

int VitaPackage::Install(InfoProgress progress)
{
    return Install(&progress);
}

int VitaPackage::Install(InfoProgress* progress)
{
    if (progress)
    {
        InfoProgress progress2;
        progress2 = progress->Range(0, 60);
        Extract(&progress2);
    }
    else
    {
        Extract();
    }

    if (progress)
    {
        InfoProgress progress3 = progress->Range(60, 100);
        return InstallExtracted(&progress3);
    }
    else
    {
        return InstallExtracted();
    }
}

int UpdaterPackage::InstallUpdater(InfoProgress progress)
{
    return InstallUpdater(&progress);
}

int UpdaterPackage::InstallUpdater(InfoProgress* progress)
{
    int ret = removePath(std::string(PACKAGE_TEMP_FOLDER));
    if (ret < 0)
    {
        log_printf(DBG_ERROR, "removePath(%s) = 0x%08X", PACKAGE_TEMP_FOLDER, ret);
    }

    ret = sceIoMkdir(PACKAGE_TEMP_FOLDER, 0777);
    if (ret < 0)
        log_printf(DBG_ERROR, "sceIoMkdir(%s, 0777) = 0x%08X", PACKAGE_TEMP_FOLDER, ret);

    ret = sceIoMkdir(UPDATER_DST_SFO_DIR, 0777);
    if (ret < 0)
        log_printf(DBG_ERROR, "sceIoMkdir(%s, 0777) = 0x%08X", UPDATER_DST_SFO_DIR, ret);

    log_printf(DBG_DEBUG, "Copying %s -> %s", UPDATER_SRC_EBOOT_PATH, UPDATER_DST_EBOOT_PATH);
    ret = copyFile(UPDATER_SRC_EBOOT_PATH, UPDATER_DST_EBOOT_PATH);

    if (progress)
        progress->percent(0);

    if (ret < 0)
    {
        log_printf(DBG_ERROR, "Update failed: Couldn't write eboot.bin");
        return ret;
    }

    log_printf(DBG_DEBUG, "Copying %s -> %s", UPDATER_SRC_SFO_PATH, UPDATER_DST_SFO_PATH);
    ret = copyFile(UPDATER_SRC_SFO_PATH, UPDATER_DST_SFO_PATH);
    if (progress)
        progress->percent(10);

    if (ret < 0)
    {
        log_printf(DBG_ERROR, "Update failed: Couldn't write param.sfo");
        return ret;
    }

    log_printf(DBG_DEBUG, "Installing extracted updater");
    if (progress)
        progress->percent(20);

    if (progress)
    {
        InfoProgress progress2 = progress->Range(20, 100);
        return InstallExtracted(&progress2);
    }
    else
    {
        return InstallExtracted();
    }
}

void UpdatePackage::MakeHeadBin()
{
    int ret = makeHeadBin();
    if (ret < 0)
    {
        log_printf(DBG_ERROR, "Can't make head.bin for Update: 0x%08X", ret);
        throw std::runtime_error("Error faking app signature");
    }
    log_printf(DBG_DEBUG, "head.bin created");
}

bool InstalledVitaPackage::IsInstalled()
{
    int res;
    int ret = scePromoterUtilityCheckExist(title_id.c_str(), &res);
    if (res < 0)
    {
        log_printf(DBG_ERROR, "scePromoterUtilityCheckExist(%s)=0x%08x", title_id.c_str(), res);
        return false;
    }
    return ret >= 0;
}

int InstalledVitaPackage::Uninstall(InfoProgress progress)
{
    return Uninstall(&progress);
}

int InstalledVitaPackage::Uninstall(InfoProgress* progress)
{
    sceAppMgrDestroyOtherApp();
    return scePromoterUtilityDeletePkg(title_id.c_str());
}
