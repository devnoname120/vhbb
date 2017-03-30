#include "install_thread.h"
#include <global_include.h>

#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <network.h>
#include <vitaPackage.h>
#include "infoProgress.h"
#include "zip.h"


void install_thread(SceSize args_size, InstallArguments *args) {
    InfoProgress progressTotal;
	Homebrew targetHb = args->hb;
    auto progressView = std::make_shared<ProgressView>(progressTotal, targetHb);

    InfoProgress progress;

    try {
        Activity::get_instance()->AddView(progressView);

        if (!args->hb.data.empty()) {
            progress = progressTotal.Range(0, 50);

            progress.message("Starting the data download...");
            Network::get_instance()->Download(args->hb.data, std::string("ux0:/temp/data.zip"), progress.Range(0, 50));

            Zipfile zip = Zipfile("ux0:/temp/data.zip");
            zip.Unzip("ux0:/data/", progress.Range(50, 100));

            progress = progressTotal.Range(50, 100);
        } else {
            progress = progressTotal;
        }

        progress.message("Starting the vpk download...");

        Network::get_instance()->Download(args->hb.url, std::string("ux0:/temp/download.vpk"), progress.Range(0, 40));

        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        pkg.Install(progress.Range(40, 100));

        YAML::Emitter info;
        info << YAML::BeginMap;
        info << YAML::Key << "name";
        info << YAML::Value << args->hb.name;
        info << YAML::Key << "version";
        info << YAML::Value << args->hb.version;
        info << YAML::Key << "date";
        info << YAML::Value << args->hb.date.str;
        info << YAML::EndMap;

      	int fd = sceIoOpen((std::string("ux0:app/") + args->hb.titleid + "/vitadb_info.yml").c_str(), SCE_O_WRONLY|SCE_O_CREAT, 0777);
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
