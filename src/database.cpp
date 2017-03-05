#include "database.h"
#include "homebrew.h"

#include <global_include.h>


Database::Database(const std::string &db_path) : db(YAML::LoadFile(db_path.c_str()))
{
	std::string version = db["api"].as<std::string>();
	if (version.compare("1")) {
		throw std::runtime_error(std::string("Database Version mismatch: got ") + version + std::string(" expected 1"));
	}

	YAML::Node lst = db["homebrews"]["vita"];
	try {
		homebrews = lst.as<std::vector<Homebrew>>();
	} catch (const std::exception& ex) {
		dbg_printf(DBG_ERROR, "Couldn't unserialize db: %s", ex.what());
	}
}

Database::~Database()
{
}

template <class UnaryPredicate>
std::vector<Homebrew> Database::Filter(UnaryPredicate pred)
{
	std::vector<Homebrew> res;
	std::copy_if(homebrews.begin(), homebrews.end(), back_inserter(res), pred);
	return res;
}


bool IsCategory::operator()(const Homebrew &hb) const {
        return cat_.compare(hb.title) == 0;
}
