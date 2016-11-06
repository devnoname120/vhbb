#pragma once

#include <global_include.h>

#ifndef DEBUG
#define dbg_init()
#define dbg_printf(...)
#else

int dbg_init();
int dbg_printf(int level, char *format, ...);
int _dbg_printf(int level, char *format, ...);

#define dbg_printf(level,format,...) _dbg_printf(level,format"\n",##__VA_ARGS__)

#endif
