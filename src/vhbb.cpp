#include "vhbb.h"

#include "Views/mainView.h"
#include "Views/splash.h"
#include "activity.h"
#include "database.h"
#include "input.h"
#include "network.h"
#include "nosleep_thread.h"
#include "splash_thread.h"

#include <atomic>
#include <Views/Dialogs/MessageDialog.h>

extern "C" {
unsigned int sleep(unsigned int seconds) {
  sceKernelDelayThread(seconds * 1000 * 1000);
  return 0;
}

int usleep(useconds_t usec) {
  sceKernelDelayThread(usec);
  return 0;
}

void __sinit(struct _reent *);
}

__attribute__((constructor(101))) void pthread_setup(void) {
  pthread_init();
  __sinit(_REENT);
}

void terminate_logger() {
  std::exception_ptr p = std::current_exception();
  try {
    std::rethrow_exception(p);
  } catch (const std::exception &e) {
    dbg_printf(DBG_ERROR, "terminate() because of %s", e.what());
  }
}

void FetchLoadIcons(SceSize arglen, std::atomic_bool** db_done)
{
  try {
    // TODO check if fails
    auto dl = Network::get_instance();
    dl->Download(API_ENDPOINT, API_LOCAL);
    auto db = Database::create_instance(API_LOCAL);
    db->DownloadIcons();
    **db_done = true;

    auto mainView = std::make_shared<MainView>();
    Activity::get_instance()->AddView(mainView);
  } catch (const std::exception &ex) {
    dbg_printf(DBG_ERROR, "Couldn't load database: %s", ex.what());
    throw ex;
  }

  sceKernelExitDeleteThread(0);
}

int main() {
  sceIoMkdir(VHBB_DATA.c_str(), 0777);

  vita2d_init();
//  vita2d_set_vblank_wait(true);
  vita2d_set_clear_color(COLOR_BLACK);

  // Sleep crashes the app
  SceUID thid_sleep = sceKernelCreateThread(
      "nosleep_thread", (SceKernelThreadEntry)nosleep_thread, 0x40, 0x1000, 0,
      0, NULL);
  sceKernelStartThread(thid_sleep, 0, NULL);

  dbg_init();

  std::set_terminate(terminate_logger);

  Network &network = *Network::create_instance();

  Input input;

  Activity &activity = *Activity::create_instance();


//  switch (network.TestConnection()) {
//
//    case INTERNET_STATUS_OK: {
//      SceUID thid_db = sceKernelCreateThread(
//              "db_thread", (SceKernelThreadEntry) FetchLoadIcons, 0x40, 0x20000, 0,
//              0, NULL);
//      sceKernelStartThread(thid_db, 0, nullptr);
//
//      auto splash = std::make_shared<Splash>();
//      splash->priority = 200;
//      activity.AddView(splash);
//      break;
//    }
//    case INTERNET_STATUS_NO_INTERNET:
//    case INTERNET_STATUS_HOTSPOT_PAGE:
//      // FIXME Don't crash if network not available, see
//      // https://bitbucket.org/xerpi/vita-ftploader/src/87ef1d13a8aaf092f376cbf2818a22cd0e481fd6/plugin/main.c?at=master&fileviewer=file-view-default#main.c-155
//      break;
//  }

  auto msgDialog = std::make_shared<MessageDialog>();
  msgDialog->priority = 1000;
  activity.AddView(msgDialog);


  while (1) {
    vita2d_start_drawing();
    vita2d_clear_screen();

    input.Get();

    activity.FlushQueue();
    activity.HandleInput(1, input);


    vita2d_end_drawing();
    activity.Display();
    vita2d_swap_buffers();
    sceDisplayWaitVblankStart();
  }

  return 0;
}
