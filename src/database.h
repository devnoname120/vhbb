#pragma once

#include <global_include.h>
#include "homebrew.h"
#include "singleton.h"

class Database: public Singleton<Database> {
friend class Singleton<Database>;
public:
	// FIXME Don't use this constructor, it's here because of singleton
	Database();
	Database(const std::string &db_path);
	~Database();
	const YAML::Node db;
	std::vector<Homebrew> homebrews;
};
