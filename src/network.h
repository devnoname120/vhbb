#pragma once

#include <global_include.h>

#include "singleton.h"
#include "infoProgress.h"

#define VHBB_UA "VHBB/1.00 libhttp/1.1"
#define MAX_QUEUE_CONN 10

class Network: public Singleton<Network> {
friend class Singleton<Network>;
public:
    Network();
    ~Network();

    int Download(std::string url, std::string dest, InfoProgress *progress = nullptr);
    int DownloadSize(std::string url, uint64_t *size, InfoProgress *progress = nullptr);

private:
    int templateId_;
};

    /*
class NetworkQueue
{
    public:
    NetworkQueue();
    ~NetworkQueue();

    Add(std::string url, std::string dest);
    Add(std::vector<std::pair<std::string, std::string>>);
};
*/