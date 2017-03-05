#pragma once

#include <global_include.h>

#ifndef _DEBUG
#define dbg_init()
#define dbg_printf(...)
#else

#define DBG_NONE 0
#define DBG_INFO 1
#define DBG_ERROR 2
#define DBG_WARNING 3
#define DBG_DEBUG 4

int dbg_init();
int dbg_printf(int level, const char *format, ...);
int _dbg_printf(int level, const char *format, ...);

#define dbg_printf(level,format,...) _dbg_printf(level,format"\n",##__VA_ARGS__)

#endif
