#include "install_thread.h"
#include <global_include.h>

#include <activity.h>
#include <Views/ProgressView/progressView.h>
#include <network.h>
#include <vitaPackage.h>


int install_thread(SceSize args_size, InstallArguments *args) {
    try {
        int percent = 0;

        std::shared_ptr<ProgressView> progress = std::make_shared<ProgressView>();
        Activity::get_instance()->AddView(progress);
        progress->SetProgress(percent, std::string("Getting size..."));

        uint64_t size;
        Network::get_instance()->DownloadSize(args->url, &size);
        dbg_printf(DBG_DEBUG, "Download size: %lu", size);
        percent += 10;
        progress->SetProgress(percent, std::string("Downloading..."));

        DownloadArguments *dlArgs = new DownloadArguments;
        dlArgs->url = args->url;
        dlArgs->dest = std::string("ux0:/temp/download.vpk");
        bool finished = false;
        dlArgs->finished = &finished;

        uint64_t cur = 0;
        dlArgs->cur = &cur;

        int percent_offset = 0;
        SceUID thid = sceKernelCreateThread("download_thread", (SceKernelThreadEntry)download_thread, 0x40, 0x10000, 0, 0, NULL);
        sceKernelStartThread(thid, sizeof(DownloadArguments), dlArgs);

        while (!*(dlArgs->finished)) {
            percent_offset = (int)(((double)(*(dlArgs->cur))/(double)size) * (double)40);
            progress->SetProgress(percent + percent_offset, std::string("Downloading..."));
            sceKernelDelayThread(1000 * 1000);
        }
        dbg_printf(DBG_DEBUG, "Downloaded");

        percent = 50;
        progress->SetProgress(percent, std::string("Finished downloading"));

        progress->SetProgress(percent, std::string("Installing..."));
        VitaPackage pkg = VitaPackage(std::string("ux0:/temp/download.vpk"));
        int res = pkg.Install();
        if (res < 0) {
            progress->SetProgress(100, std::string("Error installing the package"));
        } else {
            progress->SetProgress(100, std::string("Finished"));
        }
    } catch (const std::exception &ex) {
        dbg_printf(DBG_ERROR, "%s", ex.what());
    }


    sceKernelExitDeleteThread(0);
}

int download_thread(SceSize args_size, DownloadArguments *args) {
    Network::get_instance()->Download(args->url, args->dest, args->cur);
    *(args->finished) = true;
    sceKernelExitDeleteThread(0);
}