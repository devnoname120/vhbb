#include "install_thread.h"

#include "homebrewInfo.h"
#include "zip.h"

#include <Views/ProgressView/progressView.h>
#include <activity.h>
#include <network.h>
#include <psp2/io/fcntl.h>
#include <vitaPackage.h>

void install_thread(SceSize args_size, InstallArguments *installArgs) {
    
    InfoProgress progressTotal;
	Homebrew targetHb = installArgs->hb;
    auto progressView = std::make_shared<ProgressView>(progressTotal, targetHb);

    InfoProgress progress;

    try {
        Activity::get_instance()->AddView(progressView);

        if (!installArgs->hb.data.empty()) {
            progress = progressTotal.Range(0, 50);

            progress.message("Downloading the data...");
            Network::get_instance()->Download(installArgs->hb.data, std::string("ux0:/temp/data.zip"), progress.Range(0, 50));

            Zipfile zip = Zipfile("ux0:/temp/data.zip");
            zip.Unzip("ux0:/data/", progress.Range(50, 100));

            progress = progressTotal.Range(50, 100);
        } else {
            progress = progressTotal;
        }

        progress.message("Downloading the vpk...");

        Network::get_instance()->Download(installArgs->hb.url, std::string("ux0:/temp/download.vpk"), progress.Range(0, 40));

        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        pkg.Install(progress.Range(40, 100));

        HomebrewInfo hbInfo = HomebrewInfo();
        hbInfo.name = installArgs->hb.name;
        hbInfo.version = installArgs->hb.version;
        hbInfo.date = installArgs->hb.date;

        YAML::Node node = YAML::convert<HomebrewInfo>::encode(hbInfo);
        std::string hbInfoStr = YAML::Dump(node);

        int fd = sceIoOpen((std::string("ux0:app/") + installArgs->hb.titleid + "/vitadb_info.yml").c_str(), SCE_O_WRONLY|SCE_O_CREAT, 0777);
        sceIoWrite(fd, hbInfoStr.c_str(), strlen(hbInfoStr.c_str()));
        sceIoClose(fd);

        progress.percent(100);
        progress.message("Finished");
        progressView->Finish(2000);

    } catch (const std::exception &ex) {
        progress.message(ex.what());
        progressView->Finish(4000);
        log_printf(DBG_ERROR, "%s", ex.what());
    }

    sceKernelExitDeleteThread(0);
}
