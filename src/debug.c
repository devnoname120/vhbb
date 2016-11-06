#include <global_include.h>

#ifdef DEBUG

int dbg_init()
{
	#ifdef DEBUGNET
	debugNetInit(DEBUGNETIP, 18194, DEBUG);
	#endif

	return 0;
}

int _dbg_printf(int level, char *format, ...)
{
	va_list args;
	va_start(args, format);

	char asdf[256];
	vsnprintf(asdf, 256, format, args);
	va_end(args);

	#ifdef DEBUGNET
	debugNetPrintf(level, asdf);
	#endif

	return 0;
}

#endif