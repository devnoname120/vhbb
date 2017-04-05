#include "database.h"
#include <global_include.h>

#include "zip.h"
#include "network.h"
#include "homebrew.h"

bool IsVHBB(const Homebrew &hb)
{
	return hb.titleid == "VHBB00001";
}

Database::Database(const std::string &db_path) : db(YAML::LoadFile(db_path.c_str()))
{
	YAML::Node lst = db;
	try {
		homebrews = lst.as<std::vector<Homebrew>>();

		// Remove VHBB from the list of homebrews
		homebrews.erase(std::remove_if(homebrews.begin(), homebrews.end(), IsVHBB), homebrews.end());
	} catch (const std::exception& ex) {
		dbg_printf(DBG_ERROR, "Couldn't unserialize db: %s", ex.what());
	}
}

Database::~Database()
{
}


bool IsCategory::operator()(const Homebrew &hb) const {
	std::string hbcat_lower;
	std::transform(hb.category.begin(), hb.category.end(), std::back_inserter(hbcat_lower), tolower);

	std::string cat_lower;
	std::transform(cat_.begin(), cat_.end(), std::back_inserter(cat_lower), tolower);

	return cat_lower.compare(hbcat_lower) == 0;
}


int Database::DownloadIcons()
{
    sceIoMkdir(ICONS_FOLDER.c_str(), 0777);

	if (access((ICONS_FOLDER + "/init.txt").c_str(), F_OK) == -1) {
		// TODO display progess to user
		auto zip = Zipfile(ICON_ZIP);

		zip.Unzip(ICONS_FOLDER + "/");

		int fd = sceIoOpen((ICONS_FOLDER + "/init.txt").c_str(), SCE_O_WRONLY|SCE_O_CREAT, 0777);
		sceIoWrite(fd, "ok", 2);
		sceIoClose(fd);
	}

    for (auto hb : homebrews) {
		std::string path = ICONS_FOLDER + "/" + hb.icon;

		if (access(path.c_str(), F_OK) == -1) {
		    std::string url = ICON_URL_PREFIX + hb.icon;

		    Network::get_instance()->Download(url, path);
		}
	}
	return 0;
}