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
        progress.message("Starting the download...");

        Network::get_instance()->Download(args->hb.url, std::string("ux0:/temp/download.vpk"), progress.Range(0, 40));

        progress.percent(40);

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

        progress.message("Finished");
        progress.percent(100);
        progressView->Finish(2000);
        
    } catch (const std::exception &ex) {
        progress.message(ex.what());
        progressView->Finish(4000);
        dbg_printf(DBG_ERROR, "%s", ex.what());
    }

    sceKernelExitDeleteThread(0);
}
