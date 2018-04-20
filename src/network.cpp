#include "network.h"


Network::Network()
{
    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    SceNetInitParam netInitParam;
    int size = 1 * 1024 * 1024;
    netInitParam.memory = malloc(size);
    netInitParam.size = size;
    netInitParam.flags = 0;
    sceNetInit(&netInitParam);
    sceNetCtlInit();

    sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
    sceSslInit(300 * 1024);
    sceHttpInit(1 * 1024 * 1024);

    // Certificate verification for the API fails with SCE_HTTP_ERROR_SSL, SCE_HTTPS_ERROR_SSL_CN_CHECK
    sceHttpsDisableOption(SCE_HTTPS_FLAG_CN_CHECK);

    int ret = sceHttpCreateTemplate(VHBB_UA, SCE_HTTP_VERSION_1_1, SCE_TRUE);
    if (ret < 0)
        throw std::runtime_error("Network: Cannot create template");
    templateId_ = ret;
}

Network::~Network()
{
    sceHttpDeleteTemplate(templateId_);

    sceNetCtlTerm();
    sceNetTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);

    sceHttpTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTPS);

    sceSslEnd();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_SSL);

}

int Network::DownloadSize(std::string url, uint64_t *size, InfoProgress progress)
{
    return DownloadSize(url, size, &progress);
}

int Network::DownloadSize(std::string url, uint64_t *size, InfoProgress *progress)
{
    dbg_printf(DBG_DEBUG, "Checking size of %s", url.c_str());
    int conn = -1;
    int req = -1;

    try {
        conn = sceHttpCreateConnectionWithURL(templateId_, url.c_str(), SCE_TRUE);
        if (conn < 0)
            throw std::runtime_error("Network: Cannot create connection");

        req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url.c_str(), 0);
        if (req < 0)
            throw std::runtime_error("Network: Cannot create request");

        int res = sceHttpSendRequest(req, NULL, 0);
        if (res < 0) {

            dbg_printf(DBG_ERROR, "sceHttpSendRequest() error: 0x%08X", res);
            if (res == SCE_HTTP_ERROR_SSL) {
                int sslErr;
                unsigned int sslErrDetail;
                res = sceHttpsGetSslError(req, &sslErr, &sslErrDetail);
                if (res < 0) {
                    dbg_printf(DBG_ERROR, "sceHttpsGetSslError error: 0x%08X", res);
                } else {
                    dbg_printf(DBG_ERROR, "SSL error: 0x%08X, %u\nSee here for meaning: \
                        https://github.com/vitasdk/vita-headers/blob/master/include/psp2/net/http.h",
                        sslErr,
                        sslErrDetail);
                }
            }

            throw std::runtime_error("Network: Cannot send request");
        }

        if(progress) progress->percent(80);


        int statusCode;
        res = sceHttpGetStatusCode(req, &statusCode);
        if (res < 0)
            throw std::runtime_error("Network: Cannot get status code");

        if (statusCode != 200)
            throw std::runtime_error("Network: unexpected status code");

        res = sceHttpGetResponseContentLength(req, size);
        if (res < 0)
            throw std::runtime_error("Network: Cannot get content length");

    } catch (const std::runtime_error &ex) {
        dbg_printf(DBG_ERROR, "%s", ex.what());
        if (req >= 0) sceHttpDeleteRequest(req);
        if (conn >= 0) sceHttpDeleteConnection(conn);

        throw;
    }

    if(progress) progress->percent(100);

    if (req >= 0) sceHttpDeleteRequest(req);
    if (conn >= 0) sceHttpDeleteConnection(conn);

    return 0;
}

int Network::Download(std::string url, std::string dest, InfoProgress *progress)
{
    //if(progress) progress->message("Starting the download...");

    dbg_printf(DBG_DEBUG, "Downloading %s to %s", url.c_str(), dest.c_str());
    int conn = -1;
    int req = -1;
    int fd = -1;

    try {
        conn = sceHttpCreateConnectionWithURL(templateId_, url.c_str(), SCE_TRUE);
        if (conn < 0)
            throw std::runtime_error("Network: Cannot create connection");

        req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url.c_str(), 0);
        if (req < 0)
            throw std::runtime_error("Network: Cannot create request");

        int res = sceHttpSendRequest(req, NULL, 0);

        if(progress) progress->percent(5);

        int i=0;
        for (i=2; res < 0 && i < 6; i++) {

            dbg_printf(DBG_ERROR, "sceHttpSendRequest() error: 0x%08X", res);
            if (res == SCE_HTTP_ERROR_SSL) {
                int sslErr;
                unsigned int sslErrDetail;
                res = sceHttpsGetSslError(req, &sslErr, &sslErrDetail);
                if (res < 0) {
                    dbg_printf(DBG_ERROR, "sceHttpsGetSslError error: 0x%08X", res);
                } else {
                    dbg_printf(DBG_ERROR, "SSL error: 0x%08X, %u\nSee here for meaning: \
                        https://github.com/vitasdk/vita-headers/blob/master/include/psp2/net/http.h",
                        sslErr,
                        sslErrDetail);
                }
            }
            sceKernelDelayThread((i-1)*500 * 1000);
            if(progress) progress->message("Starting the download... (" + std::to_string(i) + ")");
            res = sceHttpSendRequest(req, NULL, 0);
        }
        if (res < 0)
            throw std::runtime_error("Network: Cannot send request");

        if(progress) progress->message("Downloading...");

        int statusCode;
        res = sceHttpGetStatusCode(req, &statusCode);
        if (res < 0)
            throw std::runtime_error("Network: Cannot get status code");
        if (statusCode != 200)
            throw std::runtime_error("Network: unexpected status code");

        uint64_t contentLength;
        res = sceHttpGetResponseContentLength(req, &contentLength);
        if (res < 0 && progress != nullptr)
            throw std::runtime_error("Network: Cannot get content length");

        if (res >= 0)
            dbg_printf(DBG_DEBUG, "Content length: %lu", contentLength);

        if(progress) progress->percent(10);

        fd = sceIoOpen(dest.c_str(), SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

        unsigned int s_downloaded = 0;
        char buf[4096];
        while(true) {
            int read = sceHttpReadData(req, buf, sizeof(buf));
            if (read < 0)
                throw std::runtime_error("Network: Couldn't read data");

            if (read == 0)
                break;

            s_downloaded += read;
            if(progress) progress->percent(10 + 90*s_downloaded/contentLength);

            int written = sceIoWrite(fd, buf, read);
            if (written < 0)
                throw std::runtime_error("Network: Couldn't write data");
        }


    } catch (const std::runtime_error &ex) {
        dbg_printf(DBG_ERROR, "%s", ex.what());
        if (fd >= 0) sceIoClose(fd);
        if (req >= 0) sceHttpDeleteRequest(req);
        if (conn >= 0) sceHttpDeleteConnection(conn);

        throw;
    }

    dbg_printf(DBG_DEBUG, "Downloaded");

    if(progress) progress->percent(100);

    if (fd >= 0) sceIoClose(fd);
    if (req >= 0) sceHttpDeleteRequest(req);
    if (conn >= 0) sceHttpDeleteConnection(conn);

    return 0;
}

int Network::Download(std::string url, std::string dest, InfoProgress progress)
{
    return Download(url, dest, &progress);
}

InternetStatus Network::TestConnection()
{
    InternetStatus ret = INTERNET_STATUS_OK;
    int req = -1;
    int read = -1;
    int sendRes = -1;
    int res = -1;
    int statusCode = 0;
    uint64_t contentLength;
    char buf[4096] = {0};

    int conn = sceHttpCreateConnectionWithURL(templateId_, PORTAL_DETECT_URL, SCE_TRUE);
    if (conn < 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, PORTAL_DETECT_URL, 0);
    if (req < 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    sendRes = sceHttpSendRequest(req, NULL, 0);

    
    res = sceHttpGetStatusCode(req, &statusCode);

    if (sendRes < 0 || res < 0 || statusCode != 200) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }
    
    res = sceHttpGetResponseContentLength(req, &contentLength);

    if (res >= 0)
        dbg_printf(DBG_DEBUG, "Content length: %lu", contentLength);

    
    read = sceHttpReadData(req, buf, sizeof(buf));
    if (res <= 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    buf[read] = '\0';

    if (strncmp(buf, PORTAL_DETECT_STR, strlen(PORTAL_DETECT_STR))) {
        ret = INTERNET_STATUS_HOTSPOT_PAGE;
        goto clean;
    }

    clean:
    if (req >= 0) {
        sceHttpDeleteRequest(req);
        sceHttpAbortRequest(req);
    }
    if (conn >= 0) sceHttpDeleteConnection(conn);

    return ret;
}