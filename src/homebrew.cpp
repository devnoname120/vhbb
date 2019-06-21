#include "homebrew.h"

#include "debug.h"
#include "homebrewInfo.h"

#include <psp2/promoterutil.h>
#include <psp2/sysmodule.h>

bool Homebrew::IsInstalled()
{
    // FIXME Don't reload the module every time

    // ScePaf is required for PromoterUtil
    uint32_t ptr[0x100] = { 0 };
    ptr[0] = 0;
    ptr[1] = (uint32_t)&ptr[0];
    uint32_t scepaf_argp[] = { 0x400000, 0xEA60, 0x40000, 0, 0 };
    sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(scepaf_argp), scepaf_argp, ptr);

    sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);

    int ret = scePromoterUtilityInit();
    if (ret < 0)
        throw std::runtime_error("scePromoterUtilityInit() = " + ret);

    int res;
    log_printf(DBG_DEBUG, "Checking if %s exists...", titleid.c_str());
    int installed = scePromoterUtilityCheckExist(titleid.c_str(), &res);
    log_printf(DBG_DEBUG, "Done checking");

    scePromoterUtilityExit();

    return installed >= 0;
}
bool Homebrew::IsUpdateAvailable()
{
    if (!IsInstalled())
    {
        return false;
    }

    std::string homebrewInfoPath = std::string("ux0:app/") + titleid + "/" + "vitadb_info.yml";

    YAML::Node node;
    try
    {
        node = YAML::LoadFile(homebrewInfoPath);
    }
    catch (YAML::BadFile& e)
    {
        // Homebrew not installed with VHBB, assume that an update is available
        return true;
    }
    catch (YAML::ParserException& e)
    {
        log_printf(DBG_ERROR, "Cannot parse [%s]: %s", homebrewInfoPath.c_str(), e.msg.c_str());
        return true;
    }

    HomebrewInfo info;
    try
    {
        info = node.as<HomebrewInfo>();
    }
    catch (YAML::InvalidNode& e)
    {
        log_printf(DBG_ERROR, "Cannot cast to HomebrewInfo [%s]: %s", homebrewInfoPath.c_str(), e.msg.c_str());

        // In case HomebrewInfo is changed incorrectly, we don't want every homebrew to show that it has an update
        return false;
    }
    return info.date > date;
}

// Add type to yaml-cpp
namespace YAML
{
    bool convert<Homebrew>::decode(const Node& node, Homebrew& hb)
    {
        hb.name = node["name"].as<std::string>();
        hb.icon = node["icon"].as<std::string>();
        hb.version = node["version"].as<std::string>();
        hb.author = node["author"].as<std::string>();
        hb.category = node["type"].as<std::string>();
        hb.description = node["description"].as<std::string>();
        hb.date = node["date"].as<Date>();
        hb.titleid = node["titleid"].as<std::string>();
        if (node["long_description"])
            hb.long_description = node["long_description"].as<std::string>();
        if (node["screenshots"])
            hb.screenshots = node["screenshots"].as<std::vector<std::string>>();

        hb.url = node["url"].as<std::string>();
        if (node["data"])
            hb.data = node["data"].as<std::string>();

        return true;
    }
}; // namespace YAML
