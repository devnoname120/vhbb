#include "vhbb.h"


#include "Views/mainView.h"
#include "Views/splash.h"
#include "activity.h"
#include "input.h"
#include "debug.h"
#include "network.h"
#include "nosleep_thread.h"
#include "splash_thread.h"
#include "fetch_load_icons_thread.h"

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

int main() {
  sceIoMkdir(VHBB_DATA.c_str(), 0777);

  SceAppUtilInitParam initParam;
  SceAppUtilBootParam bootParam;
  memset(&initParam, 0, sizeof(SceAppUtilInitParam));
  memset(&bootParam, 0, sizeof(SceAppUtilBootParam));

  int retInit = sceAppUtilInit(&initParam, &bootParam);
  SceAppUtilAppEventParam eventParam;
  memset(&eventParam, 0, sizeof(SceAppUtilAppEventParam));
  int retReceive = sceAppUtilReceiveAppEvent(&eventParam);

  // 0x05 means launched from livearea with params (currently only possible param is -file_logging)
  if (retInit < 0 || retReceive < 0 || eventParam.type == 0x05) {
    dbg_init(true);
  // If params need to be actually fetched
  //  char buffer[2048] = {0};
  //  sceAppUtilAppEventParseLiveArea(&eventParam, buffer);
  } else {
    dbg_init(false);
  }

  vita2d_init();
  vita2d_set_clear_color(COLOR_BLACK);

  // Sleep crashes the app
  SceUID thid_sleep = sceKernelCreateThread(
      "nosleep_thread", (SceKernelThreadEntry)nosleep_thread, 0x40, 0x1000, 0,
      0, NULL);
  sceKernelStartThread(thid_sleep, 0, NULL);

  std::set_terminate(terminate_logger);

  Network &network = *Network::create_instance();
  
  int netStatus = network.TestConnection();
  switch (netStatus) {
    case INTERNET_STATUS_OK:
      break;
    case INTERNET_STATUS_NO_INTERNET:
    case INTERNET_STATUS_HOTSPOT_PAGE:
      dbg_printf(DBG_ERROR, "Connection status: %d", netStatus);
      sceAppMgrLaunchAppByUri(0xFFFFF, PORTAL_DETECT_URL);
		  sceKernelDelayThread(10000);
		  sceAppMgrLaunchAppByUri(0xFFFFF, PORTAL_DETECT_URL);
		  sceKernelExitProcess(0);
      break;
  }
  

  // FIXME Handle network issues more gracefully
  // https://bitbucket.org/xerpi/vita-ftploader/src/87ef1d13a8aaf092f376cbf2818a22cd0e481fd6/plugin/main.c?at=master&fileviewer=file-view-default#main.c-155


  SceUID thid_db = sceKernelCreateThread(
          "db_thread", (SceKernelThreadEntry) FetchLoadIcons, 0x40, 0x20000, 0,
          0, nullptr);
  sceKernelStartThread(thid_db, 0, nullptr);

  Input input;

  Activity &activity = *Activity::create_instance();

  auto splash = std::make_shared<Splash>();
  splash->priority = 200;
  activity.AddView(splash);

  while (1) {
    vita2d_start_drawing();
    vita2d_clear_screen();

    input.Get();

    activity.FlushQueue();
    activity.HandleInput(1, input);
    activity.Display();

    vita2d_end_drawing();
    vita2d_swap_buffers();
    sceDisplayWaitVblankStart();
  }

  return 0;
}
