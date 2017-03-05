#pragma once

#include <global_include.h>
#include "homebrew.h"
#include "singleton.h"

class Database: public Singleton<Database> {
friend class Singleton<Database>;
public:
	std::vector<Homebrew> homebrews;

	Database(const std::string &db_path);
	~Database();

	template <class UnaryPredicate>
	std::vector<Homebrew> Filter(UnaryPredicate pred);
private:
	const YAML::Node db;
};

template <class UnaryPredicate>
std::vector<Homebrew> Database::Filter(UnaryPredicate pred)
{
	std::vector<Homebrew> res;
	std::copy_if(homebrews.begin(), homebrews.end(), back_inserter(res), pred);
	return res;
}

struct IsCategory : public std::unary_function<std::string, bool> {
    IsCategory(const std::string &cat) : cat_(cat) {}
    bool operator()(const Homebrew &hb) const;

    std::string cat_;
};
