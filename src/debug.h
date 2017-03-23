#pragma once

#include <global_include.h>

inline std::string methodName(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    return prettyFunction.substr(begin,end) + "()";
}


#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

#ifndef _DEBUG
#define dbg_init()
#define dbg_printf(...)
#define dbg_assert(...)
#else

#define DBG_NONE 0
#define DBG_INFO 1
#define DBG_ERROR 2
#define DBG_WARNING 3
#define DBG_DEBUG 4

int dbg_init();
int dbg_printf(int level, const char *format, ...);
int _dbg_printf(int level, const char *format, ...);
bool dbg_assert(bool expr);

#define dbg_printf(level,format,...) _dbg_printf(level,(std::string("[") + std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " +__METHOD_NAME__ + "]  " + format + "\n").c_str(),##__VA_ARGS__)
#define dbg_assert(expr) if(expr){dbg_printf(DBG_ERROR, (std::string("Assertion error ==> ") + #expr).c_str()); return true;}else{return false;}
//#define dbg_printf(level,format,...) _dbg_printf(level,format"\n",##__VA_ARGS__)
#endif
