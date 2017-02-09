#pragma once

#include <global_include.h>

class Database {
public:
	Database(const std::string &db_path);
	~Database();
	const YAML::Node db;
};
