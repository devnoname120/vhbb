#include "database.h"

#include "network.h"
#include "homebrew.h"

#include <global_include.h>


Database::Database(const std::string &db_path) : db(YAML::LoadFile(db_path.c_str()))
{
	// Rinnegatamante's API doesn't support this
	/*std::string version = db["api"].as<std::string>();
	if (version.compare("1")) {
		throw std::runtime_error(std::string("Database Version mismatch: got ") + version + std::string(" expected 1"));
	}*/

	YAML::Node lst = db;
	try {
		homebrews = lst.as<std::vector<Homebrew>>();
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

    for (auto hb : homebrews) {
		std::string path = ICONS_FOLDER + "/" + hb.icon;

		if (access(path.c_str(), F_OK) == -1) {
		    std::string url = ICON_URL_PREFIX + hb.icon;

		    Network::get_instance()->Download(url, path.c_str());
		}
	}
}