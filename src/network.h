#pragma once

#include <global_include.h>

#include "singleton.h"
#include "infoProgress.h"

#define PORTAL_DETECT_URL "http://detectportal.firefox.com/success.txt" // Note : similar portal detector http://www.apple.com/library/test/success.html
#define PORTAL_DETECT_STR "success"
#define VHBB_UA "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:61.0) Gecko/20100101 Firefox/61.0"

typedef enum {
    INTERNET_STATUS_OK,
    INTERNET_STATUS_NO_INTERNET,
    INTERNET_STATUS_HOTSPOT_PAGE,
} InternetStatus;

class Network: public Singleton<Network> {
friend class Singleton<Network>;
public:
    Network();
    ~Network();

    int Download(std::string url, std::string dest, InfoProgress progress);
    int Download(std::string url, std::string dest, InfoProgress *progress = nullptr);

    InternetStatus TestConnection();
private:
    int templateId_;
};

