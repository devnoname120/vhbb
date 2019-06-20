#include "debug.h"

#include <cstdarg>
#include <cstring>
#include <debugnet.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/rtc.h>

int g_log_fd = -1;
bool g_log_to_file = false;

extern "C" {
int _vshSblGetSystemSwVersion(SceKernelFwInfo* data);
}

int log_init(bool log_to_file)
{
    g_log_to_file = log_to_file;

#ifdef DEBUGNET
    debugNetInit(DEBUGNETIP, 18194, 3);
#endif

    if (log_to_file)
    {
        sceIoMkdir(VHBB_LOG_DIR, 0777);

        SceDateTime logTime;
        memset(&logTime, 0, sizeof(logTime));

        sceRtcGetCurrentClockLocalTime(&logTime);

        char formattedTime[40] = { 0 };
        snprintf(
            formattedTime, sizeof(formattedTime), "%04d-%02d-%02d_%02d-%02d-%02d", logTime.year, logTime.month, logTime.day,
            logTime.hour, logTime.minute, logTime.second);

        std::string log_file = std::string(VHBB_LOG_DIR "/VHBB_") + formattedTime + ".log";

        g_log_fd = sceIoOpen(log_file.c_str(), SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

        _log_printf(DBG_INFO, "VHBB started.\n");
        _log_printf(DBG_INFO, "- Version: %s\n", VITA_VERSION);
        _log_printf(DBG_INFO, "- Commit: %s\n", GIT_COMMIT);
        _log_printf(DBG_INFO, "- GitHub: %s%s\n", "https://github.com/devnoname120/vhbb/tree/", GIT_COMMIT);

        SceKernelFwInfo data;
        data.size = sizeof(SceKernelFwInfo);

        if (_vshSblGetSystemSwVersion(&data) >= 0) // sceKernelGetSystemSwVersion is spoofed version
        {
            char version[16];
            snprintf(version, 16, "%s", data.versionString);
            _log_printf(DBG_INFO, "- OS: %s\n", version);
        }

        _log_printf(DBG_INFO, "\n");
    }

    return 0;
}

int _log_printf(int level, const char* format, ...)
{
// If no logging is needed at all
#ifndef DEBUGNET
    if (!g_log_to_file)
        return 0;
#endif

    va_list args;
    va_start(args, format);

    char buf[512];
    vsnprintf(buf, 512, format, args);

    char buf_colored[600];
    switch (level)
    {
        case DBG_INFO:
            snprintf(buf_colored, 600, "\033[1;34;7m[INFO]\033[0m\033[1;34m %s\033[0m", buf);
            break;
        case DBG_ERROR:
            snprintf(buf_colored, 600, "\033[1;31;7m[ERROR]\033[0m\033[1;31m %s\033[0m", buf);
            break;
        case DBG_WARNING:
            snprintf(buf_colored, 600, "\033[1;35;7m[WARNING]\033[0m\033[1;35m %s\033[0m", buf);
            break;
        case DBG_DEBUG:
            snprintf(buf_colored, 600, "\033[1;30;7m[DEBUG]\033[0m\033[1;30m %s\033[0m", buf);
            break;
        default:
            snprintf(buf_colored, 600, "\033[1;31;7m[UNK]\033[0m\033[1;31m %s\033[0m", buf);
            break;
    }

    va_end(args);

#ifdef DEBUGNET
    debugNetUDPSend(buf_colored);
#endif

    if (g_log_to_file)
    {
        sceIoWrite(g_log_fd, buf, strlen(buf));
        sceIoSyncByFd(g_log_fd); // TODO Is this actually required?
    }

    return 0;
}
