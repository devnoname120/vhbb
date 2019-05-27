// FIXME Merge with src/debug.h

#pragma once

#include <string>

#define LOG_DIR "ux0:/log"
#define LOG_FILE UPDATE_TITLEID "_updater"

inline std::string methodName(const std::string &prettyFunction) {
	size_t args_start = prettyFunction.find('(');
	size_t begin = prettyFunction.substr(0, args_start).rfind(' ') + 1;
	size_t length = args_start - begin;

	return prettyFunction.substr(begin, length) + "()";
}


#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


#define DBG_NONE 0
#define DBG_INFO 1
#define DBG_ERROR 2
#define DBG_WARNING 3
#define DBG_DEBUG 4

int log_init(bool log_to_file = false);
int _log_printf(int level, const char *format, ...);

#define log_printf(level,format,...) _log_printf(level,(std::string("[") + std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + " " +__METHOD_NAME__ + "]  " + format + "\n").c_str(),##__VA_ARGS__)
