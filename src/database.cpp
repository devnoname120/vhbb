#include "database.h"

#include "macros.h"
#include "network.h"
#include "zip.h"

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

bool IsVHBB(const Homebrew& hb)
{
    return hb.titleid == "VHBB00001";
}

Database::Database(const std::string& db_path)
    : db(YAML::LoadFile(db_path))
{
    YAML::Node lst = db;
    try
    {
        homebrews = lst.as<std::vector<Homebrew>>();

        // Remove VHBB from the list of homebrews
        homebrews.erase(std::remove_if(homebrews.begin(), homebrews.end(), IsVHBB), homebrews.end());
    }
    catch (const std::exception& ex)
    {
        log_printf(DBG_ERROR, "Couldn't unserialize db: %s", ex.what());
    }
}

Database::~Database() = default;

bool IsCategory::operator()(const Homebrew& hb) const
{
    std::string hbcat_lower;
    std::transform(hb.category.begin(), hb.category.end(), std::back_inserter(hbcat_lower), tolower);

    std::string cat_lower;
    std::transform(cat_.begin(), cat_.end(), std::back_inserter(cat_lower), tolower);

    return cat_lower == hbcat_lower;
}

SearchQuery::SearchQuery(const std::string& query)
{
    std::transform(query.begin(), query.end(), std::back_inserter(query_), tolower);
}

HomebrewSearchRating SearchQuery::operator()(const Homebrew& hb) const
{
    std::string fields[] = { hb.name, hb.description, hb.long_description };
    HomebrewSearchRating hb_rating;
    hb_rating.hb = hb;
    auto fields_s = sizeof(fields) / sizeof(fields[0]);
    for (auto i = 0; i < fields_s; i++)
    {
        std::string field_lower;
        std::transform(fields[i].begin(), fields[i].end(), std::back_inserter(field_lower), tolower);
        if (field_lower.find(query_, 0) != std::string::npos)
        {
            hb_rating.rating = fields_s - i;
            break;
        }
    }
    return hb_rating;
}

int Database::DownloadIcons()
{
    sceIoMkdir(ICONS_FOLDER, 0777);

    if (access(ICONS_FOLDER "/init.txt", F_OK) == -1)
    {
        // TODO display progess to user
        auto zip = Zipfile(std::string(ICON_ZIP));

        zip.Unzip(std::string(ICONS_FOLDER "/"));

        int fd = sceIoOpen(ICONS_FOLDER "/init.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
        sceIoWrite(fd, "ok", 2);
        sceIoClose(fd);
    }

    for (auto hb : homebrews)
    {
        auto path = std::string(ICONS_FOLDER "/") + hb.icon;

        if (access(path.c_str(), F_OK) == -1)
        {
            auto url = std::string(ICON_URL_PREFIX) + hb.icon;

            Network::get_instance()->Download(url, path);
        }
    }
    return 0;
}