#include "fetch_load_icons_thread.h"

#include "Views/dialogView.h"
#include <Views/mainView.h>
#include <activity.h>
#include <database.h>
#include <network.h>

void StartFetchLoadIconsThread()
{
    SceUID thid_db = sceKernelCreateThread("db_thread", (SceKernelThreadEntry)FetchLoadIcons, 0x40, 0x20000, 0, 0, nullptr);
    sceKernelStartThread(thid_db, 0, nullptr);
}

int FetchLoadIcons(unsigned int arglen, std::atomic<bool>* db_done)
{
    try
    {
        // TODO check if fails
        auto dl = Network::get_instance();
        dl->Download(std::string(API_ENDPOINT), std::string(API_LOCAL));
        auto db = Database::create_instance(std::string(API_LOCAL));
        db->DownloadIcons();
        if (db_done)
            *db_done = true;

        auto mainView = std::make_shared<MainView>();
        Activity::get_instance()->AddView(mainView);
    }
    catch (const std::exception& ex)
    {
        log_printf(DBG_ERROR, "Couldn't load database: %s", ex.what());
        DialogView::finalErrorDialog(std::string("Couldn't reach " API_ENDPOINT ":\n") + ex.what());
        return 1;
    }

    sceKernelExitDeleteThread(0);
    return 0;
}
