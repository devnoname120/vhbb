#include "install_thread.h"
#include <global_include.h>

#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <network.h>
#include <vitaPackage.h>
#include "infoProgress.h"
#include "zip.h"


void install_thread(SceSize args_size, InstallArguments *args) {
    try {
        InfoProgress progress;

        std::shared_ptr<ProgressView> progressView = std::make_shared<ProgressView>(progress);
        Activity::get_instance()->AddView(progressView);

        DownloadArguments *dlArgs = new DownloadArguments;
        dlArgs->url = args->url;
        dlArgs->dest = std::string("ux0:/temp/download.vpk");
        InfoProgress progress2 = progress.Range(0, 40);
        dlArgs->progress = &progress2;

        SceUID thid = sceKernelCreateThread("download_thread", (SceKernelThreadEntry)download_thread, 0x40, 0x10000, 0, 0, NULL);
        sceKernelStartThread(thid, sizeof(DownloadArguments), dlArgs);
        sceKernelWaitThreadEnd(thid, NULL, NULL);

        dbg_printf(DBG_DEBUG, "Downloaded");

        progress.percent(40);

        UnzipArguments *unzipArgs = new UnzipArguments;
        InfoProgress progress3 = progress.Range(40, 100);
        unzipArgs->progress = &progress3;

        int res_zip = 0;
        unzipArgs->res = &res_zip;

        SceUID thid_zip = sceKernelCreateThread("unzip_thread", (SceKernelThreadEntry)unzip_thread, 0x40, 0x10000, 0, 0, NULL);
        sceKernelStartThread(thid_zip, sizeof(UnzipArguments), unzipArgs);
        sceKernelWaitThreadEnd(thid_zip, NULL, NULL);

        if (res_zip < 0) {
            progress.message("Error installing the package");
            progress.percent(100);
            progressView->Finish(4000);
        } else {
            progress.message("Finished");
            progress.percent(100);
            progressView->Finish(2000);
        }
    } catch (const std::exception &ex) {
        dbg_printf(DBG_ERROR, "%s", ex.what());
    }


    sceKernelExitDeleteThread(0);
}

void download_thread(SceSize args_size, DownloadArguments *args) {
    Network::get_instance()->Download(args->url, args->dest, args->progress);
    sceKernelExitDeleteThread(0);
}

void unzip_thread(SceSize args_size, UnzipArguments *args) {
        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        pkg.Install(args->progress);
        sceKernelExitDeleteThread(0);
}