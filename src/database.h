#include <utility>

#pragma once

#include <global_include.h>
#include "homebrew.h"
#include "singleton.h"

#define ICON_ZIP std::string("ux0:/app/VHBB00001/resources/icons.zip")

class Database: public Singleton<Database> {
friend class Singleton<Database>;
public:
	std::vector<Homebrew> homebrews;

	explicit Database(const std::string &db_path);
	~Database();

	int DownloadIcons();

	template <class UnaryPredicate>
	std::vector<Homebrew> Filter(UnaryPredicate pred);

	template <class UnaryPredicate>
	std::vector<Homebrew> Sort(UnaryPredicate pred);
private:
	const YAML::Node db;
};

// TODO move this into homebrew.cpp
template <class UnaryPredicate>
std::vector<Homebrew> Database::Filter(UnaryPredicate pred)
{
	std::vector<Homebrew> res;
	std::copy_if(homebrews.begin(), homebrews.end(), back_inserter(res), pred);
	return res;
}

struct IsCategory : public std::unary_function<std::string, bool> {
	explicit IsCategory(std::string cat) : cat_(std::move(cat)) {}
    bool operator()(const Homebrew &hb) const;

    std::string cat_;
};


template <class UnaryPredicate>
std::vector<Homebrew> Database::Sort(UnaryPredicate pred)
{
	std::vector<Homebrew> res = homebrews;
	std::sort(res.begin(), res.end(), pred);
	return res;
}
