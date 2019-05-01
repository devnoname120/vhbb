#include "update.h"

#include <atomic>
#include <array>

#include "network.h"
#include "filesystem.h"
#include "vitaPackage.h"
#include "zip.h"
#include "Views/dialogView.h"

// FIXME: tmp url for deving
#define BASE_ADDRESS "https://raw.githubusercontent.com/robsdedude/vhbb/autoUpdate/"
#define VERSION_URL BASE_ADDRESS "release/version.bin"
#define UPDATE_URL BASE_ADDRESS "release/" VHBB_SHORT_NAME ".vpk"
#define VERSION_PATH std::string(VHBB_DATA + "/latest_version.bin")

enum UpdateState {
	UPDATE_STATE_RUNNING,
	UPDATE_STATE_DONE,
	UPDATE_STATE_READY_TO_LAUNCH_UPDATER
};

typedef std::atomic<UpdateState> AtomicUpdateState;

constexpr int matchDigit(const char *text) {
	return *text == '0' || *text == '1' || *text == '2' || *text == '3' || *text == '4' ||
	       *text == '5' || *text == '6' || *text == '7' || *text == '8' || *text == '9';
}

constexpr int matchVersionString(const char *text) {
	// expect version to be =~ \d\d\.\d\d
	return *text != '\0' && matchDigit(text) && *(text+1) != '\0' && matchDigit(text+1) &&
	       *(text+2) != '\0' && *(text+2) == '.' &&
	       *(text+3) != '\0' && matchDigit(text+3) && *(text+4) != '\0' && matchDigit(text+4) &&
	       *(text+5) == '\0';
}

bool Update::updateExists() {
	int res;
	res = sceIoRemove(VERSION_PATH.c_str());
	log_printf(DBG_ERROR, "sceIoRemove(%s) = 0x%08x", VERSION_PATH.c_str(), res);
	try {
		res = Network::get_instance()->Download(std::string(VERSION_URL), VERSION_PATH);
	} catch (const std::runtime_error &err) {
		log_printf(DBG_ERROR, "Couldn't download version.bin: %s", err.what());
		DialogView::openDialogView(nullptr, std_string_format("Couldn't check for update\n\n%s", err.what()),
		                           DIALOG_TYPE_OK);
		return false;
	}
	if (res) {
		log_printf(DBG_ERROR, "Couldn't download version.bin 0x%08x", res);
		return false;
	}
	std::array<uint32_t, 2> latestVersion, currentVersion;
	res = readFile(VERSION_PATH, latestVersion.data(), sizeof(uint32_t)*2);
	if (res < 0) {
		log_printf(DBG_ERROR, "Couldn't read \"%s\": 0x%08x", VERSION_PATH.c_str(), res);
		return false;
	}
	log_printf(DBG_INFO, "Latest online version: %02i.%02i", latestVersion[0], latestVersion[1]);
	res = sceIoRemove(VERSION_PATH.c_str());
	if (res) {
		log_printf(DBG_ERROR, "Couldn't delete %s: 0x%08x", VERSION_PATH.c_str(), res);
	}
	auto currentVersionStr = std::string(VITA_VERSION);
	static_assert(matchVersionString(VITA_VERSION),
	              "VITA_VERSION=" VITA_VERSION " but must match \\d\\d\\.\\d\\d");
	currentVersion[0] = std::stoi(currentVersionStr.substr(0, 2));
	currentVersion[1] = std::stoi(currentVersionStr.substr(3, 2));
	if (latestVersion > currentVersion) {
		log_printf(DBG_INFO, "Current version " VITA_VERSION " is outdated.");
		return true;
	}
	log_printf(DBG_DEBUG, "Current version " VITA_VERSION " is up-to-date");
	return false;
}

extern unsigned char _binary_assets_spr_img_updater_icon_png_start;

std::shared_ptr<Background> Update::startBackgroundView() {
	auto bgView = std::make_shared<Background>();
	bgView->priority = 749;
	Activity::get_instance()->AddView(bgView);
	return bgView;
}

std::shared_ptr<ProgressView> Update::startProgressView(InfoProgress progress, std::string title) {
	auto progressView = std::make_shared<ProgressView>(progress, std::move(title), Texture(&_binary_assets_spr_img_updater_icon_png_start));
	progressView->priority = 750;
	Activity::get_instance()->AddView(progressView);
	return progressView;
}

void Update::updateThread(unsigned int arglen, void* argv[]) {
	auto updateState_ptr = (AtomicUpdateState*)argv[0];
	if (Update::updateExists()) {
		DialogViewResult res{};
		DialogView::openDialogView(&res, "A new version of VHBB is available.\nDo you want to update?", DIALOG_TYPE_YESNO);
		while (res.status == COMMON_DIALOG_STATUS_RUNNING || res.status == COMMON_DIALOG_STATUS_NONE) {
			sceKernelDelayThread(16666);  // roughly 1/60 second
		}
		if (res.accepted) {
			log_printf(DBG_INFO, "User chose to update VHBB");
			InfoProgress progress;
			auto progressView = startProgressView(progress, "Update Helper (1/2)");
			auto bgView = startBackgroundView();
			try {
				installUpdater(progress.Range(0, 20));
				progressView->hb_name = "VHBB Update (2/2)";
				prepareUpdateFiles(progress.Range(20, 100));
				progress.message("Finished");
				progressView->Finish(700);
				sceKernelDelayThread(700000);
				bgView->request_destroy = true;
				updateState_ptr->store(UPDATE_STATE_READY_TO_LAUNCH_UPDATER);
				return;
			} catch (std::exception &ex) {
				progress.message(std_string_format("Update failed: %s", ex.what()));
				progressView->Finish(4000);
				sceKernelDelayThread(4000000);
				bgView->request_destroy = true;
			}
		} else {
			log_printf(DBG_INFO, "User refused to update VHBB");
		}
	}
	updateState_ptr->store(UPDATE_STATE_DONE);
}

AtomicUpdateState updateState{UPDATE_STATE_RUNNING};

void Update::startUpdateThread() {
	SceUID thid = sceKernelCreateThread("update_check_thread", (SceKernelThreadEntry)Update::updateThread,
	                                    0x40, 0x20000, 0, 0, nullptr);
	auto updateState_ptr = &updateState;
	sceKernelStartThread(thid, sizeof(updateState_ptr), &updateState_ptr);
}

void Update::tick() {
	if (updateState.load() == UPDATE_STATE_READY_TO_LAUNCH_UPDATER)
		Update::startUpdaterApp();
}

bool Update::checkIsDone() {
	return updateState.load() == UPDATE_STATE_DONE;
}

void Update::installUpdater(InfoProgress progress) {
	log_printf(DBG_DEBUG, "Installing updater");
	int ret = UpdaterPackage().InstallUpdater(std::move(progress));
	if (ret >= 0) {
		log_printf(DBG_DEBUG, "Installed updater");
	} else {
		log_printf(DBG_DEBUG, "Installing updater failed: 0x%08x", ret);
		throw std::runtime_error(std_string_format("Couldn't install updater (0x%08x)", ret));
	}
}

void Update::prepareUpdateFiles(InfoProgress progress) {
	log_printf(DBG_DEBUG, "Downloading update vpk");
	progress.message("Downloading update...");

	Network::get_instance()->Download(UPDATE_URL, std::string("ux0:/temp/download.vpk"), progress.Range(0, 60));

	log_printf(DBG_DEBUG, "Extracting update vpk");
	auto pkg = UpdatePackage(std::string("ux0:/temp/download.vpk"));
	InfoProgress progress2 = progress.Range(60, 95);
	pkg.Extract(&progress2);
	log_printf(DBG_DEBUG, "Extracted update vpk");
	pkg.MakeHeadBin();
	progress.percent(100);
	log_printf(DBG_DEBUG, "Made update head.bin");
}

void Update::startUpdaterApp() {
	log_printf(DBG_DEBUG, "Starting updater " UPDATER_TITLEID);
	char uri[32];
	sprintf(uri, "psgm:play?titleid=%s", UPDATER_TITLEID);

	int ret = sceAppMgrLaunchAppByUri(0xFFFFF, uri);
	if (ret == 0) {
		sceKernelExitProcess(0);
	} else {
		log_printf(DBG_WARNING, "Couldn't start updater 0x%08x", ret);
	}
}
