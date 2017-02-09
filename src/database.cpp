#include <global_include.h>

Database::Database(const std::string &db_path) : db(YAML::LoadFile(db_path.c_str()))
{
	std::string version = db["api"].as<std::string>();
	if (version.compare("1")) {
		throw std::runtime_error(std::string("Database Version mismatch: got ") + version + std::string(" expected 1"));
	}
}

Database::~Database()
{
}
