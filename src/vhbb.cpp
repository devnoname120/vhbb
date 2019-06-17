#include <psp2/apputil.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h>
#include <psp2/display.h>


#include "Views/mainView.h"
#include "Views/splash.h"
#include "network.h"
#include "nosleep_thread.h"
#include "fetch_load_icons_thread.h"
#include "vitasdk_quirks.h"
#include "update.h"
#include "vitaPackage.h"


void debug_start() {
  SceAppUtilInitParam initParam;
  SceAppUtilBootParam bootParam;
  memset(&initParam, 0, sizeof(SceAppUtilInitParam));
  memset(&bootParam, 0, sizeof(SceAppUtilBootParam));

  int retInit = sceAppUtilInit(&initParam, &bootParam);
  SceAppUtilAppEventParam eventParam;
  memset(&eventParam, 0, sizeof(SceAppUtilAppEventParam));
  int retReceive = sceAppUtilReceiveAppEvent(&eventParam);

  // 0x05 means launched from livearea with params (currently only possible param is -file_logging)
  log_init(retInit < 0 || retReceive < 0 || eventParam.type == 0x05);
}

void network_test() {
  Network &network = *Network::create_instance();

  InternetStatus netStatus = network.TestConnection();

  switch (netStatus) {
    case INTERNET_STATUS_OK:
      break;
    case INTERNET_STATUS_NO_INTERNET:
    case INTERNET_STATUS_HOTSPOT_PAGE:
      log_printf(DBG_ERROR, "Connection status: %d", netStatus);

      for (int i = 0; i < 40; i++) {
        if (sceAppMgrLaunchAppByUri(0xFFFFF, PORTAL_DETECT_URL) != 0) {
          break;
        }
        sceKernelDelayThread(10000);
      }

      sceKernelExitProcess(0);
      break;
    }
    // FIXME Handle network issues more gracefully
    // https://bitbucket.org/xerpi/vita-ftploader/src/87ef1d13a8aaf092f376cbf2818a22cd0e481fd6/plugin/main.c?at=master&fileviewer=file-view-default#main.c-155
}

void mainLoopTick(Input &input, Activity &activity) {
  vita2d_start_drawing();
  vita2d_clear_screen();

  input.Get();

  activity.FlushQueue();
  activity.HandleInput(1, input);
  activity.Display();

  vita2d_end_drawing();
  vita2d_common_dialog_update();
  vita2d_swap_buffers();
  sceDisplayWaitVblankStart();
}

int main() {
  sceIoMkdir(VHBB_DATA, 0777);

  std::set_terminate(terminate_logger);
  debug_start();

  // Sleep invalidates file descriptors
  StartNoSleepThread();

  // remove updater app if installed
  {  // extra scope needed to cause VitaPackage cleanup procedure
    auto updaterPkg = InstalledVitaPackage(UPDATER_TITLEID);
    if(updaterPkg.IsInstalled()) {
      log_printf(DBG_DEBUG, "Found updater installed -> removing");
      int ret = updaterPkg.Uninstall();
      if (ret < 0)
        log_printf(DBG_ERROR, "updaterPkg.Uninstall() = 0x%08x", ret);
    }
  }

  network_test();
  Update::startUpdateThread();

  vita2d_init();
  vita2d_set_clear_color(COLOR_BLACK);

  Input input;
  Activity &activity = *Activity::create_instance();

  auto splash = std::make_shared<Splash>();
  splash->priority = 200;
  activity.AddView(splash);

  while (!Update::checkIsDone()) {
    Update::tick();
    mainLoopTick(input, activity);
  }

  StartFetchLoadIconsThread();

  while (true) {
    mainLoopTick(input, activity);
  }

  return 0;
}
