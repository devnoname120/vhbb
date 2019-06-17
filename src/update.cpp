#include "update.h"

#include <atomic>
#include <iostream>
#include <fstream>
#include <psp2/kernel/processmgr.h>
#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>

#include "network.h"
#include "filesystem.h"
#include "vitaPackage.h"
#include "Views/dialogView.h"
#include "utils.h"

#define VERSION_YAML_PATH (VHBB_DATA "/latest_version.yml")

struct VersionYAML {
	std::string version;
	std::string url;
};

namespace YAML {
	template<>
	struct convert<VersionYAML> {
		static bool decode(const Node &node, VersionYAML &version) {
			version.version = node["version"].as<std::string>();
			version.url = node["url"].as<std::string>();
			return true;
		}
	};
};

struct VersionInfo {
	std::array<int, 2> latestVersion, currentVersion;
	std::string url;
};



enum UpdateState {
	UPDATE_STATE_RUNNING,
	UPDATE_STATE_DONE,
	UPDATE_STATE_READY_TO_LAUNCH_UPDATER
};

typedef std::atomic<UpdateState> AtomicUpdateState;

extern unsigned char _binary_assets_spr_img_updater_icon_png_start;
AtomicUpdateState updateState{UPDATE_STATE_RUNNING};

// Note: std::isdigit is not a constexpr so cannot be used to replace this function
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

int readVersionYAML(const std::string &filePath, VersionInfo &vInfo) {
	std::string versionYamlContent;
	int res = readFile(std::string(VERSION_YAML_PATH), versionYamlContent);
	if (res < 0) {
		log_printf(DBG_ERROR, "Couldn't read version file %s: 0x%08x", filePath.c_str(), res);
		return res;
	}
	YAML::Node node = YAML::Load(versionYamlContent);
	VersionYAML v = node.as<VersionYAML>();

	try {
		vInfo.latestVersion[0] = std::stoi(v.version.substr(0, 2));
		vInfo.latestVersion[1] = std::stoi(v.version.substr(3, 2));
		log_printf(DBG_DEBUG, "Version field successfully parsed %i %i", vInfo.latestVersion[0], vInfo.latestVersion[1]);
	} catch (const std::invalid_argument& e) {
		log_printf(DBG_ERROR, "Couldn't parse content of version field %s: %s", filePath.c_str(), v.version.c_str());
		return -1;
	}

	vInfo.url = v.url;

	return 0;
}

int Update::getVersionInfo(bool &available, std::string &url) {
	int res;
	res = sceIoRemove(VERSION_YAML_PATH);
	log_printf(DBG_DEBUG, "sceIoRemove(%s) = 0x%08x", VERSION_YAML_PATH, res);
	try {
		res = Network::get_instance()->Download(std::string(VERSION_YAML_URL), std::string(VERSION_YAML_PATH));
	} catch (const std::runtime_error &err) {
		log_printf(DBG_ERROR, "Couldn't download version.yml: %s", err.what());
		DialogView::openDialogView(nullptr, std_string_format("Couldn't check for update\n\n%s", err.what()),
		                           DIALOG_TYPE_OK);
		return false;
	}
	if (res) {
		log_printf(DBG_ERROR, "Couldn't download version.yml 0x%08x", res);
		return res;
	}

	VersionInfo vInfo;

	res = readVersionYAML(std::string(VERSION_YAML_PATH), vInfo);
	if (res < 0) {
		return res;
	}

	log_printf(DBG_INFO, "Latest online version: %02i.%02i", vInfo.latestVersion[0], vInfo.latestVersion[1]);
	res = sceIoRemove(VERSION_YAML_PATH);
	if (res) {
		log_printf(DBG_ERROR, "Couldn't delete %s: 0x%08x", VERSION_YAML_PATH, res);
	}
	auto currentVersionStr = std::string(VITA_VERSION);
	static_assert(matchVersionString(VITA_VERSION),
	              "VITA_VERSION=" VITA_VERSION " but must match \\d\\d\\.\\d\\d");
	vInfo.currentVersion[0] = std::stoi(currentVersionStr.substr(0, 2));
	vInfo.currentVersion[1] = std::stoi(currentVersionStr.substr(3, 2));

	if (vInfo.latestVersion > vInfo.currentVersion) {
		log_printf(DBG_INFO, "Current version " VITA_VERSION " is outdated.");
		available = true;
		url = vInfo.url;
	} else {
		available = false;
		log_printf(DBG_DEBUG, "Current version " VITA_VERSION " is up-to-date");
	}

	return 0;
}

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
	bool updateExists = false;
	std::string updateURL;
	Update::getVersionInfo(updateExists, updateURL);
	if (updateExists) {
		auto res = std::make_shared<DialogViewResult>();
		DialogView::openDialogView(res, "A new version of VHBB is available.\nDo you want to update?", DIALOG_TYPE_YESNO);
		while (res->status == COMMON_DIALOG_STATUS_RUNNING || res->status == COMMON_DIALOG_STATUS_NONE) {
			sceKernelDelayThread(16666);  // roughly 1/60 second
		}
		if (res->accepted) {
			log_printf(DBG_INFO, "User chose to update VHBB");
			InfoProgress progress;
			auto progressView = startProgressView(progress, "Update Helper (1/2)");
			auto bgView = startBackgroundView();
			try {
				installUpdater(progress.Range(0, 20));
				progressView->hb_name = "VHBB Update (2/2)";
				prepareUpdateFiles(updateURL, progress.Range(20, 100));
				progress.message("Finished");
				progressView->Finish(700);
				sceKernelDelayThread(700000);
				bgView->request_destroy = true;
				updateState_ptr->store(UPDATE_STATE_READY_TO_LAUNCH_UPDATER);
				return;
			} catch (std::exception &ex) {
				const std::string &update_failed = std_string_format("Update failed: %s", ex.what());
				log_printf(DBG_ERROR, update_failed);
				progress.message(update_failed);
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

void Update::prepareUpdateFiles(const std::string &updateURL, InfoProgress progress) {
	log_printf(DBG_DEBUG, "Downloading update vpk");
	progress.message("Downloading update...");

	Network::get_instance()->Download(updateURL, std::string("ux0:/temp/download.vpk"), progress.Range(0, 60));

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
