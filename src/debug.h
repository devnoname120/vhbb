#pragma once

#include <global_include.h>

#define VHBB_LOG_DIR "ux0:/log"

inline std::string methodName(const std::string &prettyFunction) {
	size_t args_start = prettyFunction.find('(');
	size_t begin = prettyFunction.substr(0, args_start).rfind(' ') + 1;
	size_t length = args_start - begin;

	return prettyFunction.substr(begin, length) + "()";
}


#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)


#define DBG_NONE 0
#define DBG_INFO 1
#define DBG_ERROR 2
#define DBG_WARNING 3
#define DBG_DEBUG 4

int log_init(bool log_to_file = false);
int log_printf(int level, const char *format, ...);
int _log_printf(int level, const char *format, ...);
bool log_assert(bool expr);

#define log_printf(level,format,...) _log_printf(level,(std::string("[") + std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " +__METHOD_NAME__ + "]  " + format + "\n").c_str(),##__VA_ARGS__)
#define log_assert(expr) if(expr){log_printf(DBG_ERROR, (std::string("Assertion error ==> ") + #expr).c_str()); return true;}else{return false;}
//#define log_printf(level,format,...) _log_printf(level,format"\n",##__VA_ARGS__)
