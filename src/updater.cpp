#include "updater.h"

#define BASE_ADDRESS "https://raw.githubusercontent.com/robsdedude/vhbb/autoUpdate/"
#define VERSION_URL BASE_ADDRESS "release/raw/version.bin"
#define VERSION_FN "version.bin"
#define VERSION_PATH UPDATE_FOLDER "/" VERSION_FN

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

bool Updater::updateExists() {
	int res;
	res = sceIoMkdir(UPDATE_FOLDER, 0777);
	log_printf(DBG_DEBUG, "res = sceIoMkdir(%s, 0777) = %i", UPDATE_FOLDER, res);
	res = Network::get_instance()->Download(std::string(VERSION_URL), VERSION_PATH);
	if (res) {
		log_printf(DBG_ERROR, "Couldn't download version.bin %i", res);
		return false;
	}
	uint32_t latestVersion[2], currentVersion[2];
	res = readFile(VERSION_PATH, latestVersion, sizeof(uint32_t)*2);
	if (res < 0) {
		log_printf(DBG_ERROR, "Couldn't read \"" VERSION_PATH "\": %i", res);
		return false;
	}
	log_printf(DBG_INFO, "Latest online version: %02i.%02i", latestVersion[0], latestVersion[1]);
	res = sceIoRemove(VERSION_PATH);
	if (res) {
		log_printf(DBG_ERROR, "Couldn't delete " VERSION_PATH ": %0.8x", res);
	}
	// FIXME: using artificially old version for deving
//	auto currentVersionStr = std::string(VITA_VERSION);
	auto currentVersionStr = std::string("00.79");
	static_assert(matchVersionString(VITA_VERSION),
	              "VITA_VERSION=" VITA_VERSION " but must match \\d\\d\\.\\d\\d");
	currentVersion[0] = std::stoi(currentVersionStr.substr(0, 2));
	currentVersion[1] = std::stoi(currentVersionStr.substr(3, 2));
	auto lvptr = (uint64_t*) &latestVersion;
	auto cvptr = (uint64_t*) &currentVersion;
	if (*lvptr > *cvptr) {
		log_printf(DBG_INFO, "Current version " VITA_VERSION " is outdated.");
		return true;
	}
	log_printf(DBG_DEBUG, "Current version " VITA_VERSION " is up-to-date");
	return false;
}

void Updater::updateCheckThread(unsigned int arglen, void *args) {
	if (Updater::updateExists()) {
		DialogViewResult res{};
		DialogView::openDialogView(&res, "A new version of VHBB is available.\nDo you want to update?", DIALOG_TYPE_YESNO);
		while (res.status == COMMON_DIALOG_STATUS_RUNNING || res.status == COMMON_DIALOG_STATUS_NONE) {
			sceKernelDelayThread(16666);  // roughly 1/60 second
		}
		if (!res.accepted) {
			log_printf(DBG_INFO, "User refused to update VHBB");
			// TODO: do stuff
			return;
		}
		log_printf(DBG_INFO, "User chose to update VHBB");
	}
}

void Updater::startUpdateCheckThread() {
	SceUID thid_db = sceKernelCreateThread(
		"update_check_thread", (SceKernelThreadEntry) Updater::updateCheckThread, 0x40, 0x20000, 0,
		0, nullptr);
	sceKernelStartThread(thid_db, 0, nullptr);
}
