#pragma once

#include <psp2/io/fcntl.h>
#include <cstdarg>
#include <string>
#include <string.h>
#include <debugnet.h>

#define VHBB_LOG_DIR "ux0:/log"

inline std::string methodName(const std::string &prettyFunction) {
	size_t args_start = prettyFunction.find('(');
	size_t begin = prettyFunction.substr(0, args_start).rfind(' ') + 1;
	size_t end = args_start - begin;

	return prettyFunction.substr(begin, end) + "()";
}


#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)


#define DBG_NONE 0
#define DBG_INFO 1
#define DBG_ERROR 2
#define DBG_WARNING 3
#define DBG_DEBUG 4

int log_init();
int _log_printf(int level, const char *format, ...);
int log_printf(int level, const char *format, ...);
bool log_assert(bool expr);

#ifdef DEBUGNET
	#define log_printf(level,format,...) _log_printf(level,(std::string("[") + std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " +__METHOD_NAME__ + "]  " + format + "\n").c_str(),##__VA_ARGS__)
	#define log_assert(expr) if(expr){log_printf(DBG_ERROR, (std::string("Assertion error ==> ") + #expr).c_str()); return true;}else{return false;}
#else
	#define log_printf(level,format,...) ((void)0)
	#define log_assert(expr) ((void)0)
	#define log_init(...) ((void)0)
	#define _log_printf(level,format,...) ((void)0)
#endif
