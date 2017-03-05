#pragma once

#include <global_include.h>
#include "homebrew.h"
#include "singleton.h"

class Database: public Singleton<Database> {
friend class Singleton<Database>;
public:
	Database(const std::string &db_path);
	~Database();
	const YAML::Node db;
	std::vector<Homebrew> homebrews;
};
