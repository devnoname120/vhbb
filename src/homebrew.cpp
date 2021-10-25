#include "homebrew.h"

#include "debug.h"

#include <psp2/promoterutil.h>
#include <psp2/sysmodule.h>

bool Homebrew::IsInstalled()
{
    // FIXME Don't reload the module every time

    // ScePaf is required for PromoterUtil
    SceSysmoduleOpt opt = { 0 };
    opt.result = &opt.flags;
    uint32_t scepaf_argp[] = { 0x400000, 0xEA60, 0x40000, 0, 0 };
    sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(scepaf_argp), scepaf_argp, &opt);

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
        if (node["trailer"])
            hb.trailer = node["trailer"].as<std::string>();
        if (node["screenshots"])
            hb.screenshots = node["screenshots"].as<std::vector<std::string>>();

        hb.url = node["url"].as<std::string>();
        if (node["data"])
            hb.data = node["data"].as<std::string>();

        return true;
    }
}; // namespace YAML
