#include "database.h"
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
