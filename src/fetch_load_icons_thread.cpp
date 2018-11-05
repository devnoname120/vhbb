#include "fetch_load_icons_thread.h"

#include <network.h>
#include <database.h>
#include <activity.h>
#include <Views/mainView.h>

void StartFetchLoadIconsThread() {
  SceUID thid_db = sceKernelCreateThread(
            "db_thread", (SceKernelThreadEntry) FetchLoadIcons, 0x40, 0x20000, 0,
            0, nullptr);
  sceKernelStartThread(thid_db, 0, nullptr);
}

void FetchLoadIcons(unsigned int arglen, std::atomic<bool> **db_done)
{
  try {
    // TODO check if fails
    auto dl = Network::get_instance();
    dl->Download(API_ENDPOINT, API_LOCAL);
    auto db = Database::create_instance(API_LOCAL);
    db->DownloadIcons();
    if (db_done)
      **db_done = true;

    auto mainView = std::make_shared<MainView>();
    Activity::get_instance()->AddView(mainView);
  } catch (const std::exception &ex) {
    dbg_printf(DBG_ERROR, "Couldn't load database: %s", ex.what());
    throw ex;
  }

  sceKernelExitDeleteThread(0);
}