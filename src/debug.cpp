#include <global_include.h>

#ifdef _DEBUG

int dbg_init()
{
	#ifdef DEBUGNET
	debugNetInit(DEBUGNETIP, 18194, 3);
	#endif

	return 0;
}

int _dbg_printf(int level, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buf1[512];
	switch (level) {
	case DBG_INFO:
		snprintf(buf1, 512, "\033[1;34;7m[INFO]\033[0m\033[1;34m %s\033[0m", format);
		break;
	case DBG_ERROR:
		snprintf(buf1, 512, "\033[1;31;7m[ERROR]\033[0m\033[1;31m %s\033[0m", format);
		break;
	case DBG_WARNING:
		snprintf(buf1, 512, "\033[1;35;7m[WARNING]\033[0m\033[1;35m %s\033[0m", format);
		break;
	case DBG_DEBUG:
		snprintf(buf1, 512, "\033[1;30;7m[DEBUG]\033[0m\033[1;30m %s\033[0m", format);
		break;
	}

	char buf2[512];
	vsnprintf(buf2, 512, buf1, args);
	
	va_end(args);

	#ifdef DEBUGNET
	debugNetUDPSend(buf2);
	#endif

	return 0;
}

#endif
