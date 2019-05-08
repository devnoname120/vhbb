#include <utility>

#pragma once

#include <global_include.h>
#include "homebrew.h"
#include "singleton.h"

#define ICON_ZIP VHBB_RESOURCES "/resources/icons.zip"

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

	// FIXME: would be better if Filter and Sort could be concatenated to effectively perform a search
	template <class UnaryPredicate>
	std::vector<Homebrew> Search(UnaryPredicate pred);
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


struct HomebrewSearchRating {
	Homebrew hb;
	float rating = -infinityf();
};


struct SearchQuery : public std::unary_function<std::string, HomebrewSearchRating> {
	SearchQuery(const std::string &query);
	HomebrewSearchRating operator()(const Homebrew &hb) const;

	std::string query_;
};


template <class UnaryPredicate>
std::vector<Homebrew> Database::Search(UnaryPredicate pred)
{
	// rate the hbs by pred
	std::vector<HomebrewSearchRating> ratings, pos_ratings;
	std::transform(homebrews.begin(), homebrews.end(), back_inserter(ratings), pred);
	// drop negative ratings i.e. no match
	std::copy_if(ratings.begin(), ratings.end(), back_inserter(pos_ratings), [](HomebrewSearchRating r)->bool{return r.rating >= 0;});

	// sort by rating
	std::stable_sort(pos_ratings.begin(), pos_ratings.end(), [](HomebrewSearchRating r1, HomebrewSearchRating r2)->bool{return r1.rating > r2.rating;});

	std::vector<Homebrew> res;
	std::transform(pos_ratings.begin(), pos_ratings.end(), back_inserter(res), [](HomebrewSearchRating r)->Homebrew{return r.hb;});
	return res;
}
