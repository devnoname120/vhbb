#ifndef UPDATE_TITLEID
#    error UPDATE_TITLEID must be specified
#endif
#ifndef PACKAGE_DIR
#    warning PACKAGE_DIR not set using default "ux0:data/pkg/"
#    define PACKAGE_DIR "ux0:data/pkg/"
#endif

#include "debug.h"

#include <psp2/appmgr.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/promoterutil.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// strdup() mysteriously disappeared in a vitasdk update, so we use a drop-in implementation.
char *mystrdup(const char *s)
{
    size_t len = strlen(s) + 1;
    void *copy = malloc(len);
    if (copy == nullptr)
    {
        return nullptr;
    }

    return (char *)memcpy(copy, s, len);
}

struct SfoHeader
{
    uint32_t magic;
    uint32_t version;
    uint32_t keyofs;
    uint32_t valofs;
    uint32_t count;
} __attribute__((packed));

struct SfoEntry
{
    uint16_t nameofs;
    uint8_t alignment;
    uint8_t type;
    uint32_t valsize;
    uint32_t totalsize;
    uint32_t dataofs;
} __attribute__((packed));

int launchAppByUriExit(const char* titleid)
{
    char uri[32];
    sprintf(uri, "psgm:play?titleid=%s", titleid);

    sceAppMgrLaunchAppByUri(0xFFFFF, uri);
    sceKernelExitProcess(0);

    return 0;
}

static int loadScePaf()
{
    static uint32_t argp[] = { 0x180000, 0xFFFFFFFF, 0xFFFFFFFF, 1, 0xFFFFFFFF, 0xFFFFFFFF };

    int result = -1;

    SceSysmoduleOpt opt = { 0 };
    opt.flags = sizeof(opt);
    opt.result = &result;
    opt.unused[0] = -1;
    opt.unused[1] = -1;

    return sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(argp), argp, &opt);
}

static int unloadScePaf()
{
    SceSysmoduleOpt opt = { 0 };
    return sceSysmoduleUnloadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, 0, nullptr, &opt);
}

int promoteApp(const char* path)
{
    int res;

    res = loadScePaf();
    if (res < 0)
    {
        log_printf(DBG_ERROR, "loadScePaf() = 0x%08X", res);
        return res;
    }

    res = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
    if (res < 0)
    {
        log_printf(DBG_ERROR, "sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL) = 0x%08X", res);
        return res;
    }

    res = scePromoterUtilityInit();
    if (res < 0)
    {
        log_printf(DBG_ERROR, "scePromoterUtilityInit() = 0x%08X", res);
        return res;
    }

    res = scePromoterUtilityPromotePkgWithRif(path, 1);
    if (res < 0)
    {
        log_printf(DBG_ERROR, "scePromoterUtilityPromotePkgWithRif(%s, 1) = 0x%08X", path, res);
        return res;
    }

    res = scePromoterUtilityExit();
    if (res < 0)
    {
        log_printf(DBG_ERROR, "scePromoterUtilityExit() = 0x%08X", res);
        return res;
    }

    res = sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
    if (res < 0)
    {
        log_printf(DBG_ERROR, "sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL) = 0x%08X", res);
        return res;
    }

    res = unloadScePaf();
    if (res < 0)
    {
        log_printf(DBG_ERROR, "unloadScePaf() = 0x%08X", res);
        return res;
    }

    return res;
}

char* get_title_id(const char* filename)
{
    log_printf(DBG_DEBUG, "Extracting titleid from %s", filename);
    char* res = nullptr;
    long size = 0;
    FILE* fin = nullptr;
    char* buf = nullptr;
    int i, ret;

    SfoHeader* header;
    SfoEntry* entry;

    fin = fopen(filename, "rb");
    if (!fin)
    {
        log_printf(DBG_ERROR, "fopen(%s, \"rb\") = 0x%08X", filename, fin);
        goto cleanup;
    }

    ret = fseek(fin, 0, SEEK_END);
    if (ret != 0)
    {
        log_printf(DBG_ERROR, "fseek(fin, 0, SEEK_END) = 0x%08X", ret);
        goto cleanup;
    }

    size = ftell(fin);
    if (size == -1)
    {
        log_printf(DBG_ERROR, "ftell(fin) = 0x%08X", size);
        goto cleanup;
    }

    ret = fseek(fin, 0, SEEK_SET);
    if (ret != 0)
    {
        log_printf(DBG_ERROR, "fseek(fin, 0, SEEK_SET) = 0x%08X", ret);
        goto cleanup;
    }

    buf = (char*)calloc(1, size + 1);
    if (!buf)
    {
        log_printf(DBG_ERROR, "Couldn't allocate %i bytes of memory to read sfo file", size);
        goto cleanup;
    }

    ret = fread(buf, size, 1, fin);
    if (ret != 1)
    {
        log_printf(DBG_ERROR, "fread(buf, %i, 1, fin) = 0x%08X", size, ret);
        goto cleanup;
    }

    header = (SfoHeader*)buf;
    entry = (SfoEntry*)(buf + sizeof(SfoHeader));
    log_printf(DBG_DEBUG, "SFO header announces %i entries.", header->count);

    for (i = 0; i < header->count; ++i, ++entry)
    {
        const char* name = buf + header->keyofs + entry->nameofs;
        const char* value = buf + header->valofs + entry->dataofs;

        if (name >= buf + size || value >= buf + size)
            break;

        log_printf(DBG_DEBUG, "SFO body %s: %s", name, value);
        if (strcmp(name, "TITLE_ID") == 0)
            res = mystrdup(value);
    }

    log_printf(DBG_DEBUG, "Found title id: %s", res);

cleanup:
    if (buf)
        free(buf);
    if (fin)
        fclose(fin);

    return res;
}

int main()
{
    log_init(true);

    int ret;
    ret = sceShellUtilInitEvents(0);
    if (ret < 0)
        log_printf(DBG_ERROR, "sceShellUtilInitEvents(0)=0x%08X", ret);

    ret = sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);
    if (ret < 0)
        log_printf(DBG_ERROR, "sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN)=0x%08X", ret);

    ret = sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_POWEROFF_MENU);
    if (ret < 0)
        log_printf(DBG_ERROR, "sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_POWEROFF_MENU)=0x%08X", ret);

    log_printf(DBG_DEBUG, "All button locks done");

    sceAppMgrDestroyOtherApp();
    log_printf(DBG_INFO, "Killed other apps");

    char* titleid = get_title_id(PACKAGE_DIR "/sce_sys/param.sfo");
    log_printf(DBG_DEBUG, "Found staged app: %s; looking for: %s", titleid, UPDATE_TITLEID);

    if (titleid && strcmp(titleid, UPDATE_TITLEID) == 0)
    {
        log_printf(DBG_INFO, "Staged update found -> Start promoting");
        promoteApp(PACKAGE_DIR);
    }
    else
    {
        log_printf(DBG_WARNING, "Staged app %s didn't match expected app %s", titleid, UPDATE_TITLEID);
    };

    log_printf(DBG_INFO, "All done. Starting updated app: %s", UPDATE_TITLEID);

    sceKernelDelayThread(250000);
    launchAppByUriExit(UPDATE_TITLEID);

    return 0;
}
