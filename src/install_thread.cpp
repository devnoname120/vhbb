#include "install_thread.h"
#include <global_include.h>

#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <network.h>
#include <vitaPackage.h>
#include "infoProgress.h"
#include "zip.h"


void install_thread(SceSize args_size, InstallArguments *args) {
    InfoProgress progress;
    auto progressView = std::make_shared<ProgressView>(progress);
    try {
        Activity::get_instance()->AddView(progressView);

        Network::get_instance()->Download(args->url, std::string("ux0:/temp/download.vpk"), progress.Range(0, 40));
        dbg_printf(DBG_DEBUG, "Downloaded");

        progress.percent(40);

        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        pkg.Install(progress.Range(40, 100));

        progress.message("Finished");
        progress.percent(100);
        progressView->Finish(2000);
        
    } catch (const std::exception &ex) {
        progress.message(ex.what());
        progress.percent(100);
        progressView->Finish(4000);
        dbg_printf(DBG_ERROR, "%s", ex.what());
    }

    sceKernelExitDeleteThread(0);
}
