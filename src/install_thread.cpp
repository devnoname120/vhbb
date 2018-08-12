#include "install_thread.h"
#include <global_include.h>

#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <network.h>
#include <vitaPackage.h>
#include "infoProgress.h"
#include "zip.h"
#include "CancelHandler.h"


void install_thread(SceSize args_size, InstallArguments *installArgs) {
    
    InfoProgress progressTotal;
	Homebrew targetHb = installArgs->hb;

    dbg_printf(DBG_DEBUG, "1");
    CancelHandler cancelHandler;
    CancelGetter cancelGetter = std::bind(&CancelHandler::GetCancelStatus, &cancelHandler);
    dbg_printf(DBG_DEBUG, "1b");

    bool shouldCancel = cancelGetter();

    dbg_printf(DBG_DEBUG, "2: %d", shouldCancel);

    auto progressView = std::make_shared<ProgressView>(progressTotal, targetHb, &cancelHandler);

    InfoProgress progress;

    try {
        Activity::get_instance()->AddView(progressView);

        if (!installArgs->hb.data.empty()) {
            progress = progressTotal.Range(0, 50);
            dbg_printf(DBG_DEBUG, "3");

            progress.message("Downloading the data...");
            Network::get_instance()->Download(installArgs->hb.data, std::string("ux0:/temp/data.zip"), progress.Range(0, 50));

            Zipfile zip = Zipfile("ux0:/temp/data.zip");
            zip.Unzip("ux0:/data/", progress.Range(50, 100));

            progress = progressTotal.Range(50, 100);
        } else {
            progress = progressTotal;
        }
        dbg_printf(DBG_DEBUG, "4");

        progress.message("Downloading the vpk...");

        Network::get_instance()->Download(installArgs->hb.url, std::string("ux0:/temp/download.vpk"), progress.Range(0, 40), cancelGetter);
        dbg_printf(DBG_DEBUG, "5");

        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        pkg.Install(progress.Range(40, 100));

        YAML::Emitter info;
        info << YAML::BeginMap;
        info << YAML::Key << "name";
        info << YAML::Value << installArgs->hb.name;
        info << YAML::Key << "version";
        info << YAML::Value << installArgs->hb.version;
        info << YAML::Key << "date";
        info << YAML::Value << installArgs->hb.date.str;
        info << YAML::EndMap;

      	int fd = sceIoOpen((std::string("ux0:app/") + installArgs->hb.titleid + "/vitadb_info.yml").c_str(), SCE_O_WRONLY|SCE_O_CREAT, 0777);
		sceIoWrite(fd, info.c_str(), strlen(info.c_str()));
		sceIoClose(fd);

        progress.percent(100);
        progress.message("Finished");
        progressView->Finish(2000);

    } catch (const std::exception &ex) {
        progress.message(ex.what());
        progressView->Finish(4000);
        dbg_printf(DBG_ERROR, "%s", ex.what());
    }

    sceKernelExitDeleteThread(0);
}
