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

	char buf1[256];
	switch (level) {
	case DBG_INFO:
		snprintf(buf1, 256, "\033[1;34;7m[INFO]\033[0m\033[1;34m %s\033[0m", format);
		break;
	case DBG_ERROR:
		snprintf(buf1, 256, "\033[1;31;7m[ERROR]\033[0m\033[1;31m %s\033[0m", format);
		break;
	case DBG_WARNING:
		snprintf(buf1, 256, "\033[1;35;7m[WARNING]\033[0m\033[1;35m %s\033[0m", format);
		break;
	case DBG_DEBUG:
		snprintf(buf1, 256, "\033[1;30;7m[DEBUG]\033[0m\033[1;30m %s\033[0m", format);
		break;
	}

	char buf2[256];
	vsnprintf(buf2, 256, buf1, args);
	
	va_end(args);

	#ifdef DEBUGNET
	debugNetSend(buf2);
	#endif

	return 0;
}

#endif
